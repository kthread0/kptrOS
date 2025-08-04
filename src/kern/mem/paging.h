#include <system.h>

#define ENTRIES 512

extern void load_pages(void);
extern uint64_t pml4[ENTRIES] __attribute__((aligned(PAGE_SIZE)));
extern uint64_t pdpt[ENTRIES] __attribute__((aligned(PAGE_SIZE)));
extern uint64_t pd[ENTRIES] __attribute__((aligned(PAGE_SIZE)));
extern uint64_t pts[ENTRIES][ENTRIES] __attribute__((aligned(PAGE_SIZE)));
