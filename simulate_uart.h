#ifndef _SIMULATE_UART_H
#define _SIMULATE_UART_H

#include "types_base.h"

extern uint8_t su_data, su_sent_cnt;
#define simulate_uart_send_buffer(x) {su_data = x; su_sent_cnt++;}
#define simulate_uart_send_cnt() su_sent_cnt

void simulate_uart_send(void);

#endif
