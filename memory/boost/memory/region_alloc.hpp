//
//  boost/memory/region_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_REGION_ALLOC_HPP
#define BOOST_MEMORY_REGION_ALLOC_HPP

#ifndef BOOST_MEMORY_BASIC_HPP
#include "basic.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class region_alloc

template <class PolicyT>
class region_alloc
{
private:
	typedef typename PolicyT::alloc_type AllocT;

	region_alloc(const region_alloc&);
	const region_alloc& operator=(const region_alloc&);

public:
	enum { MemBlockSize = PolicyT::MemBlockBytes - AllocT::Padding };

	typedef AllocT alloc_type;

#pragma pack(1)
private:
	struct MemBlock;
	friend struct MemBlock;

	enum { HeaderSize = sizeof(void*) };
	enum { BlockSize = MemBlockSize - HeaderSize };

	struct MemBlock
	{
		MemBlock* pPrev;
		char buffer[BlockSize];
	};
	struct DestroyNode
	{
		DestroyNode* pPrev;
		destructor_t fnDestroy;
	};
#pragma pack()
	
	char* m_begin;
	char* m_end;
	AllocT m_alloc;
	DestroyNode* m_destroyChain;

private:
	__forceinline MemBlock* BOOST_MEMORY_CALL chainHeader_() const
	{
		return (MemBlock*)(m_begin - HeaderSize);
	}

	void BOOST_MEMORY_CALL init_()
	{
		MemBlock* pNew = (MemBlock*)m_alloc.allocate(sizeof(MemBlock));
		pNew->pPrev = NULL;
		m_begin = pNew->buffer;
		m_end = (char*)pNew + m_alloc.alloc_size(pNew);
	}

public:
	region_alloc() : m_destroyChain(NULL)
	{
		init_();
	}
	explicit region_alloc(AllocT alloc) : m_alloc(alloc), m_destroyChain(NULL)
	{
		init_();
	}

	~region_alloc()
	{
		clear();
	}

	alloc_type BOOST_MEMORY_CALL get_alloc() const
	{
		return m_alloc;
	}

	void BOOST_MEMORY_CALL swap(region_alloc& o)
	{
		swap_object(this, &o);
	}

	void BOOST_MEMORY_CALL clear()
	{
		while (m_destroyChain)
		{
			DestroyNode* curr = m_destroyChain;
			m_destroyChain = m_destroyChain->pPrev;
			curr->fnDestroy(curr + 1);
		}
		MemBlock* pHeader = chainHeader_();
		while (pHeader)
		{
			MemBlock* curr = pHeader;
			pHeader = pHeader->pPrev;
			m_alloc.deallocate(curr);
		}
		m_begin = m_end = (char*)HeaderSize;
	}

private:
	void* BOOST_MEMORY_CALL do_allocate_(size_t cb)
	{
		if (cb >= BlockSize)
		{
			MemBlock* pHeader = chainHeader_();
			MemBlock* pNew = (MemBlock*)m_alloc.allocate(HeaderSize + cb);
			if (pHeader)
			{
				pNew->pPrev = pHeader->pPrev;
				pHeader->pPrev = pNew;
			}
			else
			{
				m_end = m_begin = pNew->buffer;
				pNew->pPrev = NULL;
			}
			return pNew->buffer;
		}
		else
		{
			MemBlock* pNew = (MemBlock*)m_alloc.allocate(sizeof(MemBlock));
			pNew->pPrev = chainHeader_();
			m_begin = pNew->buffer;
			m_end = (char*)pNew + m_alloc.alloc_size(pNew);
			return m_end -= cb;
		}
	}

public:
	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb)
	{
		if ((size_t)(m_end - m_begin) >= cb)
		{
			return m_end -= cb;
		}
		return do_allocate_(cb);
	}

#if defined(BOOST_MEMORY_NO_STRICT_EXCEPTION_SEMANTICS)
	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb, int fnZero)
	{
		return allocate(cb);
	}

	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb, destructor_t fn)
	{
		DestroyNode* pNode = (DestroyNode*)allocate(sizeof(DestroyNode) + cb);
		pNode->fnDestroy = fn;
		pNode->pPrev = m_destroyChain;
		m_destroyChain = pNode;
		return pNode + 1;
	}
#endif
	
	__forceinline void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, destructor_t fn)
	{
		DestroyNode* pNode = (DestroyNode*)allocate(sizeof(DestroyNode) + cb);
		pNode->fnDestroy = fn;
		return pNode + 1;
	}

	__forceinline void BOOST_MEMORY_CALL manage(void* p, destructor_t fn)
	{
		DestroyNode* pNode = (DestroyNode*)p - 1;
		BOOST_MEMORY_ASSERT(pNode->fnDestroy == fn);

		pNode->pPrev = m_destroyChain;
		m_destroyChain = pNode;
	}

	__forceinline void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, int fnZero)
	{
		return allocate(cb);
	}

	__forceinline void BOOST_MEMORY_CALL manage(void* p, int fnZero)
	{
		// no action
	}

	void* BOOST_MEMORY_CALL reallocate(void* p, size_t oldSize, size_t newSize)
	{
		if (oldSize >= newSize)
			return p;
		void* p2 = allocate(newSize);
		memcpy(p2, p, oldSize);
		return p2;
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

// -------------------------------------------------------------------------
// $Log: region_alloc.hpp,v $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_REGION_ALLOC_HPP */
