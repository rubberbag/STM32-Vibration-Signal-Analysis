#ifndef COMMON_H
#define COMMON_H


#define PI 3.14159265358979323846
#define MAX_HARMONICS 16

struct SignalConfig
{
    double sample_rate;
};

struct HarmonicProfile
{
    double amplitude[MAX_HARMONICS];
    uint8_t count;
};


void RNG_Init(void);
double box_muller(void);

#endif