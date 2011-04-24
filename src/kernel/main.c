#include <stdint.h>

#include <arch/interrupt.h>
#include <kernel/thread_queue.h>
#include <kernel/thread.h>
#include <kernel/spinlock.h>

#include <arch/x86/pic.h>

static void puts(const char *str)
{
    static volatile uint16_t *screen = (uint16_t*)0xb8000;
    for(const char *ptr = str; *ptr; ++ptr)
        *(screen++) = 0x0700 | *ptr;
}

static inline uint64_t hexchar(uint64_t val)
{
    return (val < 10) ? (val + '0') : (val - 10 + 'A');
}

static inline void puthex(int x, int y, uint64_t val)
{
    volatile uint16_t* screen = ((uint16_t*)0xb8000) + y * 80 + x;
    for(size_t i = 0; i < 16; ++i)
        *(screen+i) = 0x0700 | hexchar((val << (4*i)) >> 60);
}

static void interrupt_callback(
    registers_t *registers,
    interrupt_stack_frame_t *interrupt_stack_frame,
    uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e)
{
    (void)a; (void)b; (void)c; (void)d; (void)e;

    for(size_t i = 0; i < (sizeof(registers_t)/sizeof(uint64_t)); ++i)
        puthex(0, i, ((const uint64_t*)registers)[i]);
    for(size_t i = 0; i < (sizeof(interrupt_stack_frame_t)/sizeof(uint64_t)); ++i)
        puthex(20, i, ((const uint64_t*)interrupt_stack_frame)[i]);
}

void kmain(uint64_t magic, uint64_t ptr)
{
    (void)ptr;
    const uint64_t multiboot_magic_number = 0x2badb002;
    if(magic != multiboot_magic_number) return;

    puts("the kernel is alive");

    interrupt_init();

    pic_remap(INT_IRQ0, INT_IRQ7);
    pic_irq_mask(0xffff);

    interrupt_rpc(interrupt_callback, 42, 43, 44, 45, 46);
}
