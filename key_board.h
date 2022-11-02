#ifndef _KEY_BOARD_H
#define _KEY_BOARD_H

#include "types_plus.h"

extern uint8_t key_input_res;

#define key_get_result() key_input_res //获取输入按键结果

void key_input_check_timer(void); //定期运行输入检查

#endif
