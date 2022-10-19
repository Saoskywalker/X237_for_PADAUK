#ifndef _ADC_PORT_H
#define _ADC_PORT_H

#include "types_base.h"

//ADC vref define
#define MTF_ADC_VREF_VDD 4  /*Vref = Vdd*/
#define MTF_ADC_VREF_1P2V 3 /*Vref =1.2v*/
#define MTF_ADC_VREF_2V 2   /*Vref =2.0v*/
#define MTF_ADC_VREF_2P4V 1 /*Vref =2.4v*/
#define MTF_ADC_VREF_3V 0   /*Vref =3v*/

extern uint16_t AD_DATA;
void PADAUK_adc_convert(void);

void MM_adc1_init(void);
void MM_adc1_exit(void);
void MM_adc1_start(void);
void MM_adc1_suspend(void);
void MM_adc1_set_ref(void);
// #define MM_adc1_set_channel(x) ADC_EnableChannel(x)
#define MM_adc1_convert_start() AD_START = 1 //开启转换
#define MM_adc1_get_state() !AD_DONE
#define MM_adc1_get_result() AD_DATA

#endif
