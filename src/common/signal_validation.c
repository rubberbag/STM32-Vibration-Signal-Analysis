#include "common.h"
#include <stddef.h>
#include <math.h>

/*
 * Validate the raw sample rate entry.
 */

SignalValidationStatus validate_sample_rate(float sample_rate)
{
    if (!isfinite(sample_rate) || sample_rate <= 0.0f)
        return SIGNAL_INVALID_SAMPLE_RATE;

    return SIGNAL_VALID;
}

/*
 * validate a standlone target frequency interest boundary
 */
SignalValidationStatus validate_frequency(
    float frequency,
    float max_bandwidth_hz)
{
    if (frequency <= 0.0f)
        return SIGNAL_INVALID_FREQUENCY;

    if (frequency > max_bandwidth_hz)
        return SIGNAL_NYQUIST_VIOLATION;

    return SIGNAL_VALID;
}


SignalValidationStatus validate_harmonics(
    float frequency,
    float max_bandwidth_hz,
    float nyquist_hz,
    struct HarmonicProfile *harmonics)
{
    if (harmonics == NULL) return SIGNAL_INVALID_PARAMETER;

    if (!isfinite(frequency) ||
        !isfinite(max_bandwidth_hz) ||
        !isfinite(nyquist_hz) ||
        frequency <= 0.0f ||
        max_bandwidth_hz <= 0.0f ||
        nyquist_hz <= 0.0f) 
        
        return SIGNAL_INVALID_PARAMETER;


    if (harmonics->count > 0)
    {
        float requested_max_frequency =
            frequency * (float)harmonics->count;

        if (requested_max_frequency > nyquist_hz)
            return SIGNAL_NYQUIST_VIOLATION;
    }

    
    uint32_t physical_max_harmonics = (uint32_t)(max_bandwidth_hz/frequency);
    uint32_t safe_hardware_ceiling = physical_max_harmonics;

    if(safe_hardware_ceiling > HARDWARE_CEILING_HARMONICS) 
        safe_hardware_ceiling = HARDWARE_CEILING_HARMONICS;

    /*
     * Clamp excessive harmonic count.
     */
    SignalValidationStatus status = SIGNAL_VALID;

    if (harmonics->count > safe_hardware_ceiling)
    {
        harmonics->count = safe_hardware_ceiling;
        status = SIGNAL_HARMONICS_CLAMPED;
    }

    /*
     * No harmonics to validate.
     */
    if (harmonics->count > 0)
    {
        /*
         * Highest harmonic frequency:
         *
         * f_max = fundamental × harmonic number
        */
        float max_frequency = frequency * (float)harmonics->count;
        
        if (max_frequency > nyquist_hz) return SIGNAL_NYQUIST_VIOLATION;
        if (max_frequency > max_bandwidth_hz) return SIGNAL_GUARDBAND_VIOLATION;
    }
       
    return status;
}
