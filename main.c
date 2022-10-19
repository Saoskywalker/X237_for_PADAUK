#include "app_global.h"
#include "types_base.h"
#include "timer_port.h"
#include "gpio_board.h"
#include "pwm_board.h"
#include "adc_board.h"
#include "watch_dog_port.h"
#include "display_define.h"
#include "envent_define.h"
#include "app_timer.h"
#include "extern.h"

//定义输出文件名
.outfile "MTF_%S_SOP10_%X_%T.pdk"

//除法函数
#if 0
byte division_src, division_val, division_res;
void division_function(void)
{
	/***********************
	应广的MCU系列并不支持直接除法“/" 的运算方式，因此在程序中如果要使用除法运算，那么需要用其他的方式来实现。
	一，偶数除，可以通过右移实现，比如8/2，每右移一位即除2 。
	二，奇数除，可以通过减法实现，举例9/3，如下：
	9代表被除数dividend，3代表除数divisor，结果是res，运算开始：
	1, dividend - divisor = 9-3 = 6, res++, 即res =1; 把6赋值给dividend,进行第2轮。
	2, dividend - divisor = 6-3 = 3, res++, 即res = 2; 把3赋值给dividend,进行第3轮。
	3, dividend - divisor = 3-3 = 0，res++, 即res = 3; dividend = 0, 运算结束，结果是3。
	上述方法的思想是：dividend循环减divisor，每减一次res++，直至dividend =0,或者减法的结果小于divisor，运算停止，此时res就是最终结果值。
	再举例9/4 =2.25。
	1，dividend - divisor = 9-4 = 5, res++, 即res =1; 把5赋值给dividend,进行第2轮。
	2，dividend - divisor = 5-4 = 1, res++, 即res =2; 减法的结果是1<divisor 4，运算结束，最后的结果是2(不计浮点)。
	**********************/
	if (division_val & 0X01) //奇数
	{
		division_res = 0;
		while (division_src > division_val)
		{
			division_src = division_src - division_val;
			division_res++;
		}
	}
	else //偶数
	{
		// division_res = division_src >> division_val; //mini-c的>>运算符只支持常量
	}
}
#endif

// app_flag_type app_flag_= 0;//{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bit app_flag_sleep; //睡眠
bit app_flag_sys_ready; //系统准备完毕
bit app_flag_work; //工作
bit app_flag_error; //系统错误
bit app_flag_nc0;
bit app_flag_nc1;
bit app_flag_usb_inserted; //充电线插入
bit app_flag_charge_full; //充满电
bit app_flag_ntc_error; //ntc AD错误
bit app_flag_current_error; //电流AD错误
bit app_flag_battery_error; //电池AD错误
bit app_flag_temp_unit_C; //系统温度单位
bit app_flag_disp_battery_level; //显示电池电量
bit app_flag_nc2;
bit app_flag_nc3;
bit app_flag_nc4;
uint8_t app_mode = MODE_A;
uint8_t app_battery_level = BATTERY_FULL;
static uint8_t sleep_updata = 0; //用于标记在唤醒后一段时间内, 进行一定的唤醒处理

/*****************************
 * 电机控制输出
 * ********************************/
