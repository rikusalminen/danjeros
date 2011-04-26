#include <stdint.h>

#include <arch/x86/mp_configuration.h>

static inline physical_addr_t ebda_find()
{
    volatile uint16_t* ebda_pointer = (uint16_t*)0x40e;
    return (physical_addr_t)*ebda_pointer;
}

bool mp_floating_pointer_validate(const mp_floating_pointer_t* floating_pointer)
{
    static const char mp_signature[] = "_MP_";
    if(*(uint32_t*)floating_pointer->signature != *(uint32_t*)mp_signature) return false;

    uint8_t checksum = 0;
    for(uint8_t *ptr = (uint8_t*)floating_pointer;
        ptr != ((uint8_t*)floating_pointer) + floating_pointer->length * 16;
        ++ptr)
        checksum += *ptr;
    if(checksum != 0) return false;

    return true;
}

const mp_floating_pointer_t* mp_floating_pointer_find_at(const void* base, const void* end)
{
    for(const mp_floating_pointer_t* ptr = (const mp_floating_pointer_t*)base;
        ptr != (const mp_floating_pointer_t*)end;
        ++ptr)
    {
        if(mp_floating_pointer_validate(ptr)) return ptr;
    }

    return 0;
}

const mp_floating_pointer_t* mp_floating_pointer_find(size_t lowmem_kb)
{
    physical_addr_t ebda = ebda_find();
    const mp_floating_pointer_t* ptr;
    if((ptr = mp_floating_pointer_find_at((void*)ebda, (void*)ebda + 1024)))
        return ptr;
    if((ptr = mp_floating_pointer_find_at((void*)((lowmem_kb - 1) * 1024), (void*)(lowmem_kb * 1024))))
        return ptr;
    if((ptr = mp_floating_pointer_find_at((void*)0x0f0000, (void*)0x100000)))
        return ptr;
    return 0;
}

bool mp_table_validate(const mp_table_header_t* mp_table)
{
    static const char pcmp_signature[] = "PCMP";
    if(*(uint32_t*)mp_table->signature != *(uint32_t*)pcmp_signature) return false;

    uint8_t checksum;
    for(uint8_t* ptr = (uint8_t*)mp_table;
        ptr != ((uint8_t*)mp_table) + mp_table->base_table_len;
        ++ptr)
        checksum += *ptr;

    if(checksum != 0) return false;

    return true;
}

void mp_table_read(const mp_table_header_t* mp_table, const mp_entry_handler_t* handler, void *userdata)
{
    typedef void (*handler_func)(const void*, void*);
    handler_func* callbacks = (handler_func*)handler;
    static const size_t sizes[] = {
        sizeof(mp_processor_entry_t),
        sizeof(mp_bus_entry_t),
        sizeof(mp_ioapic_entry_t),
        sizeof(mp_io_interrupt_entry_t),
        sizeof(mp_local_interrupt_entry_t) };

    const uint8_t* ptr = ((const uint8_t*)mp_table) + sizeof(mp_table_header_t);
    for(unsigned i = 0; i < mp_table->entry_count; ++i)
    {
        uint8_t entry_type = *ptr;
        handler_func callback = callbacks[entry_type];
        if(callback) callback(ptr, userdata);
        ptr += sizes[*ptr];
    }
}
