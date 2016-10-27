/*
 * Target machine definitions for the Toshiba r5900.
 *
 * Copyright (c) 2003  M. R. Brown <mrbrown@0xd6.org>
 *
 * Based on the file elf5900.h, which originally bore
 * the copyright banner:
 *
 *    Definitions of target machine for GNU compiler.
 *    Toshiba r5900 little-endian
 *    Copyright (c) 1995 Cygnus Support Inc.
 *
 * Also based on the PS2/Linux GCC port provided by SCEI and SCEA.
 *
 * Licensed under the terms of the GNU GPL version 2.
 */

/* TODO: Add Linux support. */

#include "mips/elf64.h"
#include "mips/abi64.h"

#define MIPS_CPU_STRING_DEFAULT "R5900"

#undef MIPS_ABI_DEFAULT
#define MIPS_ABI_DEFAULT ABI_EABI

#undef MIPS_ISA_DEFAULT
#define MIPS_ISA_DEFAULT 3

#undef MACHINE_TYPE
#define MACHINE_TYPE "(MIPSel R5900 ELF)"

/*
 I started doing some macro fixes, but as we plan to use 3.4 soon I'm not risking changing stuff I'm not
 certain about.
*/
/*
#define USE_MACRO_FIXES
*/

/*
 gcc-3.0.3-ps2linux has additional min.s and max.s floating point insns. These should be incorporate into mips.md.
*/

#define FILL_BDSLOT_WITH_NOP

/*
 * Use Gas, target r5900, -msingle-float, 64-bit CPU and longs, split-addr (avoids la insn emits), -mmmi
 */
#undef TARGET_DEFAULT
#define TARGET_DEFAULT ( MASK_GAS | MASK_SINGLE_FLOAT | MASK_64BIT \
			| MASK_LONG64 | MASK_MMI | MASK_VUMM | MASK_SPLIT_ADDR )

/*
 * Even though we're a 64-bit processor, we only support a 32-bit address
 * space.
 */
#undef Pmode
#define Pmode SImode

/* 128-bit alignment options */
#define DEFAULT_MIPS_ALIGNMENT 128

/* TODO: need to make this variable */
#undef  BIGGEST_ALIGNMENT 
#define BIGGEST_ALIGNMENT  DEFAULT_MIPS_ALIGNMENT

#undef SUBTARGET_TARGET_OPTIONS
#define SUBTARGET_TARGET_OPTIONS					\
  { "use-128", &mips_use_128_string,					\
      N_("Use 128-bit operations")},					\
  { "align128", &mips_align128_string,					\
      N_("Use 128-bit alignment")},					\
  { "no-align128", &mips_no_align128_string,				\
      N_("Don't use 128-bit alignment")},				\
  { "abi=", &mips_abi_string,						\
      N_("Speciy ABI to use")},

/* Stack alignment */
#undef STACK_BOUNDARY
#define STACK_BOUNDARY							\
  ((mips_abi == ABI_32 || mips_abi == ABI_O64 || (mips_abi == ABI_EABI	\
    && !TARGET_MIPS5900))						\
   ? 64 : 128)

#undef MIPS_STACK_ALIGN
#define MIPS_STACK_ALIGN(LOC)						\
  (((mips_abi == ABI_32 || mips_abi == ABI_O64 || mips_abi == ABI_EABI)	\
    && mips_alignment != 128 )	 					\
   ? ((LOC) + 7) & ~7							\
   : ((LOC) + 15) & ~15)

#define PREFERRED_STACK_BOUNDARY \
(mips_alignment == 128 ? 128 : 						\
  ((mips_abi == ABI_32 || mips_abi == ABI_O64 || mips_abi == ABI_EABI )	\
   ? 64 : 128))

/* TODO: Find a way to specify default alignments for functions, jumps,
   and loops.  These used to be machine dependant, but now live as -f
   parameters.  */

/* prefer .p2align rather than .align (gas assumed) */
#define ASM_OUTPUT_MAX_SKIP_ALIGN(FILE,LOG,MAX_SKIP) \
  if ((LOG)!=0) \
    if ((MAX_SKIP)==0) fprintf ((FILE), "\t.p2align %d\n", (LOG)); \
    else fprintf ((FILE), "\t.p2align %d,,%d\n", (LOG), (MAX_SKIP))

