#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/dl_timerg.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"
#include "src/hardware/timer_dac.h"
#include "src/dac/signal_dac.h"
#include "src/dac/signals.h "

SIG_DAC_t sig_dac;
#define DAC_PIN_MASK GPIO_DAC_B0_PIN | GPIO_DAC_B1_PIN | GPIO_DAC_B2_PIN | GPIO_DAC_B3_PIN | \
                     GPIO_DAC_B4_PIN | GPIO_DAC_B5_PIN | GPIO_DAC_B6_PIN | GPIO_DAC_B7_PIN
#define DAC_PIN_VAL_OFFSET 11

volatile uint32_t led_val = 0;

int currentItem = 0;
char menuItems[3][10]={'shape',
                        'frequency',
                        'amplitude'};

void on_dac_timer(void) {
    uint32_t val = SIG_DAC_get_current_value(&sig_dac);
    SIG_DAC_move_to_next(&sig_dac);
    DL_GPIO_writePinsVal(GPIO_DAC_PORT,  DAC_PIN_MASK, val << DAC_PIN_VAL_OFFSET);
}

int main(void) {
    SYSCFG_DL_init();
    NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);

    SIG_DAC_config_t cfg = {
        .lower_range = 0,
        .upper_range = 255,
        .signal_type = SIG_SIN,
    };

    SIG_DAC_init(&sig_dac, &cfg);
    DL_GPIO_clearPins(GPIO_DAC_PORT, DAC_PIN_MASK); // Clear all pins before starting the timer
    TIMER_DAC_init(on_dac_timer);
    // Buffer size is 100, so the signal frequency is time_us / BUFFER_SIZE
    TIMER_DAC_start(500);

    while (1) {
        __WFI();
    }
}

void GROUP1_IRQHandler(void){

    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)){

        case ENCODER_ENC_A_IIDX:
        if(DL_GPIO_readPins(ENCODER_PORT, ENCODER_ENC_A_PIN)){
            
        }

        case  ENCODER_ENC_B_IIDX:
        if(DL_GPIO_readPins(ENCODER_PORT, ENCODER_ENC_B_PIN)){
            
        }

        case ENCODER_ENC_C_IIDX:
        if(DL_GPIO_readPins(ENCODER_PORT, ENCODER_ENC_C_PIN)){
            
        }

        case BTNL_BTN_L_IIDX:
        if(DL_GPIO_readPins(BTNL_PORT, BTNL_BTN_L_PIN)){
            
        }

        case BTNR_BTN_R_IIDX:
        if(DL_GPIO_readPins(BTNR_PORT, BTNR_BTN_R_PIN)){
            
        }

        break;

    }


}