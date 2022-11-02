#include "gpio_board.h"

void main_IO_init(void)
{
    //--------------------端口配置--------------------
    // 0close, 1open, 使能数字输入和IO变化唤醒(非中断唤醒), AD功能时设为close可以防止漏电
    PADIER = 0B11111001; // bit1,2 nc
    PBDIER = 0B11111101; // PB1为电流AD引脚

    //设置输出电平
    LED_1_PIN_OFF;
    LED_2_PIN_OFF;
    LED_3_PIN_OFF;
    MOTOR_PIN_OFF;

    // PA0: IC4056_STDBY_PIN INPUT
    // PA3: LED_3_PIN OUPUT
    // PA4: IC4056_CHRG_PIN INPUT
    // PA5: POWER_KEY_PIN INPUT
    // PA6: LED_1_PIN OUTPUT
    PAC = 0B01001000; //设置PORT A方向, 0输入, 1输出

    // PA4, PA5开上拉
    PAPH = 0B00110000; //设置PORT A端口上拉, 0关, 1开

    PAPL = 0X00; //设置PORT A端口下拉, 0关, 1开

    // PB1: MOTOR_CURRENT_PIN INPUT
    // PB4: LED_2_PIN OUTPUT
    // PB7: MOTOR_PIN OUTPUT
    PBC = 0B10010000; //设置PORT B方向, 0输入, 1输出

    PBPH = 0X00; //设置PORT B端口上拉, 0关, 1开
    PBPL = 0X00; //设置PORT B端口下拉, 0关, 1开
}

//睡眠前设置gpio
void main_IO_exit(void)
{
    //设置输出电平
    LED_1_PIN_OFF;
    LED_2_PIN_OFF;
    LED_3_PIN_OFF;
    MOTOR_PIN_OFF;
}

#include "timer_port.h"
#include "gpio_board.h"
#include "pwm_board.h"
#include "adc_board.h"
#include "watch_dog_port.h"
#include "extern.h"

void gpio_test(void)
{
	static uint8_t ii = 0;
	
	if (POWER_KEY_PIN() == 0)
	{
		if (ii)
		{
			ii = 0;
			// LED_1_PIN_OFF;
		}
		else
		{
			ii = 1;
			// LED_1_PIN_ON;
		}
		// $ PA.6 TOGGLE;
	}
	else
	{
		// LED_1_PIN_OFF;

			MTF_watch_dog_exit();
			MTF_timer_suspend();
			MM_adc1_suspend();
			PWM_MOTOR_SUSPEND();
			main_IO_exit();

			STOPSYS; //进入掉电模式, 只能通过IO变化唤醒, 所有振荡器关闭, LVR无效
			
            // main_IO_init();
            PWM_MOTOR_START();
			MM_adc1_start();
			MTF_timer_start();
			MTF_watch_dog_init();
	}
}
