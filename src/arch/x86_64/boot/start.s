    .intel_syntax noprefix

    .section .text
    .code32

    .global start

start:
    cli

    # setup page table for identity mapping the lower 2 megabytes
    # zerofill pagetable
    cld
#mov edi, kernel_pagetable
    lea edi, kernel_pagetable
    mov ecx, (kernel_pagetable_end - kernel_pagetable)
    xor eax, eax
    rep stosb


    # setup minimal page table
    mov eax, 0x103003 #(kernel_pagetable + 0x1000)
#or eax, 3
    mov [pml4], eax
    mov eax, 0x104003 #(kernel_pagetable + 0x2000)
#or eax, 3
    mov [pdpt], eax
    mov eax, 0b10000011
    mov [pd], eax
pagetable_full:


    # setup long mode
    # load global descriptor table
    lgdt gdtr

    # enable physical address extension (PAE)
    mov eax, cr4
    bts eax, 5
    mov cr4, eax

    # set page table address to cr3
    lea eax, kernel_pagetable
    mov cr3, eax

    # set long mode bit in EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    bts eax, 8
    wrmsr

    # enable paging
    mov edx, cr0
    bts edx, 31
    mov cr0, edx

    jmp 0x08:long_start

    mov eax, 0xb8000
    mov bx, (0x0700 | 0x36)
    mov [eax], bx

    .code64
long_start:
    # set data segment selector to all data segments
    mov bx, 0x10
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    # zero ss
    xor bx, bx
    mov ss, bx

hang:
    hlt
    jmp hang

    .section .data
global_descriptor_table:
    .quad 0x0000000000000000    # null descriptor
    .quad 0x0020980000000000    # code descriptor
    .quad 0x0000900000000000    # data descriptor
global_descriptor_table_end:
gdtr:
    .int (global_descriptor_table_end - global_descriptor_table) - 1 #limit
    .int global_descriptor_table # pointer

    .section .bss

    .global kernel_pagetable
    .align 0x1000
kernel_pagetable:
pml4:
    .space 0x1000
pdpt:
    .space 0x1000
pd:
    .space 0x1000
kernel_pagetable_end:



