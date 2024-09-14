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

#include "parse.h"

ElfParser_Error elfparser_get_header64(const Elf64_Ehdr* header, ElfParser_Header* header_in_out) {
    // Validate header length
    if (header_in_out->elf_size < sizeof(header)) return ELFPARSER_INVALID;
    
    // Get ident and validate read ok
    if (elfparser_get_header_ident((Elf_Ident*)&header->e_ident, header_in_out) != ELFPARSER_NOERROR) return ELFPARSER_INVALID;
    
    bool is_lsb = (header_in_out->ei_data == ELFPARSER_ELFDATA2LSB);
    
    // Get version and validate
    header_in_out->e_version   = convert_endian_32(header->e_version,      is_lsb);
    if (!elfparser_is_valid_e_version(header_in_out->e_version)) return ELFPARSER_INVALID;
    
    header_in_out->e_type       = convert_endian_16(header->e_type,         is_lsb);
    header_in_out->e_machine    = convert_endian_16(header->e_machine,      is_lsb);
    header_in_out->e_entry      = convert_endian_64(header->e_entry,        is_lsb);
    header_in_out->e_phoff      = convert_endian_64(header->e_phoff,        is_lsb);
    header_in_out->e_shoff      = convert_endian_64(header->e_shoff,        is_lsb);
    header_in_out->e_flags      = convert_endian_32(header->e_flags,        is_lsb);
    header_in_out->e_ehsize     = convert_endian_16(header->e_ehsize,       is_lsb);
    header_in_out->e_phentsize  = convert_endian_16(header->e_phentsize,    is_lsb);
    header_in_out->e_phnum      = convert_endian_16(header->e_phnum,        is_lsb);
    header_in_out->e_shentsize  = convert_endian_16(header->e_shentsize,    is_lsb);
    header_in_out->e_shnum      = convert_endian_16(header->e_shnum,        is_lsb);
    header_in_out->e_shstrndx   = convert_endian_16(header->e_shstrndx,     is_lsb);

    return ELFPARSER_NOERROR;
}

void elfparser_get_section_header64(const ElfParser_Header* header, const Elf64_Shdr* sh_start,
                                    ElfParser_SectionHeader* section_header_out) {
    bool is_lsb = (header->ei_data == ELFPARSER_ELFDATA2LSB);

    section_header_out->sh_name         = convert_endian_32(sh_start->sh_name,      is_lsb);
    section_header_out->sh_type         = convert_endian_32(sh_start->sh_type,      is_lsb);
    section_header_out->sh_flags        = convert_endian_64(sh_start->sh_flags,     is_lsb);
    section_header_out->sh_addr         = convert_endian_64(sh_start->sh_addr,      is_lsb);
    section_header_out->sh_offset       = convert_endian_64(sh_start->sh_offset,    is_lsb);
    section_header_out->sh_size         = convert_endian_64(sh_start->sh_size,      is_lsb);
    section_header_out->sh_link         = convert_endian_32(sh_start->sh_link,      is_lsb);
    section_header_out->sh_info         = convert_endian_32(sh_start->sh_info,      is_lsb);
    section_header_out->sh_addralign    = convert_endian_64(sh_start->sh_addralign, is_lsb);
    section_header_out->sh_entsize      = convert_endian_64(sh_start->sh_entsize,   is_lsb);
}

void elfparser_get_symbol64(const ElfParser_Header* header, const Elf64_Sym* symbol,
                            ElfParser_Symbol* symbol_out) {
    bool is_lsb = (header->ei_data == ELFPARSER_ELFDATA2LSB);

    symbol_out->st_name     = convert_endian_32(symbol->st_name,    is_lsb);
    symbol_out->st_value    = convert_endian_64(symbol->st_value,   is_lsb);
    symbol_out->st_size     = convert_endian_64(symbol->st_size,    is_lsb);
    symbol_out->st_info     = symbol->st_info;
    symbol_out->st_other    = symbol->st_other;
    symbol_out->st_shndx    = convert_endian_16(symbol->st_shndx,   is_lsb);
}

void elfparser_get_program_header64(const ElfParser_Header* header, const Elf64_Phdr* ph_start,
                                    ElfParser_ProgramHeader* program_header_out) {
    bool is_lsb = (header->ei_data == ELFPARSER_ELFDATA2LSB);
    
    program_header_out->p_type      = convert_endian_32(ph_start->p_type,   is_lsb);
    program_header_out->p_flags     = convert_endian_32(ph_start->p_flags,  is_lsb);
    program_header_out->p_offset    = convert_endian_64(ph_start->p_offset, is_lsb);
    program_header_out->p_vaddr     = convert_endian_64(ph_start->p_vaddr,  is_lsb);
    program_header_out->p_paddr     = convert_endian_64(ph_start->p_paddr,  is_lsb);
    program_header_out->p_filesz    = convert_endian_64(ph_start->p_filesz, is_lsb);
    program_header_out->p_memsz     = convert_endian_64(ph_start->p_memsz,  is_lsb);
    program_header_out->p_align     = convert_endian_64(ph_start->p_align,  is_lsb);
}