#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

// PS/2 Keyboard ports
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// Keyboard IRQ
#define KEYBOARD_IRQ 1

// Special scan codes
#define SCANCODE_ESCAPE 0x01
#define SCANCODE_BACKSPACE 0x0E
#define SCANCODE_TAB 0x0F
#define SCANCODE_ENTER 0x1C
#define SCANCODE_CTRL_LEFT 0x1D
#define SCANCODE_SHIFT_LEFT 0x2A
#define SCANCODE_SHIFT_RIGHT 0x36
#define SCANCODE_ALT_LEFT 0x38
#define SCANCODE_SPACE 0x39
#define SCANCODE_CAPS_LOCK 0x3A

// Function keys
#define SCANCODE_F1 0x3B
#define SCANCODE_F2 0x3C
#define SCANCODE_F3 0x3D
#define SCANCODE_F4 0x3E
#define SCANCODE_F5 0x3F
#define SCANCODE_F6 0x40
#define SCANCODE_F7 0x41
#define SCANCODE_F8 0x42
#define SCANCODE_F9 0x43
#define SCANCODE_F10 0x44
#define SCANCODE_F11 0x57
#define SCANCODE_F12 0x58

// Number row
#define SCANCODE_GRAVE 0x29	 // `~
#define SCANCODE_1 0x02
#define SCANCODE_2 0x03
#define SCANCODE_3 0x04
#define SCANCODE_4 0x05
#define SCANCODE_5 0x06
#define SCANCODE_6 0x07
#define SCANCODE_7 0x08
#define SCANCODE_8 0x09
#define SCANCODE_9 0x0A
#define SCANCODE_0 0x0B
#define SCANCODE_MINUS 0x0C		// -_
#define SCANCODE_EQUALS 0x0D	// =+

// QWERTY row
#define SCANCODE_Q 0x10
#define SCANCODE_W 0x11
#define SCANCODE_E 0x12
#define SCANCODE_R 0x13
#define SCANCODE_T 0x14
#define SCANCODE_Y 0x15
#define SCANCODE_U 0x16
#define SCANCODE_I 0x17
#define SCANCODE_O 0x18
#define SCANCODE_P 0x19
#define SCANCODE_LBRACKET 0x1A	 // [{
#define SCANCODE_RBRACKET 0x1B	 // ]}
#define SCANCODE_BACKSLASH 0x2B	 // \|

// ASDF row
#define SCANCODE_A 0x1E
#define SCANCODE_S 0x1F
#define SCANCODE_D 0x20
#define SCANCODE_F 0x21
#define SCANCODE_G 0x22
#define SCANCODE_H 0x23
#define SCANCODE_J 0x24
#define SCANCODE_K 0x25
#define SCANCODE_L 0x26
#define SCANCODE_SEMICOLON 0x27	 // ;:
#define SCANCODE_QUOTE 0x28			 // '"

// ZXCV row
#define SCANCODE_Z 0x2C
#define SCANCODE_X 0x2D
#define SCANCODE_C 0x2E
#define SCANCODE_V 0x2F
#define SCANCODE_B 0x30
#define SCANCODE_N 0x31
#define SCANCODE_M 0x32
#define SCANCODE_COMMA 0x33		// ,<
#define SCANCODE_PERIOD 0x34	// .>
#define SCANCODE_SLASH 0x35		// /?

// Extended keys (preceded by 0xE0)
#define SCANCODE_ARROW_UP 0x48
#define SCANCODE_ARROW_DOWN 0x50
#define SCANCODE_ARROW_LEFT 0x4B
#define SCANCODE_ARROW_RIGHT 0x4D
#define SCANCODE_INSERT 0x52
#define SCANCODE_DELETE 0x53
#define SCANCODE_HOME 0x47
#define SCANCODE_END 0x4F
#define SCANCODE_PAGE_UP 0x49
#define SCANCODE_PAGE_DOWN 0x51

// Release mask
#define SCANCODE_RELEASE_MASK 0x80
#define EXTENDED_PREFIX 0xE0

// Keyboard state structure
typedef struct {
	bool shift_pressed;
	bool ctrl_pressed;
	bool alt_pressed;
	bool caps_lock;
	bool extended_mode;
} keyboard_state_t;

// Function declarations
void keyboard_init(void);
void keyboard_interrupt_handler(void);
char scancode_to_ascii(uint8_t scancode);
keyboard_state_t* get_keyboard_state(void);
void keyboard_test_controller(void);

#endif	// KEYBOARD_H
