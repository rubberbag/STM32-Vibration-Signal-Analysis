#ifndef VIBRATION_H
#define VIBRATION_H

#include <stdint.h>
#include <stdbool.h>


/**
 * struct VibrationConfig - Configuration for vibration generation
 * @rpm: Machine rotational speed in revolutions per minute
 * @sample_rate: Sampling frequency in Hz
 * @amplitude: Fundamental vibration amplitude
 * @harmonic_count: Number of harmonics to generate
 * @harmonics_enabled: Enable or disable harmonic generation
 */
struct VibrationConfig
{
    double rpm;
    double sample_rate;
    double amplitude;
    uint8_t harmonic_count;
    bool harmonics_enabled;
};


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

/**
 * vibration_fixed - Generate a fixed-point vibration sample
 * @config: Vibration generator configuration
 * @n: Sample index
 * @fixed_point: Pointer to store the generated sample
 *
 * Generates one vibration sample and stores it as a fixed-point
 * value scaled by 100.
 *
 * Return: VIBRATION_OK on success, otherwise an error status
 */
VibrationStatus vibration_fixed(const struct VibrationConfig *config, uint16_t n, int16_t *fixed_point);

#endif