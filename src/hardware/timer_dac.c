#include <stddef.h>
#include "timer_dac.h"
#include "ti/driverlib/dl_timerg.h"
#include "ti/driverlib/m0p/sysctl/dl_sysctl_mspm0l11xx_l13xx.h"
#include "ti_msp_dl_config.h"

// THESE VALUES SHOULD BE CALCULATED BASED ON THE GENERATED CONFIG
// but this should work until we change the timer settings.
#define TIMER_MIN_TIME_US 10
#define TIMER_MAX_TIME_US 650000
#define TIME_US_TO_COUNTS_DIV 10

static struct {
    on_timer_callback timer_callback;
    bool is_running;
    uint32_t timer_counts;
} _tim = {
    .timer_callback = NULL,
    .is_running = false,
    .timer_counts = 0,
};

bool TIMER_DAC_init(on_timer_callback callback) {
    if (callback == NULL) {
        return false;
    }

    NVIC_EnableIRQ(TIMER_DAC_INST_INT_IRQN);
    _tim.timer_callback = callback;

    return true;
}

bool TIMER_DAC_start(uint32_t time_us) {
    if (time_us < TIMER_MIN_TIME_US || time_us > TIMER_MAX_TIME_US) {
        return false;
    }

    // Timer clock frequency is set to 100kHz -> 10us - 1 count
    _tim.timer_counts = time_us / TIME_US_TO_COUNTS_DIV;

    DL_TimerG_startCounter(TIMER_DAC_INST);

    return  true;
}

void TIMER_DAC_stop() {
    DL_TimerG_stopCounter(TIMER_DAC_INST);
}

static void _on_timer_interrupt() {
    DL_TimerG_stopCounter(TIMER_DAC_INST);
    DL_Timer_setLoadValue(TIMER_DAC_INST, _tim.timer_counts);
    DL_TimerG_startCounter(TIMER_DAC_INST);

    if (_tim.timer_callback != NULL) {
        _tim.timer_callback();
    }
}

void TIMER_DAC_INST_IRQHandler(void) {
    switch (DL_TimerG_getPendingInterrupt(TIMER_DAC_INST)) {
            case DL_TIMER_IIDX_ZERO:
                _on_timer_interrupt();
            break;
        default:
            break;
    }
}
