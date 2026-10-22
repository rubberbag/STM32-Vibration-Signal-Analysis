#ifndef DSP_H
#define DSP_H

#include <stddef.h>

// No hardcoded macros here!
void dsp_apply_hanning(const float *input, const float *window_multipliers, float *output_windowed, size_t length);

void dsp_compute_fft(const float *windowed_input, float *magnitude_out, size_t length);

#endif
