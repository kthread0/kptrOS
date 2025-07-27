#ifndef PMM_H
#define PMM_H

#include <limine.h> // Make sure you have this header from the Limine project
#include <stddef.h>
#include <stdint.h>

#define MAX_ORDER 32
#define PAGE_SIZE 4096

void pmm_init(struct limine_memmap_response *memmap);

void *pmm_alloc(size_t order);
void *pmm_alloc_page();
void pmm_free(void *addr, size_t order);
void pmm_free_page(void *addr);
void start_pmm(void);

#endif // PMM_H
