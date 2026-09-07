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

   if (status != ELECTRICAL_OK && status != ELECTRICAL_HARMONICS_CLAMPED)
    {
        return status;
    }

    oscillator_init(&generator->oscillator, generator->frequency, config->sample_rate);

    return ELECTRICAL_OK;
}



static ElectricStatus electric_interference_validate(
    struct ElectricGenerator *generator,
    const struct SignalConfig *config)
{
    if (generator == NULL || config == NULL)
        return ELECTRICAL_INVALID_CONFIG;

    if (generator->frequency <= 0.0)
        return ELECTRICAL_INVALID_FREQUENCY;

    SignalValidationStatus status;

    status = validate_sample_rate(config->sample_rate);

    if (status != SIGNAL_VALID)
        return ELECTRICAL_INVALID_SAMPLE_RATE;

    status = validate_frequency(
        generator->frequency,
        config->sample_rate
    );

    if (status == SIGNAL_NYQUIST_VIOLATION)
        return ELECTRICAL_NYQUIST_VIOLATION;

    status = validate_harmonics(
        generator->frequency,
        config->sample_rate,
        &generator->harmonics
    );

    if (status == SIGNAL_HARMONICS_CLAMPED)
        return ELECTRICAL_HARMONICS_CLAMPED;

    if (status == SIGNAL_NYQUIST_VIOLATION)
        return ELECTRICAL_NYQUIST_VIOLATION;

    return ELECTRICAL_OK;
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

