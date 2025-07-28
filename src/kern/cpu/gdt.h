#include <stdint.h>

typedef struct {
	uint32_t reserved0;
	uint64_t rsp0; // Stack pointer for ring 0
	uint64_t rsp1; // Stack pointer for ring 1
	uint64_t rsp2; // Stack pointer for ring 2
	uint64_t reserved1;
	uint64_t ist1; // Interrupt Stack Table 1-7
	uint64_t ist2;
	uint64_t ist3;
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;
	uint64_t reserved2;
	uint16_t reserved3;
	uint16_t iomap_base;
} __attribute__((packed)) tss_entry_t;

typedef struct {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access; // Type should be 0x89 for 64-bit TSS
	uint8_t limit_high_and_flags;
	uint8_t base_high;
	uint32_t base_upper;
	uint32_t reserved;
} __attribute__((packed)) gdt_tss_entry_t;

typedef struct {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) gdt_pointer_t;

int gdt_init(void);
