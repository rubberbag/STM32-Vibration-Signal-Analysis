#include <math.h>
#include <stddef.h>

#include "vibration.h"




static VibrationStatus vibration_validate(const struct VibrationConfig *config);
static double vibration(const struct VibrationConfig *config, uint16_t n);



VibrationStatus vibration_fixed(const struct VibrationConfig *config, uint16_t n, int16_t *fixed_point)
{
    if (config == NULL || fixed_point == NULL)
    return VIBRATION_INVALID_CONFIG;

    VibrationStatus status = vibration_validate(config);

    if(status != VIBRATION_OK)
        return status;


    double vibration_t = vibration(config, n);

    *fixed_point = (int16_t) lround(vibration_t * 100);

    return VIBRATION_OK;
}


static VibrationStatus vibration_validate(const struct VibrationConfig *config)
{
    
    if (config->rpm <= 0.0)
        return VIBRATION_INVALID_RPM;

    double frequency = config->rpm / 60.0;


    if (config->sample_rate <= 0.0)
        return VIBRATION_INVALID_SAMPLE_RATE;

    double nyquist =
        config->sample_rate / 2.0;

    if (frequency >= nyquist)
        return VIBRATION_NYQUIST_VIOLATION;


    if (config->harmonics_enabled)
    {
        if (config->harmonic_count == 0)
        {
            return VIBRATION_INVALID_HARMONIC_COUNT;
        }

        double max_frequency =
            frequency * config->harmonic_count;

        if (max_frequency >= nyquist)
            return VIBRATION_NYQUIST_VIOLATION;
    }

    return VIBRATION_OK;
}

static double vibration(const struct VibrationConfig *config, uint16_t n)
{

    double frequency = config->rpm/60.0;

    double time = (double)n / config->sample_rate;

    double vibration  = 0.00;
    

    if (config->harmonics_enabled)
    {
        for(int i = 1;  i <= config->harmonic_count; i++)
        {
            double harmonic_frequency = frequency* i;
            double amplitude          = config->amplitude/i;

            vibration += amplitude * sin(2.0 * M_PI * harmonic_frequency * time );
        }
    }
    else 
        vibration += config->amplitude * sin(2.0 * M_PI * frequency * time );

    return vibration;
}
