#include <math.h>
#include <stdint.h>
#include <stddef.h>

#include "noise.h"
#include "vibration.h"
#include "common.h"


#define MEAN           0.0f


/*
 * Generate Gaussian noise:
 *
 *     n[n] = sigma * z[n]
 *
 * where:
 *
 *     z[n] ~ N(0, 1)
 *     n[n] ~ N(0, sigma^2)
 *
 * For a sinusoidal vibration with peak amplitude A:
 *
 *     signal_rms   = A / sqrt(2)
 *     signal_power = signal_rms^2
 *
 * Given:
 *
 *     SNR_dB = 10 * log10(Psignal / Pnoise)
 *
 * therefore:
 *
 *     Pnoise = Psignal / 10^(SNR_dB / 10)
 *
 * and:
 *
 *     sigma = sqrt(Pnoise)
 */


/* Validate noise-generator parameters. */
static NoiseStatus noise_validation(
    const struct VibrationGenerator *generator,
    const struct NoiseGenerator *noise)
{
    if (generator == NULL || noise == NULL)
        return NOISE_INVALID_PARAMETERS;

    if (generator->amplitude <= 0.0f)
        return NOISE_INVALID_AMPLITUDE;

    if (noise->snr_db < 0.0f)
        return NOISE_INVALID_SNR;

    return NOISE_OK;
}


/* Calculate noise power and Gaussian standard deviation. */
static NoiseStatus NoisePowerInit(
    struct VibrationGenerator *generator,
    struct NoiseGenerator *noise)
{
    NoiseStatus status = noise_validation(generator, noise);

    if (status != NOISE_OK)
        return status;

    /* Convert peak amplitude to RMS amplitude. */
    float signal_rms = generator->amplitude / sqrtf(2.0f);

    /* Signal power. */
    float signal_power = signal_rms * signal_rms;

    /* Noise power from the requested SNR. */
    noise->noise_power =
        signal_power / powf(10.0f, noise->snr_db / 10.0f);

    /* For zero-mean Gaussian noise: variance = sigma^2 = noise power. */
    noise->sigma = sqrtf(noise->noise_power);

    return NOISE_OK;
}


/*
 * Generate one Gaussian noise sample.
 *
 *     n ~ N(0, sigma^2)
 */
float generate_noise(const struct NoiseGenerator *noise)
{
    if (noise == NULL)
        return 0.0f;

    return MEAN + (noise->sigma * box_muller());
}
