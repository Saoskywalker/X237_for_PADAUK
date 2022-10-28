#include "key_board.h"
#include "gpio_board.h"

uint8_t key_input_res = 0;

#define KEY_PREVENT_SHAKE_TIME 50 //按键消抖时间10ms
void key_input_check_timer(void) //定期运行输入检查
{
    static uint8_t i = 0, charge_full = 0;
    static uint16_t k = 0, l = 0;

    key_input_res = 0;
    
    if (POWER_KEY_PIN()) //弹起
    {
        i = 0;
    }
    else //按下
    {
        i++;
        if (i >= KEY_PREVENT_SHAKE_TIME)
        {
            i = KEY_PREVENT_SHAKE_TIME;
            key_input_res |= 0X01;
        }
    }

    if (CHARGE_CHECK_PIN()) // usb插入
    {
        k++;
        if (k >= 2000) //400ms
        {
            k = 2000;
            key_input_res |= 0X04;
        }
    }
    else // usb拔掉
    {
        if (k > 0)
        {
            key_input_res |= 0X04;
            k--;
            // if (k > 500)
            //     k = 500;
        }
    }

    /********
     *  4054/4056充电指示:CHRG为充满状态, STD为外部电源插入状态, IO均设为输入上拉
        充电状态:CHRG=0, STD=1;
        充满状态:CHRG=1, STD=0;
        改变PROG引脚电阻可改变充电电流
     * **********/
    if(CHARGE_FULL_PIN())
    {
        l++;
        if (l >= 1000) //200ms
        {
            l = 1000;
            key_input_res |= 0X08;
        }
    }
    else
    {
        if (l > 0)
        {
            key_input_res |= 0X08;
            l--;
        }
    }
}
