#include "keyboard.h"
#include "../cpu/access.h"
#include "../cpu/clk/timer.h"
#include "../cpu/idt.h"
#include "../cpu/pic.h"
#include "debounce.h"
#include <system.h>

typedef struct
{
	uint8_t scancode;
	char normal;
	char shift;
	char ctrl;
} scancode_entry_t;

static const scancode_entry_t us_qwerty[] = {
	{ SCANCODE_ESCAPE, 0x1B, 0x1B, 0x1B },
	{ SCANCODE_1, '1', '!', 0 },
	{ SCANCODE_2, '2', '@', 0 },
	{ SCANCODE_3, '3', '#', 0 },
	{ SCANCODE_4, '4', '$', 0 },
	{ SCANCODE_5, '5', '%', 0 },
	{ SCANCODE_6, '6', '^', 0x1E },
	{ SCANCODE_7, '7', '&', 0 },
	{ SCANCODE_8, '8', '*', 0 },
	{ SCANCODE_9, '9', '(', 0 },
	{ SCANCODE_0, '0', ')', 0 },
	{ SCANCODE_MINUS, '-', '_', 0x1F },
	{ SCANCODE_EQUALS, '=', '+', 0 },
	{ SCANCODE_BACKSPACE, '\b', '\b', 0x7F },
	{ SCANCODE_TAB, '\t', '\t', '\t' },

	// Letters
	{ SCANCODE_Q, 'q', 'Q', 0x11 },
	{ SCANCODE_W, 'w', 'W', 0x17 },
	{ SCANCODE_E, 'e', 'E', 0x05 },
	{ SCANCODE_R, 'r', 'R', 0x12 },
	{ SCANCODE_T, 't', 'T', 0x14 },
	{ SCANCODE_Y, 'y', 'Y', 0x19 },
	{ SCANCODE_U, 'u', 'U', 0x15 },
	{ SCANCODE_I, 'i', 'I', 0x09 },
	{ SCANCODE_O, 'o', 'O', 0x0F },
	{ SCANCODE_P, 'p', 'P', 0x10 },
	{ SCANCODE_LBRACKET, '[', '{', 0x1B },
	{ SCANCODE_RBRACKET, ']', '}', 0x1D },
	{ SCANCODE_ENTER, '\n', '\n', '\n' },

	{ SCANCODE_A, 'a', 'A', 0x01 },
	{ SCANCODE_S, 's', 'S', 0x13 },
	{ SCANCODE_D, 'd', 'D', 0x04 },
	{ SCANCODE_F, 'f', 'F', 0x06 },
	{ SCANCODE_G, 'g', 'G', 0x07 },
	{ SCANCODE_H, 'h', 'H', 0x08 },
	{ SCANCODE_J, 'j', 'J', 0x0A },
	{ SCANCODE_K, 'k', 'K', 0x0B },
	{ SCANCODE_L, 'l', 'L', 0x0C },
	{ SCANCODE_SEMICOLON, ';', ':', 0 },
	{ SCANCODE_QUOTE, '\'', '"', 0 },
	{ SCANCODE_GRAVE, '`', '~', 0 },
	{ SCANCODE_BACKSLASH, '\\', '|', 0x1C },

	{ SCANCODE_Z, 'z', 'Z', 0x1A },
	{ SCANCODE_X, 'x', 'X', 0x18 },
	{ SCANCODE_C, 'c', 'C', 0x03 },
	{ SCANCODE_V, 'v', 'V', 0x16 },
	{ SCANCODE_B, 'b', 'B', 0x02 },
	{ SCANCODE_N, 'n', 'N', 0x0E },
	{ SCANCODE_M, 'm', 'M', 0x0D },
	{ SCANCODE_COMMA, ',', '<', 0 },
	{ SCANCODE_PERIOD, '.', '>', 0 },
	{ SCANCODE_SLASH, '/', '?', 0 },
	{ SCANCODE_SPACE, ' ', ' ', 0x20 },

	{ 0, 0, 0, 0 } // Sentinel
};

static keyboard_state_t kbd_state = { false, false, false, false, false };

static void
kbd_wait_input (void)
{
	while (inb (KEYBOARD_STATUS_PORT) & 0x2)
		{
		}
}

static void
kbd_wait_output (void)
{
	while (!(inb (KEYBOARD_STATUS_PORT) & 0x1))
		{
		}
}

char
scancode_to_ascii (uint8_t scancode)
{
	const scancode_entry_t *entry = us_qwerty;

	// Find the scancode in the table
	while (entry->scancode != 0)
		{
			if (entry->scancode == scancode)
				{
					// Return appropriate ASCII based on modifier state
					if (kbd_state.ctrl_pressed && entry->ctrl != 0)
						{
							return entry->ctrl;
						}
					else if (kbd_state.shift_pressed
									 || (kbd_state.caps_lock && scancode >= SCANCODE_A
											 && scancode <= SCANCODE_Z))
						{
							return entry->shift;
						}
					else
						{
							return entry->normal;
						}
				}
			entry++;
		}

	return 0; // Unknown scancode
}

