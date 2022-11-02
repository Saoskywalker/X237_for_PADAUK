#include "key_board.h"
#include "gpio_board.h"

uint8_t key_input_res = 0;

#define KEY_PREVENT_SHAKE_TIME 50 //按键消抖时间10ms
void key_input_check_timer(void) //定期运行输入检查
{
    static uint8_t i = 0;
    static uint16_t k = 0, l = 0;
    
    if (POWER_KEY_PIN()) //弹起
    {
        i = 0;
        key_input_res &= ~bit0;
    }
    else //按下
    {
        i++;
        if (i >= KEY_PREVENT_SHAKE_TIME)
        {
            i = KEY_PREVENT_SHAKE_TIME;
            key_input_res |= bit0;
        }
    }

    if (IC4056_STDBY_PIN()) // usb插入
    {
        k++;
        if (k >= 2000) //400ms
        {
            k = 2000;
            key_input_res |= bit2;
        }
    }
    else // usb拔掉
    {
        if (k > 0)
        {
            k--;
            // if (k > 500)
            //     k = 500;
        }
        else
        {
            key_input_res &= ~bit2;
        }
    }

    /********
        4054/4056充电指示: MCU IO均设为输入上拉
        断开充电器: CHRG=1, STD=1;
        充电中: CHRG=0, STD=1;
        充满状态:CHRG=1, STD=0;
        改变PROG引脚电阻可改变充电电流
     * **********/
    if(IC4056_CHRG_PIN())
    {
        l++;
        if (l >= 1000) //200ms
        {
            l = 1000;
            key_input_res |= bit3;
        }
    }
    else
    {
        if (l > 0)
            l--;
        else
            key_input_res &= ~bit3;
    }
}
