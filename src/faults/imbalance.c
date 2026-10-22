#include <stddef.h>
#include "imbalance.h" // Pulled out of your inc/ path automatically

float fault_imbalance_calculate(const struct ImbalanceConfig *cfg, float base_signal)
{
    if (cfg == NULL) return base_signal;
    
    // Unbalance forces grow synchronously with rotation, scale fundamental components
    return base_signal * (1.0f + cfg->severity);
}
