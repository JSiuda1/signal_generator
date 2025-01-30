#include "signal_dac.h"
#include <math.h>

static void save_dac_signal(SIG_DAC_t *sig_dac, float *normalized_signal) {
    // As lon as the sig_dac and normalzied_signal has size of SIG_DAC_BUFFER_SIZE
    // we don't have to check if they have the same size.

    for (int i = 0; i < SIG_DAC_BUFFER_SIZE; ++i) {
        // buffer_value = max_val + (norm_val * (max_val - min_val))
        sig_dac->buffer[i] = sig_dac->lower_range + 
            round(normalized_signal[i] * (sig_dac->upper_range - sig_dac->lower_range)); 
    }
}

static bool generate_signal(SIG_DAC_t *sig_dac) {
    float normalized_singal[SIG_DAC_BUFFER_SIZE];

    sig_dac->sig_gen(normalized_singal, SIG_DAC_BUFFER_SIZE);
    save_dac_signal(sig_dac, normalized_singal);

    return true;
}


bool SIG_DAC_init(SIG_DAC_t *sig_dac, SIG_DAC_config_t *cfg) {
    if (sig_dac == NULL || cfg == NULL) {
        return false;
    }

    if (cfg->lower_range >= cfg->upper_range) {
        return false;
    }

    if (cfg->sig_gen == NULL) {
        return false;
    }

    sig_dac->sig_gen = cfg->sig_gen;
    sig_dac->lower_range = cfg->lower_range;
    sig_dac->upper_range = cfg->upper_range;

    if (generate_signal(sig_dac) == false) {
        return false; 
    }

    return true;
}

int SIG_DAC_get_current_value(SIG_DAC_t *sig_dac) {
    if (sig_dac == NULL) {
        return 0;
    }

    return sig_dac->buffer[sig_dac->current_idx];
}

void SIG_DAC_move_to_next(SIG_DAC_t *sig_dac) {
     if (sig_dac == NULL) {
        return;
    }

    sig_dac->current_idx = (sig_dac->current_idx + 1) % SIG_DAC_BUFFER_SIZE;
}

bool SIG_DAC_change_upper_range(SIG_DAC_t *sig_dac, uint8_t upper_range) {
    if (sig_dac == NULL) {
        return false;
    }

    if (sig_dac->lower_range >= upper_range) {
        return false;
    }

    sig_dac->upper_range = upper_range;
    if (generate_signal(sig_dac) == false) {
        return false;
    }

    return true;
}

bool SIG_DAC_change_lower_range(SIG_DAC_t *sig_dac, uint8_t lower_range) {
    if (sig_dac == NULL) {
        return false;
    }

    if (sig_dac->upper_range <= lower_range) {
        return false;
    }

    sig_dac->lower_range = lower_range;
    if (generate_signal(sig_dac) == false) {
        return false;
    }

    return true;
}

bool SIG_DAC_change_signal(SIG_DAC_t *sig_dac, SIG_DAC_generator sig_gen) {
    if (sig_dac == NULL) {
        return false;
    }

    if (sig_gen == NULL) {
        return false;
    }

    sig_dac->sig_gen = sig_gen;
    if (generate_signal(sig_dac) == false) {
        return false;
    }

    return true;
}
