#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/dl_i2c.h"
#include "ti/driverlib/dl_timerg.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"
#include "src/hardware/timer_dac.h"
#include "src/dac/signal_dac.h"
#include "src/dac/signals.h "
#include "src/ssd1306/ssd1306.h"
#include "src/ssd1306/twi.h"

SIG_DAC_t sig_dac;
#define DAC_PIN_MASK GPIO_DAC_B0_PIN | GPIO_DAC_B1_PIN | GPIO_DAC_B2_PIN | GPIO_DAC_B3_PIN | \
                     GPIO_DAC_B4_PIN | GPIO_DAC_B5_PIN | GPIO_DAC_B6_PIN | GPIO_DAC_B7_PIN
#define DAC_PIN_VAL_OFFSET 11

volatile uint32_t led_val = 0;

void on_dac_timer(void) {
    uint32_t val = SIG_DAC_get_current_value(&sig_dac);
    SIG_DAC_move_to_next(&sig_dac);
    DL_GPIO_writePinsVal(GPIO_DAC_PORT,  DAC_PIN_MASK, val << DAC_PIN_VAL_OFFSET);
}


int main(void) {
    SYSCFG_DL_init();

    // SIG_DAC_config_t cfg = {
    //     .lower_range = 0,
    //     .upper_range = 255,
    //     .sig_gen = SIG_SAWTOOTH,
    // };

    // SIG_DAC_init(&sig_dac, &cfg);
    // DL_GPIO_clearPins(GPIO_DAC_PORT, DAC_PIN_MASK); // Clear all pins before starting the timer
    NVIC_EnableIRQ(I2C_0_INST_INT_IRQN);
    DL_SYSCTL_disableSleepOnExit();

    // TIMER_DAC_init(on_dac_timer);
    // Buffer size is 100, so the signal frequency is time_us / BUFFER_SIZE
    // TIMER_DAC_start(500);

    SSD1306_Init (SSD1306_ADDR);                                    // 0x3C

    // DRAWING
    // -------------------------------------------------------------------------------------
    SSD1306_ClearScreen ();                                         // clear screen
    SSD1306_DrawLine (0, MAX_X, 4, 4);                              // draw line
    SSD1306_SetPosition (7, 1);                                     // set position
    SSD1306_DrawString ("HELLO WORLD");                     // draw string
    // SSD1306_DrawLine (0, MAX_X, 18, 18);                            // draw line
    // SSD1306_SetPosition (40, 3);                                    // set position
    // SSD1306_DrawString ("MATIASUS");                                // draw string
    // SSD1306_SetPosition (53, 5);                                    // set position
    // SSD1306_DrawString ("2021");                                    // draw string
    SSD1306_UpdateScreen (SSD1306_ADDR);   

    // delay_cycles(32000000);
    // SSD1306_InverseScreen (SSD1306_ADDR);

    // delay_cycles(32000000);
    // SSD1306_NormalScreen (SSD1306_ADDR); 

    // for (int i = 0; i < 100; ++i) {
    //     TWI_MT_Send_Data(i);
    // }
    // TWI_Stop();

    // delay_cycles(32000000);
    // for (int i = 20; i < 40; ++i) {
    //     TWI_MT_Send_Data(i);
    // }
    // TWI_Stop();

     while (1) {
        delay_cycles(16000000);
    }
}


// #include "ti_msp_dl_config.h"
// #include "src/ssd1306/twi.h"

// int main(void)
// {
//     SYSCFG_DL_init();

//     /* Set LED to indicate start of transfer */
//     // DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

//     NVIC_EnableIRQ(I2C_0_INST_INT_IRQN);
//     DL_SYSCTL_disableSleepOnExit();

//     for (int i = 0; i < 100; ++i) {
//         TWI_MT_Send_Data(i);
//     }

//     TWI_MT_Send_SLAW(0);
    
//     delay_cycles(32000000);
//     for (int i = 20; i < 40; ++i) {
//         TWI_MT_Send_Data(i);
//     }
//     TWI_MT_Send_SLAW(0);

//     while (1) {
//         delay_cycles(16000000);
//     }
// }
