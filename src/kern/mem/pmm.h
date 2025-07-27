#ifndef PMM_H
#define PMM_H

#include <limine.h>
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096

void pmm_init(struct limine_memmap_response *memmap);
void *pmm_alloc(size_t order);
void *pmm_alloc_page();
void pmm_free(void *addr, size_t order);
void pmm_free_page(void *addr);
extern volatile struct limine_memmap_request memmap_request;

#endif // PMM_H
