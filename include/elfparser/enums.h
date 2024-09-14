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

typedef enum {
    ELFPARSER_NOERROR,      // Success
    ELFPARSER_INVALID,      // Failed to parse elf format
    ELFPARSER_NOT_FOUND     // Requested symbol or string not found
} ElfParser_Error;

typedef enum {
    ELFPARSER_ELFCLASS32    = 1,
    ELFPARSER_ELFCLASS64    = 2
} ElfParser_EI_Class;

typedef enum {
    ELFPARSER_ELFDATA2LSB   = 1,
    ELFPARSER_ELFDATA2MSB   = 2
} ElfParser_EI_Data;

typedef enum {
    ELFPARSER_EV_CURRENT    = 1
} ElfParser_EI_Version, ElfParser_E_Version;

typedef enum {
    ELFPARSER_ELFOSABI_NONE	    = 0,
    ELFPARSER_ELFOSABI_HPUX	    = 1,
    ELFPARSER_ELFOSABI_NETBSD	= 2,
    ELFPARSER_ELFOSABI_LINUX	= 3,
    ELFPARSER_ELFOSABI_SOLARIS	= 6,
    ELFPARSER_ELFOSABI_AIX	    = 7,
    ELFPARSER_ELFOSABI_IRIX	    = 8,
    ELFPARSER_ELFOSABI_FREEBSD	= 9,
    ELFPARSER_ELFOSABI_TRU64	= 10,
    ELFPARSER_ELFOSABI_MODESTO	= 11,
    ELFPARSER_ELFOSABI_OPENBSD	= 12,
    ELFPARSER_ELFOSABI_OPENVMS	= 13,
    ELFPARSER_ELFOSABI_NSK      = 14,
    ELFPARSER_ELFOSABI_LOARCH   = 64
} ElfParser_EI_OSABI;

typedef enum {
    ELFPARSER_ET_NONE   = 0,
    ELFPARSER_ET_REL    = 1,
    ELFPARSER_ET_EXEC   = 2,
    ELFPARSER_ET_DYN    = 3,
    ELFPARSER_ET_CORE   = 4,
    
    // Inclusive range, OS specific
    ELFPARSER_ET_LOOS   = 0xfe00,
    ELFPARSER_ET_HIOS   = 0xfeff,
    
    // Inclusive range, processor specific
    ELFPARSER_ET_LOPROC = 0xff00,
    ELFPARSER_ET_HIPROC = 0xffff
} ElfParser_E_Type;

