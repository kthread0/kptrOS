#include "paging/paging.h"
#include "pmm.h"
#include <limine.h>

// Memory map request
static struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

void start_pmm(void)
{
	pmm_init(memmap_request.response);
}
