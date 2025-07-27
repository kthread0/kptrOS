#include <stdbool.h>
#include <stdint.h>

#define fence() __asm__ volatile("" ::: "memory")

static inline void outb(uint16_t port, uint8_t val)
{
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void io_wait(void)
{
	outb(0x80, 0);
}

static inline bool are_interrupts_enabled()
{
	unsigned long flags;
	asm volatile("pushf\n\t"
				 "pop %0"
				 : "=g"(flags));
	return flags & (1 << 9);
}

static inline unsigned long save_irqdisable(void)
{
	unsigned long flags;
	asm volatile("pushf\n\tcli\n\tpop %0" : "=r"(flags) : : "memory");
	return flags;
}

static inline void irqrestore(unsigned long flags)
{
	asm("push %0\n\tpopf" : : "rm"(flags) : "memory", "cc");
}

static inline void lidt(void *base, uint16_t size)
{
	// This function works in 32 and 64bit mode
	struct
	{
		uint16_t length;
		void *base;
	} __attribute__((packed)) IDTR = {size, base};

	asm("lidt %0" : : "m"(IDTR)); // let the compiler choose an addressing mode
}

static inline unsigned long read_cr0(void)
{
	unsigned long val;
	asm volatile("mov %%cr0, %0" : "=r"(val));
	return val;
}

static inline void invlpg(void *m)
{
	/* Clobber memory to avoid optimizer re-ordering access before invlpg, which
	 * may cause nasty bugs. */
	asm volatile("invlpg (%0)" : : "b"(m) : "memory");
}

static inline void wrmsr(uint64_t msr, uint64_t value)
{
	uint32_t low = value & 0xFFFFFFFF;
	uint32_t high = value >> 32;
	asm volatile("wrmsr" : : "c"(msr), "a"(low), "d"(high));
}

static inline uint64_t rdmsr(uint64_t msr)
{
	uint32_t low, high;
	asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
	return ((uint64_t)high << 32) | low;
}
