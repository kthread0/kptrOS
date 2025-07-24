#include "../include/system.h"

void panic(void) {
	for (;;) {
		asm("hlt");
	}
}
