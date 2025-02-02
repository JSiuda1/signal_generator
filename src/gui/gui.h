#ifndef DISPLAY_ELEMENT_H_
#define DISPLAY_ELEMENT_H_

#include <stdint.h>
#include <stdbool.h>
#include "src/gui/gui.h"

#define MAX_ELEMENTS_NB 5

typedef enum {
    GUI_SIG_INIT,
    GUI_SIG_ENCODER_CW,
    GUI_SIG_ENCODER_CCW,
    GUI_SIG_ENCODER_SW,
    GUI_SIG_SWITCH_UP,
    GUI_SIG_SWITCH_DOWN
} GUI_signals_t;

typedef struct GUI_element_s GUI_element_t;

typedef void(*signal_handler_t)(GUI_element_t *element, uint32_t signal_id);

typedef struct GUI_element_s {
    char name[10];
    char value[10];
    signal_handler_t _sig_handler;
} GUI_element_t;

typedef struct {
    GUI_element_t elements[MAX_ELEMENTS_NB];
    GUI_signals_t last_signal;
    uint8_t elements_nb;
    uint8_t current_element_idx;
    bool is_signal_pending;
    bool is_chosen;
} GUI_layout_t;

bool GUI_init(GUI_layout_t *layout);
bool GUI_add_element(GUI_layout_t *layout, GUI_element_t element);
void GUI_process_pending_signal(GUI_layout_t *layout);
void GUI_add_signal(GUI_layout_t *layout, uint32_t signal_id);
bool GUI_is_signal_pending(GUI_layout_t *layout);
void GUI_set_element_val(GUI_element_t *element, char *str);

#endif