# a.out(5) - Executable Binary File Format

## Overview
The `a.out` format is the classic Unix executable file format used by FreeBSD and other Unix-like systems. It defines the structure of executable machine code files ("binaries").

## File Structure
An a.out binary consists of up to 7 sections in order:

1. **Exec Header** (`struct exec`) - Contains kernel loading parameters (mandatory)
2. **Text Segment** - Machine code and related data (loaded read-only)
3. **Data Segment** - Initialized data (loaded writable)
4. **Text Relocations** - Records for updating text segment pointers during linking
5. **Data Relocations** - Records for updating data segment pointers during linking
6. **Symbol Table** (`struct nlist`) - Named variables and functions
7. **String Table** - Character strings for symbol names

## Exec Header Structure
```c
struct exec {
    unsigned long   a_midmag;   // Magic number and machine ID
    unsigned long   a_text;     // Text segment size in bytes
    unsigned long   a_data;     // Data segment size in bytes
    unsigned long   a_bss;      // BSS segment size in bytes
    unsigned long   a_syms;     // Symbol table size in bytes
    unsigned long   a_entry;    // Entry point address
    unsigned long   a_trsize;   // Text relocation table size
    unsigned long   a_drsize;   // Data relocation table size
};
```

## Magic Numbers
The `a_midmag` field contains a magic number that determines loading conventions:

- **OMAGIC** - Text and data segments are contiguous and loaded into writable memory
- **NMAGIC** - Text loaded into read-only memory, data into writable memory at next page boundary
- **ZMAGIC** - Demand-paged loading with page alignment; text read-only, data writable

## Segment Types
- **Text Segment** (`a_text`): Machine code, typically loaded read-only
- **Data Segment** (`a_data`): Initialized data, loaded writable
- **BSS Segment** (`a_bss`): Uninitialized data, allocated as zero-filled memory after data segment

## Relocation Records
```c
struct relocation_info {
    int     r_address;      // Byte offset of pointer to relocate
    unsigned int r_symbolnum:24,  // Symbol number or n_type value
                 r_pcrel:1,       // PC-relative addressing
                 r_length:2,      // Pointer length (0=1B, 1=2B, 2=4B)
                 r_extern:1,      // External reference required
                 r_baserel:1,     // Relocate to Global Offset Table
                 r_jmptable:1,    // Relocate to Procedure Linkage Table
                 r_relative:1,    // Run-time load address relative
                 r_copy:1;        // Copy symbol contents
};
```

## Symbol Table
```c
struct nlist {
    union {
        const char *n_name;  // Pointer to symbol name
        long        n_strx;  // Byte offset in string table
    } n_un;
    unsigned char n_type;    // Symbol type (N_TEXT, N_DATA, N_BSS, etc.)
    char          n_other;   // Additional symbol info (AUX_FUNC, AUX_OBJECT)
    short         n_desc;    // Debugger information
    unsigned long n_value;   // Symbol value (address for text/data/bss)
};
```

## Symbol Types (N_TYPE mask)
- **N_UNDF** - Undefined symbol (requires external definition)
- **N_ABS** - Absolute symbol (not updated by linker)
- **N_TEXT** - Text address symbol
- **N_DATA** - Data address symbol
- **N_BSS** - BSS address symbol
- **N_FN** - Filename symbol (for debuggers)

## String Table
- Begins with 4-byte length (entire table size)
- Followed by null-terminated symbol strings
- Minimum length is 4 bytes (empty table)

## Key Macros
- `N_BADMAG(exec)` - Nonzero if invalid magic number
- `N_TXTOFF(exec)` - Byte offset of text segment
- `N_SYMOFF(exec)` - Byte offset of symbol table
- `N_STROFF(exec)` - Byte offset of string table

## Modern Usage
While largely superseded by the ELF format on modern systems, understanding a.out is important for:
- Historical context of Unix development
- Working with legacy systems and tools
- Understanding fundamental concepts of executable file formats

## See Also
- `as(1)` - Assembler
- `ld(1)` - Link editor
- `nlist(3)` - Symbol table access
- `elf(5)` - Modern executable format
- `stab(5)` - Symbol table types for debuggers

## Notes
- The format supports position-independent code (PIC) and dynamic linking
- Debugger information is stored in symbol table entries using stab types
- The `a_midmag` field includes machine identification for cross-architecture compatibility