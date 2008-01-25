/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/msvc/io.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-5 13:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MSVC_IO_H__
#define __STDEXT_MSVC_IO_H__

#if defined(_WIN32)
#error "Don't include <stdext/msvc/io.h>"
#endif

#include <sys/stat.h>

// =========================================================================

inline long _filelength(int fd)
{
    struct stat s;
    if (fstat(fd, &s) < 0)
        return 0;
    return s.st_size;
}

// =========================================================================
// $Log: $

#endif /* __STDEXT_MSVC_IO_H__ */
