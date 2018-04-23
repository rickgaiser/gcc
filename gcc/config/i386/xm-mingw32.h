/* Configuration for GNU C-compiler for hosting on Windows32.
   using GNU tools and the Windows32 API Library.
   Copyright (C) 1997, 1998, 1999, 2001 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* Even though we support "/", allow "\" since everybody tests both.  */
#define DIR_SEPARATOR '\\'
#define DIR_SEPARATOR_2 '/'

/* Mingw32 does not try to hide the underlying DOS-based file system
   like Cygwin does.  */
#define HAVE_DOS_BASED_FILE_SYSTEM

#define HOST_EXECUTABLE_SUFFIX ".exe"

#undef PATH_SEPARATOR
#define PATH_SEPARATOR ';'

/* This describes the machine the compiler is hosted on.  */
#define HOST_BITS_PER_CHAR  CHAR_BIT
#define HOST_BITS_PER_SHORT (CHAR_BIT * SIZEOF_SHORT)
#define HOST_BITS_PER_INT   (CHAR_BIT * SIZEOF_INT)
#define HOST_BITS_PER_LONG  (CHAR_BIT * SIZEOF_LONG)

#ifdef HAVE_LONG_LONG
# define HOST_BITS_PER_LONGLONG (CHAR_BIT * SIZEOF_LONG_LONG)
#else
#ifdef HAVE___INT64
# define HOST_BITS_PER_LONGLONG (CHAR_BIT * SIZEOF___INT64)
#else
/* If we're here and we're GCC, assume this is stage 2+ of a bootstrap
   and 'long long' has the width of the *target*'s long long.  */
# if GCC_VERSION > 3000
#  define HOST_BITS_PER_LONGLONG LONG_LONG_TYPE_SIZE
# endif /* gcc */
#endif
#endif /* no long long */

/* Find the largest host integer type and set its size and type.  */

/* Use long long on the host if the target has a wider long type than
   the host.  */

#if ! defined HOST_BITS_PER_WIDE_INT \
    && defined HOST_BITS_PER_LONGLONG \
    && (HOST_BITS_PER_LONGLONG > HOST_BITS_PER_LONG) \
    && (defined (LONG_LONG_MAX) || defined (LONGLONG_MAX) \
        || defined (LLONG_MAX) || defined (__GNUC__))

# ifdef MAX_LONG_TYPE_SIZE
#  if MAX_LONG_TYPE_SIZE > HOST_BITS_PER_LONG
#   define HOST_BITS_PER_WIDE_INT HOST_BITS_PER_LONGLONG
#   define HOST_WIDE_INT long long
#  endif
# else
#  if LONG_TYPE_SIZE > HOST_BITS_PER_LONG
#   define HOST_BITS_PER_WIDE_INT HOST_BITS_PER_LONGLONG
#   define HOST_WIDE_INT long long
#  endif
# endif

#endif

#ifndef HOST_BITS_PER_WIDE_INT

# if HOST_BITS_PER_LONG > HOST_BITS_PER_INT
#  define HOST_BITS_PER_WIDE_INT HOST_BITS_PER_LONG
#  define HOST_WIDE_INT long
# else
#  define HOST_BITS_PER_WIDE_INT HOST_BITS_PER_INT
#  define HOST_WIDE_INT int
# endif

#endif /* ! HOST_BITS_PER_WIDE_INT */

/* Provide defaults for the way to print a HOST_WIDE_INT
   in various manners.  */

#ifndef HOST_WIDE_INT_PRINT_DEC
# if HOST_BITS_PER_WIDE_INT == HOST_BITS_PER_INT
#  define HOST_WIDE_INT_PRINT_DEC "%d"
# else
#  if HOST_BITS_PER_WIDE_INT == HOST_BITS_PER_LONG
#   define HOST_WIDE_INT_PRINT_DEC "%ld"
#  else
#   define HOST_WIDE_INT_PRINT_DEC "%I64d"
#  endif
# endif
#endif /* ! HOST_WIDE_INT_PRINT_DEC */

