#ifndef _PWM_BOARD_H
#define _PWM_BOARD_H

#include "pwm_port.h"

#define PWM_DUTY_0 0
#define PWM_DUTY_80 204

#define PWM_INIT() MM_pwm3_init()
#define PWM_MOTOR_START() MM_pwm3_start()
#define PWM_MOTOR_SUSPEND() MM_pwm3_suspend()
// #define PWM_MOTOR_SET_DUTY(x) MM_pwm3_set_duty(x)

#endif
