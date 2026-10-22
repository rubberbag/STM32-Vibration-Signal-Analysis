#ifndef IMBALANCE_H
#define IMBALANCE_H

struct ImbalanceConfig {
    float severity; // Mass eccentricity amplitude multiplier 
};

float fault_imbalance_calculate(const struct ImbalanceConfig *cfg, float base_signal);

#endif
