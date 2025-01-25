#ifndef DAC_TIMER_H_
#define DAC_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

typedef void (*on_timer_callback)(void);

bool TIMER_DAC_init(on_timer_callback callback);

bool TIMER_DAC_start(uint32_t time_us);

void TIMER_DAC_stop();


#endif