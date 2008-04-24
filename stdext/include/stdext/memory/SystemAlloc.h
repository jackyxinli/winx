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
// Module: stdext/memory/SystemAlloc.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2008-1-20 13:46:39
// 
// $Id: SystemAlloc.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_SYSTEMALLOC_H__
#define __STDEXT_MEMORY_SYSTEMALLOC_H__

#ifndef __STDEXT_MEMORY_BASIC_H__
#include "Basic.h"
#endif

#ifndef __STDEXT_THREADMODEL_H__
#include "../ThreadModel.h"
#endif

#if defined(STD_UNITTEST)
#ifndef __STDEXT_COUNTER_H__
#include "../Counter.h"
#endif
#endif

#ifndef __STD_ALGORITHM_H__
#include "../../std/algorithm.h" // std::swap
#endif

#ifndef _INC_STDLIB
#include <stdlib.h> // malloc,free
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// __STD_FAKE_DBG_ALLOCATE

#if defined(_DEBUG)

#define __STD_FAKE_DBG_ALLOCATE()														\
	void* winx_call allocate(size_t cb, LPCSTR szFile, int nLine)						\
		{ return allocate(cb); }														\
	void* winx_call allocate(size_t cb, DestructorType fn, LPCSTR szFile, int nLine)	\
		{ return allocate(cb, fn); }													\
	void* winx_call allocate(size_t cb, int fnZero, LPCSTR szFile, int nLine)			\
		{ return allocate(cb); }														\
	void* winx_call reallocate(void* p, size_t oldSize, size_t newSize,					\
		LPCSTR szFile, int nLine) { return reallocate(p, oldSize, newSize); }			\
	template <class Type>																\
	Type* winx_call newArray(size_t count, Type* zero, LPCSTR szFile, int nLine)		\
		{ return newArray(count, zero); }

#else

#define __STD_FAKE_DBG_ALLOCATE()

#endif

// -------------------------------------------------------------------------
// StdLibAlloc

class StdLibAlloc
{
public:
	static void* winx_call allocate(size_t cb)						{ return malloc(cb); }
	static void* winx_call allocate(size_t cb, DestructorType fn)	{ return malloc(cb); }
	static void* winx_call allocate(size_t cb, int fnZero)			{ return malloc(cb); }

	static void* winx_call reallocate(void* p, size_t oldSize, size_t newSize) {
		return realloc(p, newSize);
	}

	static void winx_call deallocate(void* p)			{ free(p); }
	static void winx_call deallocate(void* p, size_t)	{ free(p); }
	static void winx_call swap(StdLibAlloc& o)			{}

	static size_t winx_call alloc_size(void* p)
	{
		return _msize(p);
	}

	template <class Type>
	static void winx_call destroy(Type* obj)
	{
		obj->~Type();
		free(obj);
	}

	template <class Type>
	static Type* winx_call newArray(size_t count, Type* zero)
	{
		Type* array = (Type*)malloc(sizeof(Type) * count);
		return ConstructorTraits<Type>::constructArray(array, count);
	}

	template <class Type>
	static void winx_call destroyArray(Type* array, size_t count)
	{
		DestructorTraits<Type>::destructArrayN(array, count);
		free(array);
	}

#if defined(_DEBUG)
	static void* winx_call allocate(size_t cb, LPCSTR szFile, int nLine)
		{ return _malloc_dbg(cb, _NORMAL_BLOCK, szFile, nLine); }

	static void* winx_call allocate(size_t cb, DestructorType fn, LPCSTR szFile, int nLine)
		{ return _malloc_dbg(cb, _NORMAL_BLOCK, szFile, nLine); }
	
	static void* winx_call allocate(size_t cb, int fnZero, LPCSTR szFile, int nLine)
		{ return _malloc_dbg(cb, _NORMAL_BLOCK, szFile, nLine); }

	static void* winx_call reallocate(void* p, size_t oldSize, size_t newSize, LPCSTR szFile, int nLine)
		{ return _realloc_dbg(p, newSize, _NORMAL_BLOCK, szFile, nLine); }

