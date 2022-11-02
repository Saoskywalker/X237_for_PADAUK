#ifndef _DISPLAY_BOARD_H
#define _DISPLAY_BOARD_H

#include "types_base.h"
#include "gpio_board.h"

extern uint8_t LED_data; //数码管显示输出缓存;
extern uint8_t LED_data_buf; //LED显示data
void Led_Dis_Update(void);
void Led_Clear_All(void);
void Led_dis_All(void);
void Led_Scan(void);
void Led_Display_exit(void);
void Led_Display_init(void);

#endif
