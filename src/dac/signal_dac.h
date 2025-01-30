#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <stdint.h>
#include <stdbool.h>

#define SIG_DAC_BUFFER_SIZE 100

typedef enum {
    SIG_UNKNOWN,
    SIG_SIN,
    SIG_RECT,
    SIG_TRI,
    SIG_SAW
} SIG_DAC_type_t;

typedef struct {
    SIG_DAC_type_t signal_type;
    uint8_t upper_range;
    uint8_t lower_range;
    uint8_t buffer[SIG_DAC_BUFFER_SIZE];
    uint8_t current_idx;
} SIG_DAC_t;

typedef struct {
    SIG_DAC_type_t signal_type;
    uint8_t upper_range;
    uint8_t lower_range;
} SIG_DAC_config_t;

bool SIG_DAC_init(SIG_DAC_t *sig_dac, SIG_DAC_config_t *cfg);

int SIG_DAC_get_current_value(SIG_DAC_t *sig_dac);

void SIG_DAC_move_to_next(SIG_DAC_t *sig_dac);

bool SIG_DAC_change_upper_range(SIG_DAC_t *sig_dac, uint8_t upper_range);

bool SIG_DAC_change_lower_range(SIG_DAC_t *sig_dac, uint8_t lower_range);

bool SIG_DAC_change_signal(SIG_DAC_t *sig_dac, SIG_DAC_type_t sig_type);

#endif