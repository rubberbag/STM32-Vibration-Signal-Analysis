#include <stdio.h>
#include "vibration.h"


int main(void){

    struct  VibrationConfig machine =
    {
        .rpm            = 1200,
        .amplitude      = 0.5,
        .sample_rate    = 2000,
        .harmonic_count = 4,
        .harmonics_enabled = true,
    };

    int16_t value;
    

    for( uint16_t i = 1; i <= 1000U; i++)
    {
        VibrationStatus status = vibration_fixed( &machine, i, &value);

        if (status != VIBRATION_OK)
            {
                return 0;
            }

        double newvalue = (double)value/100.00;
        
        printf("%.2f\n", newvalue);
    }

    return 0;
}