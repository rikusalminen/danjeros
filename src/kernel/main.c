#include <stdint.h>

#include <arch/interrupt.h>
#include <kernel/thread_queue.h>
#include <kernel/thread.h>
#include <kernel/spinlock.h>
#include <kernel/scheduler.h>
#include <kernel/mutex.h>
#include <kernel/condition.h>

#include <arch/x86/pic.h>

/* simple loop based delay: */
static void delay_loop(unsigned long loops)
{
    __asm__ __volatile__(
        "       test %0,%0      \n"
        "       jz 3f           \n"
        "       jmp 1f          \n"

        ".align 16              \n"
        "1:     jmp 2f          \n"

        ".align 16              \n"
        "2:     dec %0          \n"
        "       jnz 2b          \n"
        "3:     dec %0          \n"

        : /* we don't need output */
        :"a" (loops)
        );
}

static void puts(const char *str)
{
    static mutex_t mutex = MUTEX_INIT;
    static volatile uint16_t *screen = (uint16_t*)0xb8000;

    mutex_lock(&mutex);

    for(const char *ptr = str; *ptr; ++ptr)
    {
        *(screen++) = 0x0700 | *ptr;
        delay_loop(0xfffff);
    }

    mutex_unlock(&mutex);

    /*scheduler_yield(true);*/
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

void task(uint64_t initial, uint64_t increment, uint64_t x, uint64_t y, uint64_t msg_ptr, uint64_t repeat)
{
    const char *msg = (const char*)msg_ptr;
    for(size_t i = 0; i < repeat; ++i)
        puts(msg);

    uint64_t counter = initial;
    while(true)
    {
        puthex(x, y, counter += increment);
        scheduler_yield(true);
    }
}

#define NUM_THREADS 3
static thread_t threads[NUM_THREADS];
#define THREAD_STACK_SIZE 1024
static uint8_t thread_stacks[NUM_THREADS][THREAD_STACK_SIZE];
const char *greets[NUM_THREADS+1] = { "foo", "bar", "baz", "quux" };
const size_t greet_repeats = 400/(NUM_THREADS+1);

void kmain(uint64_t magic, uint64_t ptr)
{
    (void)ptr;
    const uint64_t multiboot_magic_number = 0x2badb002;
    if(magic != multiboot_magic_number) return;

    /*puts("the kernel is alive");*/

    interrupt_init();

    pic_remap(INT_IRQ0, INT_IRQ7);
    pic_irq_mask(0xfffe); // unmask timer interrupt (irq 0)

    scheduler_init();

    for(size_t i = 0; i < NUM_THREADS; ++i)
    {
        thread_init(&threads[i],
            task,
            thread_stacks[i] + THREAD_STACK_SIZE,
            (i&1) << 63,
            1 << i,
            5, 16+i,
            (uint64_t)greets[i+1], greet_repeats);
        scheduler_add_ready(&threads[i]);
    }


    interrupt_enable();
    task(0, 1, 5, 15, (uint64_t)greets[0], greet_repeats);
    interrupt_disable();
}
