#include <stddef.h>
#include <string.h>
#include "sample.h"

SampleStatus sample_generator_init(struct SampleGenerator *sample, const struct SignalConfig *config)
{
    if (sample == NULL || config == NULL) {
        return SAMPLE_STATUS_INVALID_ARGUMENT;
    }
    
    memset(sample, 0, sizeof(struct SampleGenerator));

    // Initialize core motor engine and trap hardware limitations immediately
    VibrationStatus vib_status = vibration_init(&sample->vibration, config);
    if (vib_status != VIBRATION_OK && vib_status != VIBRATION_HARMONICS_CLAMPED) {
        return SAMPLE_STATUS_VIBRATION_ERROR;
    }

    // Initialize individual secondary simulator tracking blocks
    fault_init(&sample->fault);
    disturbance_init(&sample->disturbance);

    // Set initial system baseline states
    sample->fault_mode = FAULT_NONE;
    sample->disturbance_mode = DISTURBANCE_NONE;
    sample->write_index = 0;
    sample->use_buffer_b = false;
    sample->last_completed_buffer = NULL;

    return SAMPLE_STATUS_OK;
}

bool sample_execute_tick(struct SampleGenerator *sample)
{
    if (sample == NULL) {
        return false;
    }

    // Phase 1: Core Vibration Engine Wave Generation
    float signal = vibration(&sample->vibration);

    // Phase 2: Cascading Inline Fault Modification 
    if (sample->fault_mode != FAULT_NONE) {
        if (sample->fault_mode == FAULT_IMBALANCE || sample->fault_mode == FAULT_BOTH) {
            signal = fault_imbalance(&sample->fault, signal);
        }
        if (sample->fault_mode == FAULT_LOOSENESS || sample->fault_mode == FAULT_BOTH) {
            signal = fault_looseness(&sample->fault, signal);
        }
    }

    // Phase 3: Additive Noise/Disturbance Injections
    if (sample->disturbance_mode != DISTURBANCE_NONE) {
        if (sample->disturbance_mode == DISTURBANCE_ELECTRICAL || sample->disturbance_mode == DISTURBANCE_ALL) {
            signal += disturbance_electrical(&sample->disturbance);
        }
        if (sample->disturbance_mode == DISTURBANCE_NOISE || sample->disturbance_mode == DISTURBANCE_ALL) {
            signal += disturbance_noise(&sample->disturbance);
        }
        if (sample->disturbance_mode == DISTURBANCE_DRIFT || sample->disturbance_mode == DISTURBANCE_ALL) {
            signal += disturbance_drift(&sample->disturbance);
        }
    }

    // Phase 4: Route signal output straight into the active memory channel
    float *active_buffer = sample->use_buffer_b ? sample->buffer_b : sample->buffer_a;
    active_buffer[sample->write_index++] = signal;

    // Phase 5: Ping-Pong Window Swap Boundary Logic
    if (sample->write_index >= BUFFER_BLOCK_SIZE) {
        // Instantly switch active buffer targets to preserve continuous hardware collection cycles
        sample->use_buffer_b = !sample->use_buffer_b;
        sample->write_index = 0;
        
        // Cache the safe read-only address of the block that just finished filling
        sample->last_completed_buffer = sample->use_buffer_b ? sample->buffer_a : sample->buffer_b;
        
        return true; // Alert caller that a buffer data block is complete and frozen
    }

    return false;
}

const float* sample_get_time_domain(const struct SampleGenerator *sample, size_t *out_length)
{
    if (sample == NULL || out_length == NULL) {
        return NULL;
    }
    
    // Pass the length back out to main dynamically
    *out_length = BUFFER_BLOCK_SIZE; 
    
    // Return the clean data pointer
    return sample->last_completed_buffer;
}
