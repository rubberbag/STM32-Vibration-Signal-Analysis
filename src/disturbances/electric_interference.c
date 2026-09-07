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
static ElectricStatus electric_interference_validate(const struct ElectricGenerator *generator, const struct SignalConfig *config);



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



static ElectricStatus electric_interference_validate(const struct ElectricGenerator *generator, const struct SignalConfig *config)
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


    if (generator->harmonics_enabled)
    {
        if (generator->harmonic_count == 0)
        {
            return ELECTRICAL_INVALID_HARMONIC_COUNT;
        }

        double max_frequency =
            generator->frequency * generator->harmonic_count;

        if (max_frequency >= nyquist)
            return ELECTRICAL_NYQUIST_VIOLATION;
    }

    return ELECTRICAL_OK;
}


double electric_interference(struct ElectricGenerator *generator)
{

    double value  = 0.00;
    

    if (generator->harmonics_enabled)
    {
        for(int i = 1;  i <= generator->harmonic_count; i++)
        {
            double amplitude = generator->amplitude/i;

            value += amplitude * sin(generator->oscillator.phase * i );
        }
    }
    else 
        value += generator->amplitude * sin(generator->oscillator.phase);

    oscillator_next(&generator->oscillator);

    return value;
}
