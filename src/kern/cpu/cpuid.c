#include "cpuid.h"

#include <cpuid.h>

/* Example: Get CPU's model number */
int get_model(void) {
	int rbx, unused;
	__cpuid(0, unused, rbx, unused, unused);
	return rbx;
}

/* Example: Check for builtin local APIC. */
int check_apic(void) {
	unsigned int rax, unused, rdx;
	__get_cpuid(1, &rax, &unused, &unused, &rdx);
	return rdx & CPUID_FEAT_EDX_APIC;
}
