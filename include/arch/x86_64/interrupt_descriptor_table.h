#ifndef X86_86_INTERRUPT_DESCRIPTOR_TABLE_H
#define X86_86_INTERRUPT_DESCRIPTOR_TABLE_H

#include <stdint.h>
#include <stdbool.h>

#include <arch/x86_64/cpu.h>

typedef enum interrupt_type_t
{
    INTERRUPT_TYPE_INT64 = 0xe,
    INTERRUPT_TYPE_TRAP64 = 0xf
} interrupt_type_t;

typedef struct interrupt_descriptor_t
{
    uint16_t offset_low;
    segment_selector_t segment_selector;
    int interrupt_stack_table : 3;
    int unused0 : 5;
    interrupt_type_t type : 4;
    int unused1 : 1;
    cpu_privilege_t privilege_level : 2;
    bool present : 1;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} interrupt_descriptor_t;

static inline interrupt_descriptor_t interrupt_descriptor(
    void* isr_linear_address,
    segment_selector_t segment_selector,
    int interrupt_stack_table,
    interrupt_type_t type,
    cpu_privilege_t privilege_level
    )
{
    uint64_t offset = (uint64_t)(isr_linear_address);
    interrupt_descriptor_t desc = {
        .offset_low = (offset & 0xffff),
        .segment_selector = segment_selector,
        .interrupt_stack_table = interrupt_stack_table,
        .unused0 = 0,
        .type = type,
        .unused1 = 0,
        .privilege_level = privilege_level,
        .present = 1,
        .offset_mid = (offset & 0xffff0000) >> 16,
        .offset_high = (offset >> 32),
        .reserved = 0
    };
    return desc;
}

static inline void lidt(uint16_t limit, void* linear_address)
{
    struct { uint16_t limit; void *linear_address; } __attribute__((packed)) idtr = { limit, linear_address };
    __asm__ __volatile__ ("lidt (%[idtr])" : : [idtr] "p" (&idtr));
}

#endif
