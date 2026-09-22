#ifndef ELECTRIC_INTERFERENCE_H
#define ELECTRIC_INTERFERENCE_H


#include <stdint.h>

#include "oscillator.h"


typedef enum
{
    ELECTRICAL_OK = 0,
    ELECTRICAL_INVALID_PARAMETER,
    ELECTRICAL_INVALID_SAMPLE_RATE,
    ELECTRICAL_INVALID_FREQUENCY,
    ELECTRICAL_NYQUIST_VIOLATION,
    ELECTRICAL_INVALID_HARMONIC_COUNT,
    ELECTRICAL_HARMONICS_CLAMPED,
    ELECTRICAL_INVALID_CONFIG,
} ElectricStatus;



struct ElectricGenerator
{
    float amplitude;
    float frequency;

    struct HarmonicProfile harmonics;
    struct Oscillator oscillator;
};


ElectricStatus electric_interference_init(struct ElectricGenerator *generator, const struct SignalConfig *config);

float electric_interference(struct ElectricGenerator *generator);

#endif