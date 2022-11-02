#include "app_timer.h"
#include "gpio_board.h"
#include "display_define.h"
#include "key_board.h"
#include "simulate_uart.h"

// timer_flag_type app_timer_flag= 0;// = {0, 0, 0, 0, 0, 0, 0, 0};
bit app_timer_flag_200us;
bit app_timer_flag_2ms;
bit app_timer_flag_10ms;
bit app_timer_flag_100ms;

void app_timer(void)
{
    static uint8_t count_2ms = 0, count_10ms = 0, count_100ms = 0;

    // 200us int
    app_timer_flag_200us = 1;
    // TEST_TOGGLE_PIN();
    // $ PA.6 TOGGLE;

    // simulate_uart_send();
    key_input_check_timer();
    
    count_2ms++;
    if (count_2ms >= 10)
    {
        count_2ms = 0;
        app_timer_flag_2ms = 1;
        
        Led_Scan();

        count_10ms++;
        if (count_10ms >= 5)
        {
            count_10ms = 0;
            app_timer_flag_10ms = 1;

            count_100ms++;
            if(count_100ms>=10)
            {
                count_100ms = 0;
                app_timer_flag_100ms = 1;
            }
        }
    }
}
