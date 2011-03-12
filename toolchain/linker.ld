OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(start)

phys = 0x0100000;

SECTIONS
{
    . = phys;
    kernel_start = .;

    .text ALIGN(4096) : AT( ADDR(.text) )
    {
        text = .;
        *(.mboot)  /* Put Multiboot header section in the beginning of .text section */
        *(.text)
        *(.rodata)
        text_end = .;
    }

    .data ALIGN(4096) : AT( ADDR(.data) )
    {
        data = .;
        *(.data)
        data_end = .;
    }

    .bss ALIGN(4096) : AT( ADDR(.bss) )
    {
        bss = .;
        *(.bss)
        bss_end = .;
    }

    kernel_end = .;
}
