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
#ifndef STDEXT_MEMORY_H
#define STDEXT_MEMORY_H

// -------------------------------------------------------------------------

#if (0)

#ifndef STDEXT_MEMORY_BASIC_H
#include "memory/Basic.h"
#endif

#ifndef STDEXT_MEMORY_AUTOFREEALLOC_H
#include "memory/AutoFreeAlloc.h"
#endif

#ifndef STDEXT_MEMORY_RECYCLEBUFFER_H
#include "memory/RecycleBuffer.h"
#endif

#ifndef STDEXT_MEMORY_SCOPEALLOC_H
#include "memory/ScopeAlloc.h"
#endif

class GcAlloc;
class StlAlloc;

#else

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_BOOST_MEMORY_H
#include "boost/Memory.h"
#endif

NS_STD_BEGIN

typedef system_alloc SystemAlloc;
typedef stdlib_alloc DefaultStaticAlloc;

typedef block_pool BlockPool;
typedef tls_block_pool TlsBlockPool;
typedef tls_block_pool TlsBlockPoolInit;

typedef auto_alloc AutoFreeAlloc;
typedef scoped_alloc ScopeAlloc;
typedef scoped_alloc ScopedAlloc;
typedef gc_alloc GcAlloc;
typedef tls_gc_alloc TlsGcAlloc;

#define RegionAllocT region_alloc
#define DestructorTraits destructor_traits
#define StlAlloc stl_allocator

NS_STD_END

#define STD_NO_CONSTRUCTOR(Type)				BOOST_MEMORY_NO_CONSTRUCTOR(Type)
#define STD_NO_DESTRUCTOR(Type)					BOOST_MEMORY_NO_DESTRUCTOR(Type)

#define STD_NEW(alloc, Type)					BOOST_MEMORY_NEW(alloc, Type)
#define STD_NEW_ARRAY(alloc, Type, count)		BOOST_MEMORY_NEW_ARRAY(alloc, Type, count)

#define STD_ALLOC(alloc, Type)					BOOST_MEMORY_ALLOC(alloc, Type)
#define STD_ALLOC_ARRAY(alloc, Type, count)		BOOST_MEMORY_ALLOC_ARRAY(alloc, Type, count)

#endif

// -------------------------------------------------------------------------
// --> Memory leak checker - count-checker

#ifndef STDEXT_WINAPI_WINBASE_H
#include "winapi/winbase.h"
#endif

NS_STD_BEGIN

#pragma warning(disable:4996) // XXX  was declared deprecated

inline void _ReportCountLeak(unsigned nRef, const char* szClass, const char* szFile, int nLine)
{
	char szBuf[1024];
	size_t cch = 0;

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

NS_STD_END

// -------------------------------------------------------------------------
// $Log: Memory.h,v $

#endif /* STDEXT_MEMORY_H */
