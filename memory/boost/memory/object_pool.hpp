//
//  boost/memory/object_pool.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_OBJECT_POOL_HPP
#define BOOST_MEMORY_OBJECT_POOL_HPP

#ifndef BOOST_MEMORY_FIXED_ALLOC_HPP
#include "fixed_alloc.hpp"
#endif

#ifndef BOOST_MEMORY_BLOCKPOOL_HPP
#include "block_pool.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class norm_object_pool_

template <class Type, class PolicyT>
class norm_object_pool_ : private fixed_alloc<PolicyT>
{
private:
	typedef fixed_alloc<PolicyT> PoolT;
	typedef typename PoolT::alloc_type AllocT;

#pragma pack(1)
protected:
	struct ChunkHeader
	{
		union
		{
			MemBlock* pBlock;
			size_t tag;
		};
	};
#pragma pack()

protected:	
	__forceinline size_t BOOST_MEMORY_CALL is_allocated_(void*p )
	{
		return 1 & ((ChunkHeader*)p - 1)->tag;
	}
	
	__forceinline void BOOST_MEMORY_CALL mark_allocated_(void* p)
	{
		BOOST_MEMORY_ASSERT(!is_allocated_(p));
		++((ChunkHeader*)p - 1)->tag;
	}

	__forceinline void BOOST_MEMORY_CALL mark_deallocated_(void* p)
	{
		BOOST_MEMORY_ASSERT(is_allocated_(p));
		--((ChunkHeader*)p - 1)->tag;
	}
	
public:
	typedef PoolT pool_type;
	typedef AllocT alloc_type;
	
public:
	using PoolT::get_alloc;

public:
	norm_object_pool_() : PoolT(sizeof(Type))
	{
		BOOST_MEMORY_STATIC_ASSERT(sizeof(ChunkHeader) == sizeof(typename PoolT::ChunkHeader));
	}
	
	explicit norm_object_pool_(AllocT alloc) : PoolT(alloc, sizeof(Type))
	{
		BOOST_MEMORY_STATIC_ASSERT(sizeof(ChunkHeader) == sizeof(typename PoolT::ChunkHeader));
	}
	
	~norm_object_pool_()
	{
		clear();
	}

	pool_type& BOOST_MEMORY_CALL get_pool()
	{
		return *this;
	}

private:
	void BOOST_MEMORY_CALL do_clear_block_(MemBlock* const blk)
	{
		size_t nUsed = blk->nUsed;
		char* p = blk->buffer + ChunkHeaderSize;
		for (;; p += this->m_cbChunk)
		{
			if (is_allocated_(p))
			{
				((Type*)p)->~Type();
				if (--nUsed == 0)
					break;
			}
		}
	}

public:
	void BOOST_MEMORY_CALL clear()
	{
		MemBlock* nextBlk;
		for (MemBlock* blk = m_blks.first(); !m_blks.done(blk); blk = nextBlk)
		{
			nextBlk = blk->next();
			if (blk->nUsed)
				do_clear_block_(blk);
			this->m_alloc.deallocate(blk);
		}
		this->m_blks.clear();
		this->m_freelist.clear();
	}

#if defined(BOOST_MEMORY_NO_STRICT_EXCEPTION_SEMANTICS)
	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb, destructor_t fn)
	{
		BOOST_MEMORY_ASSERT(cb == sizeof(Type) && fn == BOOST_MEMORY_DESTRUCTOR(Type));
		void* p = PoolT::allocate();
		mark_allocated_(p);
		return p;
	}
#endif
	
	__forceinline void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, destructor_t fn) {
		BOOST_MEMORY_ASSERT(cb == sizeof(Type) && fn == BOOST_MEMORY_DESTRUCTOR(Type));
		return PoolT::allocate();
	}

	__forceinline void BOOST_MEMORY_CALL manage(void* p, destructor_t fn) {
		BOOST_MEMORY_ASSERT(fn == BOOST_MEMORY_DESTRUCTOR(Type));
		mark_allocated_(p);
	}

	Type* BOOST_MEMORY_CALL construct() {
		Type* p = new(PoolT::allocate()) Type;
		mark_allocated_(p);
		return p;
	}

	void BOOST_MEMORY_CALL destroy(Type* obj) {
		mark_deallocated_(obj);
		obj->~Type();
		return PoolT::deallocate(obj);
	}
};

// -------------------------------------------------------------------------
// class pod_object_pool_

template <class Type, class PolicyT>
class pod_object_pool_ : private fixed_alloc<PolicyT>
{
private:
	typedef fixed_alloc<PolicyT> PoolT;
	typedef typename PoolT::alloc_type AllocT;

public:
	typedef PoolT pool_type;
	typedef AllocT alloc_type;

public:
	using PoolT::get_alloc;
	using PoolT::clear;

public:
	pod_object_pool_() : PoolT(sizeof(Type)) {}

	explicit pod_object_pool_(AllocT alloc)
		: PoolT(alloc, sizeof(Type)) {
	}

	pool_type& BOOST_MEMORY_CALL get_pool() {
		return *this;
	}

#if defined(BOOST_MEMORY_NO_STRICT_EXCEPTION_SEMANTICS)
	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb, int fnZero) {
		BOOST_MEMORY_ASSERT(cb == sizeof(Type));
		return PoolT::allocate();
	}
#endif

	__forceinline void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, int fnZero) {
		BOOST_MEMORY_ASSERT(cb == sizeof(Type));
		return PoolT::allocate();
	}

	__forceinline void BOOST_MEMORY_CALL manage(void* p, int fnZero) {
	}

	__forceinline Type* BOOST_MEMORY_CALL construct() {
		return new (PoolT::allocate()) Type;
	}

	__forceinline void BOOST_MEMORY_CALL destroy(Type* obj) {
	}
};

// -------------------------------------------------------------------------
// class object_pool_traits

#if defined(_MSC_VER) && (_MSC_VER <= 1200) // VC++ 6.0

template <
	class Type,
	class PolicyT = NS_BOOST_MEMORY_POLICY::stdlib,
	int hasDestructor = destructor_traits<Type>::hasDestructor>
struct object_pool_traits
{
	template <int hasDestructor>
	struct traits_ {
		typedef norm_object_pool_<Type, PolicyT> type;
	};
	
	template <>
	struct traits_<0> {
		typedef pod_object_pool_<Type, PolicyT> type;
	};
	
	typedef typename traits_<hasDestructor>::type type;
};

#else

template <
		class Type,
		class PolicyT = NS_BOOST_MEMORY_POLICY::stdlib,
		int hasDestructor = destructor_traits<Type>::hasDestructor>
struct object_pool_traits {
	typedef norm_object_pool_<Type, PolicyT> type;
};

template <class Type, class PolicyT>
struct object_pool_traits<Type, PolicyT, 0> {
	typedef pod_object_pool_<Type, PolicyT> type;
};

#endif

// -------------------------------------------------------------------------
// class object_pool

template <class Type, class PolicyT = NS_BOOST_MEMORY_POLICY::stdlib>
class object_pool : public object_pool_traits<Type, PolicyT>::type
{
private:
	typedef typename object_pool_traits<Type, PolicyT>::type Base;
	typedef typename Base::alloc_type AllocT;
	
public:
	__forceinline object_pool() {
	}
	__forceinline explicit object_pool(AllocT alloc) :
		Base(alloc) {
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_OBJECT_POOL_HPP */
