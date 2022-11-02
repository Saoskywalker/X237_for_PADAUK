#include "display_define.h"
#include "app_global.h"

/***********************************************
 * 显示放电剩余电量
*************************************************/
static void dis_residue_battery(void)
{
  //显示放电剩余电量
  if (app_battery_level <= BATTERY_LV1)
  {
    led1_locate;
  }
  else if (app_battery_level <= BATTERY_LV2)
  {
    led2_locate;
    led1_locate;
  }
  else if (app_battery_level <= BATTERY_FULL)
  {
    led3_locate;
    led2_locate;
    led1_locate;
  }
}

/*************************************************
 // 函数名称    : LedDsp_content
 // 功能描述    : 用户定义显示函数
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
static void LedDsp_content(void)
{
  static uint8_t _dis_charge_500ms = 0, _dis_charge_cnt = 0;
  static uint8_t _dis_500ms_cnt = 0, _dis_500ms = 0, _flash_cnt = 0, _dis_2s_cnt = 0;

  if(app_flag_sys_ready==0)
  {
    Led_dis_All(); //上电准备时全显
    return;
  }

  Led_Clear_All(); //清除所有显示数据

  if (app_flag_usb_insert) //整体策略上禁止充电时睡眠
  {
    if (app_flag_charge_full)
    {
      led3_locate;
      led2_locate;
      led1_locate;
    }
    else
    {
      //显示充电电量
      _dis_charge_500ms++;
      if (_dis_charge_500ms >= 50)
      {
        _dis_charge_500ms = 0;

        _dis_charge_cnt++;
        if (_dis_charge_cnt >= 4)
        {
          if (app_battery_level <= BATTERY_LV1)
          {
            _dis_charge_cnt = 0;
          }
          else if (app_battery_level <= BATTERY_LV2)
          {
            _dis_charge_cnt = 1;
          }
          else if (app_battery_level <= BATTERY_FULL)
          {
            _dis_charge_cnt = 2;
          }
        }
      }
      if (_dis_charge_cnt == 1)
      {
        led1_locate;
      }
      else if (_dis_charge_cnt == 2)
      {
        led2_locate;
        led1_locate;
      }
      else if (_dis_charge_cnt == 3)
      {
        led3_locate;
        led2_locate;
        led1_locate;
      }
    }
  }
  else
  {
    _dis_charge_cnt = 0;
    _dis_charge_500ms = 0;

    if (app_battery_level <= BATTERY_LV0 || app_flag_work == 0)
    {
      _dis_500ms_cnt++;
      if (_dis_500ms_cnt >= 50)
      {
        _dis_500ms_cnt = 0;

        if (app_battery_level <= BATTERY_LOSE)
          _flash_cnt++;
        else
          _flash_cnt = 0;

        if (app_battery_level <= BATTERY_LV0)
          _dis_500ms ^= 0X01;

        if (app_flag_work == 0)
          _dis_2s_cnt++;
        else
          _dis_2s_cnt = 0;
      }
    }
    else
    {
      _dis_500ms_cnt = 0;
      _flash_cnt = 0;
      _dis_2s_cnt = 0;
      _dis_500ms = 0;
    }

    if (app_battery_level <= BATTERY_LV0)
    {
      if (_dis_500ms) //低电闪烁
        Led_dis_All();
    }
    else
    {
      dis_residue_battery();
    }

    if (app_flag_work == 0)
    {
      if (app_battery_level <= BATTERY_LOSE)
      {
        if (_flash_cnt >= 6) //低电睡眠策略, 闪3次后睡眠
        {
          app_flag_disp_battery_level = 0;
        }
      }
      else
      {
        if (_dis_2s_cnt >= 4) //正常睡眠策略, 延时2s
        {
          app_flag_disp_battery_level = 0;
        }
      }
    }

    if (!app_flag_disp_battery_level)
    {
      Led_Clear_All();
      _dis_500ms_cnt = 0;
      _flash_cnt = 0;
      _dis_2s_cnt = 0;
      _dis_500ms = 0;
      app_flag_sleep = 1; //进入睡眠命令统一由此发出, 由sleep运行event_handle退出, 流程为, 先停止工作(但可显示), 再进睡眠
    }
  }
}

/*************************************************
 // 函数名称    : Led_display
 // 功能描述    :
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Led_display(void)
{ 
  LedDsp_content();
  Led_Dis_Update();
}
