#include <system.h>

extern void load_page(uint64_t);

extern uint64_t page_directory[512] __attribute__((aligned(KERNEL_CS)));
extern uint64_t page_table[512] __attribute__((aligned(KERNEL_CS)));
extern uint64_t pml4_table[512] __attribute__((aligned(KERNEL_CS)));
