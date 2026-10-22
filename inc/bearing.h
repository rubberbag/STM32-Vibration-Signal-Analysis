#ifndef BEARING_H
#define BEARING_H

struct BearingConfig {
    float fault_frequency_multiplier; // Non-integer geometric multiplier (e.g. 3.58f for BPFI)
    float impact_amplitude;            // Instantaneous peak intensity height of a strike
    float decay_rate;                  // Structural exponential damping factor
    float current_impact_phase;        // Cumulative rolling-element runtime phase path
};

float fault_bearing_calculate(struct BearingConfig *cfg, float base_signal, float shaft_phase);

#endif
