/************************************************
mini-c编译器: mini-c编译器是ANSI C的标准, 但部分语法不支持, 厂家也根据自己需求添加了一些新语法
缺点:
1. 不支持乘除运算符;
2. <<,>>移位参数不支持变量, 只支持常量;
3. 函数不能带传入参数, 和反回参数;
4. 基本数据类型只支持无符号类型, 无浮点类型;
5. ++, --运算符只支持后置运算, 如: ++i;错误 i++;正确;
6. if, while, switch判断式里, 不支持执行算数操作;
7. 不支持 i = !i; //不支持!运算符, 但支持if(!i);
8. 注意数组和指针的操作;
9. 不支持for循环;
10. Bit变量只支持写入0,或1,不支持直接bit变量取反;
11. 在小众编译器中, 均不建议使用结构体和联合体, 会有很多小毛病;
12. bit类型不能自行翻转: i=!i; i=~i; 均不可
13. #include <>错误; 只支持#include ""
14. uint16_t i = 0; i&0X01;报错, i$0 &0X01; 可以

特点:
1. 寄存器配置推荐使用$, 操作类似函数, 在.INC文件中有相关定义, 使用时, 部分字段缺省值自动设置为寄出器默认值,
	详细可参考IDE上的使用手册;
2. IDE上的使用手册可解决大部分问题和快速入门;
3. $和.为mini-c独有运算符可看使用手册;
4. 部分关键字不分大小写;
5. 应广的MCU程序通用性比较好, 一样的程序, 换应广另外的IC, 大部分只需要在IDE里面选择型号, 和相关配置即可;

https://blog.csdn.net/zhongvv/article/details/103610995
https://blog.csdn.net/weixin_35620685/article/details/117176849
https://blog.csdn.net/zhongvv/article/details/104310403
https://blog.csdn.net/zhongvv/article/details/124861520?spm=1001.2014.3001.5502
https://blog.csdn.net/penglijiang/article/details/118674337
************************************************/

//===============================================================================
//PMC150-S08A SOP8 150mil 引脚图    第一版 OK
//1 VDD            8 GND
//2 PA7            7 PA0/INT0
//3 PA6            6 PA4
//4 PA5/PRST       5 PA3

//PMS131-S08A SOP8 150mil 引脚图                             供应商给的规格书 OK
//1 VDD            8 GND
//2 PA6/X2         7 PA4/AD9/INT1A
//3 PA5/PRSTB      6 PA3/AD8/PWM1
//4 PB7/AD7/PWM2   5 PB1/AD1/Vref

//===============================================================================
//include---------------------------------------------------------------
#include "types_base.h"
#include "gpio_board.h"
#include "watch_dog_port.h"
#include "extern.h"

//定义输出文件名
.outfile "MTF_%S_SOP10_%X_%T.pdk"

// .ramadr 0

//.writer	package 8,1,32,6,7,3,2,32,8,0x000F,0x000F,0

//PMS132-S08A SOP8 150mil 引脚图      供应商给的规格书


CHARG  equ PA.4//PIN5  输出
PWM  equ PB.7//PIN5  输出

LED2   equ PB.4//PIN6  输出
LED1  equ PA.6//PIN2  输出
AP					equ pa.0//PIN4  输出
ATO_AD_IN equ  PB.1//PIN7  输入
Key_In    equ   PA.5//PIN3  输入    上拉
LED3    equ   PA.3//PIN3  输入    上拉
/*
//PMC150-S08A SOP8 150mil 引脚图    第一版 OK
#define KM_PTC_Out    PA.0//PIN7/KM_PTC /AD10/INT0  输出
#define FAN_Out       PA.3//PIN5/FAN    /AD8/PWM1   输出
#define UV_Out        PA.4//PIN6/UV     /AD9/INT1A  输出
#define Key_In        PA.5//PIN4/KEY    /PRSTB      输入    上拉
#define LED_Out       PA.6//PIN3/LED    /X2         输出
#define PA_7_In       PA.7//PIN2/空     /X1         输入    上拉
*/
//--------------------全局变量定义--------------------
// DWORD	div_src4;
// WORD	div_val2, div_res2;
// BYTE   Sys_Flag=0;//
// bit flag_power_low    : 	Sys_Flag.0
// bit flag_sleep    : 	Sys_Flag.1
// bit flag_50_ms    : 	Sys_Flag.2
// bit flag_bl1_status    : 	Sys_Flag.3	
// bit press_flag    : 	Sys_Flag.4
// bit pressed_flag    : 	Sys_Flag.5
// bit flag_motor_work_on    : 	Sys_Flag.6
// bit flag_power_off    : 	Sys_Flag.7
// BYTE   Sys_Flag1=0;//
// bit flag_pwm_on    : 	Sys_Flag1.0
// bit flag_led1    : 	Sys_Flag1.1
// bit flag_led2    : 	Sys_Flag1.2
// bit flag_led3    : 	Sys_Flag1.3	
// bit flag_pwm_runed    : 	Sys_Flag1.4

