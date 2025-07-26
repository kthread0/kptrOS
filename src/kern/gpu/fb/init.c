#include "flanterm_backends/fb.h"
#include "limine.h"
#include <flanterm/flanterm.h>
#include <system.h>

// Framebuffer request
static volatile struct limine_framebuffer_request limine_framebuffer = {
	.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

void fbinit()
{
	if (limine_framebuffer.response == NULL ||
		limine_framebuffer.response->framebuffer_count < 1)
	{
		serial_printf(
			"Framebuffer request failed: response is NULL or count < 1\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}
	else
	{
		serial_printf("Framebuffers: %d\n",
					  limine_framebuffer.response->framebuffer_count);
	}

	struct limine_framebuffer *framebuffer =
		limine_framebuffer.response->framebuffers[0];

	struct flanterm_context *ft_ctx = flanterm_fb_init(
		NULL, NULL, framebuffer->address, framebuffer->width,
		framebuffer->height, framebuffer->pitch, framebuffer->red_mask_size,
		framebuffer->red_mask_shift, framebuffer->green_mask_size,
		framebuffer->green_mask_shift, framebuffer->blue_mask_size,
		framebuffer->blue_mask_shift, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, 0, 0, 1, 0, 0, 0);

	const char msg[] = "Hello world\n";

	flanterm_write(ft_ctx, msg, sizeof(msg));
}
