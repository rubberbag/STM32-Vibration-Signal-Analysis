#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define PI 3.14159265358979323846f
#define ISO_GUARDBAND 2.56f
#define HARDWARE_CEILING_HARMONICS 64
#define HARDWARE_MOTOR_LIMIT_RPM 15000.0f


struct HarmonicProfile
{
    float amplitude[HARDWARE_CEILING_HARMONICS];
    uint8_t count;
};

typedef enum
{
    SIGNAL_VALID = 0,
    SIGNAL_INVALID_PARAMETER,
    SIGNAL_INVALID_SAMPLE_RATE,
    SIGNAL_INVALID_FREQUENCY,
    SIGNAL_NYQUIST_VIOLATION,
    SIGNAL_GUARDBAND_VIOLATION,
    SIGNAL_HARMONICS_CLAMPED
} SignalValidationStatus;


SignalValidationStatus validate_sample_rate(
    float sample_rate
);

SignalValidationStatus validate_frequency(
    float frequency,
    float sample_rate
);

SignalValidationStatus validate_harmonics(
    float frequency,
    float max_bandwidth_hz,
    float nyquist_hz,
    struct HarmonicProfile *harmonics
);


struct SignalConfig
{
    float sample_rate;
};

void RNG_Init(void);
float box_muller(void);

#endif