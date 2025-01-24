#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/dl_timerg.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti/driverlib/m0p/sysctl/dl_sysctl_mspm0l11xx_l13xx.h"
#include "ti_msp_dl_config.h"
#include "src/hardware/timer_dac.h"

void on_dac_timer(void) {
    DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_STATUS_PIN);
}

int main(void) {
    SYSCFG_DL_init();

    TIMER_DAC_init(on_dac_timer);
    TIMER_DAC_start(50);

    while (1) {
        __WFI();
    }
}