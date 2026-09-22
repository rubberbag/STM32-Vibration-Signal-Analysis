#ifndef OSCILLATOR_H
#define OSCILLATOR_H

struct Oscillator
{
    float phase;
    float phase_step;
};

typedef enum 
{
    OSCILLATOR_OK = 0,
    OSCILLATOR_INVALID_PARAMETERS,
}OscillatorStatus;

OscillatorStatus oscillator_init(struct Oscillator *oscillator, float frequency, float sample_rate);

OscillatorStatus oscillator_next(struct Oscillator *oscillator);

#endif