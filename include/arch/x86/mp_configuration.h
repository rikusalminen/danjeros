#ifndef MP_CONFIGURATION_H
#define MP_CONFIGURATION_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <arch/cpu.h>

typedef struct mp_floating_pointer_t
{
    char signature[4];
    physical_addr_t physical_addr_pointer : 32;
    uint8_t length;
    uint8_t spec_rev;
    uint8_t checksum;
    uint8_t mp_features[5];
} mp_floating_pointer_t;

typedef struct mp_table_header_t
{
    char signature[4];
    uint16_t base_table_len;
    uint8_t spec_rev;
    uint8_t checksum;
    char oem_id[8];
    char product_id[8];
    char space[4];
    physical_addr_t oem_table_pointer : 32;
    uint16_t oem_table_size;
    uint16_t entry_count;
    physical_addr_t lapic_physical_addr : 32;
    uint16_t extended_table_len;
    uint8_t extended_table_checksum;
    uint8_t reserved;
} mp_table_header_t;

typedef struct mp_processor_entry_t
{
    uint8_t entry_type;     // 0
    uint8_t lapic_id;
    uint8_t lapic_version;
    bool cpu_enabled : 1;
    bool boostrap_processor : 1;
    unsigned unused_cpu_flags : 6;
    unsigned processor_stepping : 4;
    unsigned processor_model : 4;
    unsigned processor_family : 4;
    unsigned unused_cpu_type : 20;
    uint32_t features;
    uint32_t reserved[2];
} mp_processor_entry_t;

typedef struct mp_bus_entry_t
{
    uint8_t entry_type;     // 1
    uint8_t bus_id;
    char bus_type[6];
} mp_bus_entry_t;

typedef struct mp_ioapic_entry_t
{
    uint8_t entry_type;     // 2
    uint8_t ioapic_id;
    uint8_t ioapic_version;
    bool enabled : 1;
    unsigned unused_ioapic_flags : 7;
    physical_addr_t memory_map_addr : 32;
} mp_ioapic_entry_t;

typedef struct mp_io_interrupt_entry_t
{
    uint8_t entry_type;     // 3
    uint8_t interrupt_type;
    unsigned polarity : 2;
    unsigned trigger_mode : 2;
    unsigned unused_interrupt_flags : 12;
    uint8_t src_bus_id;
    uint8_t src_bus_irq;
    uint8_t dest_ioapic_id;
    uint8_t dest_ioapic_intin;
} mp_io_interrupt_entry_t;

typedef struct mp_local_interrupt_entry_t
{
    uint8_t entry_type;     // 4
    uint8_t interrupt_type;
    unsigned polarity : 2;
    unsigned trigger_mode : 2;
    unsigned unused_interrupt_flags : 12;
    uint8_t src_bus_id;
    uint8_t src_bus_irq;
    uint8_t dest_lapic_id;
    uint8_t dest_lapic_lintin;
} mp_local_interrupt_entry_t;

typedef struct mp_entry_handler_t
{
    void (*handle_processor_entry)(const mp_processor_entry_t* entry, void *userdata);
    void (*handle_bus_entry)(const mp_bus_entry_t* entry, void *userdata);
    void (*handle_ioapic_entry)(const mp_ioapic_entry_t* entry, void *userdata);
    void (*handle_io_interrupt_entry)(const mp_io_interrupt_entry_t* entry, void *userdata);
    void (*handle_local_interrupt_entry)(const mp_local_interrupt_entry_t* entry, void *userdata);
} mp_entry_handler_t;

bool mp_floating_pointer_validate(const mp_floating_pointer_t* floating_pointer);
const mp_floating_pointer_t* mp_floating_pointer_find_at(const void* base, const void* end);
const mp_floating_pointer_t* mp_floating_pointer_find(size_t lowmem_kb);
bool mp_table_validate(const mp_table_header_t* mp_table);
void mp_table_read(const mp_table_header_t* mp_table, const mp_entry_handler_t* handler, void *userdata);

#endif
