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

    // Allocate internal workspaces on the stack that match the FULL length (512)
    float real_out[length];
    float imag_out[length];


    // BIT-REVERSAL PERMUTATION

    for (size_t i = 0; i < length; i++) {
        size_t j = 0;
        size_t temp_i = i;
        for (size_t bit = 1; bit < length; bit <<= 1) {
            j = (j << 1) | (temp_i & 1);
            temp_i >>= 1;
        }
        
        real_out[j] = windowed_input[i]; 
        imag_out[j] = 0.0f; 
    }


    // RADIX-2 COOLEY-TUKEY BUTTERFLY STAGES
    for (size_t size = 2; size <= length; size <<= 1) {
        size_t half_size = size >> 1;
        float tab_step = -2.0f * PI / (float)size;

        for (size_t i = 0; i < length; i += size) {
            for (size_t j = 0; j < half_size; j++) {
                size_t even_idx = i + j;
                size_t odd_idx  = even_idx + half_size;

                float angle = tab_step * (float)j;
                float twiddle_real = cosf(angle);
                float twiddle_imag = sinf(angle);

                float t_real = (real_out[odd_idx] * twiddle_real) - (imag_out[odd_idx] * twiddle_imag);
                float t_imag = (real_out[odd_idx] * twiddle_imag) + (imag_out[odd_idx] * twiddle_real);

                real_out[odd_idx] = real_out[even_idx] - t_real;
                imag_out[odd_idx] = imag_out[even_idx] - t_imag;

                real_out[even_idx] += t_real;
                imag_out[even_idx] += t_imag;
            }
        }
    }


    // SYMMETRIC MAGNITUDE EXTRACTION
  
    size_t target_bins = length / 2;
    for (size_t k = 0; k < target_bins; k++) {
        magnitude_out[k] = sqrtf((real_out[k] * real_out[k]) + (imag_out[k] * imag_out[k]));
    }
}