typedef enum {
    ELFPARSER_EM_NONE	        = 0,	
    ELFPARSER_EM_M32	        = 1,	
    ELFPARSER_EM_SPARC	        = 2,	
    ELFPARSER_EM_386	        = 3,	
    ELFPARSER_EM_68K	        = 4,	
    ELFPARSER_EM_88K	        = 5,	
    ELFPARSER_EM_860	        = 7,	
    ELFPARSER_EM_MIPS	        = 8,	
    ELFPARSER_EM_S370	        = 9,	
    ELFPARSER_EM_MIPS_RS3_LE	= 10,
    ELFPARSER_EM_PARISC	        = 15,
    ELFPARSER_EM_VPP500	        = 17,
    ELFPARSER_EM_SPARC32PLUS	= 18,
    ELFPARSER_EM_960	        = 19,
    ELFPARSER_EM_PPC	        = 20,
    ELFPARSER_EM_PPC64	        = 21,
    ELFPARSER_EM_S390	        = 22,
    ELFPARSER_EM_V800	        = 36,
    ELFPARSER_EM_FR20	        = 37,
    ELFPARSER_EM_RH32	        = 38,
    ELFPARSER_EM_RCE	        = 39,
    ELFPARSER_EM_ARM	        = 40,
    ELFPARSER_EM_ALPHA	        = 41,
    ELFPARSER_EM_SH	            = 42,
    ELFPARSER_EM_SPARCV9	    = 43,
    ELFPARSER_EM_TRICORE	    = 44,
    ELFPARSER_EM_ARC	        = 45,
    ELFPARSER_EM_H8_300	        = 46,
    ELFPARSER_EM_H8_300H	    = 47,
    ELFPARSER_EM_H8S	        = 48,
    ELFPARSER_EM_H8_500	        = 49,
    ELFPARSER_EM_IA_64	        = 50,
    ELFPARSER_EM_MIPS_X	        = 51,
    ELFPARSER_EM_COLDFIRE	    = 52,
    ELFPARSER_EM_68HC12	        = 53,
    ELFPARSER_EM_MMA	        = 54,
    ELFPARSER_EM_PCP	        = 55,
    ELFPARSER_EM_NCPU	        = 56,
    ELFPARSER_EM_NDR1	        = 57,
    ELFPARSER_EM_STARCORE	    = 58,
    ELFPARSER_EM_ME16	        = 59,
    ELFPARSER_EM_ST100	        = 60,
    ELFPARSER_EM_TINYJ	        = 61,
    ELFPARSER_EM_X86_64	        = 62,
    ELFPARSER_EM_PDSP	        = 63,
    ELFPARSER_EM_PDP10	        = 64,
    ELFPARSER_EM_PDP11	        = 65,
    ELFPARSER_EM_FX66	        = 66,
    ELFPARSER_EM_ST9PLUS	    = 67,
    ELFPARSER_EM_ST7	        = 68,
    ELFPARSER_EM_68HC16	        = 69,
    ELFPARSER_EM_68HC11	        = 70,
    ELFPARSER_EM_68HC08	        = 71,
    ELFPARSER_EM_68HC05	        = 72,
    ELFPARSER_EM_SVX	        = 73,
    ELFPARSER_EM_ST19	        = 74,
    ELFPARSER_EM_VAX	        = 75,
    ELFPARSER_EM_CRIS	        = 76,
    ELFPARSER_EM_JAVELIN	    = 77,
    ELFPARSER_EM_FIREPATH	    = 78,
    ELFPARSER_EM_ZSP	        = 79,
    ELFPARSER_EM_MMIX	        = 80,
    ELFPARSER_EM_HUANY	        = 81,
    ELFPARSER_EM_PRISM	        = 82,
    ELFPARSER_EM_AVR	        = 83,
    ELFPARSER_EM_FR30	        = 84,
    ELFPARSER_EM_D10V	        = 85,
    ELFPARSER_EM_D30V	        = 86,
    ELFPARSER_EM_V850	        = 87,
    ELFPARSER_EM_M32R	        = 88,
    ELFPARSER_EM_MN10300	    = 89,
    ELFPARSER_EM_MN10200	    = 90,
    ELFPARSER_EM_PJ	            = 91,
    ELFPARSER_EM_OPENRISC	    = 92,
    ELFPARSER_EM_ARC_A5	        = 93,
    ELFPARSER_EM_XTENSA	        = 94,
    ELFPARSER_EM_VIDEOCORE      = 95,
    ELFPARSER_EM_TMM_GPP	    = 96,
    ELFPARSER_EM_NS32K	        = 97,
    ELFPARSER_EM_TPC	        = 98,
    ELFPARSER_EM_SNP1K	        = 99,
    ELFPARSER_EM_ST200	        = 100
} ElfParser_E_Machine;

typedef enum {
    ELFPARSER_SHT_NULL          = 0,
    ELFPARSER_SHT_PROGBITS      = 1,
    ELFPARSER_SHT_SYMTAB        = 2,
    ELFPARSER_SHT_STRTAB        = 3,
    ELFPARSER_SHT_RELA          = 4,
    ELFPARSER_SHT_HASH          = 5,
    ELFPARSER_SHT_DYNAMIC       = 6,
    ELFPARSER_SHT_NOTE          = 7,
    ELFPARSER_SHT_NOBITS        = 8,
    ELFPARSER_SHT_REL           = 9,
    ELFPARSER_SHT_SHLIB         = 10,
    ELFPARSER_SHT_DYNSYM        = 11,
    ELFPARSER_SHT_INIT_ARRAY    = 14,
    ELFPARSER_SHT_FINI_ARRAY    = 15,
    ELFPARSER_SHT_PREINIT_ARRAY = 16,
    ELFPARSER_SHT_GROUP         = 17,
    ELFPARSER_SHT_SYMTAB_SHNDX  = 18,
    ELFPARSER_SHT_LOOS          = 0x60000000,
    ELFPARSER_SHT_HIOS          = 0x6fffffff,
    ELFPARSER_SHT_LOPROC        = 0x70000000,
    ELFPARSER_SHT_HIPROC        = 0x7fffffff,
    ELFPARSER_SHT_LOUSER        = 0x80000000,
    ELFPARSER_SHT_HIUSER        = 0xffffffff
} ElfParser_SH_Type;

