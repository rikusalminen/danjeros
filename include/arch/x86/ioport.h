#ifndef X86_IOPORT_H
#define X86_IOPORT_H

#include <stdint.h>

typedef uint16_t ioport_t;

static inline void outb(ioport_t port, uint8_t val)
{
    __asm__ __volatile__ ("outb %0, %1" :: "a"(val), "Nd" (port));
}

static inline uint8_t inb(ioport_t port)
{
    uint8_t val;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

#endif
