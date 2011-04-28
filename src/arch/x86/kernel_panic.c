#include <stdint.h>

#include <kernel/kernel_panic.h>

static void panic_hang()
{
    __asm__ __volatile__(
        "    cli \n"
        ".hang: \n"
        "    jmp .hang \n"
        );
}

static void panic_message(const char *msg)
{
    volatile uint16_t *screen = (uint16_t*)0xb8000;
    for(const char *ptr = msg; *ptr; ++ptr)
        *screen++ = 0x0700 | *ptr;
}

void kernel_panic(const char *msg)
{
    panic_message(msg ? msg : "KERNEL PANIC! ");
    panic_hang();
}
