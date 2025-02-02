#include <stddef.h>
#include "gui.h"

bool GUI_init(GUI_layout_t *layout) {
    if (layout == NULL) {
        return false;
    }

    layout->current_element_idx = 0;
    layout->elements_nb = 0;
    layout->is_chosen = false;

    return true;
}

bool GUI_add_element(GUI_layout_t *layout, GUI_element_t element) {
    if (layout->elements_nb >= MAX_ELEMENTS_NB) {
        return false;
    }

    if (element._sig_handler != NULL) {
        element._sig_handler(&element, GUI_SIG_INIT);
    }

    layout->elements[layout->elements_nb] = element;
    layout->elements_nb += 1;

    return  true;
}

static bool _is_element_signal(GUI_layout_t *layout) {
    return layout->is_chosen;
}

static void _process_element_signal(GUI_layout_t *layout) {
    GUI_element_t *elem = &layout->elements[layout->current_element_idx];
    if (elem->_sig_handler != NULL) {
        elem->_sig_handler(elem, layout->last_signal);
    }

    if (layout->last_signal == GUI_SIG_SWITCH_DOWN) {
        layout->is_chosen = false;
    }
}

static void _process_layout_signal(GUI_layout_t *layout) {
    switch (layout->last_signal) {
        case GUI_SIG_SWITCH_UP:
            if (layout->current_element_idx > 0) {
                layout->current_element_idx -= 1;
            }
            break;
        case GUI_SIG_SWITCH_DOWN:
            if (layout->current_element_idx < (layout->elements_nb - 1)) {
                layout->current_element_idx += 1;
            }
            break;
        case GUI_SIG_ENCODER_SW:
            layout->is_chosen = true;
            break;
        default:
            break;
    }
}

void GUI_process_pending_signal(GUI_layout_t *layout) {
    if (layout->is_signal_pending == false) {
        return;
    }

    if (_is_element_signal(layout) == true) {
        _process_element_signal(layout);
    } else {
        _process_layout_signal(layout);
    }

    layout->is_signal_pending = false;
}

void GUI_add_signal(GUI_layout_t *layout, uint32_t signal_id) {
    layout->last_signal = signal_id;
    layout->is_signal_pending = true;
}

bool GUI_is_signal_pending(GUI_layout_t *layout) {
    return layout->is_signal_pending; 
}


void GUI_set_element_val(GUI_element_t *element, char *str) {
    (void) strncpy(element->value, str, sizeof(element->value) - 1);
}