void
keyboard_interrupt_handler (void)
{
	uint8_t scancode = inb (KEYBOARD_DATA_PORT);
	char ascii;

	if (scancode == EXTENDED_PREFIX)
		{
			kbd_state.extended_mode = true;
			return;
		}

	if (scancode & SCANCODE_RELEASE_MASK)
		{
			scancode &= ~SCANCODE_RELEASE_MASK;

			// Update modifier states on release
			switch (scancode)
				{
				case SCANCODE_SHIFT_LEFT:
				case SCANCODE_SHIFT_RIGHT:
					kbd_state.shift_pressed = false;
					break;
				case SCANCODE_CTRL_LEFT:
					kbd_state.ctrl_pressed = false;
					break;
				case SCANCODE_ALT_LEFT:
					kbd_state.alt_pressed = false;
					break;
				}

			kbd_state.extended_mode = false;
			return;
		}

	switch (scancode)
		{
		case SCANCODE_SHIFT_LEFT:
		case SCANCODE_SHIFT_RIGHT:
			kbd_state.shift_pressed = true;
			kbd_state.extended_mode = false;
			return;
		case SCANCODE_CTRL_LEFT:
			kbd_state.ctrl_pressed = true;
			kbd_state.extended_mode = false;
			return;
		case SCANCODE_ALT_LEFT:
			kbd_state.alt_pressed = true;
			kbd_state.extended_mode = false;
			return;
		case SCANCODE_CAPS_LOCK:
			kbd_state.caps_lock = !kbd_state.caps_lock;
			kbd_state.extended_mode = false;
			return;
		}

	if (kbd_state.extended_mode)
		{
			kbd_state.extended_mode = false;
			// You can add specific handling for extended keys here
			// For now, just ignore them
			return;
		}

	ascii = scancode_to_ascii (scancode);
	if (ascii != 0)
		{
			serial_printf ("Key pressed: '%d' (%x)\n", ascii, ascii);
			sleep (500);
		}
}
keyboard_state_t *
get_keyboard_state (void)
{
	return &kbd_state;
}

void
keyboard_init (void)
{
	serial_write ("Initializing keyboard...\n");

	// --- Step 1: Disable the PS/2 devices ---
	kbd_wait_input ();
	outb (KEYBOARD_STATUS_PORT, 0xAD); // Disable first PS/2 port (keyboard)
	kbd_wait_input ();
	outb (KEYBOARD_STATUS_PORT, 0xA7); // Disable second PS/2 port (mouse)

	// --- Step 2: Flush the output buffer ---
	// It's possible there's leftover data from the BIOS
	while (inb (KEYBOARD_STATUS_PORT) & 0x1)
		{
			inb (KEYBOARD_DATA_PORT);
		}

	// --- Step 3: Set the controller configuration byte ---
	kbd_wait_input ();
	outb (KEYBOARD_STATUS_PORT, 0x20); // Command to read config byte
	kbd_wait_output ();
	uint8_t config = inb (KEYBOARD_DATA_PORT);

	// --- Step 4: Modify and write the new config byte ---
	// Bit 0: Enable interrupt for keyboard (port 1)
	// Bit 1: Enable interrupt for mouse (port 2) - we'll leave it disabled for
	// now Bit 6: Enable translation to scancode set 1
	config |= 1;		// Enable keyboard interrupt
	config &= ~0x2; // Disable mouse interrupt
	config |= 0x40; // Enable translation

	kbd_wait_input ();
	outb (KEYBOARD_STATUS_PORT, 0x60); // Command to write config byte
	kbd_wait_input ();
	outb (KEYBOARD_DATA_PORT, config);

	// --- Step 5: Controller Self-Test ---
	kbd_wait_input ();
	outb (KEYBOARD_STATUS_PORT, 0xAA); // Command to perform self-test
	kbd_wait_output ();
	if (inb (KEYBOARD_DATA_PORT) != 0x55)
		{
			serial_write ("Keyboard controller self-test failed!\n");
			return;
		}

	// --- Step 6: Enable the keyboard ---
	kbd_wait_input ();
	outb (KEYBOARD_STATUS_PORT, 0xAE); // Enable first PS/2 port (keyboard)

	// --- Step 7: Unmask the keyboard IRQ in the PIC ---
	pic_unmask_irq (KEYBOARD_IRQ);

	keyboard_debounce_init ();

	serial_write ("Keyboard initialized successfully.\n");
}
