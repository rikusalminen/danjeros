typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

void kmain(uint64_t magic, uint64_t ptr)
{
    (void)ptr;
    const uint64_t multiboot_magic_number = 0x2badb002;
    if(magic != multiboot_magic_number) return;

    volatile uint16_t *screen = (uint16_t*)0xb8000;
    *screen = 0x0739;
}

