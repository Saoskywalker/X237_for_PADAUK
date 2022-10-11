//===============================================================================
//PMC150-S08A SOP8 150mil ����ͼ    ��һ�� OK
//1 VDD            8 GND
//2 PA7            7 PA0/INT0
//3 PA6            6 PA4
//4 PA5/PRST       5 PA3

//PMS131-S08A SOP8 150mil ����ͼ                             ��Ӧ�̸��Ĺ���� OK
//1 VDD            8 GND
//2 PA6/X2         7 PA4/AD9/INT1A
//3 PA5/PRSTB      6 PA3/AD8/PWM1
//4 PB7/AD7/PWM2   5 PB1/AD1/Vref

//===============================================================================
//include---------------------------------------------------------------
#include  "extern.h"

.outfile "PFS122_%S_SOP10_%X_%T.pdk"
//--------------------�˿ڶ���--------------------
//PA    7  6  5  4  3  - -  0   //�˿����� (����ģʽ)������˿ڵĸߵ����� (���ģʽ)�Ӷ˿�����ߵ�����
//PAC   0  1  0  1  1  0 0  1   //1���  0����
//PAPH  1  0  1  0  0  1 1  0   //1����  0����

//PMS131-S08A SOP8 150mil ����ͼ  
//1 VDD            8 GND
//2 PA6/X2         7 PA4/AD9/INT1A
//3 PA5/PRSTB      6 PA3/AD8/PWM1
//4 PB7/AD7/PWM2   5 PB1/AD1/Vref
.ramadr 0
word 	gwPoint;
word 	gw_Point;
D_INT_BASE				equ	88				//�ж϶�ʱ��׼,100US
//.writer	package 8,1,32,6,7,3,2,32,8,0x000F,0x000F,0

//PMS132-S08A SOP8 150mil ����ͼ      ��Ӧ�̸��Ĺ����


CHARG  equ PA.4//PIN5  ���
PWM  equ PB.7//PIN5  ���

LED2   equ PB.4//PIN6  ���
LED1  equ PA.6//PIN2  ���
AP					equ pa.0//PIN4  ���
ATO_AD_IN equ  PB.1//PIN7  ����
Key_In    equ   PA.5//PIN3  ����    ����
LED3    equ   PA.3//PIN3  ����    ����
/*
//PMC150-S08A SOP8 150mil ����ͼ    ��һ�� OK
#define KM_PTC_Out    PA.0//PIN7/KM_PTC /AD10/INT0  ���
#define FAN_Out       PA.3//PIN5/FAN    /AD8/PWM1   ���
#define UV_Out        PA.4//PIN6/UV     /AD9/INT1A  ���
#define Key_In        PA.5//PIN4/KEY    /PRSTB      ����    ����
#define LED_Out       PA.6//PIN3/LED    /X2         ���
#define PA_7_In       PA.7//PIN2/��     /X1         ����    ����
*/
//--------------------ȫ�ֱ�������--------------------
DWORD	div_src4;
WORD	div_val2, div_res2;
BYTE   Sys_Flag=0;//
bit flag_power_low    : 	Sys_Flag.0
bit flag_sleep    : 	Sys_Flag.1
bit flag_50_ms    : 	Sys_Flag.2
bit flag_bl1_status    : 	Sys_Flag.3	
bit press_flag    : 	Sys_Flag.4
bit pressed_flag    : 	Sys_Flag.5
bit flag_motor_work_on    : 	Sys_Flag.6
bit flag_power_off    : 	Sys_Flag.7
BYTE   Sys_Flag1=0;//
bit flag_pwm_on    : 	Sys_Flag1.0
bit flag_led1    : 	Sys_Flag1.1
bit flag_led2    : 	Sys_Flag1.2
bit flag_led3    : 	Sys_Flag1.3	
bit flag_pwm_runed    : 	Sys_Flag1.4
/*bit pressed_flag    : 	Sys_Flag1.5
bit flag_motor_work_on    : 	Sys_Flag1.6
bit flag_power_off    : 	Sys_Flag1.7	*/
	
WORD work_led_blink_time;
BYTE led_blink_times;
byte put_Electricity;
BYTE Interval_time_scan;
BYTE T_debounce;
BYTE key_status;
BYTE key_long_pressed;
BYTE key_pressed;
BYTE key_pressing_time;
BYTE DAT_16;
BYTE timer0_num;
BYTE TIME_200MS_B;
BYTE key_idle_time;
BYTE key_pressed_times;
BYTE double_pressed_wait_time;
byte timer1_count;
WORD motor_work_time;
WORD ap_high_time;
byte batt_adc_delay;
byte led_duty;
byte motor_duty;
word VDD_PA4;
word BG_data;
byte batt_adc_wait_time;
#define ON  0
#define OFF  1
void TaskAdcato(void);
void TaskAdcbat(void);
//--------------------------------------------------AD
static	void	Word_Div_Byte (void);

static	WORD	AD_DATE;//

D_AD_SAMPLE_NUM			equ	6					//AD������Ʒ�ĸ���
D_AD_SHIFT_NUM			equ	2					//���ֵ�趨��D_AD_SAMPLE_NUM���

D_ato_TIMES				equ	D_AD_SAMPLE_NUM		//�����ֵ����Ͳ�������ֵ��ͬ(ƽ��ֵ�����������)

