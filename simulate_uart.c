#include "simulate_uart.h"

#define SU_STOP_BIT 1   //停止位 0:1bit 1:1.5bit 2:2bit
#define SU_DATA_BIT 8   //数据位
#define SU_PAIRTY_BIT 0 // 0:none 1:奇校验 2:偶校验, (使1的数量是奇数或偶数)
#define SU_BAUD 9600    //波特率
#define SU_DELAY 1 //波特率延时

#define SU_TX_H() PB.4 = 1
#define SU_TX_L() PB.4 = 0

// static uint32_t delayTime = 99; //!< 9600，理论值为104但实际测下来99时效果最好
uint8_t su_data = 0, su_sent_cnt = 0;

/*!
 * @brief 	模拟串口1发送一个字节
 * @param
 * @return	none
 * @note	数据低位在前高位在后, 建议放于定时中断最前面, 保证时间准确性
 */
void simulate_uart_send(void)
{
    static uint8_t i = 0, su_step = 0, cnt = 0;

    // while (1)
    {
        if (su_sent_cnt)
        {
            if (su_step == 0)
            {
                SU_TX_L(); //!<起始位
                cnt++;
                if (cnt >= SU_DELAY) //延时
                {
                    cnt = 0;
                    su_step++;
                }
            }
            else if (su_step == 1)
            {
                if (i < 8)
                {
                    if (su_data & 0x01)
                        SU_TX_H();
                    else
                        SU_TX_L();

                    cnt++;
                    if (cnt >= SU_DELAY) //延时
                    {
                        cnt = 0;
                        su_data >>= 1;
                        i++;
                        if (i >= 8)
                        {
                            i = 0;
                            su_step++;
                        }
                    }
                }
            }
            else if (su_step == 2)
            {
                SU_TX_H(); //!<停止位
                cnt++;
                if (cnt >= SU_DELAY) //延时
                {
                    cnt = 0;
                    su_sent_cnt--;
                    su_step = 0;
                }
            }
        }
    }
}
