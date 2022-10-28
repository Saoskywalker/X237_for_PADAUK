#ifndef _DISPLAY_DEFINE_H
#define _DISPLAY_DEFINE_H

#include "display_board.h"

/*****************************
 * 应用对应的led显存功能定义
 * **************************/
#define Dsp_RESlocate(N) (LED_data_buf &= ~(N))
#define Dsp_locate(N) LED_data_buf |= N

#define led1_locate Dsp_locate(0X01)
#define led2_locate Dsp_locate(0X02)
#define led3_locate Dsp_locate(0X04)
/////////////////////////////////////

//==============================================================================
//      3) 函数声明(对外接口)
//==============================================================================
void Led_display(void);
void Disp_All(void);

#endif
