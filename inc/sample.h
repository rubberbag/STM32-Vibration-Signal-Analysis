#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "vibration.h"
#include "faults.h"
#include "disturbances.h"

// The block size configuration is declared here as it owns the ping-pong footprint
#define BUFFER_BLOCK_SIZE 512

typedef enum {
    SAMPLE_STATUS_OK = 0,
    SAMPLE_STATUS_INVALID_ARGUMENT,
    SAMPLE_STATUS_VIBRATION_ERROR,
    SAMPLE_STATUS_DISTURBANCE_ERROR
} SampleStatus;


struct SampleGenerator {
    struct VibrationGenerator vibration;
    struct FaultGenerator fault;
    struct DisturbanceGenerator disturbance;

    ActiveFaultMode fault_mode; 
    ActiveDisturbanceMode disturbance_mode;

    // Native FPU precision data storage paths
    float buffer_a[BUFFER_BLOCK_SIZE];
    float buffer_b[BUFFER_BLOCK_SIZE];
    uint16_t write_index;
    bool use_buffer_b;
    
    // Read-only pointer provided back to main upon buffer fill completion
    const float *last_completed_buffer; 
};

/**
 * Initializes simulation engine parameters and performs core hardware validation checks.
 */
SampleStatus sample_generator_init(struct SampleGenerator *sample, const struct SignalConfig *config);

/**
 * Executes a single sensor sampling step. 
 * Returns true the exact instant a 512-sample buffer block finishes filling up.
 */
bool sample_execute_tick(struct SampleGenerator *sample);

/**
 * Clean data accessor. Returns the read-only pointer to the static full buffer block.
 */
const float* sample_get_time_domain(const struct SampleGenerator *sample);

#endif
