#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <stdint.h>
#include <stdbool.h>

#include <arch/cpu.h>

typedef struct pagetable_entry_t pagetable_entry_t;

static inline physical_addr_t pagetable_translate(pagetable_entry_t* pagetable, uint64_t linear_address);

#include <arch/x86_64/pagetable.h>

extern pagetable_entry_t kernel_pagetable[];

#endif

