#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stddef.h>

#define PACKET_MAX_SAMPLES 512

struct __attribute__((packed)) DiagnosticPacket {
    uint32_t packet_id;       // Assigned by main
    float sample_rate;          // Assigned by main
    float fundamental_rpm;      // Assigned by main
    float scaling_factor;       // Assigned by main
    uint32_t sample_count;      // Populated by packet_build based on length
    
    int32_t time_domain[PACKET_MAX_SAMPLES];    // Built and quantized by packet_build
    float fft_magnitude[PACKET_MAX_SAMPLES / 2]; // Copied over by packet_build
};

/**
 * Packs raw float time domain stream data and calculated frequency domain stream data.
 * Reads metadata fields directly from the struct to map quantization behaviors.
 */
void packet_build(
    struct DiagnosticPacket *packet,
    const float *raw_time_domain,
    const float *fft_magnitude,
    size_t length);

#endif