	template <class Type>
	static Type* winx_call newArray(size_t count, Type* zero, LPCSTR szFile, int nLine)
	{
		Type* array = (Type*)_malloc_dbg(sizeof(Type) * count, _NORMAL_BLOCK, szFile, nLine);
		return ConstructorTraits<Type>::constructArray(array, count);
	}
#endif
};

// -------------------------------------------------------------------------
// CoTaskAlloc

#if !defined(STD_NO_WINSDK)

class CoTaskAlloc
{
public:
	static void* winx_call allocate(size_t cb)						{ return CoTaskMemAlloc(cb); }
	static void* winx_call allocate(size_t cb, DestructorType fn)	{ return CoTaskMemAlloc(cb); }
	static void* winx_call allocate(size_t cb, int fnZero)			{ return CoTaskMemAlloc(cb); }
	
	static void* winx_call reallocate(void* p, size_t oldSize, size_t newSize) {
		return CoTaskMemRealloc(p, newSize);
	}

	static void winx_call deallocate(void* p)			{ CoTaskMemFree(p); }
	static void winx_call deallocate(void* p, size_t)	{ CoTaskMemFree(p); }
	static void winx_call swap(CoTaskAlloc& o)			{}

	static size_t winx_call alloc_size(void* p)
	{
		ULONG cbSize;
		IMalloc* pMalloc;
		if (CoGetMalloc(0, &pMalloc) == S_OK) {
			cbSize = pMalloc->GetSize(p);
			pMalloc->Release();
		}
		else {
			cbSize = 0;
		}
		return cbSize;
	}
	
	template <class Type>
	static void winx_call destroy(Type* obj)
	{
		obj->~Type();
		CoTaskMemFree(obj);
	}

	template <class Type>
	static Type* winx_call newArray(size_t count, Type* zero)
	{
		Type* array = (Type*)CoTaskMemAlloc(sizeof(Type) * count);
		return ConstructorTraits<Type>::constructArray(array, count);
	}

	template <class Type>
	static void winx_call destroyArray(Type* array, size_t count)
	{
		DestructorTraits<Type>::destructArrayN(array, count);
		CoTaskMemFree(array);
	}

	__STD_FAKE_DBG_ALLOCATE();
};

#endif

// -------------------------------------------------------------------------
// HeapMemAllocBase, HeapMemAlloc

template <DWORD uFlags = HEAP_GENERATE_EXCEPTIONS>
class HeapMemAllocBase
{
public:
	HANDLE m_hHeap;

public:
	HeapMemAllocBase(HANDLE hHeap) : m_hHeap(hHeap) {}

	void* winx_call allocate(size_t cb)					   { return HeapAlloc(m_hHeap, uFlags, cb); }
	void* winx_call allocate(size_t cb, DestructorType fn) { return HeapAlloc(m_hHeap, uFlags, cb); }
	void* winx_call allocate(size_t cb, int fnZero)		   { return HeapAlloc(m_hHeap, uFlags, cb); }
	
	void* winx_call reallocate(void* p, size_t oldSize, size_t newSize) {
		return HeapReAlloc(m_hHeap, uFlags, p, newSize);
	}

	void winx_call deallocate(void* p)					{ HeapFree(m_hHeap, uFlags, p); }
	void winx_call deallocate(void* p, size_t)			{ HeapFree(m_hHeap, uFlags, p); }	
	void winx_call swap(HeapMemAllocBase& o)			{ std::swap(m_hHeap, o.m_hHeap); }
	
	size_t winx_call alloc_size(void* p)
	{
		return HeapSize(m_hHeap, uFlags, p);
	}

	template <class Type>
	void winx_call destroy(Type* obj)
	{
		obj->~Type();
		HeapFree(m_hHeap, uFlags, obj);
	}

	template <class Type>
	Type* winx_call newArray(size_t count, Type* zero)
	{
		Type* array = (Type*)HeapAlloc(m_hHeap, uFlags, sizeof(Type) * count);
		return ConstructorTraits<Type>::constructArray(array, count);
	}

	template <class Type>
	void winx_call destroyArray(Type* array, size_t count)
	{
		DestructorTraits<Type>::destructArrayN(array, count);
		HeapFree(m_hHeap, uFlags, array);
	}

	__STD_FAKE_DBG_ALLOCATE();
};

