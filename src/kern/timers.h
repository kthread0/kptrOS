#ifndef TIMERS_H
#define TIMERS_H

#include <stddef.h>
#include <stdint.h>

#define HPET_IRQ_VECTOR 32

#define HPET_GENERAL_CAPABILITIES_ID 0x00
#define HPET_GENERAL_CONFIGURATION 0x10
#define HPET_MAIN_COUNTER 0xF0
#define HPET_TIMER_CONFIG_CAPABILITY(n) (0x100 + 0x20 * (n))
#define HPET_TIMER_COMPARATOR(n) (0x108 + 0x20 * (n))

#define HPET_ENABLE_CNF 0x1
#define HPET_PERIODIC_CNF (1 << 2)
#define HPET_INT_ENABLE_CNF (1 << 3)

#define HPET_REG_SIZE 1024

static void *hpet_base = NULL;
static uint64_t hpet_period = 0;

int timer_init(void);
int hpet_configure_timer(uint64_t timer, uint64_t interval_ms);
uint64_t hpet_get_counter(void);

#endif // TIMERS_H
