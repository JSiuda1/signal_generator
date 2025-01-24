#include <stddef.h>
#include "timer_dac.h"
#include "ti/driverlib/dl_timerg.h"
#include "ti/driverlib/m0p/sysctl/dl_sysctl_mspm0l11xx_l13xx.h"
#include "ti_msp_dl_config.h"

static struct {
    on_timer_callback timer_callback;
    bool is_running;
    uint32_t time_us;
} _tim = {
    .timer_callback = NULL,
    .is_running = false,
    .time_us = 0,
};

bool TIMER_DAC_init(on_timer_callback callback) {
    if (callback == NULL) {
        return false;
    }

    NVIC_EnableIRQ(TIMER_DAC_INST_INT_IRQN);
    DL_SYSCTL_enableSleepOnExit();
    _tim.timer_callback = callback;

    return true;
}

void TIMER_DAC_start(uint32_t time_us) {
    // TODO: calculate time_us to counts
    _tim.time_us = time_us;
    DL_TimerG_startCounter(TIMER_DAC_INST);
}

void TIMER_DAC_stop() {
    DL_TimerG_stopCounter(TIMER_DAC_INST);
}

static void _on_timer_interrupt() {
    DL_TimerG_stopCounter(TIMER_DAC_INST);
    DL_Timer_setLoadValue(TIMER_DAC_INST, _tim.time_us);
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
