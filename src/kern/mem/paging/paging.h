#include <stdint.h>

extern void loadPageDirectory(unsigned int *);
extern void enablePaging();
extern uint32_t page_directory[1024] __attribute__((aligned(4096)));
extern uint32_t page_table[1024] __attribute__((aligned(4096)));
