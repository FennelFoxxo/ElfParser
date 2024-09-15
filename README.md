# ElfParser
Minimal C library for parsing ELF executable files.

In particular, this library:
- Only requires <stdint.h>, <stdbool.h>, and <string.h>
- Performs no dynamic memory allocation
- Performs as few validation checks as necessary (e.g. checking elf magic number) and returns header fields as is

It supports reading the ELF header, section headers, program headers, and symbols from the .symtab section. It supports reading from 32- or 64-bit, little- or big- endian ELF files

# Documentation

## Functions


### elfparser_get_header
- `ElfParser_Error elfparser_get_header(const void* elf_start, uint64_t elf_size, ElfParser_Header* header_out)`
- Reads ELF header info - this should be the first function called to parse the ELF data
- `elf_start`: pointer to the start of an array of bytes conforming to the structure of an ELF file
- `elf_size`: total size of this array in bytes
- `header_out`: location in which to return the data contained in the ELF header
- Returns: `ELFPARSER_NOERROR` on success, `ELFPARSER_INVALID` on failure

### elfparser_get_section_header
- `ElfParser_Error elfparser_get_section_header(const void* elf_start, const ElfParser_Header* header, uint64_t index, ElfParser_SectionHeader* section_header_out)`
- Reads section header info, given the index of the section to parse
- `elf_start`: pointer to the start of an array of bytes conforming to the structure of an ELF file
- `header`: pointer to the ELF header data - should be obtained by a previous call to `elfparser_get_header`
- `index`: index of the section to read from
- `section_header_out`: location in which to return the data contained in the requested section header
- Returns: `ELFPARSER_NOERROR` on success, `ELFPARSER_INVALID` on failure

### elfparser_get_section_header_by_name
- `ElfParser_Error elfparser_get_section_header_by_name(const void* elf_start, const ElfParser_Header* header, const char* name, ElfParser_SectionHeader* section_header_out)`
- Similar to previous function, but reads from the first section matching the given name
- `elf_start`: pointer to the start of an array of bytes conforming to the structure of an ELF file
- `header`: pointer to the ELF header data - should be obtained by a previous call to `elfparser_get_header`
- `name`: name of the section to read from
- `section_header_out`: location in which to return the data contained in the requested section header
- Returns: `ELFPARSER_NOERROR` on success, `ELFPARSER_INVALID` on failure, `ELFPARSER_NOT_FOUND` if section not found

### elfparser_get_symbol
- `ElfParser_Error elfparser_get_symbol(const void* elf_start, const ElfParser_Header* header, uint64_t index, ElfParser_Symbol* symbol_out)`
- Reads symbol info from .symtab, given the index of the symbol to parse
- `elf_start`: pointer to the start of an array of bytes conforming to the structure of an ELF file
- `header`: pointer to the ELF header data - should be obtained by a previous call to `elfparser_get_header`
- `index`: index of the symbol to read from
- `symbol_out`: location in which to return the data contained in the requested symbol

### elfparser_get_symbol_by_name
- `ElfParser_Error elfparser_get_symbol_by_name(const void* elf_start, const ElfParser_Header* header, const char* name, ElfParser_Symbol* symbol_out)`
- Similar to previous function, but reads from the first symbol matching the given name
- `elf_start`: pointer to the start of an array of bytes conforming to the structure of an ELF file
- `header`: pointer to the ELF header data - should be obtained by a previous call to `elfparser_get_header`
- `name`: name of the symbol to read from
- `symbol_out`: location in which to return the data contained in the requested symbol
- Returns: `ELFPARSER_NOERROR` on success, `ELFPARSER_INVALID` on failure, `ELFPARSER_NOT_FOUND` if section not found

### elfparser_get_program_header
- `ElfParser_Error elfparser_get_program_header(const void* elf_start, const ElfParser_Header* header, uint64_t index, ElfParser_ProgramHeader* program_header_out)`
- Reads program header info, given the index of the program header to parse
- `elf_start`: pointer to the start of an array of bytes conforming to the structure of an ELF file
- `header`: pointer to the ELF header data - should be obtained by a previous call to `elfparser_get_header`
- `index`: index of the program header to read from
- `program_header_out`: location in which to return the data contained in the requested program header
- Returns: `ELFPARSER_NOERROR` on success, `ELFPARSER_INVALID` on failure





