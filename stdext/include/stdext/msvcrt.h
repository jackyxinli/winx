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
#ifndef STDEXT_MSVCRT_H
#define STDEXT_MSVCRT_H

#ifndef STDEXT_PLATFORM_H
#include "../Platform.h"
#endif

#if (0)
#define STD_NO_MAP_ALLOC
#endif

// -------------------------------------------------------------------------
// ==== crtdbg.h ====

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
#ifndef STDEXT_MSVCRT_CRTDBG_H
#include "msvcrt/crtdbg.h"
#endif
#endif // !defined(STD_NO_MSVCRT)

// -------------------------------------------------------------------------
// Visual C++ Special

//
// stdlib.h
//

#ifdef WINX_GCC

#define _MAX_PATH   260 /* max. length of full pathname */
#define _MAX_DRIVE  3   /* max. length of drive component */
#define _MAX_DIR    256 /* max. length of path component */
#define _MAX_FNAME  256 /* max. length of file name component */
#define _MAX_EXT    256 /* max. length of extension component */

#ifndef STDEXT_WINAPI_WTYPES_H
#include "winapi/wtypes.h"
#endif

#ifndef STDEXT_TCHAR_H
#include "tchar.h"
#endif

#define _itoa		std::tchar::itoa
#define _ltoa		std::tchar::ltoa
#define _ultoa		std::tchar::ultoa
#define	_i64toa		std::tchar::i64toa
#define _ui64toa	std::tchar::ui64toa
#define _ui64tow	std::tchar::ui64toa
#define _gcvt		gcvt
#define _ecvt		ecvt
#define _fcvt		fcvt

inline void __not_impl(const char* szFeature)
{
	_RPT1(_CRT_ERROR, "NotImplementFeature - %s\n", szFeature);
	throw szFeature;
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

#ifndef STDEXT_MSVCRT_ATLCONV_H
#include "msvcrt/atlconv.h"
#endif

// -------------------------------------------------------------------------
// $Log: $

#endif /* STDEXT_MSVCRT_H */
