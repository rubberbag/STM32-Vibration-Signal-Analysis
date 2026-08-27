#include <math.h>

#include "vibration.h"
#include "disturbaces.h"
#include "faults.h"

#include "signal_sample.h"


SignalStatus signal_generator_init(
    struct SignalGenerator *signal)
{
    if (signal == NULL)
        return SIGNAL_STATUS_INVALID_ARGUMENT;

    if (signal->vibration_enabled)
    {
        VibrationStatus status =
            vibration_init(&signal->vibration);

        if (status != VIBRATION_OK)
            return SIGNAL_STATUS_VIBRATION_ERROR;
    }

    if (signal->disturbance_enabled)
    {
        DisturbanceStatus status =
            disturbance_init(&signal->disturbance);

        if (status != DISTURBANCE_OK)
            return SIGNAL_STATUS_DISTURBANCE_ERROR;
    }

    if (signal->fault_enabled)
    {
        FaultStatus status =
            fault_init(&signal->fault);

        if (status != FAULT_OK)
            return SIGNAL_STATUS_FAULT_ERROR;
    }

    return SIGNAL_STATUS_OK;
}

BufferStatus sample_generate(struct SignalGenerator *signal, int32_t *buffer, size_t size)
{

    if (signal == NULL|| buffer == NULL|| size == 0)
        return BUFFER_STATUS_INVALID_ARGUMENT;

    for (size_t count = 0; count < size; count++)
    {
        double value = 0.0;

        if(signal->vibration_enabled) 
            value += vibration(&signal->vibration);
        
        if(signal->fault_enabled) 
            value = fault(&signal->fault, value);

        if(signal->disturbance_enabled) 
            value += disturbance(&signal->disturbance);

        buffer[count] = (int32_t)lround(value * 100.0);
    }

    return BUFFER_STATUS_OK;
}