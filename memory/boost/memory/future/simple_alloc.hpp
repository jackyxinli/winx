//
//  boost/memory/simple_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_SIMPLE_ALLOC_HPP
#define BOOST_MEMORY_SIMPLE_ALLOC_HPP

#ifndef BOOST_MEMORY_BASIC_HPP
#include "basic.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// =========================================================================
// class simple_alloc

template <class SysAllocT>
class simple_alloc
{
#pragma pack(1)
private:
	struct DestroyNode
	{
		DestroyNode* pPrev;
		destructor_t fnDestroy;
	};
	struct MemBlock
	{
		MemBlock* pPrev;
	};
#pragma pack()

	MemBlock* m_memChain;
	DestroyNode* m_destroyChain;

public:
	simple_alloc()
		: m_memChain(NULL), m_destroyChain(NULL)
	{
	}

	void BOOST_MEMORY_CALL swap(simple_alloc& o)
	{
		std::swap(m_memChain, o.m_memChain);
		std::swap(m_destroyChain, o.m_destroyChain);
	}

	void BOOST_MEMORY_CALL clear()
	{
		while (m_destroyChain)
		{
			DestroyNode* curr = m_destroyChain;
			m_destroyChain = m_destroyChain->pPrev;
			curr->fnDestroy(curr + 1);
		}
		while (m_memChain)
		{
			MemBlock* curr = m_memChain;
			m_memChain = m_memChain->pPrev;
			SysAllocT::deallocate(curr);
		}
	}

	void* BOOST_MEMORY_CALL allocate(size_t cb)
	{
		MemBlock* p = (MemBlock*)SysAllocT::allocate(cb + sizeof(MemBlock));
		p->pPrev = m_memChain;
		m_memChain = p;
		return p + 1;
	}

#if defined(BOOST_MEMORY_NO_STRICT_EXCEPTION_SEMANTICS)
	void* BOOST_MEMORY_CALL allocate(size_t cb, destructor_t fn)
	{
		DestroyNode* pNode = (DestroyNode*)allocate(cb + sizeof(DestroyNode));
		pNode->fnDestroy = fn;
		pNode->pPrev = m_destroyChain;
		m_destroyChain = pNode;
		return pNode + 1;
	}

	void* BOOST_MEMORY_CALL allocate(size_t cb, int fnZero)
	{
		return allocate(cb);
	}
#endif
	
	void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, destructor_t fn)
	{
		DestroyNode* pNode = (DestroyNode*)allocate(cb + sizeof(DestroyNode));
		pNode->fnDestroy = fn;
		return pNode + 1;
	}
	
	void BOOST_MEMORY_CALL manage(void* p, destructor_t fn)
	{
		DestroyNode* pNode = (DestroyNode*)p - 1;
		BOOST_MEMORY_ASSERT(pNode->fnDestroy == fn);
		
		pNode->pPrev = m_destroyChain;
		m_destroyChain = pNode;
	}

	void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, int fnZero)
	{
		return allocate(cb);
	}
	
	void BOOST_MEMORY_CALL manage(void* p, int fnZero)
	{
		// no action
	}

	void BOOST_MEMORY_CALL deallocate(void* p, size_t cb)
	{
		// no action
	}

	template <class Type>
	void BOOST_MEMORY_CALL destroy(Type* obj)
	{
		// no action
	}

	template <class Type>
	void BOOST_MEMORY_CALL destroyArray(Type* array, size_t count)
	{
		// no action
	}
};

// =========================================================================
// $Log: policy.hpp,v $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_SIMPLE_ALLOC_HPP */

