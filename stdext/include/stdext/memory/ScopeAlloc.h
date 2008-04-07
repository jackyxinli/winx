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
// $Id: RecycleBuffer.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_SCOPEALLOC_H__
#define __STDEXT_MEMORY_SCOPEALLOC_H__

#ifndef __STDEXT_MEMORY_BASIC_H__
#include "Basic.h"
#endif

#ifndef __STDEXT_MEMORY_AUTOFREEALLOC_H__
#include "AutoFreeAlloc.h"
#endif

#ifndef __STDEXT_THREADMODEL_H__
#include "../ThreadModel.h"
#endif

#ifndef _INC_LIMITS
#include <limits.h>
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class ProxyAlloc

template <class AllocT>
class ProxyAlloc
{
private:
	AllocT* m_alloc;

public:
	ProxyAlloc(AllocT& alloc) : m_alloc(&alloc) {}

public:
	void* winx_call allocate(size_t cb)	{ return m_alloc->allocate(cb); }
	void winx_call deallocate(void* p)	{ m_alloc->deallocate(p); }
	void winx_call swap(ProxyAlloc& o)	{ std::swap(m_alloc, o.m_alloc); }
};

// -------------------------------------------------------------------------
// class BlockPoolT

template <class ThreadModel>
class BlockPoolT
{
private:
	struct _Block {
		_Block* next;
	};
	_Block* m_freeList;

	int m_nFree;
	const int m_nFreeLimit;
	const int m_cbBlock;

private:
	typedef typename ThreadModel::CS CS;
	typedef typename ThreadModel::CSLock CSLock;

	CS m_cs;

private:
	BlockPoolT(const BlockPoolT&);
	void operator=(const BlockPoolT&);

public:
	BlockPoolT(int cbFreeLimit = INT_MAX, int cbBlock = MEMORY_BLOCK_SIZE)
		: m_freeList(NULL), m_cbBlock(cbBlock), m_nFree(0),
		  m_nFreeLimit(cbFreeLimit / cbBlock + 1)
	{
	}
	~BlockPoolT()
	{
		clear();
	}

public:
	void* winx_call allocate(size_t cb)
	{
		MEMORY_ASSERT(cb >= (size_t)m_cbBlock);

		if (cb > (size_t)m_cbBlock)
			return malloc(cb);
		else
		{
			CSLock aLock(m_cs);
			if (m_freeList)
			{
				MEMORY_ASSERT(_msize(m_freeList) >= cb);
				_Block* blk = m_freeList;
				m_freeList = blk->next;
				--m_nFree;
				return blk;
			}
			return malloc(m_cbBlock);
		}
	}

	void winx_call deallocate(void* p)
	{
		CSLock aLock(m_cs);
		if (m_nFree >= m_nFreeLimit) {
			free(p);
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
		CSLock aLock(m_cs);
		while (m_freeList)
		{
			_Block* blk = m_freeList;
			m_freeList = blk->next;
			free(blk);
		}
		m_nFree = 0;
	}
};

// -------------------------------------------------------------------------
// class ScopeAlloc

typedef BlockPoolT<SingleThreadModel> BlockPoolST;
typedef ProxyAlloc<BlockPoolST> ProxyBlockPoolST;

typedef BlockPoolT<MultiThreadModel> BlockPoolMT;
typedef ProxyAlloc<BlockPoolMT> ProxyBlockPoolMT;

class ProxyST : public StdAlloc
{
public:
	typedef ProxyBlockPoolST allocator_type;
};

class ProxyMT : public StdAlloc
{
public:
	typedef ProxyBlockPoolMT allocator_type;
};

typedef AutoFreeAllocT<ProxyST> ScopeAllocST;
typedef AutoFreeAllocT<ProxyMT> ScopeAllocMT;

#if defined(_MT)
typedef BlockPoolMT BlockPool;
typedef ProxyBlockPoolMT ProxyBlockPool;
typedef ScopeAllocMT ScopeAlloc;
#else
typedef BlockPoolST BlockPool;
typedef ProxyBlockPoolST ProxyBlockPool;
typedef ScopeAllocST ScopeAlloc;
#endif

// -------------------------------------------------------------------------
// class TestScopeAlloc

#if defined(STD_UNITTEST)

template <class LogT>
class TestScopeAlloc : public TestCase
{
	WINX_TEST_SUITE(TestScopeAlloc);
		WINX_TEST(testBasic);
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

#endif /* __STDEXT_MEMORY_SCOPEALLOC_H__ */
