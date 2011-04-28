#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002

typedef enum multiboot_symbols_type_t
{
    MULTIBOOT_SYMBOLS_NONE = 0x00,
    MULTIBOOT_SYMBOLS_ELF_SECTION_HEADERS = 0x01,
    MULTIBOOT_SYMBOLS_AOUT_SYMBOL_TABLE = 0x02
} multiboot_symbols_type_t;

typedef struct multiboot_info_flags_t
{
    bool memory : 1;    // is there basic lower/upper memory information?
    bool boot_device : 1;   // is there a boot device set?
    bool cmdline : 1;    // is the command-line defined?
    bool modules : 1;   // are there modules to do something with?
    multiboot_symbols_type_t symbols : 2; // symbol information type
    bool memory_map : 1; // is there a full memory map?
    bool drive_info : 1;    // Is there drive info?
    bool config_table : 1;  // Is there a config table?
    bool boot_loader_name : 1;   // Is there a boot loader name?
    bool apm_table : 1; // Is there a APM table?
    bool video_info : 1;    // Is there video information?
    unsigned unused : 20;
} multiboot_info_flags_t;

/* The symbol table for a.out. */
struct multiboot_aout_symbol_table
{
    uint32_t tabsize;
    uint32_t strsize;
    uint32_t addr;
    uint32_t reserved;
};
typedef struct multiboot_aout_symbol_table multiboot_aout_symbol_table_t;

/* The section header table for ELF. */
struct multiboot_elf_section_header_table
{
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
};
typedef struct multiboot_elf_section_header_table multiboot_elf_section_header_table_t;

typedef union multiboot_symbols_t {
    multiboot_aout_symbol_table_t aout_sym;
    multiboot_elf_section_header_table_t elf_sec;
} multiboot_symbols_t;

struct multiboot_info
{
    /* Multiboot info version number */
    multiboot_info_flags_t flags;

    /* Available memory from BIOS */
    uint32_t mem_lower;
    uint32_t mem_upper;

    /* "root" partition */
    uint32_t boot_device;

    /* Kernel command line */
    uint32_t cmdline;

    /* Boot-Module list */
    uint32_t mods_count;
    uint32_t mods_addr;

    multiboot_symbols_t symbols;

    /* Memory Mapping buffer */
    uint32_t mmap_length;
    uint32_t mmap_addr;

    /* Drive Info buffer */
    uint32_t drives_length;
    uint32_t drives_addr;

    /* ROM configuration table */
    uint32_t config_table;

    /* Boot Loader Name */
    uint32_t boot_loader_name;

    /* APM table */
    uint32_t apm_table;

    /* Video */
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
};
typedef struct multiboot_info multiboot_info_t;

#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2

struct multiboot_mmap_entry
{
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
} __attribute__((packed));
typedef struct multiboot_mmap_entry multiboot_memory_map_t;

struct multiboot_mod_list
{
    /* the memory used goes from bytes 'mod_start' to 'mod_end-1' inclusive */
    uint32_t mod_start;
    uint32_t mod_end;

    /* Module command line */
    uint32_t cmdline;

    /* padding to take it to 16 bytes (must be zero) */
    uint32_t pad;
};
typedef struct multiboot_mod_list multiboot_module_t;

#endif
