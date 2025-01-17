// -*- C++ -*- 
// Copyright (C) 2000, 2001 Free Software Foundation
//
// This file is part of GNU CC.
//
// GNU CC is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// GNU CC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU CC; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

#include <bits/c++config.h>
#include "unwind-cxx.h"

#ifdef _GLIBCPP_HAVE_UNISTD_H
# include <unistd.h>
# define writestr(str)	write(2, str, sizeof(str) - 1)
# ifdef __GNU_LIBRARY__
  /* Avoid forcing the library's meaning of `write' on the user program
     by using the "internal" name (for use within the library).  */
/*#  define write(fd, buf, n)	__write((fd), (buf), (n))*/
# endif
#else
# include <cstdio>
# define writestr(str)	std::fputs(str, stderr)
#endif

extern "C" void
__cxa_pure_virtual (void)
{
//  writestr ("pure virtual method called\n");
  std::terminate ();
}
