#ifndef COMMON_H
#define COMMON_H


#define PI 3.14159265358979323846

struct SignalConfig
{
    double sample_rate;
};


void RNG_Init(void);
double box_muller(void);

#endif