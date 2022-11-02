#include "adc_board.h"
// #include "stdio.h"

// #define MTF_ADC_debug(...) printf(__VA_ARGS__)

uint16_t _adc_value_battery = 2047, _adc_value_motor = 2047;
void adc_run(void)
{
    static uint8_t channel_count = 0, _reject_count = 0;
    static uint16_t adc_result = 10800;
    static uint8_t sample_count = 255;
    static uint16_t _adc_table[8];
    static uint16_t point_adc_table = 0;
    static uint8_t _adc_table_cnt = 0;

    if (sample_count >= 8)
    {
        sample_count = 0;                            //单通道采样次数
        _reject_count = 0;
        point_adc_table = _adc_table;
        // _adc_value[channel_count] = adc_result >> 3; // adc_result/8, 注意复位的第一次值
        //因在51 MCU上 _adc_value[channel_count] 处理慢
        if (channel_count == 0)
            _adc_value_battery = adc_result >> 3;
        else if (channel_count == 1)
            _adc_value_motor = adc_result >> 3;
        adc_result = 0;

        channel_count++;
        if (channel_count >= ADC_CHANNEL_TOTAL)
        {
            channel_count = 0;
            // printf("ad: %d, %d, %d\r\n", _adc_value[0], _adc_value[1], _adc_value[2]);
        }

        //为保证结果准确度, 切换通道后建议500ns后开采样
        if (channel_count == 0) // battery
        {
            // MM_adc1_set_ref(MTF_ADC_VREF_3V);
            // MM_adc1_set_channel(MTF_ADC_CH5);
        }
        else // NTC
        {
            // MM_adc1_set_ref(MTF_ADC_VREF_VDD);
            // MM_adc1_set_channel(MTF_ADC_CH14);
        }
    }
    else
    {
        _reject_count++;
        if (_reject_count >= 2)
        {
            // if (MM_adc1_get_state() == 0) //是否转换完成
            {
                sample_count++;
                PADAUK_adc_convert();
                adc_result += MM_adc1_get_result(); //获取AD值

                //滑动滤波, 未分通道, 只能单通道使用
                // *point_adc_table = MM_adc1_get_result();
                // point_adc_table = adc_result+(_adc_table_cnt<<1); //mini-c注意: 指针加1总是跳一个字节地址
                // _adc_table_cnt++;
                // if (_adc_table_cnt>=8)
                //     _adc_table_cnt = 0;
                // if (sample_count >= 8)
                // {
                //     sample_count = 0;
                //     point_adc_table = _adc_table;
                //     adc_result = 0;
                //     while (sample_count < 8)
                //     {
                //         sample_count++;
                //         adc_result += *point_adc_table;
                //         point_adc_table += 2; // mini-c注意: 指针加1总是跳一个字节地址
                //     }
                //     _adc_value_battery = adc_result >> 3;
                //     _reject_count = 2;
                //     sample_count--;
                // }
            }
        }
        MM_adc1_convert_start(); //开启转换
    }
}