#ifndef HOST_WIDE_INT_PRINT_UNSIGNED
# if HOST_BITS_PER_WIDE_INT == HOST_BITS_PER_INT
#  define HOST_WIDE_INT_PRINT_UNSIGNED "%u"
# else
#  if HOST_BITS_PER_WIDE_INT == HOST_BITS_PER_LONG
#   define HOST_WIDE_INT_PRINT_UNSIGNED "%lu"
#  else
#   define HOST_WIDE_INT_PRINT_UNSIGNED "%I64u"
#  endif
# endif
#endif /* ! HOST_WIDE_INT_PRINT_UNSIGNED */

#ifndef HOST_WIDE_INT_PRINT_HEX
# if HOST_BITS_PER_WIDE_INT == HOST_BITS_PER_INT
#  define HOST_WIDE_INT_PRINT_HEX "0x%x"
# else
#  if HOST_BITS_PER_WIDE_INT == HOST_BITS_PER_LONG
#   define HOST_WIDE_INT_PRINT_HEX "0x%lx"
#  else
#   define HOST_WIDE_INT_PRINT_HEX "0x%I64x"
#  endif
# endif
#endif /* ! HOST_WIDE_INT_PRINT_HEX */

#ifndef HOST_WIDE_INT_PRINT_DOUBLE_HEX
# if HOST_BITS_PER_WIDE_INT == 64
#  if HOST_BITS_PER_WIDE_INT == HOST_BITS_PER_INT
#   define HOST_WIDE_INT_PRINT_DOUBLE_HEX "0x%x%016x"
#  else
#   if HOST_BITS_PER_WIDE_INT == HOST_BITS_PER_LONG
#    define HOST_WIDE_INT_PRINT_DOUBLE_HEX "0x%lx%016lx"
#   else
#    define HOST_WIDE_INT_PRINT_DOUBLE_HEX "0x%llx%016llx"
#   endif
#  endif
# else
#  if HOST_BITS_PER_WIDE_INT == HOST_BITS_PER_INT
#   define HOST_WIDE_INT_PRINT_DOUBLE_HEX "0x%x%08x"
#  else
#   if HOST_BITS_PER_WIDE_INT == HOST_BITS_PER_LONG
#    define HOST_WIDE_INT_PRINT_DOUBLE_HEX "0x%lx%08lx"
#   else
#    define HOST_WIDE_INT_PRINT_DOUBLE_HEX "0x%llx%08llx"
#   endif
#  endif
# endif
#endif /* ! HOST_WIDE_INT_PRINT_DOUBLE_HEX */

/* Find HOST_WIDEST_INT and set its bit size, type and print macros.
   It will be the largest integer mode supported by the host which may
   (or may not) be larger than HOST_WIDE_INT.  */

#ifndef HOST_WIDEST_INT
#if defined HOST_BITS_PER_LONGLONG && HOST_BITS_PER_LONGLONG > HOST_BITS_PER_LONG
#   define HOST_BITS_PER_WIDEST_INT HOST_BITS_PER_LONGLONG
#   define HOST_WIDEST_INT __int64
#   define HOST_WIDEST_INT_PRINT_DEC "%I64d"
#   define HOST_WIDEST_INT_PRINT_UNSIGNED "%I64u"
#   define HOST_WIDEST_INT_PRINT_HEX "0x%I64x"
#  else
#   define HOST_BITS_PER_WIDEST_INT HOST_BITS_PER_LONG
#   define HOST_WIDEST_INT long
#   define HOST_WIDEST_INT_PRINT_DEC "%ld"
#   define HOST_WIDEST_INT_PRINT_UNSIGNED "%lu"
#   define HOST_WIDEST_INT_PRINT_HEX "0x%lx"
# endif /* long long wider than long */
#endif /* ! HOST_WIDEST_INT */
