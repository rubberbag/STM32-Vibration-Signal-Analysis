#include <stddef.h>
#include "faults.h"

FaultStatus fault_init(struct FaultGenerator *fault)
{
    if (fault == NULL) return FAULT_INVALID_PARAM;

    // 1. Establish stable baseline Imbalance defaults
    fault->imbalance_cfg.severity = 0.25f;    

    // 2. Establish stable baseline Looseness defaults
    fault->looseness_cfg.threshold = 0.80f;  

    // 3. Establish highly realistic fractional asynchronous Bearing defect defaults
    fault->bearing_cfg.fault_frequency_multiplier = 3.58f; // Example: Ball Pass Frequency Outer (BPFO)
    fault->bearing_cfg.impact_amplitude            = 0.35f;
    fault->bearing_cfg.decay_rate                  = 6.5f;
    fault->bearing_cfg.current_impact_phase        = 0.0f;

    return FAULT_OK;
}

float fault_imbalance(const struct FaultGenerator *fault, float signal)
{
    return fault_imbalance_calculate(&fault->imbalance_cfg, signal);
}

float fault_looseness(const struct FaultGenerator *fault, float signal)
{
    return fault_looseness_calculate(&fault->looseness_cfg, signal);
}

float fault_bearing(struct FaultGenerator *fault, float signal, float current_phase)
{
    return fault_bearing_calculate(&fault->bearing_cfg, signal, current_phase);
}
