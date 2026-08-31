#include <math.h>
#include <stdint.h>
#include <stddef.h>

#include "stm32f4xx.h"

#include "noise.h"
#include "vibration.h"
#include "common.h"

#define RNG_UINT32_MAX 4294967295.0
#define MEAN           0.0


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

    if (generator->amplitude <= 0.0)
        return NOISE_INVALID_AMPLITUDE;

    if (noise->snr_db < 0.0)
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
    double signal_rms = generator->amplitude / sqrt(2.0);

    /* Signal power. */
    double signal_power = signal_rms * signal_rms;

    /* Noise power from the requested SNR. */
    noise->noise_power =
        signal_power / pow(10.0, noise->snr_db / 10.0);

    /* For zero-mean Gaussian noise: variance = sigma^2 = noise power. */
    noise->sigma = sqrt(noise->noise_power);

    return NOISE_OK;
}


/* Initialize the STM32 hardware RNG. */
void RNG_Init(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
    RNG->CR |= RNG_CR_RNGEN;
}


/*
 * Generate a standard normal random variable using
 * the Box-Muller transform.
 *
 * Returns:
 *
 *     z ~ N(0, 1)
 */
static double box_muller(void)
{
    uint32_t r1;
    uint32_t r2;

    /* Get first random value. */
    while (!(RNG->SR & RNG_SR_DRDY))
        ;

    r1 = RNG->DR;

    /* Get second random value. */
    while (!(RNG->SR & RNG_SR_DRDY))
        ;

    r2 = RNG->DR;

    /*
     * Convert uint32_t values to uniform random variables
     * in the interval (0, 1).
     */
    double u1 =
        ((double)r1 + 1.0) /
        (RNG_UINT32_MAX + 2.0);

    double u2 =
        ((double)r2 + 1.0) /
        (RNG_UINT32_MAX + 2.0);

    /*
     * Box-Muller transform:
     *
     * z = sqrt(-2 ln(u1)) cos(2 pi u2)
     */
    return sqrt(-2.0 * log(u1)) *
           cos(2.0 * PI * u2);
}


/*
 * Generate one Gaussian noise sample.
 *
 *     n ~ N(0, sigma^2)
 */
double generate_noise(const struct NoiseGenerator *noise)
{
    if (noise == NULL)
        return 0.0;

    return MEAN + (noise->sigma * box_muller());
}
