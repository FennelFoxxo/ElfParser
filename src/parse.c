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


ElfParser_Error elfparser_get_header(const void* elf_start, uint64_t elf_size, ElfParser_Header* header_out) {
    // Start with basic checks to make sure file is long enough
    if (elf_size < sizeof(Elf32_Ehdr)) return ELFPARSER_INVALID;
    
    header_out->elf_size = elf_size;
    
    const Elf32_Ehdr* header = elf_start;
    Elf_Ident* ident = (Elf_Ident*)(&header->e_ident);

    // Parse header
    if (ident->ei_class == ELFPARSER_ELFCLASS64) {
        if (elfparser_get_header64(elf_start, header_out) != ELFPARSER_NOERROR) return ELFPARSER_INVALID;
    }
    else {
        if (elfparser_get_header32(elf_start, header_out) != ELFPARSER_NOERROR) return ELFPARSER_INVALID;
    }
    
    // Get true # of sections and string table index
    header_out->true_shnum = elfparser_get_true_shnum(elf_start, header_out);
    header_out->true_shstrndx = elfparser_get_true_shstrndx(elf_start, header_out);
    
    ElfParser_SectionHeader section;
    bool no_err;
    
    // Get file offset of string table section data
    if (header_out->true_shstrndx == 0) {
        header_out->string_table_offset = 0;
    } else {
        no_err = elfparser_get_section_header(elf_start, header_out, header_out->true_shstrndx, &section) == ELFPARSER_NOERROR;
        if (!no_err) return ELFPARSER_INVALID;  // We should have been able to read string table section - it's an error if we couldn't
    
        header_out->string_table_offset = section.sh_offset;
    }
    
    // Get symbol table data (.symtab)
    no_err = elfparser_get_section_header_by_name(elf_start, header_out, ".symtab", &section) == ELFPARSER_NOERROR;
    
    header_out->symbol_table_offset         = no_err ? section.sh_offset                        : 0;
    header_out->symbol_entry_size           = no_err ? section.sh_entsize                       : 0;
    header_out->symbol_num                  = no_err ? section.sh_size / section.sh_entsize     : 0;
    header_out->symbol_string_table_offset  = 0; // Default
    
    if (!no_err) return ELFPARSER_NOERROR; // Return early if no .symtab (it's not an error for there to be no .symtab)
    
    // Get section of symbol string table (.strtab)
    no_err = elfparser_get_section_header(elf_start, header_out, section.sh_link, &section) == ELFPARSER_NOERROR;
    if (!no_err) return ELFPARSER_NOERROR;

    header_out->symbol_string_table_offset = section.sh_offset;

    return ELFPARSER_NOERROR;
}


ElfParser_Error elfparser_get_section_header(const void* elf_start, const ElfParser_Header* header,
                                             uint64_t index, ElfParser_SectionHeader* section_header_out) {
    // Check that index is reasonable
    if (index >= header->true_shnum) return ELFPARSER_INVALID;
    
    uint64_t header_off = header->e_shoff + (uint64_t)header->e_shentsize * index;
    
    // Get section header
    if (header->ei_class == ELFPARSER_ELFCLASS64) {
        if (header_off + sizeof(Elf64_Shdr) > header->elf_size) return ELFPARSER_INVALID; // Check that offset is within bounds
        elfparser_get_section_header64(header, elf_start + header_off, section_header_out);
    }
    else {
        if (header_off + sizeof(Elf32_Shdr) > header->elf_size) return ELFPARSER_INVALID; // Check that offset is within bounds
        elfparser_get_section_header32(header, elf_start + header_off, section_header_out);
    }
    
    section_header_out->index = index;
    section_header_out->name = elfparser_get_section_header_name(elf_start, header, section_header_out);
    
    return ELFPARSER_NOERROR;
}


ElfParser_Error elfparser_get_section_header_by_name(const void* elf_start, const ElfParser_Header* header,
                                                     const char* name, ElfParser_SectionHeader* section_header_out) {
    if (name == NULL) {
        // Null string not allowed
        return ELFPARSER_INVALID;
    }
    
    for (uint64_t i = 0; i < header->true_shnum; i++) {
        ElfParser_Error err = elfparser_get_section_header(elf_start, header, i, section_header_out);
        
        if (err != ELFPARSER_NOERROR) continue;
        
        if (strcmp(name, section_header_out->name) == 0) {
            return ELFPARSER_NOERROR;
        }
    }
    return ELFPARSER_NOT_FOUND;
}


