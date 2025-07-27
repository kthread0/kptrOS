#include <stdint.h>

void idt_init(void);
void idt_set_descriptor(uint64_t vector, void *isr, uint8_t flags);
void exception_handler(uint64_t vector, uint64_t error_code);
extern void *isr_stub_table[];
