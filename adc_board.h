#ifndef _ADC_BOARD_H
#define _ADC_BOARD_H

#include "types_base.h"
#include "adc_port.h"

#define ADC_CHANNEL_TOTAL 1 //ADC需扫描通道数
extern uint16_t _adc_value_battery, _adc_value_motor;
#define ADC_BATTERY_VALUE() _adc_value_battery
#define ADC_MOTOR_CURRENT_VALUE() _adc_value_motor

void adc_run(void);

#endif
