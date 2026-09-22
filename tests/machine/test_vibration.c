#include <stdio.h>
#include "vibration.h"
#include "oscillator.h"


static int test_valid_configuration(void)
{
    struct SignalConfig config =
    {
        .sample_rate = 1000.0f
    };

    struct VibrationGenerator generator =
    {
        .rpm = 1200.0f,
        .amplitude = 0.5f,

        .harmonics = 
        {
            .amplitude = {1.0f, -10.0f, 5.4f, 2.3f},
            .count = 4
        },

        .oscillator =
        {
            .phase = 0.0f,
        }
    };


    VibrationStatus status =
        vibration_init(&generator, &config);

    if (status != VIBRATION_OK)
    {
        printf("FAIL: valid configuration\n");
        return 1;
    }

    printf("PASS: valid configuration\n");
    return 0;
}

static int test_invalid_rpm(void)
{

    struct SignalConfig config =
    {
        .sample_rate = 1000.0f
    };

    struct VibrationGenerator generator =
    {
        .rpm = 0.0f,
        .amplitude = 0.5f,

        .harmonics = 
        {
            .amplitude = {1.0f, -10.0f, 5.4f, 2.3f},
            .count = 4
        },
        
        .oscillator =
        {
            .phase = 0.0f,
        }
    };


    VibrationStatus status =
        vibration_init(&generator, &config);

    if (status != VIBRATION_INVALID_RPM)
    {
        printf("FAIL: invalid RPM\n");
        return 1;
    }

    printf("PASS: invalid RPM\n");
    return 0;
}



static int test_invalid_sample_rate(void)
{


    struct SignalConfig config =
    {
        .sample_rate = 0.0f
    };

    struct VibrationGenerator generator =
    {
        .rpm = 1000.0f,
        .amplitude = 0.5f,

        .harmonics = 
        {
            .amplitude = {1.0f, -10.0f, 5.4f, 2.3f},
            .count = 4
        },
        
        .oscillator =
        {
            .phase = 0.0f,
        }
    };


    VibrationStatus status =
        vibration_init(&generator, &config);

    if (status != VIBRATION_INVALID_SAMPLE_RATE)
    {
        printf("FAIL: invalid sample rate\n");
        return 1;
    }

    printf("PASS: invalid sample rate\n");
    return 0;
}


static int test_nyquist_violation(void)
{
    struct SignalConfig config =
    {
        .sample_rate = 1000.0f
    };

    struct VibrationGenerator generator =
    {
        .rpm = 8000.0f,
        .amplitude = 0.5f,

        .harmonics = 
        {
            .amplitude = {1.0f, -10.0f, 5.4f, 2.3f},
            .count = 4
        },
        
        .oscillator =
        {
            .phase = 0.0f,
        }
    };


    VibrationStatus status =
        vibration_init(&generator, &config);

    if (status != VIBRATION_NYQUIST_VIOLATION)
    {
        printf("FAIL: Nyquist violation\n");
        return 1;
    }

    printf("PASS: Nyquist violation\n");
    return 0;
}


static int test_invalid_harmonic_count(void)
{
    struct SignalConfig config =
    {
        .sample_rate = 1200.0f
    };

    struct VibrationGenerator generator =
    {
        .rpm = 1000.0f,
        .amplitude = 0.5f,

        .harmonics = 
        {
            .amplitude = {1.0f, -10.0f, 5.4f, 2.3f},
            .count = MAX_HARMONICS+1
        },
        
        .oscillator =
        {
            .phase = 0.0f,
        }
    };

    VibrationStatus status =
        vibration_init(&generator, &config);

    if (status != VIBRATION_HARMONICS_CLAMPED)
    {
        printf("FAIL: %d \n", status);
        return 1;
    }

    printf("PASS: Harmonics Clamped\n");
    return 0;
}


static int test_harmonics_disabled(void)
{
    struct SignalConfig config =
    {
        .sample_rate = 1000.0f
    };

    struct VibrationGenerator generator =
    {
        .rpm = 1200.0f,
        .amplitude = 0.5f,

        .harmonics = 
        {
            .amplitude = {},
            .count = 0
        },
        
        .oscillator =
        {
            .phase = 0.0f,
        }
    };


    VibrationStatus status =
        vibration_init(&generator, &config);

    if (status != VIBRATION_OK)
    {
        printf("FAIL: harmonics disabled\n");
        return 1;
    }

    printf("PASS: harmonics disabled\n");
    return 0;
}


int main(void)
{
    if (test_valid_configuration() != 0)
        return 1;

    if (test_invalid_rpm() != 0)
        return 1;

    if (test_invalid_sample_rate() != 0)
        return 1;

    if (test_nyquist_violation() != 0)
        return 1;

    if (test_invalid_harmonic_count() != 0)
        return 1;

    if (test_harmonics_disabled() != 0)
        return 1;

    printf("ALL TESTS PASSED\n");

    return 0;
}
