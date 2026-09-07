#include <math.h>
#include <stddef.h>

#include "common.h"
#include "oscillator.h"
#include "electric_interference.h"



/**
 * electric_interference_validate - Validate electric generatoruration
 * @generator: electric generator struture
 * @config: signal configuration
 * Return: electric_OK if valid, otherwise the appropriate error status
 */
static ElectricStatus electric_interference_validate(struct ElectricGenerator *generator, const struct SignalConfig *config);



ElectricStatus electric_interference_init(
    struct ElectricGenerator *generator, const struct SignalConfig *config)
{
    if (generator == NULL || config == NULL)
        return ELECTRICAL_INVALID_CONFIG;

    ElectricStatus status = electric_interference_validate(generator, config);

    if (status != ELECTRICAL_OK)
        return status;

    oscillator_init(&generator->oscillator, generator->frequency, config->sample_rate);

    return ELECTRICAL_OK;
}



static ElectricStatus electric_interference_validate(struct ElectricGenerator *generator, const struct SignalConfig *config)
{
    
    if (generator == NULL || config == NULL)
        return ELECTRICAL_INVALID_PARAMETER;


    if (config->sample_rate <= 0.0)
        return ELECTRICAL_INVALID_SAMPLE_RATE;

    double nyquist =
        config->sample_rate / 2.0;



    if (generator->frequency <= 0.0)
        return ELECTRICAL_INVALID_FREQUENCY;


    if (generator->frequency >= nyquist)
        return ELECTRICAL_NYQUIST_VIOLATION;


    ElectricStatus status = ELECTRICAL_OK;

    if (generator->harmonics.count > MAX_HARMONICS)
    {
        generator->harmonics.count = MAX_HARMONICS;

        /*
         * Harmonic count exceeded the supported maximum.
         * Configuration was clamped to MAX_HARMONICS.
         */
        status = ELECTRICAL_HARMONICS_CLAMPED;
    }

    if (generator->harmonics.count > 0)
    {
        double max_frequency =
            generator->frequency * generator->harmonics.count;

        if (max_frequency >= nyquist)
            return ELECTRICAL_NYQUIST_VIOLATION;
    }


    return status;
}


double electric_interference(struct ElectricGenerator *generator)
{
    double value = 0.0;

    if (generator->harmonics.count > 0)
    {
        for (int i = 0; i < generator->harmonics.count; i++)
        {
            value += generator->harmonics.amplitude[i] *
                     sin(generator->oscillator.phase * (i + 1));
        }
    }
    else
    {
        value = generator->amplitude *
                sin(generator->oscillator.phase);
    }

    oscillator_next(&generator->oscillator);

    return value;
}

