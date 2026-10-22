#include <math.h>
#include <stddef.h>

#include "bearing.h"
#include "common.h"

float fault_bearing_calculate(struct BearingConfig *cfg, float base_signal, float shaft_phase)
{
    if (cfg == NULL) return base_signal;

    // Synchronize localized impact phase relative to overall motor position
    cfg->current_impact_phase = shaft_phase * cfg->fault_frequency_multiplier;

    // Wrap calculations cleanly past 2*PI boundary loops using standard FPU math
    float normalized_phase = fmodf(cfg->current_impact_phase, 2.0f * PI);
    float impact_component = 0.0f;
    
    // Generate sharp structural shock rings every time a crack event crosses zero
    if (normalized_phase < 0.4f) {
        float high_freq_ringing = 12.0f; // High frequency microstructural ringing component
        float decay_envelope = expf(-cfg->decay_rate * normalized_phase);
        
        impact_component = cfg->impact_amplitude * decay_envelope * sinf(high_freq_ringing * normalized_phase);
    }

    // Stack the asynchronous high-frequency transient response directly on top
    return base_signal + impact_component;
}
