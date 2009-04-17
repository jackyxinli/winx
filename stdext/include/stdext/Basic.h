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
// Module: stdext/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: Basic.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_BASIC_H
#define STDEXT_BASIC_H

#if (0)
#define STD_NO_WINSDK
#define STD_NO_MSVCRT
#define STD_NO_GLOBAL_TESTCASE
#define STD_ISOCPP_FOR
#define STD_EXIT
#define STD_UNITTEST // Run unit testcases
#define STATIC_ASSERT
#define ASSERT_ONCE
#define ASSERT_DERIVE
#define ASSERT_OK
#define REPORT
#define REPORTA
#define VERIFY_OK
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef _SECURE_SCL
#define _SECURE_SCL 0
#endif

#ifndef _SECURE_ATL
#define _SECURE_ATL	0
#endif

#if !defined(_WIN32)
#define STD_NO_WINSDK
#endif

#if !defined(_MSC_VER)
#define STD_NO_MSVCRT
#endif

#ifndef STDEXT_PLATFORM_H
#include "Platform.h"
#endif

#ifndef STLPATCH_H
#include "../stlpatch.h"
#endif

// -------------------------------------------------------------------------

#ifndef _INC_STRING
#include <string.h>
#endif

#ifndef assert
#include <assert.h>
#endif

// -------------------------------------------------------------------------
// NS_STDEXT

#ifndef NS_STDEXT
#define NS_STDEXT			std
#define NS_STDEXT_BEGIN		namespace std {
#define NS_STDEXT_END		}
#endif

// -------------------------------------------------------------------------
// C++ Compiler

#if defined(X_CC_VC)
#	if defined(X_CC_VC_NET)
#		define WINX_VC_NET
#		pragma warning(disable:4290)
		// A function is declared using exception specification, which Visual C++
		// accepts but does not implement
#		if defined(X_CC_VC_NET_GE2005)
#			define WINX_VC_NET_GE2005 // Version >= VS.NET 2005
#		else
#			define WINX_VC_NET_LE2003 // Version <= VS.NET 2003
#		endif
#	else
#		if defined(X_CC_VC6)
#			define WINX_VC6
#		endif
#	endif
#elif defined(X_CC_GCC)
#	define WINX_GCC
#endif

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
	#define WINX_NO_PARTIAL_SPECIALIZATION
#elif defined(_MSC_VER)
	#if (_MSC_VER <= 1200)
	#define WINX_NO_PARTIAL_SPECIALIZATION
	#endif
#endif

// -------------------------------------------------------------------------
// winx_call

#ifndef winx_call
#	if defined(WINX_GCC)
#		define winx_call
#	else
#		define winx_call	__fastcall
#	endif
#endif

// -------------------------------------------------------------------------
// macro __noop - diagnost

#if !defined(__noop)
#	if defined(_MSC_VER)
#		if _MSC_VER <= 1200
#			define __noop 0
#		endif
#	else
inline int _winx_null_func(const void* fmt, ...) { return 0; }
inline int _winx_null_func(int nLevel, const void* fmt, ...) { return 0; }
#	define __noop  _winx_null_func
#	endif
#endif

// -------------------------------------------------------------------------
// macro for

#if defined(STD_ISOCPP_FOR)
#	if defined(_MSC_VER) && (_MSC_VER <= 1200)
#	undef for
#	define for	if (0); else for
#	endif
#endif

// -------------------------------------------------------------------------
// countof - for general use

#ifndef	countof
#define countof(array)						(sizeof(array)/sizeof(array[0]))
#endif

// -------------------------------------------------------------------------
// WINX_TEXT

#if defined(UNICODE)
#	if !defined(_UNICODE)
#		define _UNICODE
#	endif
#elif defined(_UNICODE)
#	if !defined(UNICODE)
#		define UNICODE
#	endif
#endif

#if defined(UNICODE)
#define WINX_TEXT(str)		L ## str
#else
#define WINX_TEXT(str)		str
#endif

#if defined(_OSTREAM_) || defined(_OSTREAM) || defined(_GLIBCXX_OSTREAM)
#define WINX_HAS_OSTREAM
#endif

// -------------------------------------------------------------------------
// winsdk & msvcrt

#if defined(WINX_GCC)
typedef unsigned long long __uint64;
#else
typedef unsigned __int64 __uint64;
#endif

#ifndef STDEXT_VC_DECLSPEC_H
#include "vc/declspec.h"
#endif

#if !defined(STD_NO_WINSDK)

#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef _OBJBASE_H_
#include <objbase.h>		// for CoTaskMemAlloc, CoTaskMemFree
#endif

#else

#ifndef STDEXT_WINAPI_WTYPES_H
#include "winapi/wtypes.h"
#endif