static uint8_t temp = 0;
#define PWM_MOTOR_SET_DUTY(x) temp = x
static void motor_function(void)
{
	static uint8_t pulse_count = 0, new_mode = 0;

	if (app_flag_work)
	{
		if (new_mode == MODE_A)
		{
            pulse_count++;
			if(pulse_count>=80)
			{
				pulse_count = 0;
				if(new_mode!=app_mode)
				{
					new_mode = app_mode;
					return;
				}
			}
			if (pulse_count>=40)
				PWM_MOTOR_SET_DUTY(PWM_DUTY_0);
			else
				PWM_MOTOR_SET_DUTY(PWM_DUTY_80);
		}
		else if (new_mode == MODE_B)
		{
            pulse_count++;
			if(pulse_count>=60)
			{
				pulse_count = 0;
				if(new_mode!=app_mode)
				{
					new_mode = app_mode;
					return;
				}
			}
			if (pulse_count>=30)
				PWM_MOTOR_SET_DUTY(PWM_DUTY_0);
			else
				PWM_MOTOR_SET_DUTY(PWM_DUTY_80);
		}
		else if (new_mode == MODE_C)
		{
            pulse_count++;
			if(pulse_count>=40)
			{
				pulse_count = 0;
				if(new_mode!=app_mode)
				{
					new_mode = app_mode;
					return;
				}
			}
			if (pulse_count>=20)
				PWM_MOTOR_SET_DUTY(PWM_DUTY_0);
			else
				PWM_MOTOR_SET_DUTY(PWM_DUTY_80);
		}
		else
		{
            pulse_count++;
			if(pulse_count>=20)
			{
				pulse_count = 0;
				if(new_mode!=app_mode)
				{
					new_mode = app_mode;
					return;
				}
			}
			if (pulse_count>=10)
				PWM_MOTOR_SET_DUTY(PWM_DUTY_0);
			else
				PWM_MOTOR_SET_DUTY(PWM_DUTY_80);
		}
	}
	else
	{
		PWM_MOTOR_SET_DUTY(PWM_DUTY_0);
		pulse_count = 0;
	}
}

/**************
 * 电池电量处理
 * *****************/
static void battery_deal(void)
{
	// Vref=3, 分压为1/2, Vin = 2*AD*3/4096+0.5(四舍五入), AD = (Vin)*4096/6+0.5(四舍五入)
	// Vin = ((ADC_BATTERY_VALUE()>>4)*300+256)/1280

	if (app_flag_sys_ready == 0 || sleep_updata)
	{
		if (ADC_BATTERY_VALUE() >= 4000) //voltage over high
		{
			app_battery_level = BATTERY_HIGH;
		}
		else if (ADC_BATTERY_VALUE() >= 2662) // 3.9V
		{
			app_battery_level = BATTERY_FULL;
		}
		else if (ADC_BATTERY_VALUE() >= 2560) // 3.75V
		{
			app_battery_level = BATTERY_LV2;
		}
		else if (ADC_BATTERY_VALUE() >= 2389) // 3.5V
		{
			app_battery_level = BATTERY_LV1;
		}
		else if (ADC_BATTERY_VALUE() >= 2219) // 3.25V
		{
			app_battery_level = BATTERY_LV0;
		}
		else // 2.9V
		{
			app_battery_level = BATTERY_LOSE;
		}
		return;
	}
	
	if(app_flag_usb_inserted)
	{
		//单向往上走
		if (ADC_BATTERY_VALUE() >= 4000) //voltage over high
		{
			app_battery_level = BATTERY_HIGH;
		}
		else if (ADC_BATTERY_VALUE() >= 2662) // 3.9V
		{
			app_battery_level = BATTERY_FULL;
		}
		else if (ADC_BATTERY_VALUE() >= 2560 && app_battery_level <= BATTERY_LV2) // 3.75V
		{
			app_battery_level = BATTERY_LV2;
		}
		else if (ADC_BATTERY_VALUE() >= 2389 && app_battery_level <= BATTERY_LV1) // 3.5V
		{
			app_battery_level = BATTERY_LV1;
		}
		else if (ADC_BATTERY_VALUE() >= 2219 && app_battery_level <= BATTERY_LV0) // 3.25V
		{
			app_battery_level = BATTERY_LV0;
		}
	}
	else
	{
		//单向往下走
		if (ADC_BATTERY_VALUE() >= 4000) //voltage over high
		{
			app_battery_level = BATTERY_HIGH;
		}
		else if (ADC_BATTERY_VALUE() >= 2662 && app_battery_level >= BATTERY_FULL) // 3.9V
		{
			app_battery_level = BATTERY_FULL;
		}
		else if (ADC_BATTERY_VALUE() >= 2560 && app_battery_level >= BATTERY_LV2) // 3.75V
		{
			app_battery_level = BATTERY_LV2;
		}
		else if (ADC_BATTERY_VALUE() >= 2389 && app_battery_level >= BATTERY_LV1) // 3.5V
		{
			app_battery_level = BATTERY_LV1;
		}
		else if (ADC_BATTERY_VALUE() >= 2219 && app_battery_level >= BATTERY_LV0) // 3.25V
		{
			app_battery_level = BATTERY_LV0;
		}
		else // 2.9V
		{
			app_battery_level = BATTERY_LOSE;
		}
	}
}

