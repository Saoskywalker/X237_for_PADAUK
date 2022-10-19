#ifndef _APP_TIMER_H
#define _APP_TIMER_H

#include "types_base.h"

// typedef struct
// {
//   uint8_t _200us : 1;
//   uint8_t _1ms : 1;
//   uint8_t _2ms : 1;
//   uint8_t _10ms : 1;
//   uint8_t _100ms : 1;
//   uint8_t _500ms : 1;
//   uint8_t _1s : 1;
//   uint8_t _1min : 1;
// } timer_flag_type;

// extern timer_flag_type app_timer_flag;
extern bit app_timer_flag_200us;
extern bit app_timer_flag_2ms;
extern bit app_timer_flag_10ms;
extern bit app_timer_flag_100ms;
void app_timer(void);

#endif
