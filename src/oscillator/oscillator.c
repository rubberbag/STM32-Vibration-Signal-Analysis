#include <stddef.h>

#include "oscillator.h"
#include "common.h"


OscillatorStatus oscillator_init(struct Oscillator *oscillator, double frequency, double sample_rate)
{
    if( oscillator == NULL || frequency <= 0 || sample_rate <=0)
        return OSCILLATOR_INVALID_PARAMETERS;

    oscillator->phase = 0.0;
    oscillator->phase_step =
        2.0 * PI *
        frequency / sample_rate;

    return OSCILLATOR_OK;
};


OscillatorStatus oscillator_next(struct Oscillator *osc)
{
    if( osc == NULL)
        return OSCILLATOR_INVALID_PARAMETERS;


    osc->phase += osc->phase_step;

    if(osc->phase >= 2.0 * PI)
        osc->phase -=2.0 * PI;
    
    return OSCILLATOR_OK;    
};
