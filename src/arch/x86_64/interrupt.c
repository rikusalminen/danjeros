#include <stdint.h>
#include <stddef.h>

#include <arch/x86_64/interrupt.h>
#include <arch/x86_64/interrupt_descriptor_table.h>

extern interrupt_descriptor_t interrupt_descriptor_table[];

extern const int INTERRUPT_MAX;
extern const int INTERRUPT_ISR_ALIGN;
extern void *isr0x00;

void interrupt_init()
{
    uint64_t max = (uint64_t)&INTERRUPT_MAX;
    size_t align = (size_t)&INTERRUPT_ISR_ALIGN;
    void *interrupt_service_routines = &isr0x00;

    for(uint64_t i = 0; i < max; ++i)
    {
        segment_selector_t code_segment = 0x08; // TODO: compute this from descriptor addresses instead of using a magic constant
        int interrupt_stack_table = 0;
        cpu_privilege_t privilege_level = 0;
        void *isr = ((char*)interrupt_service_routines + i * align);
        interrupt_descriptor_t desc = interrupt_descriptor(
            isr,
            code_segment,
            interrupt_stack_table,
            INTERRUPT_TYPE_INT64,
            privilege_level);
        interrupt_descriptor_table[i] = desc;
    }

    lidt(max * sizeof(interrupt_descriptor_t) - 1, interrupt_descriptor_table);
}
