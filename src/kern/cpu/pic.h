#ifndef KERN_CPU_PIC_H
#define KERN_CPU_PIC_H

#include <stdint.h>

// I/O Port Addresses for PICs
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

// End-of-Interrupt command
#define PIC_EOI 0x20

// --- PIC Remapping Constants ---

// The vector offsets to map IRQs to.
// IRQs 0-7 (Master) will be mapped to Interrupts 32-39.
// IRQs 8-15 (Slave) will be mapped to Interrupts 40-47.
#define PIC1_VECTOR_OFFSET 0x20
#define PIC2_VECTOR_OFFSET 0x28

// Initialization Command Word 1 (ICW1)
#define ICW1_INIT 0x10 // Required for initialization
#define ICW1_ICW4 0x01 // Indicates that ICW4 will be present

// Initialization Command Word 4 (ICW4)
#define ICW4_8086 0x01 // 8086/88 (x86) mode

// --- Function Prototypes ---

// Disables the PICs by masking all IRQs.
void pic_disable(void);

// Remaps the PIC controllers to the specified vector offsets.
void pic_remap(void);

// Sends the End-of-Interrupt signal to the appropriate PIC.
void pic_send_eoi(uint8_t irq);

// Unmasks (enables) a given IRQ line.
void pic_unmask_irq(uint8_t irq);

#endif // KERN_CPU_PIC_H
