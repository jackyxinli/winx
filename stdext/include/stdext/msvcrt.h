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
// Visual C++ Special

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
#ifndef _SYS_STAT_H
#include <sys/stat.h>
#endif
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

//
// stdlib.h
//

#ifdef WINX_GCC

#ifndef __STDEXT_TCHAR_H__
#include "tchar.h"
#endif

#define _itoa		tchar::itoa
#define _ltoa		tchar::ltoa
#define _ultoa		tchar::ultoa
#define	_i64toa		tchar::i64toa
#define _ui64toa	tchar::ui64toa

inline void __not_impl(const char* szFeature)
{
	_RPT1(_CRT_ERROR, "NotImplementFeature - %s\n", szFeature);
	throw szFeature;
}

inline char* _gcvt(double value, int ndec, char* buf)
{
	__not_impl("_gcvt");
	return buf;
}

#endif // WINX_GCC

//
// malloc.h
//

#ifdef WINX_GCC

#ifndef _ALLOCA_H
#include <alloca.h>
#endif

#define _alloca	alloca

#ifndef _MALLOC_H
#include <malloc.h>
#endif

#define _msize malloc_usable_size
// NOTE: 
//	malloc_usable_size() returns the number of bytes you actually can use in a previously 
//	allocated memory block. This value may be more than the amount you originally requested,
//	due to alignment and minimum size constraints. For example, if you allocate 30 bytes, 
//	the usable size is actually 36.

#endif // WINX_GCC

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
