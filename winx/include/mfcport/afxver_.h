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
// Module: mfcport/afxver_.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-26 11:57:31
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __MFCPORT_AFXVER__H__
#define __MFCPORT_AFXVER__H__

/////////////////////////////////////////////////////////////////////////////
// Master version numbers

#define _AFX     1      // Microsoft Application Framework Classes
#ifndef _MFC_VER
#define _MFC_VER 0x0800 // Microsoft Foundation Classes version 8.00
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
#ifndef _AFX_DISABLE_INLINES
	#define _AFX_ENABLE_INLINES
#endif
#endif

#define _AFX_NO_NESTED_DERIVATION

/////////////////////////////////////////////////////////////////////////////
// special include files

#ifndef AFX_INLINE
	#define AFX_INLINE inline /*__forceinline*/
#endif

// Include any non-Intel platform specific items
#ifndef _X86_
	#include "afxv_cpu.h"
#endif

#ifdef _X86_
	#define _AFX_MINREBUILD
#endif

// setup default packing value
#ifndef _AFX_PACKING
	#define _AFX_PACKING    4   // default packs structs at 4 bytes
#endif

// Define this virtual key for use by status bar
#ifndef VK_KANA
#define VK_KANA             0x15
#endif

/////////////////////////////////////////////////////////////////////////////
// Special AfxDebugBreak: used to break into debugger at critical times

#ifndef AfxDebugBreak
#define AfxDebugBreak() _CrtDbgBreak()
#endif

#ifndef _DEBUG
#ifdef AfxDebugBreak
#undef AfxDebugBreak
#endif
#define AfxDebugBreak()
#endif  // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// Standard preprocessor symbols if not already defined
/////////////////////////////////////////////////////////////////////////////

// SIZE_T_MAX is used by the collection classes
#ifndef SIZE_T_MAX
	#define SIZE_T_MAX  UINT_MAX
#endif

// PASCAL is used for static member functions
#ifndef PASCAL
	#define PASCAL  __stdcall
#endif

// FASTCALL is used for static member functions with little or no params
#ifndef FASTCALL
	#define FASTCALL __fastcall     
#endif

// CDECL and EXPORT are defined in case WINDOWS.H doesn't
#ifndef CDECL
	#define CDECL __cdecl
#endif

#ifndef EXPORT
	#define EXPORT
#endif

// UNALIGNED is used for unaligned data access (in CArchive mostly)
#if !defined(UNALIGNED)
#if defined(_M_IA64) || defined(_M_AMD64)
#define UNALIGNED __unaligned
#else
#define UNALIGNED
#endif
#endif

// AFX_DEPRECATED is used for functions that should no longer be used
#ifndef AFX_DEPRECATED
#ifdef _AFX_DISABLE_DEPRECATED
	#define AFX_DEPRECATED(_Message) 
#else
	#define AFX_DEPRECATED(_Message) __declspec(deprecated(_Message))
#endif
#endif

// _AFX_INSECURE_DEPRECATE is used for deprecated, insecure functions.
#ifndef _AFX_INSECURE_DEPRECATE
#ifdef _AFX_SECURE_NO_DEPRECATE
#define _AFX_INSECURE_DEPRECATE(_Message)
#else
#define _AFX_INSECURE_DEPRECATE(_Message) __declspec(deprecated(_Message))
#endif // _AFX_SECURE_NO_DEPRECATE
#endif // _AFX_INSECURE_DEPRECATE

// AFXAPI is used on global public functions
#ifndef AFXAPI
	#define AFXAPI __stdcall
#endif

// AFXOLEAPI is used for some special OLE functions
#ifndef AFXOLEAPI
	#define AFXOLEAPI __stdcall
#endif

// AFX_CDECL is used for rare functions taking variable arguments
#ifndef AFX_CDECL
	#define AFX_CDECL __cdecl
#endif

// AFX_EXPORT is used for functions which need to be exported
#ifndef AFX_EXPORT
	#define AFX_EXPORT EXPORT
#endif

#ifndef AFX_STATIC
	#define AFX_STATIC extern
	#define AFX_STATIC_DATA extern __declspec(selectany)
#endif

// The following macros are used to enable export/import

// for data
#ifndef AFX_DATA_EXPORT
	#define AFX_DATA_EXPORT __declspec(dllexport)
#endif
#ifndef AFX_DATA_IMPORT
	#define AFX_DATA_IMPORT __declspec(dllimport)
#endif

// for classes
#ifndef AFX_CLASS_EXPORT
	#define AFX_CLASS_EXPORT __declspec(dllexport)
#endif
#ifndef AFX_CLASS_IMPORT
	#define AFX_CLASS_IMPORT __declspec(dllimport)
#endif

// for global APIs
#ifndef AFX_API_EXPORT
	#define AFX_API_EXPORT __declspec(dllexport)
#endif
#ifndef AFX_API_IMPORT
	#define AFX_API_IMPORT __declspec(dllimport)
#endif

// This macro is used to reduce size requirements of some classes
#ifndef AFX_ALWAYS_VTABLE
#ifndef AFX_NOVTABLE
#if _MSC_VER >= 1100 && !defined(_DEBUG)
#define AFX_NOVTABLE __declspec(novtable)
#else
#define AFX_NOVTABLE
#endif
#endif
#endif

// for global data that should be in COMDATs (packaged data)
#ifndef AFX_COMDAT
#define AFX_COMDAT __declspec(selectany)
#endif

// The following macros are used on data declarations/definitions
//  (they are redefined for extension DLLs and the shared MFC DLL)
#define AFX_DATA
#define AFX_DATADEF
#define AFX_API

// used when building the "core" MFC80.DLL
#ifndef AFX_CORE_DATA
	#define AFX_CORE_DATA
	#define AFX_CORE_DATADEF
#endif

// used when building the MFC/OLE support MFCO80.DLL
#ifndef AFX_OLE_DATA
	#define AFX_OLE_DATA
	#define AFX_OLE_DATADEF
#endif

// used when building the MFC/DB support MFCD80.DLL
#ifndef AFX_DB_DATA
	#define AFX_DB_DATA
	#define AFX_DB_DATADEF
#endif

// used when building the MFC/NET support MFCN80.DLL
#ifndef AFX_NET_DATA
	#define AFX_NET_DATA
	#define AFX_NET_DATADEF
#endif

// used when building extension DLLs
#ifndef AFX_EXT_DATA
	#define AFX_EXT_DATA
	#define AFX_EXT_DATADEF
	#define AFX_EXT_CLASS
	#define AFX_EXT_API
#endif

// BASED_XXXX macros are provided for backward compatibility
#ifndef BASED_CODE
	#define BASED_CODE
#endif

#ifndef BASED_DEBUG
	#define BASED_DEBUG
#endif

#ifndef BASED_STACK
	#define BASED_STACK
#endif

/////////////////////////////////////////////////////////////////////////////
// $Log: $

#endif /* __MFCPORT_AFXVER__H__ */
