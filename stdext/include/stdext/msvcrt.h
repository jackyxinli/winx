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
// Module: stdext/msvcrt.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-14 15:24:12
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MSVCRT_H__
#define __STDEXT_MSVCRT_H__

#ifndef __STDEXT_PLATFORM_H__
#include "../Platform.h"
#endif

// -------------------------------------------------------------------------
// Visual C++ special

#if defined(STD_NO_MSVCRT)

//
// stdio.h
//
#ifdef WINX_GCC
#ifndef _STDIO_H
#include <stdio.h>
#endif
#define _vsnprintf(buf, n, fmt, arglist) vsnprintf(buf, n, fmt, arglist)
#endif

//
// io.h
//
#include <sys/stat.h>
inline long _filelength(int fd)
{
    struct stat s;
    if (fstat(fd, &s) < 0)
        return 0;
    return s.st_size;
}

//
// assert.h
//
#ifdef WINX_GCC
#ifndef _ASSERT_H
#include <assert.h>
#endif
#define _assert(exp, file, line) __assert_fail(exp, (const char*)(file), line, __ASSERT_FUNCTION)
#endif

/*
_itoa
_ltoa
_ultoa
_i64toa
_ui64toa
_gcvt
_alloca
_msize
*/

#endif

// -------------------------------------------------------------------------
// ==== crtdbg.h ====

#if (0)
#define STD_NO_MAP_ALLOC
#endif

#ifndef STD_NO_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif

#ifndef _INC_STDLIB
#include <stdlib.h>
#endif

#ifndef _INC_MALLOC
#include <malloc.h>
#endif

#if !defined(STD_NO_MSVCRT)
#ifndef _INC_CRTDBG
#include <crtdbg.h>
#endif
#else
#ifndef __STDEXT_MSVCRT_CRTDBG_H__
#include "msvcrt/crtdbg.h"
#endif
#endif // !defined(STD_NO_MSVCRT)

// -------------------------------------------------------------------------
// ==== std::exception ====

#if defined(WINX_GCC)

#ifndef __EXCEPTION__
#include <exception>
#endif

namespace std
{
	class msvc_exception : public exception
	{
	private:
		const char* _m_what;
	public:
		msvc_exception() : _m_what("") {}
		msvc_exception(const char* msg) : _m_what(msg) {}
		virtual const char* what() const throw() { return _m_what; }
	};
}

#define exception msvc_exception

#endif // defined(WINX_GCC)

// -------------------------------------------------------------------------
// ==== atlconv.h ====

#ifndef __STDEXT_MSVCRT_ATLCONV_H__
#include "msvcrt/atlconv.h"
#endif

// -------------------------------------------------------------------------
// $Log: $

#endif /* __STDEXT_MSVCRT_H__ */
