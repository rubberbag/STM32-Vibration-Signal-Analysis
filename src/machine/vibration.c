#include <math.h>
#include <stddef.h>

#include "common.h"
#include "vibration.h"



/**
 * vibration_validate - Validate vibration generatoruration
 * @generator: Vibration generator generatoruration
 *
 * Return: VIBRATION_OK if valid, otherwise the appropriate error status
 */
static VibrationStatus vibration_validate(const struct VibrationGenerator *generator, const struct SignalConfig *config);



VibrationStatus vibration_init(
    struct VibrationGenerator *generator, const struct SignalConfig *config)
{
    if (generator == NULL || config == NULL)
        return VIBRATION_INVALID_CONFIG;

    VibrationStatus status = vibration_validate(generator, config);

    if (status != VIBRATION_OK)
        return status;

    double frequency = generator->rpm / 60.0;

    oscillator_init(&generator->oscillator, frequency, config->sample_rate);

    return VIBRATION_OK;
}



static VibrationStatus vibration_validate(const struct VibrationGenerator *generator, const struct SignalConfig *config)
{
    
    if (generator->rpm <= 0.0)
        return VIBRATION_INVALID_RPM;

    double frequency = generator->rpm / 60.0;


    if (config->sample_rate <= 0.0)
        return VIBRATION_INVALID_SAMPLE_RATE;

    double nyquist =
        config->sample_rate / 2.0;

    if (frequency >= nyquist)
        return VIBRATION_NYQUIST_VIOLATION;


    if (generator->harmonics_enabled)
    {
        if (generator->harmonic_count == 0)
        {
            return VIBRATION_INVALID_HARMONIC_COUNT;
        }

        double max_frequency =
            frequency * generator->harmonic_count;

        if (max_frequency >= nyquist)
            return VIBRATION_NYQUIST_VIOLATION;
    }

    return VIBRATION_OK;
}


double vibration(struct VibrationGenerator *generator)
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
