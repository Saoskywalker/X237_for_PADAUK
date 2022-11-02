#ifndef _PWM_BOARD_H
#define _PWM_BOARD_H

#include "pwm_port.h"

#define PWM_DUTY_0 0
#define PWM_DUTY_50 130
#define PWM_DUTY_80 204

#define PWM_INIT() MM_pwm3_init()
#define PWM_MOTOR_START() //MM_pwm3_start(); TM3B = 0 //PWM开启时默认无波形输出
#define PWM_MOTOR_SUSPEND() MM_pwm3_suspend()
#define PWM_MOTOR_SET_DUTY(x) {if(x==PWM_DUTY_0) \
                                {MM_pwm3_suspend();} \
                                else \
                                {MM_pwm3_start(); TM3B = x;}}

#endif
