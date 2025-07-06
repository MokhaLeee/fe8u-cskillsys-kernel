#include "gbafe.h"
#include "kthread.h"

void kthread_test(void)
{
	while (1)
		YieldSubThread();
}
