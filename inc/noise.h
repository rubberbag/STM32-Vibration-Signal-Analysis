#ifndef NOISE_H
#define NOISE_H

#include "vibration.h"
#include "common.h"

struct NoiseGenerator
{
    float snr_db;
    float noise_power;
    float sigma;
};

typedef enum
{
    NOISE_OK = 0,
    NOISE_INVALID_PARAMETERS,
    NOISE_INVALID_SNR,
    NOISE_INVALID_AMPLITUDE,

} NoiseStatus;



NoiseStatus NoisePowerInit(
    struct VibrationGenerator *generator,
    struct NoiseGenerator *noise);

float generate_noise(const struct NoiseGenerator *noise);

#endif