#include <math.h>
#include "dsp.h"
#include "common.h"

void dsp_apply_hanning(
    const float *input, 
    const float *window_multipliers, 
    float *output_windowed, 
    size_t length)
{
    if (input == NULL || window_multipliers == NULL || output_windowed == NULL || length == 0) {
        return;
    }

    // Process optimization loop: 'input' is completely un-mutated 
    for (size_t i = 0; i < length; i++) {
        output_windowed[i] = input[i] * window_multipliers[i];
    }
}

void dsp_compute_fft(
    const float *windowed_input, 
    float *magnitude_out, 
    size_t length)
{
    if (windowed_input == NULL || magnitude_out == NULL || length == 0) {
        return;
    }

    size_t half_size = length / 2;

    // Run real-numbered Discrete Fourier Transform math
    for (size_t k = 0; k < half_size; k++) {
        float real_sum = 0.0f;
        float imag_sum = 0.0f;

        for (size_t n = 0; n < length; n++) {
            float angle = (2.0f * (float)PI * (float)k * (float)n) / (float)length;
            real_sum += windowed_input[n] * cosf(angle);
            imag_sum -= windowed_input[n] * sinf(angle);
        }

        // Calculate direct magnitude spectrum vector length
        magnitude_out[k] = sqrtf((real_sum * real_sum) + (imag_sum * imag_sum));
    }
}
