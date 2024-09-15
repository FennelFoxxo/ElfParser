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

#include <elfparser.h>

#include <stdio.h>
#include <stdlib.h>

const char* filename =  "testelf";

uint64_t read_file(const char* name, void* buffer);

int main() {
    
    uint64_t size = read_file(filename, NULL);
    
    if (size == 0) return 1;
    
    
    // Create buffer
    void* data = malloc(size);
    if (data == NULL) {
        printf("Could not allocate elf data buffer!\n");
        return 1;
    }
    
    read_file(filename, data);
    
   ElfParser_Header header;
   // Get ELF header
   ElfParser_Error err = elfparser_get_header(data, size, &header);
   
   if (err != ELFPARSER_NOERROR) {
       printf("Error while parsing ELF header!\n");
       return 1;
   }
   
    printf("------ELF header info------\n");
    printf("  ei_class (32/64 bit):\t%s\n", header.ei_class == ELFPARSER_ELFCLASS64 ? "64" : "32");
    printf("  ei_data (endianness):\t%s\n", header.ei_data == ELFPARSER_ELFDATA2LSB ? "little" : "big");
    printf("  ei_version:\t\t%d\n",                         header.ei_version);
    printf("  ei_osabi:\t\t%d\n",                           header.ei_osabi);
    printf("  ei_abiversion:\t%d\n",                        header.ei_abiversion);
    printf("  e_type:\t\t%d\n",                             header.e_type);
    printf("    - is valid:\t\t%s\n",   elfparser_is_valid_e_type(header.e_type) ? "Yes" : "No");
    printf("  e_machine:\t\t%d\n",                          header.e_machine);
    printf("  e_version:\t\t%d\n",                          header.e_version);
    printf("  e_entry:\t\t0x%llx\n",    (long long unsigned)header.e_entry);
    printf("  e_phoff:\t\t0x%llx\n",    (long long unsigned)header.e_phoff);
    printf("  e_shoff:\t\t0x%llx\n",    (long long unsigned)header.e_shoff);
    printf("  e_flags:\t\t0x%x\n",                          header.e_flags);
    printf("  e_phentsize:\t\t0x%x\n",                      header.e_phentsize);
    printf("  e_phnum:\t\t%d\n",                            header.e_phnum);
    printf("  e_shentsize:\t\t0x%x\n",                      header.e_shentsize);
    printf("  e_shnum:\t\t%d (true_shnum: %llu)\n",         header.e_shnum,     (long long unsigned)header.true_shnum);
    printf("  e_shstrndx:\t\t%d (true_shstrndx: %llu)\n",   header.e_shstrndx,  (long long unsigned)header.true_shstrndx);
   
   printf("\n\n");
   
    printf("------Elf program headers info------\n");
    ElfParser_ProgramHeader program_header;
    for (uint64_t i = 0; i < header.e_phnum; i++) {
        // Get program header
        err = elfparser_get_program_header(data, &header, i, &program_header);
        if (err != ELFPARSER_NOERROR) {
            printf("Error while parsing ELF program headers!\n");
            return 1;
        }
        printf("  Program header #%llu:\n", (long long unsigned)i);
        printf("    p_type:\t\t%d\n",                           program_header.p_type);
        printf("      - is valid:\t%s\n",   elfparser_is_valid_p_type(program_header.p_type) ? "Yes" : "No");
        printf("    p_offset:\t\t0x%llx\n", (long long unsigned)program_header.p_offset);
        printf("    p_vaddr:\t\t0x%llx\n",  (long long unsigned)program_header.p_vaddr);
        printf("    p_paddr:\t\t0x%llx\n",  (long long unsigned)program_header.p_paddr);
        printf("    p_filesz:\t\t0x%llx\n", (long long unsigned)program_header.p_filesz);
        printf("    p_memsz:\t\t0x%llx\n",  (long long unsigned)program_header.p_memsz);
        printf("    p_flags:\t\t0x%x\n",                        program_header.p_flags);
        printf("    p_align:\t\t0x%llx\n",  (long long unsigned)program_header.p_align);
        printf("\n");
   }
   
   printf("\n\n");
   
   printf("------Elf section headers info------\n");
   ElfParser_SectionHeader section;
   for (uint64_t i = 0; i < header.true_shnum; i++) {
       // Get section header
       err = elfparser_get_section_header(data, &header, i, &section);
       if (err != ELFPARSER_NOERROR) {
            printf("Error while parsing ELF section header!\n");
            return 1;
        }
        printf("  Section header #%llu:\n",     (long long unsigned)i);
        printf("    name:\t\t%s\n",                                 section.name);
        printf("    sh_type:\t\t%d\n",                              section.sh_type);
        printf("      - is valid:\t%s\n",       elfparser_is_valid_sh_type(section.sh_type) ? "Yes" : "No");
        printf("    sh_flags:\t\t0x%x\n",                           section.sh_flags);
        printf("    sh_addr:\t\t0x%llx\n",      (long long unsigned)section.sh_addr);
        printf("    sh_offset:\t\t0x%llx\n",    (long long unsigned)section.sh_offset);
        printf("    sh_size:\t\t0x%llx\n",      (long long unsigned)section.sh_size);
        printf("    sh_link:\t\t%d\n",                              section.sh_link);
        printf("    sh_info:\t\t%d\n",                              section.sh_info);
        printf("    sh_addralign:\t0x%llx\n",   (long long unsigned)section.sh_addralign);
        printf("    sh_entsize:\t\t0x%llx\n",   (long long unsigned)section.sh_entsize);
        printf("\n");
   }
   
   printf("\n\n");
   
   printf("------Elf symbols info------\n");
   ElfParser_Symbol symbol;
   for (uint64_t i = 0; i < header.symbol_num; i++) {
       err = elfparser_get_symbol(data, &header, i, &symbol);
       if (err != ELFPARSER_NOERROR) {
            printf("Error while parsing ELF symbol!\n");
            return 1;
        }
        printf("  Symbol #%llu:\n",         (long long unsigned)i);
        printf("    name:\t\t%s\n",                             symbol.name);
        printf("    st_value:\t\t0x%llx\n", (long long unsigned)symbol.st_value);
        printf("    st_size:\t\t0x%llx\n",  (long long unsigned)symbol.st_size);
        printf("    st_bind:\t\t%d\n",                          symbol.st_bind);
        printf("      - is valid:\t%s\n",   elfparser_is_valid_st_bind(symbol.st_bind) ? "Yes" : "No");
        printf("    st_type:\t\t%d\n",                          symbol.st_type);
        printf("      - is valid:\t%s\n",   elfparser_is_valid_st_type(symbol.st_type) ? "Yes" : "No");
        printf("    st_visibility:\t%d\n",                      symbol.st_visibility);
        printf("    st_shndx:\t\t%d\n",                         symbol.st_shndx);
        printf("\n");
   }
   
    printf("\n\n");
    
    err = elfparser_get_section_header_by_name(data, &header, ".myspecialsection1", &section);
    if (err == ELFPARSER_NOERROR) {
        printf("Section '.myspecialsection1' found! Vaddr = 0x%llx\n", (long long unsigned)section.sh_addr);
    } else {
        printf("Section '.myspecialsection1' not found!\n");
    }
    
   err = elfparser_get_symbol_by_name(data, &header, "myint1", &symbol);
   if (err == ELFPARSER_NOERROR) {
       printf("Symbol 'myint1' found! Value = 0x%llx\n", (long long unsigned)symbol.st_value);
   } else {
       printf("Symbol 'myint1' not found!\n");
   }
   
   printf("\n\n");
   
   // Example of copying data from segment to buffer
   
   #define BUFFER_SIZE 100
   uint8_t buffer[BUFFER_SIZE];
   const int segment_to_read_from = 1;
   
   uint64_t total_bytes_to_copy = elfparser_copy_segment(data, &header, segment_to_read_from, NULL, 0, 0);
   
   if (total_bytes_to_copy == ELFPARSER_INVALID) {
       printf("Could not read data from segment #%d!\n", segment_to_read_from);
    } else {
        uint64_t bytes_left = elfparser_copy_segment(data, &header, segment_to_read_from, buffer, 0, BUFFER_SIZE);
        uint64_t bytes_copied = total_bytes_to_copy - bytes_left;
        printf("Copied %llu bytes from segment #%d:\n", (long long unsigned)bytes_copied, segment_to_read_from);
        for (int i = 0; i < bytes_copied; i++) {
            if (i % 16 == 0) printf("\n");
            printf("0x%.2x ", buffer[i]);
        }
        printf("\n");
    }

    return 0;
}


uint64_t read_file(const char* name, void* buffer) {
    // Open file
    FILE* fptr = fopen(name, "rb");

    if (fptr == NULL) {
        printf("Could not open file!\n");
        return 0;
    }
    
    // Get file size
    fseek(fptr, 0L, SEEK_END);
    uint64_t size = ftell(fptr);
    rewind(fptr);
    
    if (buffer) {
        // Read data into buffer
        fread(buffer, size, 1, fptr);
    }
    
    fclose(fptr);
    
    return size;
}