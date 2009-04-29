//
//  boost/memory/fixed_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_FIXED_ALLOC_HPP
#define BOOST_MEMORY_FIXED_ALLOC_HPP

#ifndef BOOST_MEMORY_BASIC_HPP
#include "basic.hpp"
#endif

#ifndef BOOST_MEMORY_CONTAINER_DCL_LIST_HPP
#include "container/dcl_list.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class fixed_alloc

#ifndef ROUND
#define ROUND(x, y)		(((x)+((y)-1)) & ~((y)-1))
#endif

#ifndef MAX
#define MAX(x, y)		((x) > (y) ? (x) : (y))
#define MIN(x, y)		((x) < (y) ? (x) : (y))
#endif

template <class PolicyT>
class fixed_alloc
{
private:
	typedef typename PolicyT::alloc_type AllocT;

	fixed_alloc(const fixed_alloc&);
	void operator=(const fixed_alloc&);

public:
	enum { MemBlockSize = PolicyT::MemBlockBytes - AllocT::Padding };

	typedef AllocT alloc_type;
	typedef size_t size_type;

#pragma pack(1)
protected:
	struct MemBlock;
	friend struct MemBlock;
	
	struct Header : public dcl_list_node<MemBlock>
	{
		size_t nUsed;
	};

	enum { HeaderSize = sizeof(Header) };
	enum { BlockSize = MemBlockSize - HeaderSize };

	struct MemBlock : Header
	{
		char buffer[BlockSize];
	};

	struct ChunkHeader
	{
		MemBlock* pBlock;
	};

	struct FreeChunk : public dcl_list_node<FreeChunk>
	{
	};

	enum { ChunkHeaderSize = sizeof(ChunkHeader) };
	enum { MinElemBytes = sizeof(FreeChunk) };

#pragma pack()

	dcl_list<FreeChunk> m_freelist;
	dcl_list<MemBlock> m_blks;
	AllocT m_alloc;
	size_type m_cbChunk;
	size_type m_nMaxPerBlock;
	MemBlock* m_lastBlock;

private:
	void init_(size_type cbElem)
	{
		cbElem = ROUND(cbElem, sizeof(void*));
		m_cbChunk = MAX(cbElem, MinElemBytes) + ChunkHeaderSize;
		m_nMaxPerBlock = BlockSize / m_cbChunk;
		m_lastBlock = NULL;

		BOOST_MEMORY_ASSERT(m_nMaxPerBlock > 0);
	}

public:
	explicit fixed_alloc(size_type cbElem)
	{
		init_(cbElem);
	}
	fixed_alloc(AllocT alloc, size_type cbElem)
		: m_alloc(alloc)
	{
		init_(cbElem);
	}

	~fixed_alloc()
	{
		clear();
	}

	alloc_type BOOST_MEMORY_CALL get_alloc() const
	{
		return m_alloc;
	}

	void BOOST_MEMORY_CALL swap(fixed_alloc& o)
	{
		swap_object(this, &o);
	}

	void BOOST_MEMORY_CALL clear()
	{
		MemBlock* nextBlk;
		for (MemBlock* blk = this->m_blks.first(); !this->m_blks.done(blk); blk = nextBlk)
		{
			nextBlk = blk->next();
			this->m_alloc.deallocate(blk);
		}
		this->m_blks.clear();
		this->m_freelist.clear();
	}

private:
	__forceinline MemBlock*& BOOST_MEMORY_CALL chunkHeader_(void* const p)
	{
		return ((ChunkHeader*)p - 1)->pBlock;
	}

	void BOOST_MEMORY_CALL do_allocate_block_()
	{
		MemBlock* const blk = (MemBlock*)m_alloc.allocate(sizeof(MemBlock));
		m_blks.push_front(blk);
		m_lastBlock = blk;

		blk->nUsed = 0;

		char* p = blk->buffer + ChunkHeaderSize;
		for (size_type i = m_nMaxPerBlock; i--; p += m_cbChunk)
		{
			chunkHeader_(p) = blk;
			m_freelist.push_front((FreeChunk*)p);
		}

		BOOST_MEMORY_ASSERT(!m_freelist.empty());
	}

	void BOOST_MEMORY_CALL do_deallocate_block_(MemBlock* const blk)
	{
		char* p = blk->buffer + ChunkHeaderSize;
		for (size_type i = m_nMaxPerBlock; i--; p += m_cbChunk)
		{
			((FreeChunk*)p)->erase();
		}

		blk->erase();

		m_alloc.deallocate(blk);
	}

public:
	__forceinline void* BOOST_MEMORY_CALL allocate()
	{
		if (m_freelist.empty())
			do_allocate_block_();

		void* p = &m_freelist.front();
		++chunkHeader_(p)->nUsed;
		m_freelist.pop_front();
		return p;
	}

	__forceinline void BOOST_MEMORY_CALL deallocate(void* const p)
	{
		BOOST_MEMORY_ASSERT(chunkHeader_(p)->nUsed <= m_nMaxPerBlock);

		MemBlock* const blk = chunkHeader_(p);
		if (--blk->nUsed > 0)
			m_freelist.push_front((FreeChunk*)p);
		else if (blk != m_lastBlock)
			do_deallocate_block_(blk);
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_FIXED_ALLOC_HPP */
