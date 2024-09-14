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

#include <stdbool.h>

static inline bool elfparser_is_valid_ei_class(ElfParser_EI_Class value) {
    return value == ELFPARSER_ELFCLASS32 || value == ELFPARSER_ELFCLASS64;
}

static inline bool elfparser_is_valid_ei_data(ElfParser_EI_Data value) {
    return value == ELFPARSER_ELFDATA2LSB || value == ELFPARSER_ELFDATA2MSB;
}

static inline bool elfparser_is_valid_ei_version(ElfParser_EI_Version value) {
    return value == ELFPARSER_EV_CURRENT;
}

static inline bool elfparser_is_valid_ei_osabi(ElfParser_EI_OSABI value) {
    return  value <= ELFPARSER_ELFOSABI_LINUX ||
            (ELFPARSER_ELFOSABI_SOLARIS <= value && value <= ELFPARSER_ELFOSABI_NSK) ||
            ELFPARSER_ELFOSABI_LOARCH <= value;
}

static inline bool elfparser_is_valid_e_type(ElfParser_E_Type value) {
    return value <= ELFPARSER_ET_CORE ||  ELFPARSER_ET_LOOS <= value;
}

static inline bool elfparser_is_valid_e_machine(ElfParser_E_Machine value) {
    // While only some e_machine values are defined, new ones might be assigned in later standards, so accept all values
    return true;
}

static inline bool elfparser_is_valid_e_version(ElfParser_E_Version value) {
    return value == ELFPARSER_EV_CURRENT;
}

static inline bool elfparser_is_valid_sh_type(ElfParser_SH_Type value) {
    return  (ELFPARSER_SHT_NULL         <= value && value <= ELFPARSER_SHT_DYNSYM) ||
            (ELFPARSER_SHT_INIT_ARRAY   <= value && value <= ELFPARSER_SHT_SYMTAB_SHNDX) ||
            (ELFPARSER_SHT_LOOS         <= value && value <= ELFPARSER_SHT_HIUSER);
}

static inline bool elfparser_is_valid_sh_flags(ElfParser_SH_Flags value) {
    // These are just bit flags, any combination of them is valid
    return true;
}

static inline bool elfparser_is_valid_st_bind(ElfParser_ST_Bind value) {
    return value <= ELFPARSER_STB_WEAK || ELFPARSER_STB_LOOS <= value;
}

static inline bool elfparser_is_valid_st_type(ElfParser_ST_Type value) {
    return value <= ELFPARSER_STT_TLS || ELFPARSER_STT_LOOS <= value;
}

static inline bool elfparser_is_valid_st_visibility(ElfParser_ST_Visibility value) {
    return true; // The visibility is only 2 bits, and all 4 combinations are defined
}

static inline bool elfparser_is_valid_p_type(ElfParser_P_Type value) {
    return  value <= ELFPARSER_PT_TLS ||
            (ELFPARSER_PT_LOOS <= value && value <= ELFPARSER_PT_HIPROC);
}

static inline bool elfparser_is_valid_p_flags(ElfParser_P_Flags value) {
    // These are just bit flags, any combination of them is valid
    return true;
}