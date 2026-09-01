#ifndef DRIFT_H
#define DRIFT_H

struct DriftGenerator
{
    double sigma;
    double value;
    double dt;
};

typedef enum
{
    DRIFT_OK = 0,
    DRIFT_INVALID_PARAMETERS,
    DRIFT_INVALID_SIGMA_VALUE,

} DriftStatus;

DriftStatus drift_init(struct DriftGenerator *drift, const double sigma, struct VibrationGenerator *generator );
double generate_drift(struct DriftGenerator *drift);

#endif