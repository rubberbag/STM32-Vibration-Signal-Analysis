#ifndef VIBRATION_H
#define VIBRATRION_H

#include <stdint.h>
#include <stdbool.h>

#define HARMONICS_ENABLED 1U

struct VibrationConfig
{
    double rpm;
    double sample_rate;
    double amplitude;
    uint8_t harmonic_count;
    bool harmonics_enabled;
};

typedef enum
{
    VIBRATION_OK = 0,
    VIBRATION_INVALID_SAMPLE_RATE,
    VIBRATION_NYQUIST_VIOLATION,
    VIBRATION_INVALID_RPM,
    VIBRATION_INVALID_HARMONIC_COUNT,
    VIBRATION_INVALID_CONFIG,
} VibrationStatus;

VibrationStatus vibration_fixed(const struct VibrationConfig *config, uint16_t n, int16_t *output);

#endif