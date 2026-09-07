#ifndef COMMON_H
#define COMMON_H


#define PI 3.14159265358979323846
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
    double sample_rate
);

SignalValidationStatus validate_frequency(
    double frequency,
    double sample_rate
);

SignalValidationStatus validate_harmonics(
    double frequency,
    double sample_rate,
    struct HarmonicProfile *harmonics
);

struct HarmonicProfile
{
    double amplitude[MAX_HARMONICS];
    uint8_t count;
};


struct SignalConfig
{
    double sample_rate;
};

void RNG_Init(void);
double box_muller(void);

#endif