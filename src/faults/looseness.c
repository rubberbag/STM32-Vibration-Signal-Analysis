#include <stddef.h>
#include "looseness.h"

float fault_looseness_calculate(const struct LoosenessConfig *cfg, float base_signal)
{
    if (cfg == NULL) return base_signal;

    // Simulate structural rattling. Flat symmetrical clipping models parts 
    // repeatedly colliding against base mounts, injecting multi-orders in FFT.
    if (base_signal > cfg->threshold) {
        return cfg->threshold;
    } 
    else if (base_signal < -cfg->threshold) {
        return -cfg->threshold;
    }
    return base_signal;
}
