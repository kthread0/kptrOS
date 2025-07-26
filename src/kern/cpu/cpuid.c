#include "cpuid.h"
#include <cpuid.h>

/* Example: Get CPU's model number */
int get_model(void)
{
	int ebx, unused;
	__cpuid(0, unused, ebx, unused, unused);
	return ebx;
}

/* Example: Check for builtin local APIC. */
int check_apic(void)
{
	unsigned int eax, unused, edx = 0;
	__get_cpuid(1, &eax, &unused, &unused, &edx);
	return edx & CPUID_FEAT_EDX_APIC;
}
