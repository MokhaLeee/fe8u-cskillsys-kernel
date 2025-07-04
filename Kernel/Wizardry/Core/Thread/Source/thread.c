#include "common-chax.h"
#include "thread.h"

void kthread_init(void)
{
	memset(kthreads, 0, sizeof(kthreads));
	current_kthread = 0;

	timer_enable(systick_isr, 0x400);
}