/* How to output a quadword for the r5900.  */
#define ASM_OUTPUT_QUADRUPLE_INT(STREAM,VALUE)                          \
do {                                                                    \
  if (TARGET_64BIT)                                                     \
    {                                                                   \
      fprintf (STREAM, "\t.octa\t");                                    \
      if (HOST_BITS_PER_WIDE_INT < 64 || GET_CODE (VALUE) != CONST_INT) \
        /* We can't use 'X' for negative numbers, because then we won't \
           get the right value for the upper 32 bits.  */               \
        output_addr_const (STREAM, VALUE);                              \
      else                                                              \
        /* We must use 'X', because otherwise LONG_MIN will print as    \
           a number that the Irix 6 assembler won't accept.  */         \
        print_operand (STREAM, VALUE, 'X');                             \
      fprintf (STREAM, "\n");                                           \
    }                                                                   \
  else                                                                  \
    {                                                                   \
      assemble_integer (operand_subword ((VALUE), 0, 0, TImode),        \
                        UNITS_PER_WORD, 1);                             \
      assemble_integer (operand_subword ((VALUE), 1, 0, TImode),        \
                        UNITS_PER_WORD, 1);                             \
    }                                                                   \
} while (0)


#undef CPP_PREDEFINES
#define CPP_PREDEFINES "\
-D__ELF__ \
-Dmips -D_mips -D__mips -D__mips__ \
%{!miop: -DR5900 -D_R5900 -D__R5900 -D_EE } \
%{miop: -DR3000 -D_3000 -D__3000 } \
-DMIPSEL -D_MIPSEL -D__MIPSEL -D__MIPSEL__ \
-Acpu(mips) -Amachine(mips) \
"

/* Default to 32-bit ints and 64-bit longs. */
#undef SUBTARGET_CPP_SIZE_SPEC 
#define SUBTARGET_CPP_SIZE_SPEC "\
%{mgp64: -D_MIPS_SZPTR=32 } \
%{!mgp64: -D_MIPS_SZPTR=32 } \
\
%{mlong64:-D__SIZE_TYPE__=long\\ unsigned\\ int \
  -D__SSIZE_TYPE__=long\\ int \
  -D__PTRDIFF_TYPE__=long\\ int \
  -D_MIPS_SZLONG=64}  \
\
%{mint64:-D_MIPS_SZINT=64 \
  %{!mlong64:-D__SIZE_TYPE__=long\\ unsigned\\ int \
    -D__SSIZE_TYPE__=long\\ int \
    -D__PTRDIFF_TYPE__=long\\ int \
    -D_MIPS_SZLONG=64}} \
\
%{!mint64:-D_MIPS_SZINT=32 \
  %{!mlong64:-D__SIZE_TYPE__=unsigned\\ int \
    -D__SSIZE_TYPE__=int \
    -D__PTRDIFF_TYPE__=int \
    -D_MIPS_SZLONG=64}} \
"

#undef SUBTARGET_CPP_SPEC 
#define SUBTARGET_CPP_SPEC "\
%{mfp32: -D_MIPS_FPSET=16}%{!mfp32: -D_MIPS_FPSET=32} \
%{mips1: -U_MIPS_ISA -D_MIPS_ISA=_MIPS_ISA_MIPS1} \
%{mips2: -U_MIPS_ISA -D_MIPS_ISA=_MIPS_ISA_MIPS2} \
%{mips3: -U_MIPS_ISA -D_MIPS_ISA=_MIPS_ISA_MIPS3 -UR3000 -U_R3000} \
%{mips4: -U_MIPS_ISA -D_MIPS_ISA=_MIPS_ISA_MIPS4 -UR3000 -U_R3000} \
%{!mips*: -U__mips -U_MIPS_ISA -D_MIPS_ISA=_MIPS_ISA_MIPS3 -D__mips=3 -D__mips64} \
%{!mabi=32: %{!mabi=n32: %{!mabi=64: -D__mips_eabi}}} \
%{!msoft-float: %{!mdouble-float : -D__mips_single_float}} \
%{-D__HAVE_FPU__ } \
%{posix: -D_POSIX_SOURCE} \
%{.cc:	-D__LANGUAGE_C_PLUS_PLUS__  %{!ansi:-DLANGUAGE_C_PLUS_PLUS}} \
%{.cxx:	-D__LANGUAGE_C_PLUS_PLUS__  %{!ansi:-DLANGUAGE_C_PLUS_PLUS}} \
%{.C:	-D__LANGUAGE_C_PLUS_PLUS__  %{!ansi:-DLANGUAGE_C_PLUS_PLUS}} \
%{.m:	-D__LANGUAGE_OBJECTIVE_C__ %{!ansi:-DLANGUAGE_OBJECTIVE_C}} \
%{.S:	-D__LANGUAGE_ASSEMBLY__ %{!ansi:-DLANGUAGE_ASSEMBLY}} \
%{.s:	-D__LANGUAGE_ASSEMBLY__ %{!ansi:-DLANGUAGE_ASSEMBLY}} \
%{!.S: %{!.s: %{!.cc: %{!.cxx: %{!.C: %{!.m: -D__LANGUAGE_C__ %{!ansi:-DLANGUAGE_C}}}}}} } \
%{ansi:-Uunix -Ulinux -Umips -UR3000 -UMIPSEB -UMIPSEL} \
%{ffast-math: -D__FAST_MATH__} \
 "