#ifndef STDEXT_WINAPI_OBJBASE_H
#include "winapi/objbase.h"
#endif

#endif

#if !defined(_W64)
typedef signed char INT8, *PINT8;
typedef signed short INT16, *PINT16;
typedef unsigned char UINT8, *PUINT8;
typedef unsigned short UINT16, *PUINT16;
#endif

#if !defined(WINX_VC_NET)
#ifndef __w64
#define __w64
#endif
#endif // !defined(WINX_VC_NET)

#ifndef STDEXT_MSVCRT_H
#include "msvcrt.h"
#endif

// -------------------------------------------------------------------------
// __deprecated(msg) - for code management

#if !defined(__deprecated)

#if !defined(_MSC_VER) || (_MSC_VER <= 1200)
#define __deprecated(msg)
#elif defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 140050320)
#define __deprecated(msg)		__declspec(deprecated(msg))
#else
#define __deprecated(msg)		__declspec(deprecated)
#endif

#endif

// -------------------------------------------------------------------------
// __typename

#if !defined(__typename)
#define __typename(Type)		typeid(Type).name()
#endif

// -------------------------------------------------------------------------
// TokenVal

#ifndef TokenVal
#define TokenVal(p, type)		(*((type*&)(p))++)
#endif

// -------------------------------------------------------------------------
// RELEASE

#ifndef RELEASE
#define RELEASE(pInterface) 												\
	if (pInterface) {														\
		pInterface->Release();												\
		pInterface = NULL;													\
	}
#endif

// -------------------------------------------------------------------------
// CHECK..EXIT

#if (0)
#define EXIT							STD_EXIT
#endif

#ifndef CHECK_BOOLEX
#define CHECK_BOOLEX(result, expr)											\
	if (!(result))															\
	{																		\
		expr;																\
		goto EXIT;															\
	}
#define	CHECK_BOOL(result)				CHECK_BOOLEX(result, 0)
#define CHECK(hr)						CHECK_BOOLEX(SUCCEEDED(hr), 0)
#endif

// -------------------------------------------------------------------------
// WINX_TRY { ... } WINX_TRY_END;

#define WINX_TRY				try
#define WINX_TRY_END			catch (...) { WINX_ASSERT_ONCE(!"WINX_TRY_END"); }

// -------------------------------------------------------------------------
// ABS, ROUND, MAX, MIN - for general use

#ifndef ABS
#define ABS(x)			((x) >= 0 ? (x) : -(x))
#endif

#ifndef ROUND
#define ROUND(x, y)		(((x)+((y)-1)) & ~((y)-1))
#endif

#ifndef MAX
#define MAX(x, y)		((x) > (y) ? (x) : (y))
#define MIN(x, y)		((x) < (y) ? (x) : (y))
#endif

// -------------------------------------------------------------------------
// WINX_PTHIS - for general use

#ifndef WINX_PTHIS
#define WINX_PTHIS			(static_cast<FinalClass*>(this))
#define WINX_NULL_PTHIS		(static_cast<FinalClass*>(0))
#endif

#ifndef WINX_BASE
#define WINX_BASE			(*static_cast<BaseClass*>(this))
#define WINX_PBASE			(static_cast<BaseClass*>(this))
#endif

#ifndef WINX_BASECLASS
#define WINX_BASECLASS(Type)												\
	typedef Type BaseClass;
#endif

// -------------------------------------------------------------------------
// class NullClass

NS_STDEXT_BEGIN

class NullClass {};

NS_STDEXT_END

// -------------------------------------------------------------------------
// Debug Utilities

#ifndef STDEXT_DEBUG_H
#include "Debug.h"
#endif

using NS_STDEXT::DllMainInit;

// -------------------------------------------------------------------------
// TestCase class

#ifndef STDEXT_TESTCASE_H
#include "TestCase.h"
#endif

#if !defined(STD_NO_GLOBAL_TESTCASE)
typedef NS_STDEXT::TestCase TestCase;
#endif

// -------------------------------------------------------------------------
// Log class

#ifndef STDEXT_LOG_H
#include "Log.h"
#endif

// -------------------------------------------------------------------------
// namespace stdext

#if defined(X_STL_GCC)
namespace stdext = __gnu_cxx;
#endif

// -------------------------------------------------------------------------
// --> interface Support

#if defined(__GNUC__) && (__GNUC__ >= 4 && __GNUC_MINOR__ >= 2)
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
// struct 'XXX' has virtual functions but non-virtual destructor
	// see http://www.khp.com.cn/zhuanti/0613_1360GCC/c-002.htm
	// see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html
#endif

// -------------------------------------------------------------------------
// $Log: Basic.h,v $

#endif /* STDEXT_BASIC_H */
