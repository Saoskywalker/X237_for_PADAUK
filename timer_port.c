#include "timer_port.h"
#include "types_base.h"

//开启定时器
uint16_t t16_cnt = 256-197; //中断定时基准, 200US
void MTF_timer_init_handle(void)
{
	//配置T16
	STT16 t16_cnt;
	$ T16M IHRC, /16, BIT8;

	INTRQ = 0; //清所有中断标志
	$ INTEN T16; // t16中断使能

	ENGINT; //允许全局中断
	// DISGINT; //停用全局中断
}

void MTF_timer_start(void)
{
	T16M |= 0X80; //开启定时器
}

void MTF_timer_suspend(void)
{
	T16M &= ~0XE0; //暂停定时器
}
