#include "pwm_port.h"
#include "gpio_board.h"

void MM_pwm3_init(void)
{
	TM3CT = 0;
	TM3B = 204; //PWM模式下此为占空比, 周期固定为256
	TM3S = 2; //PWM 8bit, 2分频
	$ TM3C STOP;
}

/* void MM_pwm3_exit(void)
{
} */

void MM_pwm3_start(void)
{
	$ TM3C IHRC, PB7, PWM;
	// MOTOR_PIN_ON;
}

void MM_pwm3_suspend(void)
{
	$ TM3C STOP;
	MOTOR_PIN_OFF;
}
