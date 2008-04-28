//
//  boost/memory/scoped_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef __BOOST_MEMORY_SCOPED_ALLOC_HPP__
#define __BOOST_MEMORY_SCOPED_ALLOC_HPP__

#ifndef __BOOST_MEMORY_BASIC_HPP__
#include "basic.hpp"
#endif

#ifndef __BOOST_MEMORY_AUTO_ALLOC_HPP__
#include "auto_alloc.hpp"
#endif

#ifndef __BOOST_MEMORY_THREAD_TLS_HPP__
#include "thread/tls.hpp"
#endif

#if !defined(_CLIMITS_) && !defined(_CLIMITS)
#include <climits> // INT_MAX
#endif

__NS_BOOST_BEGIN

// -------------------------------------------------------------------------
// class proxy_alloc

template <class AllocT, class TlsAllocT>
class proxy_alloc
{
private:
	AllocT* m_alloc;

public:
	proxy_alloc(AllocT& alloc) : m_alloc(&alloc) {}
	proxy_alloc() : m_alloc(&TlsAllocT::instance()) {}

public:
	void* BOOST_MEMORY_CALL allocate(size_t cb)	{ return m_alloc->allocate(cb); }
	void BOOST_MEMORY_CALL deallocate(void* p)	{ m_alloc->deallocate(p); }
	void BOOST_MEMORY_CALL swap(proxy_alloc& o)	{ std::swap(m_alloc, o.m_alloc); }
};

// -------------------------------------------------------------------------
// class block_pool

template <class _Policy>
class block_pool_imp
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
	block_pool_imp(const block_pool_imp&);
	void operator=(const block_pool_imp&);

public:
	block_pool_imp(int cbFreeLimit = INT_MAX)
		: m_freeList(NULL), m_nFree(0),
		  m_nFreeLimit(cbFreeLimit / m_cbBlock + 1)
	{
	}
	~block_pool_imp()
	{
		clear();
	}

public:
	void* BOOST_MEMORY_CALL allocate(size_t cb)
	{
		BOOST_MEMORY_ASSERT(cb >= (size_t)m_cbBlock);

		if (cb > (size_t)m_cbBlock)
			return _Alloc::allocate(cb);
		else
		{
			if (m_freeList)
			{
				BOOST_MEMORY_ASSERT(_Alloc::alloc_size(m_freeList) >= cb);
				_Block* blk = m_freeList;
				m_freeList = blk->next;
				--m_nFree;
				return blk;
			}
			return _Alloc::allocate(m_cbBlock);
		}
	}

	void BOOST_MEMORY_CALL deallocate(void* p)
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

	void BOOST_MEMORY_CALL clear()
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

typedef block_pool_imp<_sys_alloc> block_pool;

// -------------------------------------------------------------------------
// class tls_block_pool

typedef tls_object<block_pool> tls_block_pool_t;

STDAPI_(tls_block_pool_t*) _boost_TlsBlockPool();

template <class _Unused>
class tls_block_pool_imp
{
private:
	static tls_block_pool_t* _tls_blockPool;
	
public:
	tls_block_pool_imp() {
		_tls_blockPool->init();
	}
	~tls_block_pool_imp() {
		_tls_blockPool->term();
	}

	static block_pool& BOOST_MEMORY_CALL instance()
	{
		return _tls_blockPool->get();
	}
};

template <class _Unused>
tls_block_pool_t* tls_block_pool_imp<_Unused>::_tls_blockPool = _boost_TlsBlockPool();

typedef tls_block_pool_imp<int> tls_block_pool;

// -------------------------------------------------------------------------
// class scoped_alloc

typedef proxy_alloc<block_pool, tls_block_pool> proxy_block_pool;

class _pool_alloc
{
public:
	enum { MemBlockSize = BOOST_MEMORY_BLOCK_SIZE };
	typedef proxy_block_pool allocator_type;
};

typedef gc_alloc_imp<_pool_alloc> scoped_alloc;

// -------------------------------------------------------------------------
// $Log: $

__NS_BOOST_END

#endif /* __BOOST_MEMORY_SCOPED_ALLOC_HPP__ */
