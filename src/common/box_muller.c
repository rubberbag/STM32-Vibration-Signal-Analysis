#include <stdint.h>

#include "stm32f4xx.h"
#include "common.h"

#define RNG_UINT32_MAX 4294967295.0

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
double box_muller(void)
{
    uint32_t r1;
    uint32_t r2;

    /* Get first random value. */
    while (!(RNG->SR & RNG_SR_DRDY))
        ;

    r1 = RNG->DR;

    /* Get second random value. */
    while (!(RNG->SR & RNG_SR_DRDY));

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