template <DWORD uFlags>
class HeapAllocT
{
public:
	static HANDLE hProcessHeap;

	static void* winx_call allocate(size_t cb) {
		WINX_ASSERT_INITIALIZED(hProcessHeap);
		return HeapAlloc(hProcessHeap, uFlags, cb);
	}

	static void* winx_call allocate(size_t cb, DestructorType fn) {
		WINX_ASSERT_INITIALIZED(hProcessHeap);
		return HeapAlloc(hProcessHeap, uFlags, cb);
	}

	static void* winx_call allocate(size_t cb, int fnZero) {
		WINX_ASSERT_INITIALIZED(hProcessHeap);
		return HeapAlloc(hProcessHeap, uFlags, cb);
	}
	
	static void* winx_call reallocate(void* p, size_t oldSize, size_t newSize) {
		return HeapReAlloc(hProcessHeap, uFlags, p, newSize);
	}

	static void winx_call deallocate(void* p) { HeapFree(hProcessHeap, uFlags, p); }
	static void winx_call deallocate(void* p, size_t) { HeapFree(hProcessHeap, uFlags, p); }
	static void winx_call swap(HeapAllocT& o) {}
	
	static size_t winx_call alloc_size(void* p)
	{
		return HeapSize(hProcessHeap, uFlags, p);
	}

	template <class Type>
	static void winx_call destroy(Type* obj)
	{
		obj->~Type();
		HeapFree(hProcessHeap, uFlags, obj);
	}

	template <class Type>
	static Type* winx_call newArray(size_t count, Type* zero)
	{
		Type* array = (Type*)HeapAlloc(hProcessHeap, uFlags, sizeof(Type) * count);
		return ConstructorTraits<Type>::constructArray(array, count);
	}

	template <class Type>
	static void winx_call destroyArray(Type* array, size_t count)
	{
		DestructorTraits<Type>::destructArrayN(array, count);
		HeapFree(hProcessHeap, uFlags, array);
	}

	__STD_FAKE_DBG_ALLOCATE();
};

template <DWORD uFlags>
HANDLE HeapAllocT<uFlags>::hProcessHeap = ::GetProcessHeap();

typedef HeapAllocT<0> HeapMemAlloc;

// -------------------------------------------------------------------------
// class DefaultStaticAlloc

typedef StdLibAlloc DefaultStaticAlloc;

// -------------------------------------------------------------------------
// class SystemPoolT

template <class _Policy>
class SystemPoolT
{
private:
	typedef typename _Policy::threadmodel_type ThreadModel;
	typedef typename _Policy::allocator_type _Alloc;
	enum { cbBlock = _Policy::MemBlockSize };

	struct _Block {
		_Block* next;
	};
	_Block* m_freeList;

private:
	typedef typename ThreadModel::CS CS;
	typedef typename ThreadModel::CSLock CSLock;

	CS m_cs;

private:
	SystemPoolT(const SystemPoolT&);
	void operator=(const SystemPoolT&);

public:
	SystemPoolT()
		: m_freeList(NULL)
	{
	}
	~SystemPoolT()
	{
		clear();
	}

public:
	void* winx_call allocate(size_t cb)
	{
		WINX_ASSERT(m_cs.good());
		WINX_ASSERT(cb >= (size_t)cbBlock);

		if (cb > (size_t)cbBlock)
			return _Alloc::allocate(cb);
		{
			CSLock aLock(m_cs);
			if (m_freeList)
			{
				WINX_ASSERT(_Alloc::alloc_size(m_freeList) >= cb);
				_Block* blk = m_freeList;
				m_freeList = blk->next;
				return blk;
			}
		}
		return _Alloc::allocate(cbBlock);
	}

	void winx_call deallocate(void* p)
	{
		_Block* blk = (_Block*)p;
		CSLock aLock(m_cs);
		blk->next = m_freeList;
		m_freeList = blk;
	}

	static size_t winx_call alloc_size(void* p)
	{
		return _Alloc::alloc_size(p);
	}

	void winx_call clear()
	{
		CSLock aLock(m_cs);
		while (m_freeList)
		{
			_Block* blk = m_freeList;
			m_freeList = blk->next;
			_Alloc::deallocate(blk);
		}
	}
};

