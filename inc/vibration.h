#ifndef VIBRATION_H
#define VIBRATION_H

#include <stdint.h>
#include <stdbool.h>
#include "oscillator.h"


/**
 * enum VibrationStatus - Vibration generator status codes
 * @VIBRATION_OK: Operation completed successfully
 * @VIBRATION_INVALID_SAMPLE_RATE: Invalid sample rate
 * @VIBRATION_NYQUIST_VIOLATION: Signal exceeds Nyquist frequency
 * @VIBRATION_INVALID_RPM: Invalid RPM
 * @VIBRATION_INVALID_HARMONIC_COUNT: Invalid harmonic count
 * @VIBRATION_INVALID_CONFIG: Invalid configuration or pointer
 */
typedef enum
{
    VIBRATION_OK = 0,
    VIBRATION_INVALID_SAMPLE_RATE,
    VIBRATION_NYQUIST_VIOLATION,
    VIBRATION_INVALID_RPM,
    VIBRATION_INVALID_HARMONIC_COUNT,
    VIBRATION_INVALID_CONFIG,
} VibrationStatus;



struct VibrationGenerator
{
    double rpm;
    double amplitude;

    uint8_t harmonic_count;
    bool harmonics_enabled;

    struct Oscillator oscillator;
};


VibrationStatus vibration_init(struct VibrationGenerator *generator, const struct SignalConfig *config);

double vibration(struct VibrationGenerator *generator);

#endif