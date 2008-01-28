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
// Module: stdext/msvc/declspec.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-5 13:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MSVC_DECLSPEC_H__
#define __STDEXT_MSVC_DECLSPEC_H__

#if defined(_MSC_VER)
#error "Don't include <stdext/msvc/declspec.h>"
#endif

// =========================================================================

#ifndef __cdecl
#define __cdecl
#endif

#ifndef __stdcall
#define __stdcall // __attribute__((stdcall))
#endif

#ifndef __fastcall
#define __fastcall // __attribute__((fastcall))
#endif

#ifndef __forceinline
#define __forceinline inline
#endif

// =========================================================================

#ifndef __STDEXT_MSVC_WTYPES_H__
#include "wtypes.h"
#endif

#ifndef __STDEXT_MSVC_UUIDOF_H__
#include "uuidof.h"
#endif

// =========================================================================
// $Log: $

#endif /* __STDEXT_MSVC_DECLSPEC_H__ */
