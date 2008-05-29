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
// Module: stdext/memory/ScopeAlloc.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2008-1-20 13:46:39
// 
// $Id: ScopeAlloc.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MEMORY_SCOPEALLOC_H
#define STDEXT_MEMORY_SCOPEALLOC_H

#error "outdated: move to http://svn.boost.org/svn/boost/sandbox/memory/boost/memory/scoped_alloc.hpp"

#ifndef STDEXT_MEMORY_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_MEMORY_AUTOFREEALLOC_H
#include "AutoFreeAlloc.h"
#endif

#ifndef STDEXT_THREAD_TLS_H
#include "../thread/TLS.h"
#endif

#ifndef _INC_LIMITS
#include <limits.h>
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class ProxyAlloc

template <class AllocT, class TlsAllocT = NullClass>
class ProxyAlloc
{
private:
	AllocT* m_alloc;

public:
	ProxyAlloc(AllocT& alloc) : m_alloc(&alloc) {}
#if !defined(STD_NO_SYSTEM_POOL)
	ProxyAlloc() : m_alloc(&TlsAllocT::instance()) {}
#endif

public:
	void* winx_call allocate(size_t cb)	{ return m_alloc->allocate(cb); }
	void winx_call deallocate(void* p)	{ m_alloc->deallocate(p); }
	void winx_call swap(ProxyAlloc& o)	{ std::swap(m_alloc, o.m_alloc); }
};

// -------------------------------------------------------------------------
// class BlockPool

template <class _Policy>
class BlockPoolT
{
private:
	typedef typename _Policy::allocator_type _Alloc;
	enum { m_cbBlock = _Policy::MemBlockSize };

	struct _Block {
		_Block* next;
	};
	_Block* m_freeList;

	int m_nFree;
	const int m_nFreeLimit;

private:
	BlockPoolT(const BlockPoolT&);
	void operator=(const BlockPoolT&);

public:
	BlockPoolT(int cbFreeLimit = INT_MAX)
		: m_freeList(NULL), m_nFree(0),
		  m_nFreeLimit(cbFreeLimit / m_cbBlock + 1)
	{
	}
	~BlockPoolT()
	{
		clear();
	}

public:
	void* winx_call allocate(size_t cb)
	{
		WINX_ASSERT(cb >= (size_t)m_cbBlock);

		if (cb > (size_t)m_cbBlock)
			return _Alloc::allocate(cb);
		else
		{
			if (m_freeList)
			{
				WINX_ASSERT(_Alloc::alloc_size(m_freeList) >= cb);
				_Block* blk = m_freeList;
				m_freeList = blk->next;
				--m_nFree;
				return blk;
			}
			return _Alloc::allocate(m_cbBlock);
		}
	}

	void winx_call deallocate(void* p)
	{
		if (m_nFree >= m_nFreeLimit) {
			_Alloc::deallocate(p);
		}
		else {
			_Block* blk = (_Block*)p;
			blk->next = m_freeList;
			m_freeList = blk;
			++m_nFree;
		}
	}

	void winx_call clear()
	{
		while (m_freeList)
		{
			_Block* blk = m_freeList;
			m_freeList = blk->next;
			_Alloc::deallocate(blk);
		}
		m_nFree = 0;
	}
};

typedef BlockPoolT<SysAlloc> BlockPool;

// -------------------------------------------------------------------------
// class TlsBlockPoolInit, TlsBlockPool

#if !defined(STD_NO_SYSTEM_POOL)

typedef TlsObject<BlockPool> _TlsBlockPool;

STDAPI_(_TlsBlockPool*) _stdext_TlsBlockPool();

template <class _Unused>
class TlsBlockPoolT
{
private:
	static _TlsBlockPool* _tls_blockPool;
	
public:
	TlsBlockPoolT() {
		_tls_blockPool->init();
	}
	~TlsBlockPoolT() {
		_tls_blockPool->term();
	}
	
