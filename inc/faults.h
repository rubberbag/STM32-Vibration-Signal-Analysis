#ifndef FAULTS_H
#define FAULTS_H

#include <stdint.h>
#include "imbalance.h"
#include "looseness.h"
#include "bearing.h"

typedef enum {
    FAULT_OK = 0,
    FAULT_INVALID_PARAM
} FaultStatus;

typedef enum {
    FAULT_MODE_NONE = 0,
    FAULT_MODE_IMBALANCE,
    FAULT_MODE_LOOSENESS,
    FAULT_MODE_BEARING,
    FAULT_MODE_ALL
} ActiveFaultMode;


struct FaultGenerator {
    struct ImbalanceConfig imbalance_cfg;
    struct LoosenessConfig looseness_cfg;
    struct BearingConfig   bearing_cfg;
};

FaultStatus fault_init(struct FaultGenerator *fault);

// Clean mathematical routing wrappers called by sample_execute_tick
float fault_imbalance(const struct FaultGenerator *fault, float signal);
float fault_looseness(const struct FaultGenerator *fault, float signal);
float fault_bearing(struct FaultGenerator *fault, float signal, float current_phase);

#endif
