#include "timer.h"

extern uint64_t count;

int
sleep (uint32_t millis)
{
	count = millis;
	while (count > 0)
		{
			--count;
		}
	return count;
}
