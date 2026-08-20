#ifndef DISTURBANCE_H
#define DISTURBANCE_H

#include <stdbool.h>

typedef enum
{
    DISTURBANCE_OK = 0,
    DISTURBANCE_INVALID_ARGUMENT,
} DisturbanceStatus;

struct DisturbanceGenerator
{
    struct NoiseGenerator noise;
    struct DriftGenerator drift;
    struct ElectricalGenerator electrical;

    bool noise_enabled;
    bool drift_enabled;
    bool electrical_enabled;
};

DisturbanceStatus disturbance_init(
    struct DisturbanceGenerator *generator);

double disturbance_generate(
    struct DisturbanceGenerator *generator);

#endif