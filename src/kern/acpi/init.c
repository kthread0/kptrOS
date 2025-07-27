#include "../serial/serial.h"
#include "uacpi/platform/config.h"

#include <uacpi/acpi.h>
#include <uacpi/internal/context.h>
#include <uacpi/internal/event.h>
#include <uacpi/internal/interpreter.h>
#include <uacpi/internal/log.h>
#include <uacpi/internal/namespace.h>
#include <uacpi/internal/notify.h>
#include <uacpi/internal/opregion.h>
#include <uacpi/internal/osi.h>
#include <uacpi/internal/registers.h>
#include <uacpi/internal/tables.h>
#include <uacpi/internal/utilities.h>
#include <uacpi/uacpi.h>

int acpi_init(void) {
	/*
	 * Start with this as the first step of the initialization. This loads all
	 * tables, brings the event subsystem online, and enters ACPI mode. We pass
	 * in 0 as the flags as we don't want to override any default behavior for
	 * now.
	 */
	void *tmp = 0;
	uacpi_status ret = uacpi_setup_early_table_access(&tmp, UACPI_STATIC_TABLE_ARRAY_LEN);
	if (uacpi_unlikely_error(ret)) {
		serial_printf("uacpi_initialize error: %s", uacpi_status_to_string(ret));
		return -1;
	}
	/*
	 * That's it, uACPI is now fully initialized and working! You can proceed to
	 * using any public API at your discretion. The next recommended step is
	 * namespace enumeration and device discovery so you can bind drivers to
	 * ACPI objects.
	 */
	return 0;
}
