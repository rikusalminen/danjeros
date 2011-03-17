#include <arch/x86/ioport.h>
#include <arch/x86/pic.h>

#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xa0
#define PIC2_DATA       0xa1

#define ICW1_ICW4       0x01
#define ICW1_SINGLE     0x02
#define ICW1_INTERVAL4  0x04
#define ICW1_LEVEL      0x08
#define ICW1_INIT       0x10

#define ICW4_8086       0x01
#define ICW4_AUTO       0x02
#define ICW4_BUF_SLAVE  0x08
#define ICW4_BUF_MASTER 0x0c
#define ICW4_SFNM       0x1

#define PIC_EOI         0x20

void pic_remap(interrupt_vector_t irq0, interrupt_vector_t irq8)
{
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC1_DATA, irq0);
    outb(PIC2_DATA, irq8);
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);
}

void pic_irq_mask(uint16_t irq_mask)
{
    outb(PIC1_DATA, (uint8_t)(irq_mask & 0xff));
    outb(PIC2_DATA, (uint8_t)(irq_mask >> 8));
}

void pic_eoi_master()
{
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_eoi_slave()
{
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}