template <class _Policy>
class SystemPoolS
{
private:
	typedef SystemPoolT<_Policy> _SysPool;
	static _SysPool s_impl;

public:
	static void* winx_call allocate(size_t cb) { return s_impl.allocate(cb); }
	static void winx_call deallocate(void* p) { s_impl.deallocate(p); }
	static size_t winx_call alloc_size(void* p) {
		return s_impl.alloc_size(p);
	}
};

template <class _Policy>
SystemPoolT<_Policy> SystemPoolS<_Policy>::s_impl;

class _SysPolicy
{
public:
	enum { MemBlockSize = MEMORY_BLOCK_SIZE };
	typedef DefaultStaticAlloc allocator_type;
	typedef DefaultThreadModel threadmodel_type;
};

typedef SystemPoolS<_SysPolicy> SystemPool;

// -------------------------------------------------------------------------
// class DynSystemPool

STDAPI_(void*) _stdext_SysPoolAlloc(size_t cb);
STDAPI_(void) _stdext_SysPoolFree(void* p);
STDAPI_(size_t) _stdext_SysPoolSize(void* p);

class DynSystemPool
{
public:
	static void* winx_call allocate(size_t cb) { return _stdext_SysPoolAlloc(cb); }
	static void winx_call deallocate(void* p) { _stdext_SysPoolFree(p); }
	static size_t winx_call alloc_size(void* p) {
		return _stdext_SysPoolSize(p);
	}
};

// -------------------------------------------------------------------------
// class SystemAlloc

#if (0)
#define STD_NO_SYSTEM_POOL
#define STD_DYN_SYSTEM_POOL
#define STD_STATIC_SYSTEM_POOL
#endif

#if defined(STD_STATIC_SYSTEM_POOL)
#define STD_NO_SYSTEM_POOL
#endif

#if defined(_WIN32) && !defined(STD_NO_SYSTEM_POOL)
#define STD_DYN_SYSTEM_POOL
#endif

#if !defined(_WIN32) // non-windows default
typedef StdLibAlloc SystemAlloc;
#elif defined(STD_DYN_SYSTEM_POOL)	// recommended
typedef DynSystemPool SystemAlloc;
#elif defined(STD_STATIC_SYSTEM_POOL) // not recommended
typedef SystemPool SystemAlloc;
#else // no system-pool
typedef HeapMemAlloc SystemAlloc;
#endif

// -------------------------------------------------------------------------
// class TestSystemAlloc

#if defined(STD_UNITTEST)

template <class LogT>
class TestSystemAlloc : public TestCase
{
	WINX_TEST_SUITE(TestSystemAlloc);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	template <class AllocT>
	void doTestAlloc(LogT& log, AllocT& alloc)
	{
		const int Total = 5000;
		void** p = new void*[Total];

		std::Accumulator acc;
		for (int i, j = 0; j < 16; ++j)
		{
			std::PerformanceCounter counter;
			{
				for (i = 0; i < Total; ++i)
				{
					p[i] = alloc.allocate(MEMORY_BLOCK_SIZE);
				}
				for (i = 0; i < Total; ++i)
				{
					alloc.deallocate(p[i]);
				}
			}
			acc.accumulate(counter.trace(log));
		}
		acc.trace_avg(log);

		delete[] p;
	}

	void test(LogT& log)
	{
		std::SystemPool sysPool;
		std::StdLibAlloc stdLib;
		std::HeapMemAlloc heapMem;
#if !defined(STD_NO_WINSDK)
		std::CoTaskAlloc cotask;
#endif

		log.print("\n===== StdLibAlloc =====\n");
		doTestAlloc(log, stdLib);

#if !defined(STD_NO_WINSDK)
		log.print("\n===== CoTaskAlloc =====\n");
		doTestAlloc(log, cotask);
#endif

		log.print("\n===== HeapMemAlloc =====\n");
		doTestAlloc(log, heapMem);

		log.print("\n===== SysPoolAlloc =====\n");
		doTestAlloc(log, sysPool);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: $

__NS_STD_END

#endif /* __STDEXT_MEMORY_SYSTEMALLOC_H__ */
