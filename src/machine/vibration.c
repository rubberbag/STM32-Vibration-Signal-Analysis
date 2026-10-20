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


    SignalValidationStatus status = validate_sample_rate(config->sample_rate);

    if (status != SIGNAL_VALID)
        return VIBRATION_INVALID_SAMPLE_RATE;

    
    float nyquist_hz = config->sample_rate / 2.0f;
    float max_bandwidth_hz = config->sample_rate/ISO_GUARDBAND;

    float max_bandwidth_rpm = max_bandwidth_hz * 60.0f;

    float max_allowable_rpm = (max_bandwidth_rpm < HARDWARE_MOTOR_LIMIT_RPM) ? max_bandwidth_rpm : HARDWARE_MOTOR_LIMIT_RPM;

    if (generator->rpm <= 0.0f || generator->rpm > max_allowable_rpm)
        return VIBRATION_INVALID_RPM;

    float frequency = generator->rpm / 60.0f;

    /* Fundamental frequency must not exceed Nyquist. */
    if (frequency > nyquist_hz) return VIBRATION_NYQUIST_VIOLATION;


    status = validate_frequency(frequency, max_bandwidth_hz);
    if (status == SIGNAL_INVALID_FREQUENCY) return VIBRATION_INVALID_RPM;
    if (status == SIGNAL_NYQUIST_VIOLATION)return VIBRATION_NYQUIST_VIOLATION;


    status = validate_harmonics(frequency, max_bandwidth_hz, nyquist_hz, &generator->harmonics);
    if (status == SIGNAL_NYQUIST_VIOLATION)
    return VIBRATION_NYQUIST_VIOLATION;

    if (status == SIGNAL_GUARDBAND_VIOLATION) return  VIBRATION_GUARDBAND_VIOLATION;

    if (status == SIGNAL_INVALID_PARAMETER) return VIBRATION_INVALID_CONFIG;

    if (status == SIGNAL_HARMONICS_CLAMPED) return VIBRATION_HARMONICS_CLAMPED;

    if (status != SIGNAL_VALID) return VIBRATION_INVALID_CONFIG;

    return VIBRATION_OK;
}


float vibration(struct VibrationGenerator *generator)
{

    if (generator==NULL) return 0.0f;

    float value  = 0.0f;
    
    uint8_t count = generator->harmonics.count;
    
    if (count > HARDWARE_CEILING_HARMONICS) return 0.0f;

    if (count > 0)
    {
        for (int i = 0; i < generator->harmonics.count; i++)
        {
            value += generator->harmonics.amplitude[i] *
                     sinf(generator->oscillator.phase * (i + 1));
        }
    }
    else 
        value += generator->amplitude * sinf(generator->oscillator.phase);

    oscillator_next(&generator->oscillator);

    return value;
}