ElfParser_Error elfparser_get_symbol(const void* elf_start, const ElfParser_Header* header,
                                     uint64_t index, ElfParser_Symbol* symbol_out) {
    // Check that index is reasonable
    if (index >= header->symbol_num) return ELFPARSER_INVALID;
    
    uint64_t symbol_off = header->symbol_table_offset + (uint64_t)header->symbol_entry_size * index;

    // Get symbol
    if (header->ei_class == ELFPARSER_ELFCLASS64) {
        if (symbol_off + sizeof(Elf64_Sym) > header->elf_size) return ELFPARSER_INVALID; // Check that offset is within bounds
        elfparser_get_symbol64(header, elf_start + symbol_off, symbol_out);
        
    } else {
        if (symbol_off + sizeof(Elf32_Sym) > header->elf_size) return ELFPARSER_INVALID; // Check that offset is within bounds
        elfparser_get_symbol32(header, elf_start + symbol_off, symbol_out);
    }
    
    symbol_out->st_bind         = symbol_out->st_info  >> 4;
    symbol_out->st_type         = symbol_out->st_info  & 0xf;
    symbol_out->st_visibility   = symbol_out->st_other & 0x3;
    symbol_out->index           = index;
    symbol_out->name            = elfparser_get_symbol_name(elf_start, header, symbol_out);
    
    return ELFPARSER_NOERROR;
}


ElfParser_Error elfparser_get_symbol_by_name(const void* elf_start, const ElfParser_Header* header,
                                             const char* name, ElfParser_Symbol* symbol_out) {
    if (name == NULL) {
        // Null string not allowed
        return ELFPARSER_INVALID;
    }
    
    for (uint64_t i = 0; i < header->symbol_num; i++) {
        ElfParser_Error err = elfparser_get_symbol(elf_start, header, i, symbol_out);
        
        if (err != ELFPARSER_NOERROR) continue;
        
        if (strcmp(name, symbol_out->name) == 0) {
            return ELFPARSER_NOERROR;
        }
    }
    return ELFPARSER_NOT_FOUND;
}


ElfParser_Error elfparser_get_program_header(const void* elf_start, const ElfParser_Header* header,
                                             uint64_t index, ElfParser_ProgramHeader* program_header_out) {
    // Check that index is reasonable
    if (index >= header->e_phnum) return ELFPARSER_INVALID;
    
    uint64_t header_off = header->e_phoff + (uint64_t)header->e_phentsize * index;
    
    // Get program header
    if (header->ei_class == ELFPARSER_ELFCLASS64) {
        if (header_off + sizeof(Elf64_Phdr) > header->elf_size) return ELFPARSER_INVALID; // Check that offset is within bounds
        elfparser_get_program_header64(header, elf_start + header_off, program_header_out);
    }
    else {
        if (header_off + sizeof(Elf32_Phdr) > header->elf_size) return ELFPARSER_INVALID; // Check that offset is within bounds
        elfparser_get_program_header32(header, elf_start + header_off, program_header_out);
    }
    
    program_header_out->index = index;
    
    return ELFPARSER_NOERROR;
}

#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a < _b ? _a : _b; })

uint64_t elfparser_copy_segment(const void* elf_start, const ElfParser_Header* header, uint64_t segment_index,
                                void* dest, uint64_t skip, uint64_t num_bytes) {

    ElfParser_ProgramHeader program_header;
    ElfParser_Error err = elfparser_get_program_header(elf_start, header, segment_index, &program_header);
    if (err != ELFPARSER_NOERROR) return ELFPARSER_WRITE_SEGMENT_ERROR; // Can't proceed if program header is unable to be read

    if (dest == NULL) return program_header.p_memsz; // No valid destination
    
    if (program_header.p_offset + program_header.p_filesz > header->elf_size) return ELFPARSER_WRITE_SEGMENT_ERROR; // Program header has bad data
    
    // Limit number of bytes that will be read
    num_bytes = min(num_bytes, program_header.p_memsz - skip);
    
    uint64_t total_bytes_copied = 0;
    
    // It's possible that memory size is greater than file size, in which case the rest of the memory should be set to 0
    // Start with copying data from file first
    if (skip < program_header.p_filesz) {
        uint64_t num_file_bytes = min(num_bytes, program_header.p_filesz - skip);
        const void* src_ptr = elf_start + program_header.p_offset + skip;
        
        memcpy(dest, src_ptr, num_file_bytes);
        dest        += num_file_bytes;
        num_bytes   -= num_file_bytes;
        total_bytes_copied += num_file_bytes;
    }
    
    // Now fill the rest of dest with 0s
    memset(dest, 0, num_bytes);
    total_bytes_copied += num_bytes;
    
    // Return # of bytes left to copy
    return program_header.p_memsz - skip - total_bytes_copied;
}


