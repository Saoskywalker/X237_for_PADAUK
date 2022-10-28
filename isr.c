#include "types_base.h"
#include "app_timer.h"

extern uint16_t t16_cnt;
void Interrupt(void)
{
	PUSHAF; //存储ALU和FLAG寄存器
	
	if (INTRQ.TM2)
	{
		INTRQ.TM2 = 0;
	}

	if (INTRQ.T16) // T16 Trig
	{
		STT16 t16_cnt;
		INTRQ.T16 = 0;

		// $ PA.3 TOGGLE;
		app_timer();
		// $ PA.3 TOGGLE;
	}

	POPAF; //恢复ALU和FLAG寄存器
}
