#include <stdio.h>
#include "vibration.h"


static int test_valid_configuration(void)
{
    struct VibrationConfig config =
    {
        .rpm = 1200,
        .amplitude = 0.5,
        .sample_rate = 2000,
        .harmonic_count = 4,
        .harmonics_enabled = true,
    };

    int16_t value;

    VibrationStatus status =
        vibration_fixed(&config, 1, &value);

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
    struct VibrationConfig config =
    {
        .rpm = 0,
        .amplitude = 0.5,
        .sample_rate = 2000,
        .harmonic_count = 4,
        .harmonics_enabled = true,
    };

    int16_t value;

    VibrationStatus status =
        vibration_fixed(&config, 1, &value);

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
    struct VibrationConfig config =
    {
        .rpm = 1200,
        .amplitude = 0.5,
        .sample_rate = 0,
        .harmonic_count = 4,
        .harmonics_enabled = true,
    };

    int16_t value;

    VibrationStatus status =
        vibration_fixed(&config, 1, &value);

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
    struct VibrationConfig config =
    {
        .rpm = 1200,
        .amplitude = 0.5,
        .sample_rate = 100,
        .harmonic_count = 4,
        .harmonics_enabled = true,
    };

    int16_t value;

    VibrationStatus status =
        vibration_fixed(&config, 1, &value);

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
    struct VibrationConfig config =
    {
        .rpm = 1200,
        .amplitude = 0.5,
        .sample_rate = 2000,
        .harmonic_count = 0,
        .harmonics_enabled = true,
    };

    int16_t value;

    VibrationStatus status =
        vibration_fixed(&config, 1, &value);

    if (status != VIBRATION_INVALID_HARMONIC_COUNT)
    {
        printf("FAIL: invalid harmonic count\n");
        return 1;
    }

    printf("PASS: invalid harmonic count\n");
    return 0;
}


static int test_harmonics_disabled(void)
{
    struct VibrationConfig config =
    {
        .rpm = 1200,
        .amplitude = 0.5,
        .sample_rate = 2000,
        .harmonic_count = 4,
        .harmonics_enabled = false,
    };

    int16_t value;

    VibrationStatus status =
        vibration_fixed(&config, 1, &value);

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
