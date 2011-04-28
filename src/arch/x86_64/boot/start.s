    .extern kmain

    .section .data

    .align 16
gdtr:
gdtr_limit:
    .word (global_descriptor_table_end - global_descriptor_table) - 1
gdtr_pointer:
    .int global_descriptor_table

    .global global_descriptor_table
global_descriptor_table:
null_descriptor:
    .quad 0x0000000000000000
code_descriptor:
    .quad 0x0020980000000000
data_descriptor:
    .quad 0x0000900000000000
global_descriptor_table_end:

    .global null_segment
    .set null_segment, (null_descriptor - global_descriptor_table)
    .global code_segment
    .set code_segment, (code_descriptor - global_descriptor_table)
    .global data_segment
    .set data_segment, (data_descriptor - global_descriptor_table)

multiboot_magic:
    .space 4
multiboot_info:
    .space 4

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

    .global kernel_stack
kernel_stack:
    .space 0x1000
kernel_stack_end:

    .section .text
    .code32

    .global start

start:
    cli

# store multiboot parameters in .data
    mov %eax, multiboot_magic
    mov %ebx, multiboot_info

# zerofill .bss
    cld
    mov $bss, %edi
    mov $bss_end, %ecx
    sub %edi, %ecx
    xor %eax, %eax
    rep stosb

# create pagetable for identity mapping lower 2 megabytes
# make minimal page table entries
    .set pml4_entry, (pdpt + 0x03)
    .set pdpt_entry, (pd + 0x03)
    .set pd_entry, 0b10000011
    movl $pml4_entry, pml4
    movl $pdpt_entry, pdpt
    movl $pd_entry, pd

# setup long mode
# load global descriptor table
    lgdt (gdtr)

# enable Physical Address Extension (PAE)
    mov %cr4, %eax
    bts $5, %eax
    mov %eax, %cr4

# set up page table
    mov $kernel_pagetable, %eax
    mov %eax, %cr3

# set up long mode
    .set EFER_MSR_ADDRESS, 0xC0000080
    mov $EFER_MSR_ADDRESS, %ecx
    rdmsr
    bts $8, %eax
    wrmsr

# enable paging
    mov %cr0, %eax
    bts $31, %eax
    mov %eax, %cr0

# long jump to set code segment
    ljmp $code_segment, $longmode_start

    .code64
longmode_start:
# data segment selector to all data segments
    mov $data_segment, %bx
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs

# null segment selector to ss
    mov $null_segment, %bx
    mov %bx, %ss

# set up kernel stack
    mov $kernel_stack_end, %rsp
    push $0     # debugger backtrace stops here

# call kmain
    mov multiboot_magic, %edi
    mov multiboot_info, %esi
    call kmain

# hang the computer
    cli
hang:
    hlt
    jmp hang