/*bit pressed_flag    : 	Sys_Flag1.5
bit flag_motor_work_on    : 	Sys_Flag1.6
bit flag_power_off    : 	Sys_Flag1.7	*/
	
// WORD work_led_blink_time;
// BYTE led_blink_times;
// byte put_Electricity;
// BYTE Interval_time_scan;
// BYTE T_debounce;
// BYTE key_status;
// BYTE key_long_pressed;
// BYTE key_pressed;
// BYTE key_pressing_time;
// BYTE DAT_16;
// BYTE timer0_num;
// BYTE TIME_200MS_B;
// BYTE key_idle_time;
// BYTE key_pressed_times;
// BYTE double_pressed_wait_time;
// byte timer1_count;
// WORD motor_work_time;
// WORD ap_high_time;
// byte batt_adc_delay;
// byte led_duty;
// byte motor_duty;
// word VDD_PA4;
// word BG_data;
// byte batt_adc_wait_time;

// void TaskAdcato(void);
// void TaskAdcbat(void);
//--------------------------------------------------AD

D_AD_SAMPLE_NUM equ 6	 // AD采样样品的个数
	D_AD_SHIFT_NUM equ 2 //这个值设定和D_AD_SAMPLE_NUM相关

	D_ato_TIMES equ D_AD_SAMPLE_NUM //这里的值必须和采样个数值相同(平均值计算采样次数)

		D_ato_CHANGE_TIMES equ 5

	D_bat_TIMES equ D_AD_SAMPLE_NUM //这里的值必须和采样个数值相同(平均值计算采样次数)

		D_bat_CHANGE_TIMES equ 5

void MM_adc1_init(void)
{
	//Vref = VDD, bandgap = 1.2V
	$ ADCC ENABLE, BANDGAP; //开启ADC模块, 并选择BANDGAP通道
	// $ADCC ENABLE, PB1; //开启ADC模块, 并选择PB1通道
	// $ ADCC DISABLE; //关闭ADC模块

	$ ADCM /8; //ADC时钟:Fsys/8
}

#define ADC_INIT() MM_adc1_init()

static uint16_t AD_DATE = 0;
void startAD(void)
{
	AD_START = 1; //开启转换
	while (!AD_DONE) NULL; //等待
	//	gwAdVal = (adcrh << 8 | adcrl);
	//	gwAdVal =  gwAdVal >> 4;
	AD_DATE$1 = ADCRH; //读取
	AD_DATE$0 = ADCRL;
	AD_DATE >>= 4;
}

uint16_t t16_cnt = 256-197; //中断定时基准, 200US
void MTF_timer_init_handle(void)
{
	//配置T16
	STT16 t16_cnt;
	$ T16M IHRC, /16, BIT8;

	INTRQ = 0; //清所有中断标志
	$ INTEN T16; // t16中断使能
}

void MM_pwm3_init(void)
{
	TM3B = 204; //PWM模式下此为占空比, 周期固定为256
	TM3S = 2; //PWM 8bit, 2分频
	$ TM3C STOP;
}

void MM_pwm3_start(void)
{
	$ TM3C IHRC, PB7, PWM;
	MOTOR_PIN_ON;
}

void MM_pwm3_suspend(void)
{
	$ TM3C STOP;
	MOTOR_PIN_OFF;
}

