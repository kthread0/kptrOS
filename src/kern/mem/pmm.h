#ifndef PMM_H
#define PMM_H

#include <limine.h>
#include <stddef.h>
#include <stdint.h>

void pmm_init(struct limine_memmap_response *memmap);
void *pmm_alloc(void *addr, size_t len);
void *pmm_alloc_page(void *addr, size_t len);
void pmm_free(void *addr, size_t len);
void pmm_free_page(void *addr);
extern volatile struct limine_memmap_request memmap_request;

#endif // PMM_H
