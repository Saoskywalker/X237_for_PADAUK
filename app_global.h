#ifndef _APP_GLOBAL_H
#define _APP_GLOBAL_H

#include "types_base.h"

// typedef struct
// {
//   uint8_t sleep : 1; //睡眠
//   uint8_t sys_ready : 1; //系统准备完毕
//   uint8_t work : 1; //工作
//   uint8_t error : 1; //系统错误
//   uint8_t sleep_updata : 1; //用于标记在唤醒后一段时间内, 进行一定的唤醒处理
//   uint8_t nc1 : 1;
//   uint8_t usb_inserted : 1; //充电线插入
//   uint8_t charge_full : 1; //充满电
//   uint8_t ntc_error : 1; //ntc AD错误
//   uint8_t current_error : 1; //电流AD错误
//   uint8_t battery_error : 1; //电池AD错误
//   uint8_t temp_unit_C : 1; //系统温度单位
//   uint8_t disp_battery_level : 1; //显示电池电量
//   uint8_t nc2 : 1;
//   uint8_t nc3 : 1;
//   uint8_t nc4 : 1;
// } app_flag_type;

// extern app_flag_type app_flag; //小众编译器不建议用结构体等, 有很多小毛病
extern bit app_flag_sleep; //睡眠
extern bit app_flag_sys_ready; //系统准备完毕
extern bit app_flag_work; //工作
extern bit app_flag_error; //系统错误
extern bit app_flag_sleep_updata; //用于标记在唤醒后一段时间内, 进行一定的唤醒处理
extern bit app_flag_nc1;
extern bit app_flag_usb_insert; //充电线插入
extern bit app_flag_charge_full; //充满电
extern bit app_flag_ntc_error; //ntc AD错误
extern bit app_flag_current_error; //电流AD错误
extern bit app_flag_battery_error; //电池AD错误
extern bit app_flag_temp_unit_C; //系统温度单位
extern bit app_flag_disp_battery_level; //显示电池电量
extern bit app_flag_nc2;
extern bit app_flag_nc3;
extern bit app_flag_nc4;

// typedef enum
// {
//   AD_NORMAL = 0,
//   AD_SHORT = 1,
//   AD_CUT = 2
// } AISTATUS;

#define AD_NORMAL 0
#define AD_SHORT 1
#define AD_CUT 2

// typedef struct
// {
//   int8_t C_value;
//   int8_t value;
//   uint16_t AD_value;
//   uint8_t status; // AISTATUS status;
// } ADC_PARA;

// extern ADC_PARA water_temp;

// typedef enum
// {
//   MODE_A = 0,
//   MODE_B,
//   MODE_C,
//   MODE_D
// };

// typedef enum
// {
//   BATTERY_LOSE = 0,
//   BATTERY_LV0,
//   BATTERY_LV1,
//   BATTERY_LV2,
//   BATTERY_FULL,
//   BATTERY_HIGH
// };

#define MODE_A 0
#define MODE_B 1
#define MODE_C 2
#define MODE_D 3

#define BATTERY_LOSE 0
#define BATTERY_LV0 1
#define BATTERY_LV1 2
#define BATTERY_LV2 3
#define BATTERY_FULL 4
#define BATTERY_HIGH 5

extern uint8_t app_mode;
extern uint8_t app_battery_level;

#endif
