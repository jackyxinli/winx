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
// Module: stdext/msvc/wtypes.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-5 13:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MSVC_WTYPES_H__
#define __STDEXT_MSVC_WTYPES_H__

#if !defined(STD_NO_WINSDK)
#error "Don't include <stdext/msvc/wtypes.h>"
#endif

#ifndef __STDEXT_MSVC_WINDEF_H__
#include "windef.h"
#endif

#ifndef __RPC_FAR
#define __RPC_FAR
#endif

// =========================================================================

typedef unsigned short WCHAR;

// =========================================================================

#if defined(_UNICODE)
typedef WCHAR TCHAR;
#else
typedef CHAR TCHAR;
#endif

// =========================================================================

typedef /* [string] */ WCHAR __RPC_FAR *LPWSTR;

typedef /* [string] */ TCHAR __RPC_FAR *LPTSTR;

typedef /* [string] */ const WCHAR __RPC_FAR *LPCWSTR;

typedef /* [string] */ const TCHAR __RPC_FAR *LPCTSTR;

// =========================================================================

typedef __int64 LONG64, *PLONG64;
typedef __int64 INT64, *PINT64;

typedef unsigned __int64 ULONG64, *PULONG64;
typedef unsigned __int64 DWORD64, *PDWORD64;
typedef unsigned __int64 UINT64, *PUINT64;

typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;

// =========================================================================

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    };
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER;

typedef union _ULARGE_INTEGER {
    struct {
        DWORD LowPart;
        DWORD HighPart;
    };
    struct {
        DWORD LowPart;
        DWORD HighPart;
    } u;
    ULONGLONG QuadPart;
} ULARGE_INTEGER;

typedef ULARGE_INTEGER *PULARGE_INTEGER;

// =========================================================================

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
typedef struct _GUID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} GUID;
#pragma pack()

typedef GUID IID;
typedef GUID CLSID;

#ifdef __cplusplus
} // extern "C"
#endif

// =========================================================================

typedef const IID& REFIID;
typedef const CLSID& REFCLSID;

// =========================================================================
// $Log: $

#endif /* __STDEXT_MSVC_WTYPES_H__ */
