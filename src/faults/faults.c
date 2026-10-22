#include <stddef.h>
#include "faults.h"

FaultStatus fault_init(struct FaultGenerator *fault)
{
    if (fault == NULL) return FAULT_INVALID_PARAM;

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
