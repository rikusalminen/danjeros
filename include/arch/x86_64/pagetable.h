#ifndef X86_64_PAGETABLE_H
#define X86_64_PAGETABLE_H

#include <stdint.h>
#include <stdbool.h>

#include <arch/cpu.h>

#define PAGETABLE_ADDRESS_MASK ((physical_addr_t)~0xfff)
#define PAGETABLE_ADDRESS_SHIFT (12)
#define PAGETABLE_ADDRESS_BITS  (40)
#define PAGETABLE_LINEAR_ADDRESS_BITS   (48)

typedef struct pagetable_flags_t
{
    bool present : 1;
    bool read_only : 1;
    bool supervisor : 1;
    bool page_write_through : 1;
    bool page_cache_disable : 1;
    bool accessed : 1;
    bool dirty : 1;
    bool pat_or_large_page : 1;
} pagetable_flags_t;

typedef struct pagetable_entry_t
{
    pagetable_flags_t flags;
    bool global : 1;
    unsigned ignored0 : 3;

    physical_addr_t address : PAGETABLE_ADDRESS_BITS;

    unsigned ignored1 : 11;
    bool execute_disable : 1;
} pagetable_entry_t;


static inline pagetable_entry_t pagetable_entry_large_page(physical_addr_t frame_addr, pagetable_flags_t flags, bool global, bool pat, bool execute_disable)
{
    pagetable_entry_t entry = {
        .flags = flags,
        .global = global ? 1 : 0,
        .address = ((frame_addr & PAGETABLE_ADDRESS_MASK) >> PAGETABLE_ADDRESS_SHIFT)
                    | (pat ? 1 : 0),
        .execute_disable = execute_disable ? 1 : 0
        };
    entry.flags.pat_or_large_page = 1;
    return entry;
}

#define PAGETABLE_LEVELS            (4)
#define PAGETABLE_LEVEL_SHIFT       (9)
#define PAGETABLE_LEVEL_MASK        (0x1ff)

static inline physical_addr_t pagetable_translate(pagetable_entry_t* pagetable, uint64_t linear_address)
{
    pagetable_entry_t *ptr = pagetable;
    for(int level = PAGETABLE_LEVELS; level > 0; --level)
    {
        uint64_t selector = (linear_address >> (PAGETABLE_ADDRESS_SHIFT + (level - 1) * PAGETABLE_LEVEL_SHIFT)) & PAGETABLE_LEVEL_MASK;
        pagetable_entry_t *entry = ptr + selector;
        if(!entry->flags.present) return 0;
        if(level == 1 || (entry->flags.pat_or_large_page && level < PAGETABLE_LEVELS))
        {
            uint64_t offset = linear_address &
                (~0 >> (sizeof(uint64_t) * 8 - PAGETABLE_LINEAR_ADDRESS_BITS + (PAGETABLE_LEVELS - level + 1) * PAGETABLE_LEVEL_SHIFT));
            return (entry->address << PAGETABLE_ADDRESS_SHIFT) | offset;
        }
        ptr = (pagetable_entry_t*)((uint64_t)entry->address << PAGETABLE_ADDRESS_SHIFT);
    }

    return 0;
}

#endif
