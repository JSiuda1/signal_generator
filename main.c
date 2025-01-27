#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/dl_timerg.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti/driverlib/m0p/sysctl/dl_sysctl_mspm0l11xx_l13xx.h"
#include "ti_msp_dl_config.h"
#include "src/hardware/timer_dac.h"
#include "src/dac/signal_dac.h"

SIG_DAC_t sig_dac;
#define DAC_PIN_MASK GPIO_DAC_B0_PIN | GPIO_DAC_B1_PIN | GPIO_DAC_B2_PIN | GPIO_DAC_B3_PIN | \
                     GPIO_DAC_B4_PIN | GPIO_DAC_B5_PIN | GPIO_DAC_B6_PIN | GPIO_DAC_B7_PIN

void on_dac_timer(void) {
    DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_STATUS_PIN);

    // uint32_t val = SIG_DAC_get_current_value(&sig_dac);
    // SIG_DAC_move_to_next(&sig_dac);
    // int val = 0b01010101;
    // DL_GPIO_writePinsVal(GPIO_DAC_PORT,  DAC_PIN_MASK, val << 11);
    // DL_GPIO_togglePins(GPIO_DAC_PORT, DAC_PIN_MASK);

}

int main(void) {
    SYSCFG_DL_init();

    SIG_DAC_config_t cfg = {
        .lower_range = 0,
        .upper_range = 255,
        .signal_type = SIG_RECT,
    };

    SIG_DAC_init(&sig_dac, &cfg);
    DL_GPIO_clearPins(GPIO_DAC_PORT, DAC_PIN_MASK);
    TIMER_DAC_init(on_dac_timer);
    // Buffer size is 100, so the signal frequency is time_us / BUFFER_SIZE
    TIMER_DAC_start(500);

    while (1) {
        __WFI();
    }
}