# ElfParser
Minimal C library for parsing ELF executable files.

In particular, this library:
- Only requires <stdint.h>, <stdbool.h>, and <string.h>
- Performs no dynamic memory allocation
- Performs as few validation checks as necessary (e.g. checking elf magic number) and returns header fields as is

It supports reading the ELF header, section headers, program headers, and symbols from the .symtab section. It supports reading from 32- or 64-bit, little- or big- endian ELF files

# Documentation

## Parsing functions

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

### elfparser_copy_segment
- ` uint64_t elfparser_copy_segment(const void* elf_start, const ElfParser_Header* header, uint64_t segment_index, void* dest, uint64_t skip, uint64_t num_bytes)`
- Copies a segment referenced by a program header into a buffer. Respects segment file size and memory size - if file size is 100 bytes but memory size is 200 bytes, it will copy 100 bytes from the file and fill the next 100 bytes with 0s
- `elf_start`: pointer to the start of an array of bytes conforming to the structure of an ELF file
- `header`: pointer to the ELF header data - should be obtained by a previous call to `elfparser_get_header`
- `segment_index`: index of the segment to copy. Can be obtained from `index` field of `ElfParser_ProgramHeader`
- `dest`: pointer to the buffer in which to place the data. If NULL, the total memory size (i.e. # of bytes to copy) is returned
- `skip`: offset in bytes to skip ahead in the segment before copying begins - allows for incremental reads
- `num_bytes`: reads up to this many bytes from the segment, but the actual amount of bytes copied may be lower if copying would run past the end of the segment
- Returns: `ELFPARSER_INVALID` on failure (equivalent to `UINT64_MAX`), else # of bytes copied on success. If `dest` is NULL, returns total memory size of the segment

## Validation functions
These are optional functions which may be used to check if certain fields are valid, (i.e. checking if section type is valid). This library attempts to be as liberal as possible in what input it accepts, so many of these checks are not performed when reading the ELF file. Some of these functions always return true because all possible inputs are potentially valid (e.g. e_machine) - these are here for completeness. Every `x` enum has a matching `elfparser_is_valid_x` function

### elfparser_is_valid_ei_class
- static inline bool elfparser_is_valid_ei_class(ElfParser_EI_Class value)
### elfparser_is_valid_ei_data
- static inline bool elfparser_is_valid_ei_data(ElfParser_EI_Data value)
### elfparser_is_valid_ei_version
- static inline bool elfparser_is_valid_ei_version(ElfParser_EI_Version value)
### elfparser_is_valid_ei_osabi
- static inline bool elfparser_is_valid_ei_osabi(ElfParser_EI_OSABI value)
### elfparser_is_valid_e_type
- static inline bool elfparser_is_valid_e_type(ElfParser_E_Type value)
### elfparser_is_valid_e_machine
- static inline bool elfparser_is_valid_e_machine(ElfParser_E_Machine value)
### elfparser_is_valid_e_version
- static inline bool elfparser_is_valid_e_version(ElfParser_E_Version value)
### elfparser_is_valid_sh_type
- static inline bool elfparser_is_valid_sh_type(ElfParser_SH_Type value)
### elfparser_is_valid_sh_flags
- static inline bool elfparser_is_valid_sh_flags(ElfParser_SH_Flags value)
### elfparser_is_valid_st_bind
- static inline bool elfparser_is_valid_st_bind(ElfParser_ST_Bind value)
### elfparser_is_valid_st_type
- static inline bool elfparser_is_valid_st_type(ElfParser_ST_Type value)
### elfparser_is_valid_st_visibility
- static inline bool elfparser_is_valid_st_visibility(ElfParser_ST_Visibility value)
### elfparser_is_valid_p_type
- static inline bool elfparser_is_valid_p_type(ElfParser_P_Type value)
### elfparser_is_valid_p_flags
- static inline bool elfparser_is_valid_p_flags(ElfParser_P_Flags value)
