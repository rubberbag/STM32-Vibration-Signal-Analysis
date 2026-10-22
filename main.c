#include <stdbool.h>
#include <stddef.h>
#include <math.h>

#include "stm32f4xx.h"

#include "common.h"
#include "sample.h"
#include "dsp.h"
#include "packet.h"
#include "uart.h"

#define SAMPLE_BLOCK_SIZE  512
#define SPECTRUM_BIN_SIZE  (SAMPLE_BLOCK_SIZE / 2)

// Allocated static tracking scratchpads to safeguard stack constraints
static float hanning_lookup_table[SAMPLE_BLOCK_SIZE];
static float workspace_windowed[SAMPLE_BLOCK_SIZE];
static float workspace_fft[SPECTRUM_BIN_SIZE];

static void generate_hanning_window_lut(float *lut, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        lut[i] = 0.5f * (1.0f - cosf((2.0f * PI * (float)i) / (float)(length - 1)));
    }
}

int main(void)
{
    uart_init(); 

    USART1_SendString("--- DIGITAL SIGNAL PROCESSING ---\r\n");

    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));
    __DSB(); 
    __ISB(); 

    USART1_SendString("--- SCB->CPACR Enabled ---\r\n");

    // Initialize our optimized Hanning multiplication window at system boot
    generate_hanning_window_lut(hanning_lookup_table, SAMPLE_BLOCK_SIZE);

    USART1_SendString("--- Generated Multipliers ---\r\n");
    // Boot system UART peripheral block directly using your register configuration variables
    // Target APB2 Peripheral clock frequency runs at 84MHz when System Core runs at 168MHz
   
    RNG_Init();
    USART1_SendString("--- RNG INITIALIZED ---\r\n");

    //  FIRMWARE APPLICATION TUNING COEFFICIENTS (EASY PARAMETER SWAPPING)

    float system_sample_rate   = 1000.0f;  // Core sampling frame speed frequency
    float system_scale_factor  = 1000.0f;  // Symmetrical 1000x fixed-point conversion scale
    uint32_t start_packet_id   = 0;        // Initial numeric block starting index marker

    //  Core Vibration Simulation Parameters
    float vibration_rpm        = 1200.0f;
    float vibration_amplitude  = 0.5f;
    int   vibration_harm_count = 4;
    float vibration_harm_amps[4] = {1.0f, -10.0f, 5.4f, 2.3f};

    //  Fault Severity Parameter Configurations
    float fault_severity_imbalance = 0.35f;  // 35% force modulation amplification
    float fault_threshold_looseness = 0.80f; // Symmetrical mechanical rattle clipping bound
    float fault_bearing_multiplier  = 3.58f; // Asynchronous Outer-Race Ball Defect Order (BPFO)
    float fault_bearing_amplitude   = 0.40f; // Pure impact peak force magnitude
    float fault_bearing_decay_rate  = 6.0f;  // Structural damping decay envelope speed

    //  Electrical Interference Noise Parameters
    float electric_frequency   = 50.0f;     // Power grid alternating hum frequency line
    float electric_amplitude   = 0.15f;     // Base power-line interference spike height
    int   electric_harm_count  = 2;
    float electric_harm_amps[2] = {0.03f, 0.01f};

    //  Gaussian Noise & Drift Parameter Intensities
    float target_snr_db        = 28.0f;     // High-resolution Signal-To-Noise ratio configuration
    float drift_intensity_sigma = 0.04f;     // Sensor temperature drift walk intensity deviation

    //  Operational Simulator Active State Selection Modes
    ActiveFaultMode chosen_fault_mode      = FAULT_MODE_ALL;
    ActiveDisturbanceMode chosen_disturbance_mode     = DISTURBANCE_ALL;

    USART1_SendString("--- CONFIG VALUES ASSIGNED ---\r\n");
 
    struct SignalConfig config = {
        .sample_rate = system_sample_rate
    };

    USART1_SendString("---Signal CONFIG VALUES ASSIGNED ---\r\n");

    struct SampleGenerator sampler = {
        // Core Vibration Parameters
        .vibration = {
            .rpm = vibration_rpm,
            .amplitude = vibration_amplitude,
            .harmonics = {
                .count = vibration_harm_count,
                .amplitude = {
                    vibration_harm_amps[0], vibration_harm_amps[1],
                    vibration_harm_amps[2], vibration_harm_amps[3]
                }
            },
            .oscillator = { .phase = 0.0f }
        },
        // Fault Parameter Mappings
        .fault = {
            .imbalance_cfg = { .severity = fault_severity_imbalance },
            .looseness_cfg = { .threshold = fault_threshold_looseness },
            .bearing_cfg = {
                .fault_frequency_multiplier = fault_bearing_multiplier,
                .impact_amplitude = fault_bearing_amplitude,
                .decay_rate = fault_bearing_decay_rate,
                .current_impact_phase = 0.0f
            }
        },
        // Environmental Disturbance Multi-Layer Mappings
        .disturbance = {
            .electric = {
                .frequency = electric_frequency,
                .amplitude = electric_amplitude,
                .harmonics = {
                    .count = electric_harm_count,
                    .amplitude = { electric_harm_amps[0], electric_harm_amps[1] }
                },
                .oscillator = { .phase = 0.0f }
            },
            .noise = {
                .snr_db = target_snr_db,
                .noise_power = 0.0f,
                .sigma = 0.0f
            },
            .drift = {
                .sigma = drift_intensity_sigma,
                .value = 0.0f,
                .dt = 0.0f
            },
            // Direct activation flags mapping
            .electrical_enabled = (chosen_disturbance_mode == DISTURBANCE_ELECTRICAL || chosen_disturbance_mode == DISTURBANCE_ALL),
            .noise_enabled      = (chosen_disturbance_mode == DISTURBANCE_NOISE      || chosen_disturbance_mode == DISTURBANCE_ALL),
            .drift_enabled      = (chosen_disturbance_mode == DISTURBANCE_DRIFT      || chosen_disturbance_mode == DISTURBANCE_ALL)
        },
        .fault_mode = chosen_fault_mode,
        .disturbance_mode = chosen_disturbance_mode,
        .write_index = 0,
        .use_buffer_b = false,
        .last_completed_buffer = NULL
    };

    USART1_SendString("---Sample Generator VALUES ASSIGNED ---\r\n");

    struct DiagnosticPacket uart_packet;
    uint32_t dynamic_tracking_packet_id = start_packet_id;

    

    // Validate system parameters and pass config down across internal components safely
    if (sample_generator_init(&sampler, &config) != SAMPLE_STATUS_OK) {
        USART1_SendString("\r\nCRITICAL ERROR: sample_gensample_generator_init failed validation routines!\r\n");
        while(1);
        
        // Trap safety lock if hardware definitions fail validation
    }

    USART1_SendString("--- [SUCCESS] Generator Initialized! Starting Poll Loop ---\r\n");
 
    while (1) {
        // Process a single high-speed sensor step update
        bool buffer_is_complete = sample_execute_tick(&sampler);

        if (buffer_is_complete) {
            // Pull the un-mutated read-only data block pointer directly out of the sampler
            USART1_SendString("--- Buffer is Complete ---\r\n");
            const float *raw_time_data = sample_get_time_domain(&sampler);
            USART1_SendString("--- Received Time Domain data ---\r\n");

            if (raw_time_data != NULL) {
                
                // xecute math pipelines natively onto separate local workspace arrays
                USART1_SendString("--- Applying Hanning Window ---\r\n");
                dsp_apply_hanning(raw_time_data, hanning_lookup_table, workspace_windowed, SAMPLE_BLOCK_SIZE);

                USART1_SendString("--- Computing Fast Fourier Transform ---\r\n");
                dsp_compute_fft(workspace_windowed, workspace_fft, SAMPLE_BLOCK_SIZE);

                // Assign structural telemetry tracking metadata fields natively using dot syntax
                uart_packet.packet_id       = dynamic_tracking_packet_id++; // Auto-incrementing identifier
                uart_packet.sample_rate     = system_sample_rate;
                uart_packet.fundamental_rpm = sampler.vibration.rpm;
                uart_packet.scaling_factor  = system_scale_factor;

                // Quantize both streams into fixed point integers to minimize wire footprints
                USART1_SendString("--- Building Packets ---\r\n");
                packet_build(&uart_packet, raw_time_data, workspace_fft, SAMPLE_BLOCK_SIZE);

                // Direct serial dispatch of the packed payload byte-by-byte over the wire
                USART1_SendString("--- Transmitting Packets ---\r\n");

                uart_transmit_packet(&uart_packet);
            }
        }
    }

    return 0;
}