D_ato_CHANGE_TIMES		equ	5

D_bat_TIMES				equ	D_AD_SAMPLE_NUM		//�����ֵ����Ͳ�������ֵ��ͬ(ƽ��ֵ�����������)

D_bat_CHANGE_TIMES		equ	5

m_addone_wait			macro
	AD_START = 1;
	while(!AD_DONE) NULL;
//	gwAdVal = (adcrh << 8 | adcrl);
//	gwAdVal =  gwAdVal >> 4;
	AD_DATE$1 = adcrh;
	AD_DATE$0 = adcrl;
	AD_DATE >>= 4;
						endm
tm3_turn_on   macro
 
 {
  $ tm3c IHRC,PWM,pwm;
 }
      endm
      
tm3_turn_off   macro
 $ tm3c stop;
 PWM = 0;
      endm 



void t6_init(void)
{
	gw_Point = 256 - D_INT_BASE;			
	stt16 gw_Point;
	$ t16m IHRC,/4,bit8;
}
void tm3_init(void)
{
	tm3b = 100;
	tm3s = 2;
	$ tm3c stop;
	
}



void	TM2_Period(void)
{
	tm2ct = 0;						//�����Ĵ���
	tm2b = 49;						//���޼Ĵ���������ֵ�������޼Ĵ������趨ֵʱ������жϣ�
	$ TM2C SYSCLK,Disable,Period;	//ʱ��Դѡ�������ѡ��ģʽѡ������ģʽ��
									//ʱ��Դ��ѡ��STOP, SYSCLK, IHRC, EOSC, ILRC, GPCRS, PA0_R, PA0_F, PB0_R, PB0_F, PA4_R, PA4_F
									//ע�������ж�ʱ����Ҫ����źͷ����ԣ�
	$ TM2S 8BIT,/1,/25;				//�ֱ���ѡ�񣬿���ѡ��8bit��6bit��ʱ��Ԥ��Ƶѡ����/1, /4, /16, /64��ʱ�ӷ�Ƶѡ��/1 ~ /32
					//���㹫ʽΪ 1/{ʱ��Դ*2/[2*(TM2B+1)*Ԥ��Ƶ*��Ƶ]}
					//	 	   = 1/(1M*2/(2*TM2B+1)*4*25) = 1/(1000000*2/(2*(49+1)*4*25)) = 0.005s
}
//--------------------------------------------------Define End
//--------------------------------------------------main Star
void FPPA0(void)
{
//--------------------ʱ������--------------------
	.ADJUST_IC	SYSCLK=IHRC/4, IHRC=15.140MHz, VDD=5V;
//--------------------�˿�����--------------------
  PADIER=0xF9;//���÷�����
//  $  PADIER PA;//���÷�����
/*
//PMS131-S08A SOP8 150mil ����ͼ
#define LEDGL   PA.3//PIN6  ���
#define LEDBL   PB.1//PIN5  ���
#define LEDRL   PA.6//PIN2  ���
#define PWM     PB.7//PIN4  ���
#define ATO_AD_IN   PA.4//PIN7  ����
#define Key_In       PA.5//PIN3  ����    ����
*/

  PA=0xC8;   //0--3 4567//�˿����ݼĴ��� (����ģʽ)������˿ڵĸߵ����� (���ģʽ)�Ӷ˿�����ߵ�����
  PAC=0xcE;  //0--3 4567//���ƼĴ���     0���� 1���
  PAPH=0x30; //0--3 4567//�������ƼĴ��� 0���� 1��������
  PB=0x00;   //0--3 4567//�˿����ݼĴ��� (����ģʽ)������˿ڵĸߵ����� (���ģʽ)�Ӷ˿�����ߵ�����
  PBC=0xFD;  //0--3 4567//���ƼĴ���     0���� 1���
  PBPH=0x00; //0--3 4567//�������ƼĴ��� 0���� 1��������
  PADIER = 0b1111_1001;
  PBDIER = 0b1111_1001;
  t6_init();
  tm3_init();
  //���жϱ�־
	intrq = 0;
	//t16�ж�ʹ��
	$ inten t16;
	//���Ź�ʹ��
	wdreset;
	clkmd.1 = 1;		//enable wdt
	TM2_Period();
	INTEN.TM2 = 1;				//�ж������Ĵ�������TM3�ж�
//	INTEN.TM3 = 0;				//�ж������Ĵ�������TM3�ж�
	INTRQ.TM2 = 0;				//�ж�����Ĵ���������INTRQ�Ĵ�����
//------------------------------------------------------------	
	//����ȫ���ж�
	engint;
gwPoint = _SYS(RAM_SIZE)-1;
	a = 0;
	do
	{
		*gwPoint = a;
	}while(--gwPoint$0);
		flag_led1=1;
 	  flag_led2=1;
		flag_led3=1;
	put_Electricity=31;
	
while(1)//----------------------------------------------------------------------
{
	    wdreset;
     
}//end of while(1)
}//end of main
byte led_time;
//----------------------------------------------------------------------
void Interrupt(void)
{ 
	pushaf;
	if(Intrq.TM2)
	{
	
		Intrq.TM2	=	0;
	}
  if(Intrq.T16)//T16 Trig
  { 
  	stt16 gw_Point;
		intrq.t16 = 0;
    //User can add code
  }
  popaf;
}

//--------------------------------------------------