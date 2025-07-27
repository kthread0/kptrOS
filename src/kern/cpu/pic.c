#include "pic.h"
#include "access.h"
#include <stdint.h>

// Function to send End-of-Interrupt signal to the PICs.
void
pic_send_eoi (unsigned char irq)
{
	if (irq >= 8)
		{
			outb (PIC2_COMMAND, PIC_EOI);
		}
	outb (PIC1_COMMAND, PIC_EOI);
}

// Function to disable the PIC. This is useful at the start of initialization.
void
pic_disable (void)
{
	// Mask all interrupts on both PICs
	outb (PIC1_DATA, 0xff);
	io_wait ();
	outb (PIC2_DATA, 0xff);
	io_wait ();
}

// This is the main initialization function that remaps the PIC IRQs.
// IRQs 0-7 from the master PIC will be mapped to vectors 32-39.
// IRQs 8-15 from the slave PIC will be mapped to vectors 40-47.
void
pic_remap (void)
{
	// Before remapping, we save the current interrupt masks.
	uint8_t mask1 = inb (PIC1_DATA);
	io_wait ();
	uint8_t mask2 = inb (PIC2_DATA);
	io_wait ();

	// Start the initialization sequence (in cascade mode).
	outb (PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait ();
	outb (PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait ();

	// ICW2: Set the master PIC's vector offset to 32 (0x20).
	outb (PIC1_DATA, PIC1_VECTOR_OFFSET);
	io_wait ();
	// ICW2: Set the slave PIC's vector offset to 40 (0x28).
	outb (PIC2_DATA, PIC2_VECTOR_OFFSET);
	io_wait ();

	// ICW3: Tell master PIC that there is a slave PIC at IRQ2 (0000 0100).
	outb (PIC1_DATA, 0x04);
	io_wait ();
	// ICW3: Tell slave PIC its cascade identity is 2 (0000 0010).
	outb (PIC2_DATA, 0x02);
	io_wait ();

	// ICW4: Set 8086/88 (x86) mode.
	outb (PIC1_DATA, ICW4_8086);
	io_wait ();
	outb (PIC2_DATA, ICW4_8086);
	io_wait ();

	// Restore the saved masks.
	outb (PIC1_DATA, mask1);
	io_wait ();
	outb (PIC2_DATA, mask2);
	io_wait ();
}

// Function to unmask (enable) a specific IRQ line.
void
pic_unmask_irq (uint8_t irq)
{
	uint16_t port;
	uint8_t value;

	if (irq < 8)
		{
			port = PIC1_DATA;
		}
	else
		{
			port = PIC2_DATA;
			irq -= 8;
		}

	value = inb (port) & ~(1 << irq);
	outb (port, value);
	io_wait ();
}