// Private functions for implementation below here

ElfParser_Error elfparser_get_header_ident(const Elf_Ident* ident, ElfParser_Header* header_out) {
    // In general we want to be as liberal as possible in what we accept,
    // but these fields absolutely need to be valid
    if (ident->ei_mag0 != IDENT_MAG0) return ELFPARSER_INVALID;
    if (ident->ei_mag1 != IDENT_MAG1) return ELFPARSER_INVALID;
    if (ident->ei_mag2 != IDENT_MAG2) return ELFPARSER_INVALID;
    if (ident->ei_mag3 != IDENT_MAG3) return ELFPARSER_INVALID;
    
    if (!elfparser_is_valid_ei_class(ident->ei_class))      return ELFPARSER_INVALID;
    if (!elfparser_is_valid_ei_data(ident->ei_data))        return ELFPARSER_INVALID;
    if (!elfparser_is_valid_ei_version(ident->ei_version))  return ELFPARSER_INVALID;
    
    header_out->ei_class        = ident->ei_class;
    header_out->ei_data         = ident->ei_data;
    header_out->ei_version      = ident->ei_version;
    header_out->ei_osabi        = ident->ei_osabi;
    header_out->ei_abiversion   = ident->ei_abiversion;
    
    return ELFPARSER_NOERROR;
}


uint64_t elfparser_get_true_shnum(const void* elf_start, ElfParser_Header* header_in_out) {
    if (header_in_out->e_shnum == 0) {
        // Figure out if # of sections might be >= SHN_LORESERVE or if there really are 0 sections
        ElfParser_SectionHeader section;
        
        // Start by assuming there is one section (the null one at index 0)
        header_in_out->true_shnum = 1;
        ElfParser_Error err = elfparser_get_section_header(elf_start, header_in_out, 0, &section);
        
        if (err || !elfparser_is_null_section(&section)) {
            // Oops, section #0 isn't valid! Guess there really are 0 sections
            return 0;
        }
        // Section #0 is valid! True # of sections stored in size field
        return section.sh_size;
    }
    // If e_shnum != 0, then it is the true # of sections
    return header_in_out->e_shnum;
}


uint64_t elfparser_get_true_shstrndx(const void* elf_start, const ElfParser_Header* header_in_out) {
    if (header_in_out->true_shnum == 0) {
        // No sections means no string table
        return 0;
    }
    
    if (header_in_out->e_shstrndx == ELFPARSER_SHN_XINDEX) {
        // Index stored in section #0
        ElfParser_SectionHeader section;
        ElfParser_Error err = elfparser_get_section_header(elf_start, header_in_out, 0, &section);
        if (err != ELFPARSER_NOERROR) {
            // This shouldn't ever happen, but in case it somehow does
            return 0;
        // True string table section index stored in link field
        return section.sh_link;
        }
    }
    // If e_shstrndx != ELFPARSER_SHN_XINDEX, then it is the true string table section index
    return header_in_out->e_shstrndx;
}


const char* elfparser_get_section_header_name(const void* elf_start, const ElfParser_Header* header,
                                              const ElfParser_SectionHeader* section) {
    // If there is no string table section, then we don't have a name to return
    if (header->true_shstrndx == 0) return "";
    
    // If we're looking at the null/undefined section header, we already know there's no name
    if (section->index == 0) return "";
    
    uint64_t name_off; // Offset of name in file
    if (section->index == header->true_shstrndx) {
        // If this current section *is* the string table, get the name offset using own section offset
        name_off = section->sh_offset + section->sh_name;
    } else { 
        // Else, get offset using header field previously set by elfparser_get_header
        name_off = header->string_table_offset + section->sh_name;
    }
    
    // Return empty string if name out of bounds
    if (!elfparser_is_string_in_bounds(elf_start, header, name_off)) return "";
    
    // Else return name
    return elf_start + name_off;
}


const char* elfparser_get_symbol_name(const void* elf_start, const ElfParser_Header* header,
                                      const ElfParser_Symbol* symbol) {
    // If there is no symbol string table section, then we don't have a name to return
    if (header->symbol_string_table_offset == 0) return "";
    
    // If we're looking at the null/undefined symbol, we already know there's no name
    if (symbol->index == 0) return "";
    
    uint64_t name_off = header->symbol_string_table_offset + symbol->st_name; // Offset of name in file

    // Return empty string if name out of bounds
    if (!elfparser_is_string_in_bounds(elf_start, header, name_off)) return "";
    
    // Else return name
    return elf_start + name_off;
}