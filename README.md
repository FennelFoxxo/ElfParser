# ElfParser
Minimal C library for parsing ELF executable files.

In particular, this library:
- Only requires <stdint.h>, <stdbool.h>, and <string.h>
- Performs no dynamic memory allocation
- Performs as few validation checks as necessary (e.g. checking elf magic number) and returns header fields as is

It supports reading the ELF header, section headers, program headers, and symbols from the .symtab section. It supports reading from 32- or 64- bit, little- or big- endian ELF files

[Much of the information about the ELF format was obtained from here](https://refspecs.linuxfoundation.org/elf/gabi4+/contents.html)

# How to run
Simply run `make` in the root project directory, and then run the `example` program, which will print information about the `testelf` ELF file

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



## Structs

### ElfParser_Header
- `ei_class`: `ElfParser_EI_Class`
- `ei_data`: `ElfParser_EI_Data`
- `ei_version`: `ElfParser_EI_Version`
- `ei_osabi`: `ElfParser_EI_OSABI`
- `ei_abiversion`: `uint8_t`
- `e_type`: `ElfParser_E_Type`
- `e_machine`: `ElfParser_E_Machine`
- `e_version`: `ElfParser_E_Version`
- `e_entry`: `uint64_t`
- `e_phoff`: `uint64_t`
- `e_shoff`: `uint64_t`
- `e_flags`: `uint32_t`
- `e_ehsize`: `uint16_t`
- `e_phentsize`: `uint16_t`
- `e_phnum`: `uint16_t`
- `e_shentsize`: `uint16_t`
- `e_shnum`: `uint16_t`
- `e_shstrndx`: `uint16_t`
- `string_table_offset`: `uint64_t` (byte offset of .shstrtab data referenced by `e_shstrndx`)
- `symbol_table_offset`: `uint64_t` (byte offset of .symtab data)
- `symbol_string_table_offset`: `uint64_t` (byte offset of .strtab data referenced by .symtab)
- `symbol_entry_size`: `uint64_t` (size in bytes of each symbol entry)
- `symbol_num`: `uint64_t` (total number of symbols)
- `true_shnum`: `uint64_t` (if there are too many sections to store in `e_shnum`, the true number of sections is stored elsewhere. This value accounts for that case, and should be used when determining how many sections are actually present)
- `true_shstrndx`: `uint64_t` (same as previous member, accounts for the case if `e_shstrndx` is not big enough to store string table section index)
- `elf_size`: `uint64_t` (total size of the elf file, this is not read from the file but is copied from `elf_size` parameter in `elfparser_get_header`)

### ElfParser_SectionHeader
- `sh_name`: `uint64_t`
- `sh_type`: `ElfParser_SH_Type`
- `sh_flags`: `ElfParser_SH_Flags`
- `sh_addr`: `uint64_t`
- `sh_offset`: `uint64_t`
- `sh_size`: `uint64_t`
- `sh_link`: `uint32_t`
- `sh_info`: `uint32_t`
- `sh_addralign`: `uint64_t`
- `sh_entsize`: `uint64_t`
- `name`: `const char*` (calculated from `sh_name` and .shstrtab offset. Will always point to null-terminated string)
- `index`: `uint64_t` (index of this section, useful if section was obtained by name)

### ElfParser_Symbol
- `st_name`: `uint32_t`
- `st_value`: `uint64_t`
- `st_size`: `uint64_t`
- `st_info`: `uint8_t`
- `st_other`: `uint8_t`
- `st_shndx`: `st_shndx`
- `st_bind`: `ElfParser_ST_Bind` (upper nibble of `st_info`)
- `st_type`: `ElfParser_ST_Type` (lower nibble of `st_info`)
- `st_visibility`: `ElfParser_ST_Visibility` (lower 2 bites of `st_other`)
- `name`: `const char*` (calculated from `st_name` and .symtab offset. Will always point to null-terminated string)
- `index`: `uint64_t` (index of this symbol, useful if symbol was obtained by name)

### ElfParser_ProgramHeader
- `p_type`: `ElfParser_P_Type`
- `p_offset`: `uint64_t`
- `p_vaddr`: `uint64_t`
- `p_paddr`: `uint64_t`
- `p_filesz`: `uint64_t`
- `p_memsz`: `uint64_t`
- `p_flags`: `ElfParser_P_Flags`
- `p_align`: `uint64_t`
- `index`: `uint64_t` (index of this program header)



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



## Enums

### ElfParser_Error
- `ELFPARSER_NOERROR` (success)
- `ELFPARSER_NOT_FOUND` (requested symbol or string not found)
- `ELFPARSER_INVALID` (failed to parse ELF format)

### ElfParser_EI_Class
- `ELFPARSER_ELFCLASS32`
- `ELFPARSER_ELFCLASS64`

### ElfParser_EI_Data
- `ELFPARSER_ELFDATA2LSB`
- `ELFPARSER_ELFDATA2MSB`

### ElfParser_EI_Version / ElfParser_E_Version
- `ELFPARSER_EV_CURRENT`

### ElfParser_EI_OSABI
- `ELFPARSER_ELFOSABI_NONE`
- `ELFPARSER_ELFOSABI_HPUX`
- `ELFPARSER_ELFOSABI_NETBSD`
- `ELFPARSER_ELFOSABI_LINUX`
- `ELFPARSER_ELFOSABI_SOLARIS`
- `ELFPARSER_ELFOSABI_AIX`
- `ELFPARSER_ELFOSABI_IRIX`
- `ELFPARSER_ELFOSABI_FREEBSD`
- `ELFPARSER_ELFOSABI_TRU64`
- `ELFPARSER_ELFOSABI_MODESTO`
- `ELFPARSER_ELFOSABI_OPENBSD`
- `ELFPARSER_ELFOSABI_OPENVMS`
- `ELFPARSER_ELFOSABI_NSK`
- `ELFPARSER_ELFOSABI_LOARCH`

### ElfParser_E_Type
- `ELFPARSER_ET_NONE`
- `ELFPARSER_ET_REL`
- `ELFPARSER_ET_EXEC`
- `ELFPARSER_ET_DYN`
- `ELFPARSER_ET_CORE`
- `ELFPARSER_ET_LOOS`
- `ELFPARSER_ET_HIOS`
- `ELFPARSER_ET_LOPROC`
- `ELFPARSER_ET_HIPROC`

### ElfParser_E_Machine
- `ELFPARSER_EM_NONE`
- `ELFPARSER_EM_M32`
- `ELFPARSER_EM_SPARC`
- `ELFPARSER_EM_386`
- `ELFPARSER_EM_68K`
- `ELFPARSER_EM_88K`
- `ELFPARSER_EM_860`
- `ELFPARSER_EM_MIPS`
- `ELFPARSER_EM_S370`
- `ELFPARSER_EM_MIPS_RS3_LE`
- `ELFPARSER_EM_PARISC`
- `ELFPARSER_EM_VPP500`
- `ELFPARSER_EM_SPARC32PLUS`
- `ELFPARSER_EM_960`
- `ELFPARSER_EM_PPC`
- `ELFPARSER_EM_PPC64`
- `ELFPARSER_EM_S390`
- `ELFPARSER_EM_V800`
- `ELFPARSER_EM_FR20`
- `ELFPARSER_EM_RH32`
- `ELFPARSER_EM_RCE`
- `ELFPARSER_EM_ARM`
- `ELFPARSER_EM_ALPHA`
- `ELFPARSER_EM_SH`
- `ELFPARSER_EM_SPARCV9`
- `ELFPARSER_EM_TRICORE`
- `ELFPARSER_EM_ARC`
- `ELFPARSER_EM_H8_300`
- `ELFPARSER_EM_H8_300H`
- `ELFPARSER_EM_H8S`
- `ELFPARSER_EM_H8_500`
- `ELFPARSER_EM_IA_64`
- `ELFPARSER_EM_MIPS_X`
- `ELFPARSER_EM_COLDFIRE`
- `ELFPARSER_EM_68HC12`
- `ELFPARSER_EM_MMA`
- `ELFPARSER_EM_PCP`
- `ELFPARSER_EM_NCPU`
- `ELFPARSER_EM_NDR1`
- `ELFPARSER_EM_STARCORE`
- `ELFPARSER_EM_ME16`
- `ELFPARSER_EM_ST100`
- `ELFPARSER_EM_TINYJ`
- `ELFPARSER_EM_X86_64`
- `ELFPARSER_EM_PDSP`
- `ELFPARSER_EM_PDP10`
- `ELFPARSER_EM_PDP11`
- `ELFPARSER_EM_FX66`
- `ELFPARSER_EM_ST9PLUS`
- `ELFPARSER_EM_ST7`
- `ELFPARSER_EM_68HC16`
- `ELFPARSER_EM_68HC11`
- `ELFPARSER_EM_68HC08`
- `ELFPARSER_EM_68HC05`
- `ELFPARSER_EM_SVX`
- `ELFPARSER_EM_ST19`
- `ELFPARSER_EM_VAX`
- `ELFPARSER_EM_CRIS`
- `ELFPARSER_EM_JAVELIN`
- `ELFPARSER_EM_FIREPATH`
- `ELFPARSER_EM_ZSP`
- `ELFPARSER_EM_MMIX`
- `ELFPARSER_EM_HUANY`
- `ELFPARSER_EM_PRISM`
- `ELFPARSER_EM_AVR`
- `ELFPARSER_EM_FR30`
- `ELFPARSER_EM_D10V`
- `ELFPARSER_EM_D30V`
- `ELFPARSER_EM_V850`
- `ELFPARSER_EM_M32R`
- `ELFPARSER_EM_MN10300`
- `ELFPARSER_EM_MN10200`
- `ELFPARSER_EM_PJ`
- `ELFPARSER_EM_OPENRISC`
- `ELFPARSER_EM_ARC_A5`
- `ELFPARSER_EM_XTENSA`
- `ELFPARSER_EM_VIDEOCORE`
- `ELFPARSER_EM_TMM_GPP`
- `ELFPARSER_EM_NS32K`
- `ELFPARSER_EM_TPC`
- `ELFPARSER_EM_SNP1K`
- `ELFPARSER_EM_ST200`

### ElfParser_SH_Type
- `ELFPARSER_SHT_NULL`
- `ELFPARSER_SHT_PROGBITS`
- `ELFPARSER_SHT_SYMTAB`
- `ELFPARSER_SHT_STRTAB`
- `ELFPARSER_SHT_RELA`
- `ELFPARSER_SHT_HASH`
- `ELFPARSER_SHT_DYNAMIC`
- `ELFPARSER_SHT_NOTE`
- `ELFPARSER_SHT_NOBITS`
- `ELFPARSER_SHT_REL`
- `ELFPARSER_SHT_SHLIB`
- `ELFPARSER_SHT_DYNSYM`
- `ELFPARSER_SHT_INIT_ARRAY`
- `ELFPARSER_SHT_FINI_ARRAY`
- `ELFPARSER_SHT_PREINIT_ARRAY`
- `ELFPARSER_SHT_GROUP`
- `ELFPARSER_SHT_SYMTAB_SHNDX`
- `ELFPARSER_SHT_LOOS`
- `ELFPARSER_SHT_HIOS`
- `ELFPARSER_SHT_LOPROC`
- `ELFPARSER_SHT_HIPROC`
- `ELFPARSER_SHT_LOUSER`
- `ELFPARSER_SHT_HIUSER`

### ElfParser_SH_Flags
- `ELFPARSER_SHF_WRITE`
- `ELFPARSER_SHF_ALLOC`
- `ELFPARSER_SHF_EXECINSTR`
- `ELFPARSER_SHF_MERGE`
- `ELFPARSER_SHF_STRINGS`
- `ELFPARSER_SHF_INFO_LINK`
- `ELFPARSER_SHF_LINK_ORDER`
- `ELFPARSER_SHF_OS_NONCONFORMING`
- `ELFPARSER_SHF_GROUP`
- `ELFPARSER_SHF_TLS`
- `ELFPARSER_SHF_MASKOS`
- `ELFPARSER_SHF_MASKPROC`

### ElfParser_SpecialSectionIndexes
- `ELFPARSER_SHN_UNDEF`
- `ELFPARSER_SHN_LORESERVE`
- `ELFPARSER_SHN_LOPROC`
- `ELFPARSER_SHN_HIPROC`
- `ELFPARSER_SHN_LOOS`
- `ELFPARSER_SHN_HIOS`
- `ELFPARSER_SHN_ABS`
- `ELFPARSER_SHN_COMMON`
- `ELFPARSER_SHN_XINDEX`
- `ELFPARSER_SHN_HIRESERVE`

### ElfParser_ST_Bind
- `ELFPARSER_STB_LOCAL`
- `ELFPARSER_STB_GLOBAL`
- `ELFPARSER_STB_WEAK`
- `ELFPARSER_STB_LOOS`
- `ELFPARSER_STB_HIOS`
- `ELFPARSER_STB_LOPROC`
- `ELFPARSER_STB_HIPROC`

### ElfParser_ST_Type
- `ELFPARSER_STT_NOTYPE`
- `ELFPARSER_STT_OBJECT`
- `ELFPARSER_STT_FUNC`
- `ELFPARSER_STT_SECTION`
- `ELFPARSER_STT_FILE`
- `ELFPARSER_STT_COMMON`
- `ELFPARSER_STT_TLS`
- `ELFPARSER_STT_LOOS`
- `ELFPARSER_STT_HIOS`
- `ELFPARSER_STT_LOPROC`
- `ELFPARSER_STT_HIPROC`

### ElfParser_ST_Visibility
- `ELFPARSER_STV_DEFAULT`
- `ELFPARSER_STV_INTERNAL`
- `ELFPARSER_STV_HIDDEN`
- `ELFPARSER_STV_PROTECTED`

### ElfParser_P_Type
- `ELFPARSER_PT_NULL`
- `ELFPARSER_PT_LOAD`
- `ELFPARSER_PT_DYNAMIC`
- `ELFPARSER_PT_INTERP`
- `ELFPARSER_PT_NOTE`
- `ELFPARSER_PT_SHLIB`
- `ELFPARSER_PT_PHDR`
- `ELFPARSER_PT_TLS`
- `ELFPARSER_PT_LOOS`
- `ELFPARSER_PT_HIOS`
- `ELFPARSER_PT_LOPROC`
- `ELFPARSER_PT_HIPROC`

### ElfParser_P_Flags
- `ELFPARSER_PF_X`
- `ELFPARSER_PF_W`
- `ELFPARSER_PF_R`
- `ELFPARSER_PF_MASKOS`
- `ELFPARSER_PF_MASKPROC`
