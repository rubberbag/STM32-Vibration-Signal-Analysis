#ifndef DRIFT_H
#define DRIFT_H

struct DriftGenerator
{
    float sigma;
    float value;
    float dt;
};

typedef enum
{
    DRIFT_OK = 0,
    DRIFT_INVALID_PARAMETERS,
    DRIFT_INVALID_SIGMA_VALUE,

} DriftStatus;

DriftStatus drift_init(struct DriftGenerator *drift, const float sigma, struct SignalConfig *config);
float generate_drift(struct DriftGenerator *drift);

#endif