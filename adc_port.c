#include "adc_port.h"

void MM_adc1_init(void)
{
	//固定Vref = VDD, bandgap = 1.2V
	$ ADCC ENABLE, BANDGAP; //开启ADC模块, 并选择BANDGAP通道
	// $ADCC ENABLE, PB1; //开启ADC模块, 并选择PB1通道
	// $ ADCC DISABLE; //关闭ADC模块

	$ ADCM /8; //ADC时钟:Fsys/8
}

/* void MM_adc1_exit(void)
{
} */

void MM_adc1_start(void)
{
    ADCC |= 0X80; //开启ADC模块
}

void MM_adc1_suspend(void)
{
    ADCC &= ~0X80; //关闭ADC模块
}

/* void MM_adc1_set_ref(void)
{
    //设置参考电压
} */

uint16_t AD_DATA = 0;
void PADAUK_adc_convert(void)
{
	AD_DATA$1 = ADCRH; //读取
	AD_DATA$0 = ADCRL;
	AD_DATA >>= 4;
}
