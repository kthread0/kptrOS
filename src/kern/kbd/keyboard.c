#include "keyboard.h"
#include "../cpu/access.h"
#include "../cpu/idt.h"
#include "../cpu/pic.h"
#include <system.h>

typedef struct
{
	uint8_t scancode;
	char normal;
	char shift;
	char ctrl;
} scancode_entry_t;

static const scancode_entry_t us_qwerty[] = {
	{SCANCODE_ESCAPE, 0x1B, 0x1B, 0x1B},
	{SCANCODE_1, '1', '!', 0},
	{SCANCODE_2, '2', '@', 0},
	{SCANCODE_3, '3', '#', 0},
	{SCANCODE_4, '4', '$', 0},
	{SCANCODE_5, '5', '%', 0},
	{SCANCODE_6, '6', '^', 0x1E},
	{SCANCODE_7, '7', '&', 0},
	{SCANCODE_8, '8', '*', 0},
	{SCANCODE_9, '9', '(', 0},
	{SCANCODE_0, '0', ')', 0},
	{SCANCODE_MINUS, '-', '_', 0x1F},
	{SCANCODE_EQUALS, '=', '+', 0},
	{SCANCODE_BACKSPACE, '\b', '\b', 0x7F},
	{SCANCODE_TAB, '\t', '\t', '\t'},

	// Letters
	{SCANCODE_Q, 'q', 'Q', 0x11},
	{SCANCODE_W, 'w', 'W', 0x17},
	{SCANCODE_E, 'e', 'E', 0x05},
	{SCANCODE_R, 'r', 'R', 0x12},
	{SCANCODE_T, 't', 'T', 0x14},
	{SCANCODE_Y, 'y', 'Y', 0x19},
	{SCANCODE_U, 'u', 'U', 0x15},
	{SCANCODE_I, 'i', 'I', 0x09},
	{SCANCODE_O, 'o', 'O', 0x0F},
	{SCANCODE_P, 'p', 'P', 0x10},
	{SCANCODE_LBRACKET, '[', '{', 0x1B},
	{SCANCODE_RBRACKET, ']', '}', 0x1D},
	{SCANCODE_ENTER, '\n', '\n', '\n'},

	{SCANCODE_A, 'a', 'A', 0x01},
	{SCANCODE_S, 's', 'S', 0x13},
	{SCANCODE_D, 'd', 'D', 0x04},
	{SCANCODE_F, 'f', 'F', 0x06},
	{SCANCODE_G, 'g', 'G', 0x07},
	{SCANCODE_H, 'h', 'H', 0x08},
	{SCANCODE_J, 'j', 'J', 0x0A},
	{SCANCODE_K, 'k', 'K', 0x0B},
	{SCANCODE_L, 'l', 'L', 0x0C},
	{SCANCODE_SEMICOLON, ';', ':', 0},
	{SCANCODE_QUOTE, '\'', '"', 0},
	{SCANCODE_GRAVE, '`', '~', 0},
	{SCANCODE_BACKSLASH, '\\', '|', 0x1C},

	{SCANCODE_Z, 'z', 'Z', 0x1A},
	{SCANCODE_X, 'x', 'X', 0x18},
	{SCANCODE_C, 'c', 'C', 0x03},
	{SCANCODE_V, 'v', 'V', 0x16},
	{SCANCODE_B, 'b', 'B', 0x02},
	{SCANCODE_N, 'n', 'N', 0x0E},
	{SCANCODE_M, 'm', 'M', 0x0D},
	{SCANCODE_COMMA, ',', '<', 0},
	{SCANCODE_PERIOD, '.', '>', 0},
	{SCANCODE_SLASH, '/', '?', 0},
	{SCANCODE_SPACE, ' ', ' ', 0x20},

	{0, 0, 0, 0} // Sentinel
};

static keyboard_state_t kbd_state = {false, false, false, false, false};

void keyboard_init(void)
{
	// Test the controller first
	keyboard_test_controller();

	// Disable keyboard
	outb(0x64, 0xAD);
	io_wait();

	// Flush the output buffer
	while (inb(0x64) & 0x01)
	{
		inb(0x60);
	}

	// Enable keyboard
	outb(0x64, 0xAE);
	io_wait();

	// Reset keyboard
	outb(0x60, 0xFF);
	io_wait();

	// Wait for ACK
	while (!(inb(0x64) & 0x01))
		;
	uint8_t ack = inb(0x60);
	serial_printf("Keyboard reset ACK: %x\n", ack);

	// Unmask IRQ1 in PIC
	uint8_t mask = inb(PIC1_DATA);
	mask &= ~(1 << 1);
	outb(PIC1_DATA, mask);

	serial_write("Keyboard initialized successfully\n");
}

void keyboard_interrupt_handler(void)
{
	uint8_t scancode = inb(KEYBOARD_DATA_PORT);
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

	ascii = scancode_to_ascii(scancode);
	if (ascii != 0)
	{
		serial_printf("Key pressed: '%c' (0x%02X)\n", ascii, ascii);
	}
}

char scancode_to_ascii(uint8_t scancode)
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
			else if (kbd_state.shift_pressed ||
					 (kbd_state.caps_lock && scancode >= SCANCODE_A &&
					  scancode <= SCANCODE_Z))
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

keyboard_state_t *get_keyboard_state(void)
{
	return &kbd_state;
}
