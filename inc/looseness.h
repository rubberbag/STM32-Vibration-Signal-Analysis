#ifndef LOOSENESS_H
#define LOOSENESS_H

struct LoosenessConfig {
    float threshold; // Physical structural clipping constraint clearance limit
};

float fault_looseness_calculate(const struct LoosenessConfig *cfg, float base_signal);

#endif