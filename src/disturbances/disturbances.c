#include "disturbances.h"

#include "electric_interference.h"
#include "noise.h"
#include "drift.h"

DisturbanceStatus disturbance_init(struct DisturbanceGenerator *generator)
{
    if (generator == NULL) {
        return DISTURBANCE_INVALID_ARGUMENT;
    }

    return DISTURBANCE_OK;
}

float disturbance_generate(struct DisturbanceGenerator *generator)
{
    if (generator == NULL) {
        return 0.0f;
    }

    float combined_disturbance_value = 0.0f;

    // Process and add Electrical Line Interference if enabled
    if (generator->electrical_enabled) {
        combined_disturbance_value += electric_interference(&generator->electric);
    }

    // Process and add Gaussian White Noise if enabled
    if (generator->noise_enabled) {
        combined_disturbance_value += generate_noise(&generator->noise);
    }

    // Process and add Sensor Random-Walk Drift if enabled
    if (generator->drift_enabled) {
        combined_disturbance_value += generate_drift(&generator->drift);
    }

    // Return the combined additive total back to the master pipeline caller
    return combined_disturbance_value;
}
