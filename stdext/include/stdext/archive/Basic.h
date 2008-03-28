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
// Module: stdext/archive/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: Basic.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_BASIC_H__
#define __STDEXT_ARCHIVE_BASIC_H__

#if (0)
#define BYTESWAP
#define __BYTESWAP__
#endif

#ifndef __STDEXT_MEMORY_SCOPEALLOC_H__
#include "../memory/ScopeAlloc.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// toInt, toUInt

__forceinline int  winx_call toInt(int li)		{ return li; }
__forceinline UINT winx_call toUInt(UINT li)	{ return li; }

// -------------------------------------------------------------------------
// copyMemory

__forceinline void winx_call copyMemory(char* dest, const char* src, size_t n)
{
	memcpy(dest, src, n);
}

__forceinline void winx_call copyMemory(WCHAR* dest, const WCHAR* src, size_t n)
{
	wmemcpy(dest, src, n);
}

// -------------------------------------------------------------------------
// isStgWritable

inline BOOL winx_call isStgWritable(IStorage* pstg)
{
	STATSTG statstg;
	pstg->Stat(&statstg, STATFLAG_NONAME);
	return statstg.grfMode & (STGM_READWRITE|STGM_WRITE);
}

inline BOOL winx_call isStgWritable(IStream* pstm)
{
	STATSTG statstg;
	pstm->Stat(&statstg, STATFLAG_NONAME);
	return statstg.grfMode & (STGM_READWRITE|STGM_WRITE);
}

// -------------------------------------------------------------------------
// class IoException

class IoException : public std::exception
{
private:
	HRESULT m_scode;

public:
	IoException(HRESULT sc, const char* errMsg = "Io Exception")
		: m_scode(sc), std::exception(errMsg) {
	}
	
	HRESULT winx_call getError() const {
		return m_scode;
	}
};

// -------------------------------------------------------------------------
// class ArchiveCache

#ifndef ARCHIVE_CACHE_SIZE
#define ARCHIVE_CACHE_SIZE	4096
#endif

class ArchiveCache
{
public:
	typedef size_t size_type;

private:
	ScopeAlloc& m_alloc;
	const size_type m_item_size;

public:
	ArchiveCache(ScopeAlloc& alloc, size_type cb = ARCHIVE_CACHE_SIZE)
		: m_alloc(alloc), m_item_size(cb)
	{
	}

	void* winx_call item_alloc()
	{
		return m_alloc.allocate(m_item_size);
	}

	void winx_call item_free(void* p)
	{
		m_alloc.deallocate(p, m_item_size);	
	}

	size_type winx_call alloc_size() const
	{
		return m_item_size;
	}
};

// -------------------------------------------------------------------------
// WINX_BYTESWAP

#if defined(BYTESWAP) || defined(__BYTESWAP__)
#define WINX_BYTESWAP
#endif

#if !defined(WINX_BYTESWAP)
	#define _WinxByteSwap(val)
	#define _WinxByteSwap16(val)
	#define _WinxByteSwap32(val)
	#define _WinxByteSwapStruct(val)
	#define _WinxByteSwap16Array(val)
	#define _WinxByteSwap32Array(val)
	#define _WinxByteSwapStructArray(val)
#else
	#error "todo: BYTESWAP support"
#endif

// -------------------------------------------------------------------------
// $Log: Basic.h,v $

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_BASIC_H__ */
