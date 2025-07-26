#include "../kern/serial/serial.h"
#include "stdio.h"
#include <limine.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <uacpi/helpers.h>
#include <uacpi/kernel_api.h>

typedef struct
{
	uint64_t rip; // Instruction pointer
	uint64_t rsp; // Stack pointer
	uint64_t rbp; // Base pointer
	uint64_t rflags;
	uint64_t registers[16]; // General-purpose registers
} cpu_state_t;

#define PSF1_FONT_MAGIC 0x0436

typedef struct
{
	uint16_t magic;		   // Magic bytes for identification.
	uint8_t fontMode;	   // PSF font mode.
	uint8_t characterSize; // PSF character size.
} PSF1_Header;

#define PSF_FONT_MAGIC 0x864ab572

typedef struct
{
	uint32_t magic;			/* magic bytes to identify PSF */
	uint32_t version;		/* zero */
	uint32_t headersize;	/* offset of bitmaps in file, 32 */
	uint32_t flags;			/* 0 if there's no unicode table */
	uint32_t numglyph;		/* number of glyphs */
	uint32_t bytesperglyph; /* size of each glyph */
	uint32_t height;		/* height in pixels */
	uint32_t width;			/* width in pixels */
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

// Memory map request
static volatile struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

// Entry point request
static volatile struct limine_executable_address_request
	executable_address_request __attribute__((aligned(8))) = {
		.id = LIMINE_EXECUTABLE_ADDRESS_REQUEST, .revision = 0};

// SMBIOS request
static volatile struct limine_smbios_request smbios_request
	__attribute__((aligned(8))) = {.id = LIMINE_SMBIOS_REQUEST, .revision = 0};

static volatile struct limine_rsdp_request rsdp_request
	__attribute__((aligned(8))) = {.id = LIMINE_RSDP_REQUEST, .revision = 0};

// Paging mode request for 4-level paging
static volatile struct limine_paging_mode_request paging_mode_request = {
	.id = LIMINE_PAGING_MODE_REQUEST,
	.revision = 0,
	.mode = LIMINE_PAGING_MODE_X86_64_4LVL,
	.max_mode = LIMINE_PAGING_MODE_X86_64_4LVL,
	.min_mode = LIMINE_PAGING_MODE_X86_64_4LVL};

// Framebuffer request
static volatile struct limine_framebuffer_request limine_framebuffer = {
	.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};
