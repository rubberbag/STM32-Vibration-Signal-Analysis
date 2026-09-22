#ifndef COMMON_H
#define COMMON_H


#define PI 3.14159265358979323846f
#define MAX_HARMONICS 16

typedef enum
{
    SIGNAL_VALID = 0,
    SIGNAL_INVALID_PARAMETER,
    SIGNAL_INVALID_SAMPLE_RATE,
    SIGNAL_INVALID_FREQUENCY,
    SIGNAL_NYQUIST_VIOLATION,
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
    float sample_rate,
    struct HarmonicProfile *harmonics
);

struct HarmonicProfile
{
    float amplitude[MAX_HARMONICS];
    uint8_t count;
};


struct SignalConfig
{
    float sample_rate;
};

void RNG_Init(void);
float box_muller(void);

#endif