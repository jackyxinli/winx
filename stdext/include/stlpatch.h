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
// Module: stlpatch.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-22 18:01:56
// 
// $Id: stlpatch.h,v 1.1 2006/12/22 10:19:55 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STLPATCH_H__
#define __STLPATCH_H__

#ifndef __STDEXT_PLATFORM_H__
#include "stdext/Platform.h"
#endif

// -------------------------------------------------------------------------

#pragma warning(disable:4786)

#ifndef __STDEXT_PATCH_STRING_H__
#include "stdext/patch/string.h"
#endif

// -------------------------------------------------------------------------
// _ConvIt - vector iterator convert

namespace std {

#if !defined(_MSC_VER)

#ifndef __forceinline
#define __forceinline inline
#endif

#endif

#if defined(X_STL_NET) // visual c++ .net

template <class _It>
__forceinline typename _It::pointer _ConvIt(_It it)
	{ return it._Myptr; }

#else

template <class _It>
__forceinline _It _ConvIt(_It it)
	{ return it; }

#endif

} // namespace std

// -------------------------------------------------------------------------
// $Log: stlpatch.h,v $
// Revision 1.1  2006/12/22 10:19:55  xushiwei
// STL-Patch: std::basic_string::_Split function bugfix (vc6)
//

#endif /* __STLPATCH_H__ */
