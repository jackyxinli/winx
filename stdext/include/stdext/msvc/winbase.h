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
// Module: stdext/msvc/winbase.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: Basic.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MSVC_WINBASE_H__
#define __STDEXT_MSVC_WINBASE_H__

#ifndef STD_NO_WINSDK
#error "Don't include <stdext/msvc/winbase.h>"
#endif

#ifndef __STDEXT_MSVC_WTYPES_H__
#include "wtypes.h"
#endif

#define WINBASEAPI inline

// -------------------------------------------------------------------------

WINBASEAPI BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER* lp) //@@todo
{
    return TRUE;
}

WINBASEAPI BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER* lp)
{
    return TRUE;
}

// -------------------------------------------------------------------------

WINBASEAPI HANDLE WINAPI HeapCreate(
	DWORD flOptions,
    DWORD dwInitialSize,
    DWORD dwMaximumSize) //@@todo
{
	return NULL;
}

WINBASEAPI BOOL WINAPI HeapDestroy(
    HANDLE hHeap)
{
	return TRUE;
}


WINBASEAPI LPVOID WINAPI HeapAlloc(
    HANDLE hHeap,
    DWORD dwFlags,
    DWORD dwBytes)
{
	return NULL;
}

WINBASEAPI LPVOID WINAPI HeapReAlloc(
    HANDLE hHeap,
    DWORD dwFlags,
    LPVOID lpMem,
    DWORD dwBytes)
{
	return NULL;
}

WINBASEAPI BOOL WINAPI HeapFree(
    HANDLE hHeap,
    DWORD dwFlags,
    LPVOID lpMem)
{
	return TRUE;
}

WINBASEAPI DWORD WINAPI HeapSize(
    HANDLE hHeap,
    DWORD dwFlags,
    LPCVOID lpMem)
{
	return 0;
}

WINBASEAPI HANDLE WINAPI GetProcessHeap()
{
	return NULL;
}

// -------------------------------------------------------------------------

WINBASEAPI LONG WINAPI InterlockedIncrement(
    LPLONG lpAddend)
{
	return *lpAddend;
}

WINBASEAPI LONG WINAPI InterlockedDecrement(
    LPLONG lpAddend)
{
	return *lpAddend;
}

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// $Log: $

#endif /* __STDEXT_MSVC_WINBASE_H__ */
