#include <stdint.h>

#include <arch/interrupt.h>
#include <kernel/thread_queue.h>
#include <kernel/thread.h>

#include <arch/x86/pic.h>
#include <arch/x86/ioport.h>

static threadq_t thread_queue;
static thread_t *current_thread;

void interrupt_handler(
    interrupt_vector_t vector,
    uint64_t error_code,
    registers_t *registers,
    interrupt_stack_frame_t *interrupt_stack_frame
    )
{
    (void)vector;
    (void)error_code;
    (void)registers;
    (void)interrupt_stack_frame;
    // this is an excellent place to put debugger breakpoints

    if(vector == INT_IRQ0) // timer interrupt
    {
        current_thread->registers = *registers;
        current_thread->interrupt_stack_frame = *interrupt_stack_frame;

        threadq_enqueue(&thread_queue, &current_thread->queue_node);
        current_thread = threadq_dequeue(&thread_queue)->thread;

        *registers = current_thread->registers;
        *interrupt_stack_frame = current_thread->interrupt_stack_frame;
    }

    if(vector == INT_IRQ1) inb(0x60); // read data from keyboard controller, for it to fire more than once

    // send end-of-interrupt (EOI) to PIC
    if(vector >= INT_IRQ0 && vector <= INT_IRQ7) pic_eoi_master();
    else if(vector >= INT_IRQ8 && vector <= INT_IRQ15) pic_eoi_slave();

    volatile uint16_t *screen = (uint16_t*)0xb8000;
    *screen = 0x0739;
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

static void task(uint64_t x, uint64_t y, uint64_t initial_value, uint64_t increment, uint64_t unused0, uint64_t unused1)
{
    (void)unused0;
    (void)unused1;

    uint64_t counter = initial_value;
    while(true) puthex(x, y, counter += increment);
}

static thread_t main_thread, other_thread;
static uint8_t thread_stack[1024];

void kmain(uint64_t magic, uint64_t ptr)
{
    (void)ptr;
    const uint64_t multiboot_magic_number = 0x2badb002;
    if(magic != multiboot_magic_number) return;

    interrupt_init();

    pic_remap(INT_IRQ0, INT_IRQ7);
    pic_irq_mask(0xfffc); // unmask keyboard interrupt (irq #1) and timer interrupt (irq #0)

    // initialize a few threads
    thread_init(&main_thread, NULL, NULL, 0, 0, 0, 0, 0, 0);
    thread_init(&other_thread, &task, thread_stack + sizeof(thread_stack), 5, 11, 1ul << 63, 2, 0xf00, 0xf00);

    threadq_init(&thread_queue);
    threadq_enqueue(&thread_queue, &other_thread.queue_node);
    current_thread = &main_thread;

    interrupt_enable();

    task(5, 10, 0, 1, 0xf00, 0xf00);

    interrupt_disable();
}
