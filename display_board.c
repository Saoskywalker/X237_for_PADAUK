#include "display_board.h"

/*************************************************
 // 函数名称    : uint8_t Led_Display_init(void)
 // 功能描述    : 开启LED显示
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
/* uint8_t Led_Display_init(void)
{
  return 0;
} */

/*************************************************
 // 函数名称    : uint8_t Led_Display_exit(void)
 // 功能描述    : 关闭LED显示
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
/* uint8_t Led_Display_exit(void)
{
  COMDIG1_PIN_OFF;
  COMDIG2_PIN_OFF;
  COMDIG3_PIN_OFF;
  COMDIG4_PIN_OFF;
  return 0;
} */

/*************************************************
 // 函数名称    : Led_Dis_Update
 // 功能描述    : 显存更新到led
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Led_Dis_Update(void)
{
    LED_data = LED_data_buf;
}

/*************************************************
 // 函数名称    : Led_Clear_All
 // 功能描述    : 清除所有显示
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Led_Clear_All(void)
{
  LED_data_buf = 0;
}

/*************************************************
 // 函数名称    : Led_dis_All
 // 功能描述    : 所有显示
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Led_dis_All(void)
{
    LED_data_buf = 0XFF;
}

/*************************************************
 // 函数名称    : Led_scan
 // 功能描述    : 此函数在定时中执行
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
uint8_t LED_data = 0; //数码管显示输出缓存
uint8_t LED_data_buf = 0; //LED显示data
void Led_Scan(void)
{
  if (LED_data&0X01)
    LED_1_PIN_ON;
  else
    LED_1_PIN_OFF;

  if (LED_data&0X02)
    LED_2_PIN_ON;
  else
    LED_2_PIN_OFF;

  if (LED_data&0X04)
    LED_3_PIN_ON;
  else
    LED_3_PIN_OFF;
}
