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

#include "enums.h"

#include <stdint.h>

typedef struct {
    ElfParser_EI_Class      ei_class;
    ElfParser_EI_Data       ei_data;
    ElfParser_EI_Version    ei_version;
    ElfParser_EI_OSABI      ei_osabi;
    uint8_t                 ei_abiversion;
    ElfParser_E_Type        e_type;
    ElfParser_E_Machine     e_machine;
    ElfParser_E_Version     e_version;
    uint64_t                e_entry;
    uint64_t                e_phoff;
    uint64_t                e_shoff;
    uint32_t                e_flags;
    uint16_t                e_ehsize;
    uint16_t                e_phentsize;
    uint16_t                e_phnum;
    uint16_t                e_shentsize;
    uint16_t                e_shnum;
    uint16_t                e_shstrndx;
    
    // Members below this aren't part of the elf header, but they're useful to have here
    
    // These 5 members will 0 if their associated section is not present (or if there are 0 sections)
    uint64_t                string_table_offset;        // byte offset of .shstrtab data referenced by e_shstrndx
    uint64_t                symbol_table_offset;        // byte offset of .symtab data
    uint64_t                symbol_string_table_offset; // byte offset of .strtab data referenced by .symtab
    uint64_t                symbol_entry_size;          // Size in bytes of each symbol entry
    uint64_t                symbol_num;                 // Number of symbols
    
    // If the number of sections >= SHN_LORESERVE, then e_shnum will be 0 and the true number of sections is stored elsewhere
    // This value accounts for that case, and should be used when determining how many sections are actually present
    uint64_t                true_shnum;
    
    // Same as previous member - accounts for edge case if string table section index >= SHN_LORESERVE
    // Set to 0 if there are no sections
    uint64_t                true_shstrndx;
    
    uint64_t                elf_size;                   // Total size in bytes of elf data
} ElfParser_Header;

typedef struct {
    uint64_t            sh_name;
    ElfParser_SH_Type   sh_type;
    ElfParser_SH_Flags  sh_flags;
    uint64_t            sh_addr;
    uint64_t            sh_offset;
    uint64_t            sh_size;
    uint32_t            sh_link;
    uint32_t            sh_info;
    uint64_t            sh_addralign;
    uint64_t            sh_entsize;
    
    const char*         name;   // Calculated from sh_name and .shstrtab offset. Will always point to valid string
    uint64_t            index;    // Section index, useful if section was obtained by name
} ElfParser_SectionHeader;

typedef struct {
    uint32_t                st_name;
    uint64_t                st_value;
    uint64_t                st_size;
    uint8_t                 st_info;
    uint8_t                 st_other;
    uint16_t                st_shndx;
    
    ElfParser_ST_Bind       st_bind;        // Upper nibble of st_info
    ElfParser_ST_Type       st_type;        // Lower nibble of st_info
    ElfParser_ST_Visibility st_visibility;  // Lower 2 bits of st_other
    
    const char*             name;   // Calculated from st_name and .strtab offset. Will always point to valid string
    uint64_t                index;    // Symbol index, useful if symbol was obtained by name
} ElfParser_Symbol;

typedef struct {
    ElfParser_P_Type    p_type;
    uint64_t            p_offset;
    uint64_t            p_vaddr;
    uint64_t            p_paddr;
    uint64_t            p_filesz;
    uint64_t            p_memsz;
    ElfParser_P_Flags   p_flags;
    uint64_t            p_align;
    
    uint64_t            index;
} ElfParser_ProgramHeader;
