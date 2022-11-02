#ifndef _GPIO_BOARD_H
#define _GPIO_BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "types_base.h"

/****************************
应用定义接口
*****************************/
//io定义
//OUTPUT
#define LED_1_PIN(x) PA.6 = x
#define LED_2_PIN(x) PB.4 = x
#define LED_3_PIN(x) PA.3 = x
#define MOTOR_PIN(x) PB.7 = x

#define LED_1_PIN_ON LED_1_PIN(0)
#define LED_2_PIN_ON LED_2_PIN(0)
#define LED_3_PIN_ON LED_3_PIN(0)
#define MOTOR_PIN_ON MOTOR_PIN(1)

#define LED_1_PIN_OFF LED_1_PIN(1)
#define LED_2_PIN_OFF LED_2_PIN(1)
#define LED_3_PIN_OFF LED_3_PIN(1)
#define MOTOR_PIN_OFF MOTOR_PIN(0)

//INPUT
#define MOTOR_CURRENT_PIN() PB.1
#define IC4056_CHRG_PIN() PA.4
#define IC4056_STDBY_PIN() PA.0
#define POWER_KEY_PIN() PA.5

void main_IO_init(void);
void main_IO_exit(void);
///////////////////////////////

#define TEST_TOGGLE_PIN() $ PA.6 TOGGLE //应广不支持51式IO翻转, 此语句不支持宏
#define TEST_PIN(x) LED_1_PIN(x) //测试引脚

void gpio_test(void);

#ifdef __cplusplus
}
#endif

#endif
