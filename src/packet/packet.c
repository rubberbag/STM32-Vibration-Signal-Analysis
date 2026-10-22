#include <math.h>
#include "packet.h"

static inline int32_t embedded_lroundf(float value)
{
    return (value >= 0.0f) ? (int32_t)(value + 0.5f) : (int32_t)(value - 0.5f);
}


void packet_build(
    struct DiagnosticPacket *packet,
    const float *raw_time_domain,
    const float *fft_magnitude,
    size_t length)
{
    if (packet == NULL || raw_time_domain == NULL || fft_magnitude == NULL || length == 0) {
        return;
    }

    if (length > PACKET_MAX_SAMPLES) {
        length = PACKET_MAX_SAMPLES;
    }

    packet->sample_count = (uint32_t)length;

    // 2. Quantize time domain floating-point array into fixed-point using main's scaling factor
    float factor = packet->scaling_factor;
    
    for (size_t i = 0; i < length; i++) {
        packet->time_domain[i] = embedded_lroundf(raw_time_domain[i] * factor);
    }

    // 3. Mirror the calculated FFT frequency spectrum straight into payload allocation space
    size_t half_size = length / 2;

    for (size_t k = 0; k < half_size; k++) {
        packet->fft_magnitude[k] = embedded_lroundf(fft_magnitude[k] * factor);
    }
}
