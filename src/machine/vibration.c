#include <math.h>
#include <stddef.h>

#include "vibration.h"



/**
 * vibration_validate - Validate vibration generatoruration
 * @generator: Vibration generator generatoruration
 *
 * Return: VIBRATION_OK if valid, otherwise the appropriate error status
 */
static VibrationStatus vibration_validate(struct VibrationGenerator *generator, const struct SignalConfig *config);



VibrationStatus vibration_init(
    struct VibrationGenerator *generator, const struct SignalConfig *config)
{
    if (generator == NULL || config == NULL)
        return VIBRATION_INVALID_CONFIG;

    VibrationStatus status = vibration_validate(generator, config);

    if (status != VIBRATION_OK && status != VIBRATION_HARMONICS_CLAMPED)
    {
        return status;
    }

    float frequency = generator->rpm / 60.0f;

    oscillator_init(&generator->oscillator, frequency, config->sample_rate);

    return status;
}



static VibrationStatus vibration_validate(
    struct VibrationGenerator *generator,
    const struct SignalConfig *config)
{
    if (generator == NULL || config == NULL)
        return VIBRATION_INVALID_CONFIG;

    if (generator->rpm <= 0.0f)
        return VIBRATION_INVALID_RPM;

    SignalValidationStatus status;

    status = validate_sample_rate(config->sample_rate);

    if (status != SIGNAL_VALID)
        return VIBRATION_INVALID_SAMPLE_RATE;

    float frequency = generator->rpm / 60.0f;

    status = validate_frequency(
        frequency,
        config->sample_rate
    );

    if (status == SIGNAL_INVALID_FREQUENCY)
        return VIBRATION_INVALID_RPM;

    if (status == SIGNAL_NYQUIST_VIOLATION)
        return VIBRATION_NYQUIST_VIOLATION;

    status = validate_harmonics(
        frequency,
        config->sample_rate,
        &generator->harmonics
    );

    if (status == SIGNAL_HARMONICS_CLAMPED)
        return VIBRATION_HARMONICS_CLAMPED;

    if (status == SIGNAL_NYQUIST_VIOLATION)
        return VIBRATION_NYQUIST_VIOLATION;

    return VIBRATION_OK;
}


float vibration(struct VibrationGenerator *generator)
{

    float value  = 0.0f;
    

    if (generator->harmonics.count)
    {
        for (int i = 0; i < generator->harmonics.count; i++)
        {
            value += generator->harmonics.amplitude[i] *
                     sin(generator->oscillator.phase * (i + 1));
        }
    }
    else 
        value += generator->amplitude * sin(generator->oscillator.phase);

    oscillator_next(&generator->oscillator);

    return value;
}
