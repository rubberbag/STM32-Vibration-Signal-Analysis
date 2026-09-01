#include <stddef.h>
#include <math.h>

#include "common.h"
#include "drift.h"

/**
 * drift_init - initialise a drift structure
 * @drift:  pointer to the drift  structure
 * @sigma: standard deviation of the drift increment
 * 
 * Return: DRIFT_OK on success, DRIFT_INAVLID_PARAMETERS if drift is NULL or sigmal is not finite *         or positive 
*/
DriftStatus drift_init(struct DriftGenerator *drift, const double sigma )
{
    if(drift == NULL || !isfinite(sigma) || sigma <= 0.0) return DRIFT_INVALID_PARAMETERS;

    drift->sigma = sigma;
    drift->value = 0.0;

    return DRIFT_OK;

}


/**
 * generate_drift - generate drift value
 * @drift:  pointer to the drift  structure
 * 
 * Random-walk model:
 *
 *     d[n] = d[n-1] + (sigma * z[n])
 *
 * where:
 *
 *     d[n]
 *          accumulated drift at sample n.
 * 
 *     z[n] ~ N(0, 1)
 *          standard Gaussian random varibale from Box-muller function.
 * 
 *     sigma
 *          standard deviation of dift increment.
 *
 * Decription: Sensor drift is modeled as a discrete random walk driven by zero-mean gaussian
 *             increments.The drift parameter "sigma" controls the standard deviation of the
 *             chnage in the sensor bias per  smaple
 * 
*/
double generate_drift(struct DriftGenerator *drift)
{
    if (drift == NULL)
        return 0.0;

    return drift->value += drift->sigma * box_muller();
}