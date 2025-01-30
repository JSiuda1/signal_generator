#ifndef SIGNALS_H_
#define SIGNALS_H_

void __sig_sinus(float *normalized_singal, size_t buffer_size);

void __sig_rect(float *normalized_signal, size_t buffer_size);

void __sig_sawtooth(float *normalized_signal, size_t buffer_size);

void __sig_triangle(float *normalized_signal, size_t buffer_size);

#define SIG_SINUS       __sig_sinus
#define SIG_RECTANGLE   __sig_rect
#define SIG_SAWTOOTH    __sig_sawtooth
#define SIG_TRIANGLE    __sig_triangle

#endif