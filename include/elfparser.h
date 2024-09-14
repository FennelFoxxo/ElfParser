/*
 * MIT License
 * 
 * Copyright (c) 2024 FennelFoxxo
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#pragma once

#include "elfparser/enums.h"
#include "elfparser/objects.h"
#include "elfparser/validation.h"

#include <stdbool.h>
#include <stdint.h>

/* Reads elf header from buffer and returns it in header_out.
 * Returns ELFLOADER_NOERROR on success - contents of header_out undefined on failure
 * This should be the first function called to parse the elf data */
ElfParser_Error elfparser_get_header(const void* elf_start, uint64_t elf_size, ElfParser_Header* header_out);

/* Reads the section header at index and returns it in section_header_out
 * Returns ELFPARSER_NOERROR on success - contents of section_header_out undefined on failure */
ElfParser_Error elfparser_get_section_header(const void* elf_start, const ElfParser_Header* header,
                                             uint64_t index, ElfParser_SectionHeader* section_header_out);

ElfParser_Error elfparser_get_section_header_by_name(const void* elf_start, const ElfParser_Header* header,
                                                     const char* name, ElfParser_SectionHeader* section_header_out);

/* Reads the symbol at index and returns it in symbol_out
 * Returns ELFPARSER_NOERROR on success - contents of symbol_out undefined on failure */
ElfParser_Error elfparser_get_symbol(const void* elf_start, const ElfParser_Header* header,
                                     uint64_t index, ElfParser_Symbol* symbol_out);

ElfParser_Error elfparser_get_symbol_by_name(const void* elf_start, const ElfParser_Header* header,
                                             const char* name, ElfParser_Symbol* symbol_out);

/* Reads the program header at index and returns it in program_header_out
 * Returns ELFPARSER_NOERROR on success - contents of program_header_out undefined on failure */
ElfParser_Error elfparser_get_program_header(const void* elf_start, const ElfParser_Header* header,
                                             uint64_t index, ElfParser_ProgramHeader* program_header_out);


/* Reads up to `num_bytes` of data from the segment (program header) referenced by `segment_index` and writes it to `dest`
 * Reading starts at an offset of `skip` bytes allowing for incremental reads
 * Returns how many bytes are left to read, or ELFPARSER_WRITE_SEGMENT_ERROR (equivalent to UINTMAX_MAX) on failure
 * if `dest` is NULL, returns the total memory size (not file size) of the segment*/
#define ELFPARSER_WRITE_SEGMENT_ERROR UINTMAX_MAX
uint64_t elfparser_copy_segment(const void* elf_start, const ElfParser_Header* header, uint64_t segment_index,
                                void* dest, uint64_t skip, uint64_t num_bytes);

// Header ident fields checks
static inline bool elfparser_is_valid_ei_class(ElfParser_EI_Class value);
static inline bool elfparser_is_valid_ei_data(ElfParser_EI_Data value);
static inline bool elfparser_is_valid_ei_version(ElfParser_EI_Version value);
static inline bool elfparser_is_valid_ei_osabi(ElfParser_EI_OSABI value);

// Header fields checks
static inline bool elfparser_is_valid_e_type(ElfParser_E_Type value);
static inline bool elfparser_is_valid_e_machine(ElfParser_E_Machine value);
static inline bool elfparser_is_valid_e_version(ElfParser_E_Version value);

// Section header fields checks
static inline bool elfparser_is_valid_sh_type(ElfParser_SH_Type value);
static inline bool elfparser_is_valid_sh_flags(ElfParser_SH_Flags value);

// Symbol fields checks
static inline bool elfparser_is_valid_st_bind(ElfParser_ST_Bind value);
static inline bool elfparser_is_valid_st_type(ElfParser_ST_Type value);
static inline bool elfparser_is_valid_st_visibility(ElfParser_ST_Visibility value);

// Program header checks
static inline bool elfparser_is_valid_p_type(ElfParser_P_Type value);
static inline bool elfparser_is_valid_p_flags(ElfParser_P_Flags value);