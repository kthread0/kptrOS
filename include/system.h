#include "../src/kern/serial/serial.h"

#include <limine.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#define KERNEL_CS 0x08
#define IDT_MAX_DESCRIPTORS 256
#define PAGE_SIZE 4096

typedef struct {
	uint64_t rip; // Instruction pointer
	uint64_t rsp; // Stack pointer
	uint64_t rbp; // Base pointer
	uint64_t rflags;
	uint64_t registers[16]; // General-purpose registers
} cpu_state_t;

#define PSF1_FONT_MAGIC 0x0436

typedef struct {
	uint16_t magic;				 // Magic bytes for identification.
	uint8_t fontMode;			 // PSF font mode.
	uint8_t characterSize; // PSF character size.
} PSF1_Header;

#define PSF_FONT_MAGIC 0x864ab572

typedef struct {
	uint32_t magic;					/* magic bytes to identify PSF */
	uint32_t version;				/* zero */
	uint32_t headersize;		/* offset of bitmaps in file, 32 */
	uint32_t flags;					/* 0 if there's no unicode table */
	uint32_t numglyph;			/* number of glyphs */
	uint32_t bytesperglyph; /* size of each glyph */
	uint32_t height;				/* height in pixels */
	uint32_t width;					/* width in pixels */
} PSF_font;

void enable_cursor(uint8_t, uint8_t);
void panic(cpu_state_t *);
void capture_cpu_state(cpu_state_t *);
int memcmp(const void *, const void *, size_t);
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
void debug_limine_requests();
void debug();
void fbinit();