	static void winx_call init()
	{
		_tls_blockPool->init();
	}
	
	static void winx_call term()
	{
		_tls_blockPool->term();
	}

	static bool winx_call isTlsBlockPoolInitialized()
	{
		return _tls_blockPool && _tls_blockPool->good();
	}

	static BlockPool& winx_call instance()
	{
		WINX_ASSERT( isTlsBlockPoolInitialized() );
		return _tls_blockPool->get();
	}
};

template <class _Unused>
_TlsBlockPool* TlsBlockPoolT<_Unused>::_tls_blockPool = _stdext_TlsBlockPool();

typedef TlsBlockPoolT<int> TlsBlockPool;
typedef TlsBlockPool TlsBlockPoolInit;

#else

typedef NullClass TlsBlockPool;

#endif

// -------------------------------------------------------------------------
// class ScopeAlloc

typedef ProxyAlloc<BlockPool, TlsBlockPool> ProxyBlockPool;

class PoolAlloc
{
public:
	enum { MemBlockSize = MEMORY_BLOCK_SIZE };
	typedef ProxyBlockPool allocator_type;
};

typedef RegionAllocT<PoolAlloc> ScopeAlloc;

// -------------------------------------------------------------------------
// class TestScopeAlloc

#if defined(STD_UNITTEST)

template <class LogT>
class TestScopeAlloc : public TestCase
{
	WINX_TEST_SUITE(TestScopeAlloc);
		WINX_TEST(testBasic);
		WINX_TEST(testTLS);
		WINX_TEST(testScope);
	WINX_TEST_SUITE_END();

public:
	class Obj
	{
	private:
		int m_val;
	public:
		Obj(int arg = 0) {
			m_val = arg;
			printf("construct Obj: %d\n", m_val);
		}
		~Obj() {
			printf("destruct Obj: %d\n", m_val);
		}
	};

	void testTLS(LogT& log)
	{
		std::ScopeAlloc alloc;
		int* a = STD_NEW(alloc, int);
	}

	void testBasic(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		alloc.allocate(1024);
		alloc.allocate(1024);
		alloc.allocate(23);
		alloc.allocate(2096);
	}

	void testScope(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		printf("\n------------------- global: have 3 objs ----------------\n");
		{
			Obj* a1 = STD_NEW(alloc, Obj)(0);
			Obj* a2 = STD_NEW_ARRAY(alloc, Obj, 2);
			printf("------------------- child 1: have 4 objs ----------------\n");
			{
				std::ScopeAlloc child1(alloc);
				Obj* o1 = STD_NEW(child1, Obj)(1);
				Obj* o2 = STD_NEW_ARRAY(child1, Obj, 3);
				printf("------------------- child 11: have 3 objs ----------------\n");
				{
					std::ScopeAlloc* child11 = STD_NEW(child1, std::ScopeAlloc)(child1);
					Obj* o11 = STD_NEW(*child11, Obj)(11);
					Obj* o12 = STD_NEW_ARRAY(*child11, Obj, 2);
				}
				printf("------------------- leave child 11 ----------------\n");
				printf("------------------- child 12: have 3 objs ----------------\n");
				{
					std::ScopeAlloc child12(child1);
					Obj* o11 = STD_NEW(child12, Obj)(12);
					Obj* o12 = STD_NEW_ARRAY(child12, Obj, 2);
				}
				printf("------------------- leave child 12 ----------------\n");
			}
			printf("------------------- leave child 1 ----------------\n");
			printf("------------------- child 2: have 4 objs ----------------\n");
			{
				std::ScopeAlloc child2(alloc);
				Obj* o1 = STD_NEW(child2, Obj)(2);
				Obj* o2 = STD_NEW_ARRAY(child2, Obj, 3);
			}
			printf("------------------- leave child 2 ----------------\n");
		}
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: $

__NS_STD_END

#endif /* STDEXT_MEMORY_SCOPEALLOC_H */
