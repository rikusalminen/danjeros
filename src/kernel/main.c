#include <stdint.h>

#include <arch/interrupt.h>
#include <arch/pagetable.h>
#include <kernel/thread_queue.h>
#include <kernel/thread.h>
#include <kernel/spinlock.h>
#include <kernel/scheduler.h>

#include <arch/x86/pic.h>

static void puts(const char *str)
{
    static volatile uint16_t *screen = (uint16_t*)0xb8000;
    for(const char *ptr = str; *ptr; ++ptr)
        *(screen++) = 0x0700 | *ptr;
}

void kmain(uint64_t magic, uint64_t ptr)
{
    (void)ptr;
    const uint64_t multiboot_magic_number = 0x2badb002;
    assert(magic == multiboot_magic_number);

    puts("the kernel is alive");

    interrupt_init();

    pic_remap(INT_IRQ0, INT_IRQ7);
    pic_irq_mask(0xffff); // unmask timer interrupt (irq 0)

    // trying to read or write outside the 2M page of virtual memory causes a page fault
    volatile uint64_t *pagefault_ptr = (uint64_t*)((2 * (1024*1024)) | 0xbeef);
    uint64_t value = *pagefault_ptr;
    (void)value;
    *pagefault_ptr = 0x4242424213131313;
}
