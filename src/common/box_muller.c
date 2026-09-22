#include <stdint.h>
#include <math.h>

#include "stm32f4xx.h"
#include "common.h"


#define RNG_UINT32_MAX 4294967295

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
float box_muller(void)
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
    float u1 =
        ((float)r1 + 1.0f) /
        (RNG_UINT32_MAX + 2.0f);

    float u2 =
        ((float)r2 + 1.0f) /
        (RNG_UINT32_MAX + 2.0f);

    /*
     * Box-Muller transform:
     *
     * z = sqrt(-2 ln(u1)) cos(2 pi u2)
     */
    return sqrtf(-2.0f * logf(u1)) *
           cosf(2.0f * PI * u2);
}