typedef enum {
    ELFPARSER_SHF_WRITE             = 0x001,
    ELFPARSER_SHF_ALLOC             = 0x002,
    ELFPARSER_SHF_EXECINSTR         = 0x004,
    ELFPARSER_SHF_MERGE             = 0x010,
    ELFPARSER_SHF_STRINGS           = 0x020,
    ELFPARSER_SHF_INFO_LINK         = 0x040,
    ELFPARSER_SHF_LINK_ORDER        = 0x080,
    ELFPARSER_SHF_OS_NONCONFORMING  = 0x100,
    ELFPARSER_SHF_GROUP             = 0x200,
    ELFPARSER_SHF_TLS               = 0x400,
    ELFPARSER_SHF_MASKOS            = 0x0ff00000,
    ELFPARSER_SHF_MASKPROC          = 0xf0000000
} ElfParser_SH_Flags;

typedef enum {
    ELFPARSER_SHN_UNDEF     = 0,
    ELFPARSER_SHN_LORESERVE = 0xff00,
    ELFPARSER_SHN_LOPROC    = 0xff00,
    ELFPARSER_SHN_HIPROC    = 0xff1f,
    ELFPARSER_SHN_LOOS      = 0xff20,
    ELFPARSER_SHN_HIOS      = 0xff3f,
    ELFPARSER_SHN_ABS       = 0xfff1,
    ELFPARSER_SHN_COMMON    = 0xfff2,
    ELFPARSER_SHN_XINDEX    = 0xffff,
    ELFPARSER_SHN_HIRESERVE = 0xffff
} ElfParser_SpecialSectionIndexes;

typedef enum {
    ELFPARSER_STB_LOCAL     = 0,
    ELFPARSER_STB_GLOBAL    = 1,
    ELFPARSER_STB_WEAK      = 2,
    ELFPARSER_STB_LOOS      = 10,
    ELFPARSER_STB_HIOS      = 12,
    ELFPARSER_STB_LOPROC    = 13,
    ELFPARSER_STB_HIPROC    = 15
} ElfParser_ST_Bind;

typedef enum {
    ELFPARSER_STT_NOTYPE    = 0,
    ELFPARSER_STT_OBJECT    = 1,
    ELFPARSER_STT_FUNC      = 2,
    ELFPARSER_STT_SECTION   = 3,
    ELFPARSER_STT_FILE      = 4,
    ELFPARSER_STT_COMMON    = 5,
    ELFPARSER_STT_TLS       = 6,
    ELFPARSER_STT_LOOS      = 10,
    ELFPARSER_STT_HIOS      = 12,
    ELFPARSER_STT_LOPROC    = 13,
    ELFPARSER_STT_HIPROC    = 15
} ElfParser_ST_Type;

typedef enum {
    ELFPARSER_STV_DEFAULT   = 0,
    ELFPARSER_STV_INTERNAL  = 1,
    ELFPARSER_STV_HIDDEN    = 2,
    ELFPARSER_STV_PROTECTED = 3,
} ElfParser_ST_Visibility;

typedef enum {
    ELFPARSER_PT_NULL       = 0,
    ELFPARSER_PT_LOAD       = 1,
    ELFPARSER_PT_DYNAMIC    = 2,
    ELFPARSER_PT_INTERP     = 3,
    ELFPARSER_PT_NOTE       = 4,
    ELFPARSER_PT_SHLIB      = 5,
    ELFPARSER_PT_PHDR       = 6,
    ELFPARSER_PT_TLS        = 7,
    ELFPARSER_PT_LOOS       = 0x60000000,
    ELFPARSER_PT_HIOS       = 0x6fffffff,
    ELFPARSER_PT_LOPROC     = 0x70000000,
    ELFPARSER_PT_HIPROC     = 0x7fffffff
} ElfParser_P_Type;

typedef enum {
    ELFPARSER_PF_X          = 0x1,
    ELFPARSER_PF_W          = 0x2,
    ELFPARSER_PF_R          = 0x4,
    ELFPARSER_PF_MASKOS     = 0x0ff00000,
    ELFPARSER_PF_MASKPROC   = 0xf0000000
} ElfParser_P_Flags;