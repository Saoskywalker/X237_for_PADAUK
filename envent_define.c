#include "envent_define.h"
#include "key_board.h"
#include "app_global.h"

uint16_t sys_envent = 0;

//-----------------------------------------------------------
//	事件产生
//-----------------------------------------------------------
void event_produce(void)
{
    uint8_t key = 0;
    static uint8_t power_key_long_count = 0;
    static uint8_t key_old = 0;

    key = key_get_result();

    if ((key & 0X01) && !(key_old & 0X01)) //单击电源键
    {
        sys_envent |= DIS_BATTERY;
        sys_envent |= POWER_KEY;
    }

    if ((key & 0X04)) //插入充电线
        sys_envent |= USB_INSERT;
    else
        sys_envent |= USB_NO_INSERT;

    if ((key & 0X08)) //充满电
        sys_envent |= CHANGE_FULL;

    if (key & 0X01) //电源键长按
    {
        power_key_long_count++;
        if (power_key_long_count >= 200) // 2s
        {
            power_key_long_count = 0;
            // sys_envent |= POWER_KEY;
        }
    }
    else
    {
        power_key_long_count = 0;
    }

    key_old = key;
}

//-----------------------------------------------------------
//	事件处理
//-----------------------------------------------------------
void event_handle(void)
{
    static uint16_t _30s_cnt = 0;
    uint16_t temp = 1;

    if (app_flag_sys_ready==0)
        return;

    while (temp)
    {   
        switch (sys_envent & temp) //注意事件顺序
        {
        case MODE_KEY:
            if(app_flag_work)
            {
                app_mode++;
                if (app_mode > MODE_D)
                    app_mode = MODE_A;
            }
            break;
        case POWER_KEY:
            if (app_flag_usb_insert == 0)
            {
                if (app_flag_work)
                {
                    app_flag_work = 0;
                }
                else
                {
                    if (app_battery_level > BATTERY_LOSE)
                    {
                        app_flag_work = 1;
                        app_flag_sleep = 0;
                    }
                }
            }
            break;
        case USB_INSERT:
            app_flag_sleep = 0;
            app_flag_charge_full = 0;
            app_flag_usb_insert = 1;
            app_flag_work = 0;
            break;
        case USB_NO_INSERT:
            app_flag_usb_insert = 0;
            break;
        case DIS_BATTERY:
            app_flag_sleep = 0;
            app_flag_disp_battery_level = 1;
            break;
        case CHANGE_TEMP_UNIT:
            if (app_flag_temp_unit_C)
                app_flag_temp_unit_C = 0;
            else
                app_flag_temp_unit_C = 1;
            break;
        case CHANGE_FULL:
            app_flag_charge_full = 1;
            break;
        default:
            break;
        }
        temp <<= 1;
    }
    sys_envent = 0;

    if (app_battery_level <= BATTERY_LV0 && app_flag_work) //低电关机策略, 30s后关机
    {
        _30s_cnt++;
        if (_30s_cnt >= 3000)
        {
            _30s_cnt = 0;
            app_flag_work = 0;
        }
    }
    else
    {
        _30s_cnt = 0;
    }

    //error handle
    if(app_flag_current_error) //电流错误
        app_flag_work = 0;
    // if (app_battery_level <= BATTERY_LOSE) //电池耗尽停止工作
    //     app_flag_work = 0;
}