/*****************************
 * 睡眠处理
***************************/
static void sleep(void)
{
	uint16_t check_count = 0;
	
	if(app_flag_sleep)
	{
		MM_adc1_suspend();
		PWM_MOTOR_SUSPEND();
		main_IO_exit();

		while (app_flag_sleep)
		{
			MTF_sys_stop(); //进入休眠模式
			check_count = 0;
			while (check_count < 300)
			{
#ifndef DEBUG
				MTF_watch_dog_feed();
#endif
				sleep_updata = 1;
				if (app_timer_flag_10ms)
				{
					app_timer_flag_10ms = 0;
					event_produce();
					event_handle();
					check_count++;
				}
				if (app_flag_sleep == 0)
					break;
			}
		}
		
		main_IO_init();
		PWM_MOTOR_START();
		MM_adc1_start();
	}
}

//应广单核MCU的main函数, 多核FPPA0, FPPA1, FPPA2...
void FPPA0(void)
{	
	//应广MCU开始必有, 没有时IDE自动提醒配置并添加
	//--------------时钟配置--------------------
	//Init_RAM, 会把没有指定初始值的 RAM 全部清除，只有在 Mini-C 的项目才能另外指定初始值。
	//因旧版本编译器不支持全局变量定义同时进行初始化, 后来支持了, 故不用
	.ADJUST_IC	SYSCLK=IHRC/4, IHRC=16MHz, VDD=4.2V; //Bandgap=On;//Init_RAM;
	// .ADJUST_IC	SYSCLK=IHRC/4, IHRC=15.140MHz, VDD=5V;

	//复位RAM, 该程序段不能用子程序，因为清零了所有RAM，包括了堆栈，调用程序会无法返回。
	// word gwPoint;
	// a = 0;
	// gwPoint = _SYS(RAM_SIZE) - 1; //获取RAM大小
	// do
	// {
	// 	*gwPoint = a; //填充
	// } 
	// while (--gwPoint$0);
    ///////////////////////////////////////////////////////////
    
    uint8_t sys_read_delay = 0; 

	main_IO_init();
	PWM_INIT();
	MM_adc1_init();
	MTF_timer_init_handle();
    MTF_watch_dog_init();

	while (1)
	{
		if (app_timer_flag_2ms)
		{
			app_timer_flag_2ms = 0;
			adc_run();
		}

		if (app_timer_flag_10ms)
		{
			app_timer_flag_10ms = 0;
			// temp_deal();
			battery_deal();
			// motor_current_deal();
			event_produce();
			event_handle();
			motor_function();
			Led_display();
		}

		if (app_timer_flag_100ms)
		{
			app_timer_flag_100ms = 0;

			if(app_flag_sys_ready==0)
			{
                sys_read_delay++;
				if(sys_read_delay>=10) //上电等待系统稳定
				{
					sys_read_delay = 0;
					app_flag_sys_ready = 1;
					app_flag_sleep = 0;
					app_flag_work = 1;
				}
			}
			sleep_updata = 0;
		}
		
		sleep();

        //外设和程序测试
#if 1
        $ PB.4 TOGGLE;
        .delay 8000;
        gpio_test();
        
        //电池AD = 1.2*4096/VDD
        if (ADC_BATTERY_VALUE() >= 1328 - 10 && ADC_BATTERY_VALUE() <= 1328 + 10)
            PWM_MOTOR_START();
        else
            PWM_MOTOR_SUSPEND();
#endif
    }
}