/* The GNU C++ standard library requires that these macros be defined.  */
#undef CPLUSPLUS_CPP_SPEC
#define CPLUSPLUS_CPP_SPEC "\
-D__LANGUAGE_C_PLUS_PLUS -D_LANGUAGE_C_PLUS_PLUS \
-D_GNU_SOURCE %(cpp) \
 "

#undef SUBTARGET_ASM_SPEC
#define SUBTARGET_ASM_SPEC "\
%{!march*: %{!miop: -mwarn-short-loop -march=r5900} %{miop: -march=r3000}} \
%{!mips*: %{!miop: -mips3 %{!mfp*: -mfp32} %{!mgp*: -mgp64}} %{miop: -mips1 %{!mfp*: -mfp32} %{!mgp*: -mgp32}}} \
%{!mabi*: %{!miop: -mabi=eabi}} \
%{!mdouble-float: -msingle-float} \
 "

/*
 * We want to include alignment directives for the r5900 to ensure that
 * TImode values are properly aligned.  It would be best to do this in
 * mips.h, but it's unclear if all mips assemblers can handle alignments
 * for local common/bss objects.
 */
#undef ASM_OUTPUT_LOCAL

/* 
 * This says how to output an assembler line to define a global common symbol
 * with size SIZE (in bytes) and alignment ALIGN (in bits).
 */

#undef ASM_OUTPUT_ALIGNED_COMMON
#define ASM_OUTPUT_ALIGNED_COMMON(STREAM, NAME, SIZE, ALIGN)		\
do									\
{									\
  mips_declare_object (STREAM, NAME, "\n\t.comm\t", ",%u,", (SIZE));	\
  fprintf ((STREAM), "%d\n", ((ALIGN) / BITS_PER_UNIT));  	\
}									\
while (0)

#define POPSECTION_ASM_OP       "\t.previous"
#define BSS_SECTION_ASM_OP      "\t.section\t.bss"
#define SBSS_SECTION_ASM_OP     "\t.section\t.sbss"
#undef ASM_OUTPUT_ALIGNED_LOCAL
#define ASM_OUTPUT_ALIGNED_LOCAL(STREAM, NAME, SIZE, ALIGN)		\
do									\
  {									\
    if ((SIZE) > 0 && (SIZE) <= mips_section_threshold)			\
      fprintf (STREAM, "%s\n", SBSS_SECTION_ASM_OP);			\
    else				       				\
      fprintf (STREAM, "%s\n", BSS_SECTION_ASM_OP);			\
    mips_declare_object (STREAM, NAME, "", ":\n", 0);		   	\
    ASM_OUTPUT_ALIGN (STREAM, floor_log2 (ALIGN / BITS_PER_UNIT));	\
    ASM_OUTPUT_SKIP (STREAM, SIZE);					\
    fprintf (STREAM, "%s\n", POPSECTION_ASM_OP);			\
  }									\
while (0)

/* 
 * Avoid returning unaligned structures > 64bits, but <= 128bits wide
 * in registers.
 *
 * This avoids a bad interaction between the code to return BLKmode
 * structures in registers and the wider than word_mode registers
 * found on the r5900.
 *
 * This does not effect returning an aligned 128bit value in a register,
 * which should work.
 */

#undef RETURN_IN_MEMORY
#define RETURN_IN_MEMORY(TYPE)						\
  ((mips_abi == ABI_32 || mips_abi == ABI_O64)				\
   ? TYPE_MODE (TYPE) == BLKmode					\
   : (mips_abi == ABI_EABI && TYPE_MODE (TYPE) == BLKmode		\
      ? (int_size_in_bytes (TYPE) > UNITS_PER_WORD)			\
      : (int_size_in_bytes (TYPE)					\
	 > (mips_abi == ABI_EABI ? 2 * UNITS_PER_WORD : 16))))

#define EH_FRAME_IN_DATA_SECTION 1

/*
 * This patch is ugly. But it hopefully will put an end to that
 * 128-bits alignment madness.
 */

#undef CONSTANT_ALIGNMENT
#define CONSTANT_ALIGNMENT(EXP, ALIGN)                                  \
  ((TREE_CODE (EXP) == STRING_CST  || TREE_CODE (EXP) == CONSTRUCTOR)   \
   && (ALIGN) < (mips_use_128 ? 128 : 64) ? (mips_use_128 ? 128 : 64) : (ALIGN))

#undef DATA_ALIGNMENT
#define DATA_ALIGNMENT(TYPE, ALIGN)                                     \
  ((((ALIGN) < (mips_use_128 ? 128 : 64))                               \
    && (TREE_CODE (TYPE) == ARRAY_TYPE                                  \
        || TREE_CODE (TYPE) == UNION_TYPE                               \
        || TREE_CODE (TYPE) == RECORD_TYPE)) ? (mips_use_128 ? 128 : 64) : (ALIGN))
