#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "vibration.h"
#include "disturbaces.h"
#include "faults.h"

typedef enum
{
    SIGNAL_STATUS_OK = 0,
    SIGNAL_STATUS_INVALID_ARGUMENT,
    SIGNAL_STATUS_VIBRATION_ERROR,
    SIGNAL_STATUS_DISTURBANCE_ERROR,
    SIGNAL_STATUS_FAULT_ERROR,
    
} SignalStatus;


typedef enum
{
    BUFFER_STATUS_OK = 0,
    BUFFER_STATUS_INVALID_ARGUMENT,
    
} BufferStatus;



struct SignalGenerator
{
    struct VibrationGenerator vibration;
    struct FaultGenerator fault;
    struct DisturbanceGenerator disturbance;

    bool vibration_enabled;
    bool disturbance_enabled;
    bool fault_enabled;
};

BufferStatus sample_generate(struct SignalGenerator *signal, int32_t *buffer, size_t size);

#endif