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

#if defined(_WIN32)
#error "Don't include <stdext/msvc/wtypes.h>"
#endif

// =========================================================================

typedef const char* LPCSTR;

// =========================================================================

extern "C" {

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

extern IID IID_IUnknown;
extern IID IID_IClassFactory;
extern IID IID_IMalloc;
extern IID IID_IMallocSpy;
extern IID IID_ILockBytes;
extern IID IID_ISequentialStream;
extern IID IID_IStream;
extern IID IID_IStorage;
extern IID IID_IPersistStorage;
extern IID IID_IEnumSTATSTG;
extern IID IID_IDispatch;
extern IID IID_ITypeLib;
extern IID IID_ITypeLib2;
extern IID IID_ICreateErrorInfo;
extern IID IID_IErrorInfo;

} // extern "C"

typedef const IID& REFIID;
typedef const CLSID& REFCLSID;

// =========================================================================
// $Log: $

#endif /* __STDEXT_MSVC_WTYPES_H__ */
