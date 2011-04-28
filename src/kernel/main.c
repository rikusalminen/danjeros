#include <stdint.h>

#include <arch/interrupt.h>
#include <kernel/thread_queue.h>
#include <kernel/thread.h>
#include <kernel/spinlock.h>
#include <kernel/scheduler.h>

#include <arch/x86/pic.h>
#include <arch/x86/boot/multiboot.h>

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

void task(uint64_t initial, uint64_t increment, uint64_t x, uint64_t y, uint64_t e, uint64_t f)
{
    (void)e; (void)f;
    uint64_t counter = initial;
    while(true)
    {
        puthex(x, y, counter += increment);
    }
}

thread_t other_thread;
uint8_t thread_stack[1024];

void kmain(uint64_t magic, uint64_t ptr)
{
    (void)ptr;
    assert(magic == MULTIBOOT_BOOTLOADER_MAGIC);

    puts("the kernel is alive");

    interrupt_init();

    pic_remap(INT_IRQ0, INT_IRQ7);
    pic_irq_mask(0xfffe); // unmask timer interrupt (irq 0)

    thread_init(&other_thread, task, thread_stack + sizeof(thread_stack), 1ul << 63, 2, 5, 16, 0, 0);

    scheduler_init();
    scheduler_add_ready(&other_thread);

    interrupt_enable();
    task(0, 1, 5, 15, 0, 0);
    interrupt_disable();
}
