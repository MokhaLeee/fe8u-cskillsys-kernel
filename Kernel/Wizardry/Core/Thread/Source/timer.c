#include "common-chax.h"
#include "thread.h"

STATIC_DECLAR void timer_load(int cnt)
{
	REG_TM0CNT_L = -0x4000;
	REG_TM0CNT_H = TIMER_1024CLK;
	REG_TM0CNT_H = TIMER_ENABLE | TIMER_INTR_ENABLE;
}

void timer_enable(void (*call_back)(void), int cnt)
{
	SetIRQHandler(3, call_back);
	REG_IE |= INTR_FLAG_TIMER0;
	timer_load(cnt);
}

void timer_disable(void)
{
	REG_IE &= ~INTR_FLAG_TIMER0;
	SetIRQHandler(3, NULL);
}
