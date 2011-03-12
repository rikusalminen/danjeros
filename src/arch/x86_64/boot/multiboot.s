    .set MULTIBOOT_PAGE_ALIGN, 1 << 0
    .set MULTIBOOT_MEM_INFO, 1 << 1
    .set MULTIBOOT_AOUT_KLUDGE, 1 << 16
    .set MULTIBOOT_MAGIC, 0x1BADB002
    .set MULTIBOOT_FLAGS, MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEM_INFO | MULTIBOOT_AOUT_KLUDGE
    .set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

    .section .mboot
    .align 4

    .global multiboot_header

multiboot_header:
    .int MULTIBOOT_MAGIC
    .int MULTIBOOT_FLAGS
    .int MULTIBOOT_CHECKSUM
    .int multiboot_header
    .int text
    .int data_end
    .int kernel_end
    .int start


