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
// Module: stdext/Memory.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 15:43:17
// 
// $Id: Memory.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_H__
#define __STDEXT_MEMORY_H__

// -------------------------------------------------------------------------

#ifndef __STDEXT_MEMORY_BASIC_H__
#include "memory/Basic.h"
#endif

#ifndef __STDEXT_MEMORY_AUTOFREEALLOC_H__
#include "memory/AutoFreeAlloc.h"
#endif

#ifndef __STDEXT_MEMORY_RECYCLEBUFFER_H__
#include "memory/RecycleBuffer.h"
#endif

#ifndef __STDEXT_MEMORY_SCOPEALLOC_H__
#include "memory/ScopeAlloc.h"
#endif

// -------------------------------------------------------------------------
// --> Memory leak checker - count-checker

__NS_STD_BEGIN

#pragma warning(disable:4996) // XXX  was declared deprecated

inline void _ReportCountLeak(unsigned nRef, const char* szClass, const char* szFile, int nLine)
{
	char szBuf[1024];
	int cch = 0;

	sprintf(szBuf, "%s(%d):", szFile, nLine);
	for (cch = strlen(szBuf); cch < 70; ++cch)
	{
		szBuf[cch] = ' ';
	}
	szBuf[cch] = '\0';
	OutputDebugStringA(szBuf);

	sprintf(szBuf, "%s (%d) is leaked\n", szClass, nRef);
	OutputDebugStringA(szBuf);
}

#pragma warning(default:4996) // XXX  was declared deprecated

class _CountChecker
{
public:
	_CountChecker(const char* szClass, const char* szFile, int nLine)
		: m_nRef(0), m_szClass(szClass), m_szFile(szFile), m_nLine(nLine) {}
	~_CountChecker()
		{ if (m_nRef > 0)
			_ReportCountLeak(m_nRef, m_szClass, m_szFile, m_nLine); }
	void __stdcall operator++()	{ ++m_nRef; }
	void __stdcall operator--()	{ --m_nRef; }
 
private:
	unsigned m_nRef;
	const char* m_szClass;
	const char* m_szFile;
	int m_nLine;
};

#if !defined(_DEBUG)
#	define WINX_DECLARE_COUNT(Class)
#else
#	define WINX_DECLARE_COUNT(Class)										\
	class _XCountChecker													\
	{																		\
	public:																	\
		_XCountChecker()							{ ++counter(); }		\
		_XCountChecker(const _XCountChecker& rhs)	{ ++counter(); }		\
		~_XCountChecker()							{ --counter(); }		\
	private:																\
		_CountChecker& __stdcall counter()									\
			{ static std::_CountChecker r(#Class, __FILE__, __LINE__);		\
			  return r; }													\
	} __cntchecker;
#endif

__NS_STD_END

// -------------------------------------------------------------------------
// class StlAlloc

__NS_STD_BEGIN

template <class _Ty, class _Alloc = ScopeAlloc>
class StlAlloc
{
private:
	_Alloc* m_alloc;

public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _Ty* pointer;
	typedef const _Ty* const_pointer;
	typedef _Ty& reference;
	typedef const _Ty& const_reference;
	typedef _Ty value_type;

    template <class U>
    struct rebind { typedef StlAlloc<U, _Alloc> other; };

public:
	pointer address(reference val) const
		{ return &val; }
	const_pointer address(const_reference val) const
		{ return &val; }

	size_type max_size() const
		{ size_type count = (size_type)(-1) / sizeof (_Ty);
		  return (0 < count ? count : 1); }

public:
	StlAlloc(_Alloc& alloc) : m_alloc(&alloc) {}

    template <class U>
	StlAlloc(const StlAlloc<U, _Alloc>& rhs) : m_alloc(rhs._Getalloc()) {}

	pointer allocate(size_type count, const void* = NULL)
		{ return (pointer)m_alloc->allocate(count * sizeof(_Ty)); }
	void deallocate(void* p, size_type cb)
		{ m_alloc->deallocate(p, cb); }
	void construct(pointer p, const _Ty& val)
		{ new(p) _Ty(val); }
	void destroy(pointer p)
		{ p->~_Ty(); }

public:
	char* _Charalloc(size_type cb)
		{ return (char*)m_alloc->allocate(cb); }

	_Alloc* _Getalloc() const { return m_alloc; }
};

template<> class StlAlloc<void, ScopeAlloc>
{
    typedef void        value_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
 
    template <class U>
    struct rebind { typedef StlAlloc<U, ScopeAlloc> other; };
};

template<> class StlAlloc<void, AutoFreeAlloc>
{
    typedef void        value_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
 
    template <class U>
    struct rebind { typedef StlAlloc<U, ScopeAlloc> other; };
};

template <class _Ty, class _Alloc>
inline bool operator==(const StlAlloc<_Ty, _Alloc>&,
                       const StlAlloc<_Ty, _Alloc>&) {
    return true;
}

template <class _Ty, class _Alloc>
inline bool operator!=(const StlAlloc<_Ty, _Alloc>&,
                       const StlAlloc<_Ty, _Alloc>&) {
    return false;
}

__NS_STD_END

// -------------------------------------------------------------------------
// $Log: Memory.h,v $

#endif /* __STDEXT_MEMORY_H__ */
