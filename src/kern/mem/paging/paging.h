#include <stdint.h>

extern void load_page(unsigned int *);
extern uint64_t page_directory[512] __attribute__((aligned(0x1000)));
extern uint64_t page_table[512] __attribute__((aligned(0x1000)));
