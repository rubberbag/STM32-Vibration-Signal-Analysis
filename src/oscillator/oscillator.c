#include <stddef.h>

#include "oscillator.h"
#include "common.h"


OscillatorStatus oscillator_init(struct Oscillator *oscillator, float frequency, float sample_rate)
{
    if( oscillator == NULL || frequency <= 0.0f || sample_rate <=0.0f)
        return OSCILLATOR_INVALID_PARAMETERS;

    oscillator->phase = 0.0f;
    oscillator->phase_step =
        2.0f * PI *
        frequency / sample_rate;

    return OSCILLATOR_OK;
};


OscillatorStatus oscillator_next(struct Oscillator *osc)
{
    if( osc == NULL)
        return OSCILLATOR_INVALID_PARAMETERS;


    osc->phase += osc->phase_step;

    if(osc->phase >= 2.0f * PI)
        osc->phase -=2.0f * PI;
    
    return OSCILLATOR_OK;    
};
