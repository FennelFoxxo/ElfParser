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

#include "../include/elfparser.h"

#include "elfstructs.h"
#include "endian.h"

#include <string.h>

// This file should *not* be included! It is used as an interface for the implementation
// Any functions here should be considered private

ElfParser_Error elfparser_get_header_ident(const Elf_Ident* ident, ElfParser_Header* header_out);

// Get true # of sections and string table section index
uint64_t elfparser_get_true_shnum(const void* elf_start, ElfParser_Header* header_in_out);
uint64_t elfparser_get_true_shstrndx(const void* elf_start, const ElfParser_Header* header_in_out);

const char* elfparser_get_section_header_name(const void* elf_start, const ElfParser_Header* header,
                                              const ElfParser_SectionHeader* section);

const char* elfparser_get_symbol_name(const void* elf_start, const ElfParser_Header* header,
                                      const ElfParser_Symbol* symbol);


// 32-bit specific functions
ElfParser_Error elfparser_get_header32(const Elf32_Ehdr* header, ElfParser_Header* header_in_out);

void elfparser_get_section_header32(const ElfParser_Header* header, const Elf32_Shdr* sh_start,
                                    ElfParser_SectionHeader* section_header_out);

void elfparser_get_symbol32(const ElfParser_Header* header, const Elf32_Sym* symbol,
                            ElfParser_Symbol* symbol_out);

void elfparser_get_program_header32(const ElfParser_Header* header, const Elf32_Phdr* ph_start,
                                    ElfParser_ProgramHeader* program_header_out);

// 64-bit specific functions
ElfParser_Error elfparser_get_header64(const Elf64_Ehdr* header, ElfParser_Header* header_in_out);

void elfparser_get_section_header64(const ElfParser_Header* header, const Elf64_Shdr* sh_start, 
                                    ElfParser_SectionHeader* section_header_out);

void elfparser_get_symbol64(const ElfParser_Header* header, const Elf64_Sym* symbol,
                            ElfParser_Symbol* symbol_out);

void elfparser_get_program_header64(const ElfParser_Header* header, const Elf64_Phdr* ph_start,
                                    ElfParser_ProgramHeader* program_header_out);


// Inline functions below here

// Check if null/magic section @ index 0 is valid
static inline bool elfparser_is_null_section(const ElfParser_SectionHeader* section) {
    return  section->sh_name        == 0 &&
            section->sh_type        == ELFPARSER_SHT_NULL &&
            section->sh_flags       == 0 &&
            section->sh_addr        == 0 &&
            section->sh_offset      == 0 &&
            section->sh_info        == 0 &&
            section->sh_addralign   == 0 &&
            section->sh_entsize     == 0;
}

// Check that string is null-terminated and does not go past end of file
static inline bool elfparser_is_string_in_bounds(const void* elf_start, const ElfParser_Header* header,
                                                 uint64_t string_off) {
    if (string_off >= header->elf_size) {
        // Name out of bounds
        return false;
    }
    
    // Check that string is null-terminated
    const char* str_ptr = elf_start + string_off;
    return memchr(str_ptr, '\0', header->elf_size - string_off) != NULL;
}