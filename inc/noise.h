#ifndef NOISE_H
#define NOISE_H

struct NoiseGenerator
{
    double snr_db;
    double noise_power;
    double sigma;
};

typedef enum
{
    NOISE_OK = 0,
    NOISE_INVALID_PARAMETERS,
    NOISE_INVALID_SNR,
    NOISE_INVALID_AMPLITUDE,

} NoiseStatus;

void RNG_Init(void);
double generate_noise(const struct NoiseGenerator *noise);

#endif