#define PWM_INIT() MM_pwm3_init()
#define PWM_MOTOR_START() MM_pwm3_start()
#define PWM_MOTOR_SUSPEND() MM_pwm3_suspend()

void	TM2_Period(void)
{
	TM2CT = 0;						//计数寄存器
	TM2B = 49;						//上限寄存器，计数值超出上限寄存器的设定值时会产生中断；
	$ TM2C SYSCLK,Disable,Period;	//时钟源选择，输出脚选择，模式选择定周期模式；
									//时钟源可选择STOP, SYSCLK, IHRC, EOSC, ILRC, GPCRS, PA0_R, PA0_F, PB0_R, PB0_F, PA4_R, PA4_F
									//注：产生中断时不需要输出脚和反极性；
	$ TM2S 8BIT,/1,/25;				//分辨率选择，可以选择8bit或6bit；时钟预分频选择，有/1, /4, /16, /64；时钟分频选择/1 ~ /32
					//计算公式为 1/{时钟源*2/[2*(TM2B+1)*预分频*分频]}
					//	 	   = 1/(1M*2/(2*TM2B+1)*4*25) = 1/(1000000*2/(2*(49+1)*4*25)) = 0.005s
}

// typedef struct
// {
//   uint8_t sleep : 1; //睡眠
//   uint8_t sys_ready : 1; //系统准备完毕
//   uint8_t work : 1; //工作
//   uint8_t error : 1; //系统错误
//   uint8_t nc0 : 1;
//   uint8_t nc1 : 1;
//   uint8_t usb_insert : 1; //充电线插入
//   uint8_t charge_full : 1; //充满电
//   uint8_t ntc_error : 1; //ntc AD错误
//   uint8_t current_error : 1; //电流AD错误
//   uint8_t battery_error : 1; //电池AD错误
//   uint8_t temp_unit_C : 1; //系统温度单位
//   uint8_t disp_battery_level : 1; //显示电池电量
//   uint8_t nc2 : 1;
//   uint8_t nc3 : 1;
//   uint8_t nc4 : 1;
// } EarClean_flag_type;
// EarClean_flag_type ioio={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

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

	main_IO_init();
	PWM_INIT();
	ADC_INIT();
	MTF_timer_init_handle();
	
	// TM2_Period();
	// INTEN.TM2 = 1; //中断允许寄存器，开TM3中断
	// 			   //	INTEN.TM3 = 0;				//中断允许寄存器，关TM3中断
	// INTRQ.TM2 = 0; //中断请求寄存器，清零INTRQ寄存器。
				   //------------------------------------------------------------
	MTF_watch_dog_init();
	// INTRQ = 0; //清除INTRQ
	ENGINT; //允许全局中断
	// DISGINT; //停用全局中断

	while (1)
	{
		$ PB.4 TOGGLE;
		MTF_watch_dog_feed();
		.delay 4000;
		gpio_test();

		startAD();
		if (AD_DATE>=1328-5&&AD_DATE<=1328+5)
			PWM_MOTOR_START();
		else
			PWM_MOTOR_SUSPEND();
		
		// if (work)
		{
			MTF_watch_dog_exit();
			$ T16M STOP;
			$ ADCC DISABLE;
			PWM_MOTOR_SUSPEND();
			main_IO_exit();
			while (POWER_KEY_PIN())
			{
				STOPSYS; //进入掉电模式, 只能通过IO变化唤醒, 所有振荡器关闭, LVR无效
			}
			
			ADC_INIT();
			MTF_timer_init_handle();
			MTF_watch_dog_init();
		}
	}
}

static uint8_t cnt_1ms = 0;
void Interrupt(void)
{
	PUSHAF; //存储ALU和FLAG寄存器
	
	if (INTRQ.TM2)
	{
		INTRQ.TM2 = 0;
	}

	if (INTRQ.T16) // T16 Trig
	{
		STT16 t16_cnt;
		INTRQ.T16 = 0;

		$ PA.3 TOGGLE;
		cnt_1ms++;
		if (cnt_1ms>=10)
		{
			cnt_1ms = 0;
			
		}
	}

	POPAF; //恢复ALU和FLAG寄存器
}
