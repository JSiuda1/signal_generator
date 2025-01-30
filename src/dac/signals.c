#include <stddef.h>
#include <math.h>
#include "signals.h"

void __sig_sinus(float *normalized_singal, size_t buffer_size) {
    float rad_step = 2 * 3.14 / buffer_size;

    for (size_t i = 0; i < buffer_size; ++i) {
        normalized_singal[i] = (sin(rad_step * i) + 1) / 2.0;
    }
}

void __sig_rect(float *normalized_signal, size_t buffer_size) {
    size_t half_buffer_size = buffer_size / 2;

    for (size_t i = 0; i < buffer_size; ++i) {
        if (i < half_buffer_size) {
            normalized_signal[i] = 1; 
        } else {
            normalized_signal[i] = 0;
        }
    }
}

void __sig_sawtooth(float *normalized_signal, size_t buffer_size) {
    float step = 1.0 / buffer_size;  // Normalized signal in range from 0 to 1
    for (size_t i = 0; i < buffer_size; ++i) {
        normalized_signal[i] = i * step;
    }
}

void __sig_triangle(float *normalized_signal, size_t buffer_size) {
    float step = 2.0 / buffer_size; // Range from 0 to 2

    // First half
    for (size_t i = 0; i < buffer_size; ++i) {
        // Substract one to get from -1 to 1, then abs to get triangle
        normalized_signal[i] = fabs(step * i - 1);
    }
}
