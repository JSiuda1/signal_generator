#include "ti/devices/msp/m0p/mspm0l130x.h"
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
#include "src/gui/gui.h"
#include <stdio.h>

SIG_DAC_t sig_dac;
#define DAC_PIN_MASK GPIO_DAC_B0_PIN | GPIO_DAC_B1_PIN | GPIO_DAC_B2_PIN | GPIO_DAC_B3_PIN | \
                     GPIO_DAC_B4_PIN | GPIO_DAC_B5_PIN | GPIO_DAC_B6_PIN | GPIO_DAC_B7_PIN
#define DAC_PIN_VAL_OFFSET 11


void on_dac_timer(void) {
    uint32_t val = SIG_DAC_get_current_value(&sig_dac);
    SIG_DAC_move_to_next(&sig_dac);
    DL_GPIO_writePinsVal(GPIO_DAC_PORT,  DAC_PIN_MASK, val << DAC_PIN_VAL_OFFSET);
}

GUI_layout_t layout;

void update_screen(GUI_layout_t *layout);

typedef struct {
    char sig_name[10];
    SIG_DAC_generator gen;
} sig_spec_t;

#define SIG_NB 4
#define MAX_SIG_NB_IDX (SIG_NB - 1)

sig_spec_t signals[SIG_NB] = {
    {"RECTANGLE", SIG_RECTANGLE},
    {"SINUS", SIG_SINUS},
    {"TRIANGLE", SIG_TRIANGLE},
    {"SAWTOOTH", SIG_SAWTOOTH},
};

uint8_t current_signal_idx = 0;

void signal_move_to_next() {
    if (current_signal_idx < MAX_SIG_NB_IDX) {
        current_signal_idx += 1;
    } else {
        current_signal_idx = 0;
     }
}

void signal_move_to_prev() {
    if (current_signal_idx > 0) {
        current_signal_idx -= 1;
    } else {
        current_signal_idx = MAX_SIG_NB_IDX;
    }
}


void gui_dac_signal(GUI_element_t *element, uint32_t signal_id) {
    switch (signal_id) {
        case GUI_SIG_SWITCH_DOWN:
            (void) SIG_DAC_change_signal(&sig_dac, signals[current_signal_idx].gen);
            break;
        
        case GUI_SIG_ENCODER_CW:
            signal_move_to_next();
            GUI_set_element_val(element, signals[current_signal_idx].sig_name);
            break;
        case GUI_SIG_ENCODER_CCW:
            signal_move_to_prev();
            GUI_set_element_val(element, signals[current_signal_idx].sig_name);
            break;
        case GUI_SIG_INIT:
            GUI_set_element_val(element, signals[current_signal_idx].sig_name);
            break;
        default:
            break;
    }

    
}

void gui_freq(GUI_element_t *element, uint32_t signal_id) {

}

int main(void) {
    SYSCFG_DL_init();

    SIG_DAC_config_t cfg = {
        .lower_range = 0,
        .upper_range = 255,
        .sig_gen = signals[current_signal_idx].gen,
    };

    NVIC_EnableIRQ(GPIO_INPUT_INT_IRQN);

    SIG_DAC_init(&sig_dac, &cfg);
    DL_GPIO_clearPins(GPIO_DAC_PORT, DAC_PIN_MASK); // Clear all pins before starting the timer
    delay_cycles(2 * 32000000);
    NVIC_EnableIRQ(I2C_0_INST_INT_IRQN);
    DL_SYSCTL_disableSleepOnExit();

    TIMER_DAC_init(on_dac_timer);
    // Buffer size is 100, so the signal frequency is time_us / BUFFER_SIZE
    TIMER_DAC_start(500);

    SSD1306_Init (SSD1306_ADDR);                                    // 0x3C

    GUI_element_t elem_signal = {
        .name = "Signal",
        ._sig_handler = gui_dac_signal
    };
    GUI_element_t elem_freq = {
        .name = "Freq: ",
        ._sig_handler = gui_freq
    };
    GUI_init(&layout);
    GUI_add_element(&layout, elem_signal);
    GUI_add_element(&layout, elem_freq);
    update_screen(&layout);
    update_screen(&layout);

     while (1) {
        if (GUI_is_signal_pending(&layout) == true) {
            GUI_process_pending_signal(&layout);
            update_screen(&layout);
            update_screen(&layout);
        }
        // delay_cycles(4000000);
    }
}

void write_int_to_screen(int num) {
    char buffer[10];  // Buffer to hold digits (max 10 digits for 32-bit int)
    int i = 0;

    do {
        buffer[i++] = (num % 10) + '0';  // Extract last digit
        num /= 10;
    } while (num > 0);

    // Send digits in reverse order
    while (i > 0) {
        SSD1306_DrawChar(buffer[--i]);
    }
}

void update_screen(GUI_layout_t *layout) {
    SSD1306_ClearScreen();
    
    size_t start_nb, stop_nb = 0;
    if (layout->elements_nb > 4) {
        if (layout->elements_nb - layout->current_element_idx < 4) {
            start_nb = layout->elements_nb - 4;
        } else {
            start_nb = layout->current_element_idx;
        }
        stop_nb = 4;
    } else {
        start_nb = 0;
        stop_nb = layout->elements_nb;
    }

    for (size_t i = 0; i < stop_nb; ++i) {
        if (start_nb + i == layout->current_element_idx) {
            SSD1306_SetPosition(1, i);
            if (layout->is_chosen == true) {
                SSD1306_DrawChar('*');
            } else {
                SSD1306_DrawChar('>');
            }
        }
        SSD1306_SetPosition(10, i);
        SSD1306_DrawString(layout->elements[start_nb + i].name);
        SSD1306_SetPosition(70, i);                  
        SSD1306_DrawString(layout->elements[start_nb + i].value);  
    }
    SSD1306_UpdateScreen(SSD1306_ADDR);   
}

void GROUP1_IRQHandler(void) {
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_INPUT_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIO_INPUT_PORT)) {
                case GPIO_INPUT_ENCODER_A_IIDX:
                    if (DL_GPIO_readPins(GPIO_INPUT_PORT, GPIO_INPUT_ENCODER_A_PIN)) {
                        if (DL_GPIO_readPins(GPIO_INPUT_PORT, GPIO_INPUT_ENCODER_B_PIN)) {
                            GUI_add_signal(&layout, GUI_SIG_ENCODER_CW);
                        } else {
                            GUI_add_signal(&layout, GUI_SIG_ENCODER_CCW);
                        }
                    } else {
                        if (DL_GPIO_readPins(GPIO_INPUT_PORT, GPIO_INPUT_ENCODER_B_PIN)) {
                            GUI_add_signal(&layout, GUI_SIG_ENCODER_CCW);
                        } else {
                            GUI_add_signal(&layout, GUI_SIG_ENCODER_CW);
                        }
                    }
                    break;
                case GPIO_INPUT_ENCODER_SW_IIDX:
                    DL_GPIO_togglePins(GPIO_INPUT_PORT, GPIO_LED_STATUS_PIN);
                    GUI_add_signal(&layout, GUI_SIG_ENCODER_SW);
                    break;
                case GPIO_INPUT_SWITCH_UP_IIDX:
                    GUI_add_signal(&layout, GUI_SIG_SWITCH_UP);
                    break;
                case GPIO_INPUT_SWITCH_DOWN_IIDX:
                    GUI_add_signal(&layout, GUI_SIG_SWITCH_DOWN);
                    break;
                default:
                    break;
            } 
    }
}