#include "common.h"
#include <stddef.h>


SignalValidationStatus validate_sample_rate(float sample_rate)
{
    if (sample_rate <= 0.0f)
        return SIGNAL_INVALID_SAMPLE_RATE;

    return SIGNAL_VALID;
}


SignalValidationStatus validate_frequency(
    float frequency,
    float sample_rate)
{
    if (frequency <= 0.0f)
        return SIGNAL_INVALID_FREQUENCY;

    float nyquist = sample_rate / 2.0f;

    if (frequency >= nyquist)
        return SIGNAL_NYQUIST_VIOLATION;

    return SIGNAL_VALID;
}


SignalValidationStatus validate_harmonics(
    float frequency,
    float sample_rate,
    struct HarmonicProfile *harmonics)
{
    if (harmonics == NULL)
        return SIGNAL_INVALID_PARAMETER;

    SignalValidationStatus status = SIGNAL_VALID;

    /*
     * Don't block the caller.
     * Clamp excessive harmonic count.
     */
    if (harmonics->count > MAX_HARMONICS)
    {
        harmonics->count = MAX_HARMONICS;
        status = SIGNAL_HARMONICS_CLAMPED;
    }

    /*
     * No harmonics to validate.
     */
    if (harmonics->count == 0)
        return status;

    /*
     * Highest harmonic frequency:
     *
     * f_max = fundamental × harmonic number
     */
    float nyquist = sample_rate / 2.0f;

    float max_frequency =
        frequency * harmonics->count;

    if (max_frequency >= nyquist)
        return SIGNAL_NYQUIST_VIOLATION;

    return status;
}
