//
//  boost/memory/gc_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_GC_ALLOC_HPP
#define BOOST_MEMORY_GC_ALLOC_HPP

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#ifndef BOOST_MEMORY_SCOPED_ALLOC_HPP
#include "scoped_alloc.hpp"
#endif

#ifndef BOOST_MEMORY_STL_QUEUE_HPP
#include "stl/queue.hpp" // NS_BOOST_MEMORY::ext_priority_queue
#endif

#if !defined(_VECTOR_) && !defined(_VECTOR)
#include <vector> // std::vector
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class gen_alloc

#if defined(_DEBUG) && !defined(BOOST_MEMORY_TRACE_GC)
#define BOOST_MEMORY_TRACE_GC
#endif

#ifndef BOOST_MEMORY_ALIGN
#define BOOST_MEMORY_ALIGN(x, y) (((x)+((y)-1)) & ~((y)-1))
#endif

#ifndef MAX
#define MAX(a, b)		((a) < (b) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b)		((a) < (b) ? (a) : (b))
#endif

template <class PolicyT>
class gen_alloc
{
private:
	typedef typename PolicyT::allocator_type AllocT;
	typedef typename PolicyT::huge_gc_allocator HugeGCAllocT;
	typedef unsigned HeaderSizeT;

public:
	enum { MemBlockSize = PolicyT::MemBlockSize };
	enum { IsGCAllocator = TRUE };

	typedef AllocT allocator_type;
	typedef HugeGCAllocT huge_gc_allocator;

#pragma pack(1)
private:
	enum { NodeSizeBits = 30 };
	enum MemNodeType {
		nodeFree = 0 << NodeSizeBits,
		nodeAlloced = 1 << NodeSizeBits,
		nodeAllocedWithDestructor = 3 << NodeSizeBits,
	};
	enum {
		NodeTypeMask = 3 << NodeSizeBits,
		NodeSizeMask = (1 << NodeSizeBits) - 1,
	};
	
	struct MemHeader
	{
		HeaderSizeT dataMemHeader;
		// HeaderSizeT cbNodeSize : 30; // cbNodeSize = cbSize + sizeof(MemHeader)
		// HeaderSizeT nodeType : 2;	// enum MemNodeType
		
		int BOOST_MEMORY_CALL getNodeType() const {
			return NodeTypeMask & dataMemHeader;
		}
		HeaderSizeT BOOST_MEMORY_CALL getNodeSize() const {
			return NodeSizeMask & dataMemHeader;
		}
		char* BOOST_MEMORY_CALL begin() const {
			return (char*)this + sizeof(MemHeader);
		}
		char* BOOST_MEMORY_CALL end() const {
			BOOST_MEMORY_ASSERT(getNodeType() == nodeFree);
			return (char*)this + dataMemHeader;
		}
		void BOOST_MEMORY_CALL freeNode() {
			dataMemHeader &= ~NodeTypeMask;
		}
	};

	typedef MemHeader FreeMemHeader;

	struct MemHeaderEx;
	struct DestroyInfo
	{
		MemHeaderEx* pPrev;
		destructor_t fnDestroy;
	};

	struct MemHeaderEx : public MemHeader
	{
		MemHeaderEx* pPrev;
		destructor_t fnDestroy;

		void BOOST_MEMORY_CALL destruct() {
			if (this->getNodeType() == nodeAllocedWithDestructor) {
				this->freeNode();
				fnDestroy(this + 1);
			}
		}
	};

	friend struct MemHeader;
	friend struct MemHeaderEx;

	struct MemBlock
	{
		MemBlock* pPrev;
		char buffer[1];
		
		class Enumerator
		{
		private:
			char* m_start;
			char* m_last;

		public:
			Enumerator(AllocT& alloc, MemBlock* block) {
				m_start = block->buffer;
				m_last = (char*)block + alloc.alloc_size(block);
			}
			MemHeader* BOOST_MEMORY_CALL first() const {
				return (MemHeader*)m_start;
			}
			MemHeader* BOOST_MEMORY_CALL current() const {
				return (MemHeader*)m_start;
			}
			MemHeader* BOOST_MEMORY_CALL next() {
				BOOST_MEMORY_ASSERT(!done());
				m_start += ((MemHeader*)m_start)->getNodeSize();
				return (MemHeader*)m_start;
			}
			bool BOOST_MEMORY_CALL done() const {
				return m_start >= m_last;
			}
		};
	};
#pragma pack()

	struct Pred : std::binary_function<FreeMemHeader*, FreeMemHeader*, bool>
	{
		bool operator()(FreeMemHeader* a, FreeMemHeader* b) const {
			return a->dataMemHeader < b->dataMemHeader;
		}
	};
	typedef std::vector<FreeMemHeader*> Container;
	typedef NS_BOOST_MEMORY::ext_priority_queue<FreeMemHeader*, Container, Pred> PriorityQ;
	
	char* m_begin;
	char* m_end;
	AllocT m_alloc;
	MemHeaderEx* m_destroyChain;
	MemBlock* m_blockList;
	PriorityQ m_freeList;
	HugeGCAllocT m_hugeAlloc;
	size_t m_destroyCount, m_freeSize, m_GCLimitSize;
	static FreeMemHeader _null;

private:
	enum { HeaderSize = sizeof(void*) };
	enum { AlignSize = sizeof(HeaderSizeT) };
	enum { BlockSize = MemBlockSize - HeaderSize };
	enum { AllocSizeBigDef_ = MAX(PolicyT::AllocSizeBig, BlockSize/4) };
	enum { AllocSizeHugeDef_ = MAX(PolicyT::AllocSizeHuge, 64*1024) };
	enum { GCLimitSizeDef_ = MAX(PolicyT::GCLimitSizeDef, 64*1024) };
	enum { AllocSizeBig = MIN(AllocSizeBigDef_, BlockSize/2) };
	enum { AllocSizeHuge = MIN(AllocSizeHugeDef_, (1 << 29)) };
	enum { GCLimitSizeDef = MIN(GCLimitSizeDef_, (1 << 29)) };
	enum { RecycleSizeMin = MAX(PolicyT::RecycleSizeMin, 128) };

private:
	const gen_alloc& operator=(const gen_alloc&);
	
	static bool BOOST_MEMORY_CALL _isEqual(size_t cbAlloc, size_t cb)
	{
		return cbAlloc >= cb && cb + 64 > cbAlloc;
	}

	static bool BOOST_MEMORY_CALL _isValid(void* obj, size_t cb, destructor_t fn)
	{
		MemHeaderEx* node = (MemHeaderEx*)obj - 1;
		BOOST_MEMORY_ASSERT(node->fnDestroy == fn);
		BOOST_MEMORY_ASSERT(_isEqual(node->getNodeSize(), cb + sizeof(MemHeaderEx)));
		BOOST_MEMORY_ASSERT(node->getNodeType() == nodeAllocedWithDestructor);
		return node->fnDestroy == fn &&
			_isEqual(node->getNodeSize(), cb + sizeof(MemHeaderEx)) &&
			node->getNodeType() == nodeAllocedWithDestructor;
	}

	static bool BOOST_MEMORY_CALL _isValid(void* obj, size_t cb, int fnZero)
	{
		MemHeader* node = (MemHeader*)obj - 1;
		BOOST_MEMORY_ASSERT(_isEqual(node->getNodeSize(), sizeof(MemHeader) + cb));
		BOOST_MEMORY_ASSERT(node->getNodeType() == nodeAlloced);
		return _isEqual(node->getNodeSize(), sizeof(MemHeader) + cb) &&
			node->getNodeType() == nodeAlloced;
	}

	template <class Type>
	static bool BOOST_MEMORY_CALL _isValid(Type* obj)
	{
		return _isValid(obj, sizeof(Type), destructor_traits<Type>::destruct);
	}

	template <class Type>
	static bool BOOST_MEMORY_CALL _isValidArray(Type* array, size_t count)
	{
		void* buf = destructor_traits<Type>::getArrayBuffer(array);
		size_t cb = destructor_traits<Type>::getArrayAllocSize(count);
		if (buf == array)
		{
			return _isValid(buf, cb, 0);
		}
		else
		{
			size_t count1 = destructor_traits<Type>::getArraySize(array);
			BOOST_MEMORY_ASSERT(count1 == count);
			bool fValid = _isValid(buf, cb, destructor_traits<Type>::destructArray);
			return count1 == count && fValid;
		}
	}

public:
	void BOOST_MEMORY_CALL force_gc()
	{
#if defined(BOOST_MEMORY_TRACE_GC)
		printf("boost::gc_alloc - starting to gc...\n");
#endif
		// 0. Prepare

		// 0.1. Commit current block:
		_commitCurrentNode();
		m_begin = m_end = _null.begin();

		// 0.2. Reduce destroy chain
		_reduceDestroyChain();

		// 0.3. Clear free list
		m_freeList.clear();
		m_freeSize = 0;

		// 1. Collect free nodes
		MemBlock* pHeader = m_blockList;
		while (pHeader)
		{
			typename MemBlock::Enumerator coll(m_alloc, pHeader);
			pHeader = pHeader->pPrev;
			for (;;)
			{
				if (coll.done())
					break;
				FreeMemHeader* it = (FreeMemHeader*)coll.current();
				if (it->getNodeType() == nodeFree)
				{
					// merge nodes marked with nodeFree
					for (;;) {
						FreeMemHeader* it2 = (FreeMemHeader*)coll.next();
						if (coll.done() || it2->getNodeType() != nodeFree)
							break;
						it->dataMemHeader += it2->dataMemHeader;
					}
					if (it->dataMemHeader >= RecycleSizeMin)
						m_freeList.push(it);
					if (coll.done())
						break;
				}
				// skip nodes marked with nodeAlloced
				coll.next();
			}
		}
	}
	
	void BOOST_MEMORY_CALL try_gc()
	{
#if !defined(BOOST_MEMORY_DEBUG_GC)
		if (m_freeSize >= m_GCLimitSize)
#endif
			force_gc();
	}

	size_t BOOST_MEMORY_CALL get_free_size() const
	{
		return m_freeSize;
	}

	void BOOST_MEMORY_CALL set_gclim(size_t gcLimit)
	{
		m_GCLimitSize = gcLimit;
	}

private:
	void BOOST_MEMORY_CALL _reduceDestroyChain()
	{
		MemHeaderEx** pp = &m_destroyChain;
		while (m_destroyCount)
		{
			MemHeaderEx* curr = *pp;
			if (curr->getNodeType() == nodeFree) {
				*pp = curr->pPrev;
				--m_destroyCount;
			}
			else {
				pp = &curr->pPrev;
			}
		}
	}

	void BOOST_MEMORY_CALL _commitCurrentNode()
	{
		FreeMemHeader* pNode = (FreeMemHeader*)m_begin - 1;
		BOOST_MEMORY_ASSERT(
			pNode->getNodeType() == nodeFree ||
			pNode->getNodeType() == nodeAlloced &&
			_null.begin() == m_begin && m_begin == m_end);
		
		pNode->dataMemHeader = sizeof(FreeMemHeader) + (HeaderSizeT)(m_end - m_begin);
	}

	FreeMemHeader* BOOST_MEMORY_CALL _newBlock(size_t cbBlock)
	{
		MemBlock* pBlock = (MemBlock*)m_alloc.allocate(cbBlock);
		pBlock->pPrev = m_blockList;
		m_blockList = pBlock;

		FreeMemHeader* pNew = (FreeMemHeader*)pBlock->buffer;
		pNew->dataMemHeader = (HeaderSizeT)(m_alloc.alloc_size(pBlock) - HeaderSize);
		return pNew;
	}

	void BOOST_MEMORY_CALL _init()
	{
		m_blockList = NULL;
		m_destroyChain = NULL;
		m_freeSize = 0;
		m_destroyCount = 0;
		m_GCLimitSize = GCLimitSizeDef;

		FreeMemHeader* pNew = _newBlock(MemBlockSize);
		m_begin = pNew->begin();
		m_end = pNew->end();
	}

public:
	gen_alloc() {
		_init();
	}
	explicit gen_alloc(AllocT alloc) : m_alloc(alloc) {
		_init();
	}
	explicit gen_alloc(gen_alloc& owner) : m_alloc(owner.m_alloc) {
		_init();
	}

	~gen_alloc()
	{
		clear();
	}

	void BOOST_MEMORY_CALL swap(gen_alloc& o)
	{
		std::swap(m_begin, o.m_begin);
		std::swap(m_end, o.m_end);
		std::swap(m_blockList, o.m_blockList);
		std::swap(m_destroyChain, o.m_destroyChain);
		std::swap(m_freeSize, o.m_freeSize);
		std::swap(m_destroyCount, o.m_destroyCount);
		std::swap(m_GCLimitSize, o.m_GCLimitSize);
		m_alloc.swap(o.m_alloc);
		m_freeList.swap(o.m_freeList);
		m_hugeAlloc.swap(o.m_hugeAlloc);
	}

	void BOOST_MEMORY_CALL clear()
	{
		m_hugeAlloc.clear();
		while (m_destroyChain)
		{
			MemHeaderEx* curr = m_destroyChain;
			m_destroyChain = m_destroyChain->pPrev;
			curr->destruct();
		}
		MemBlock* pHeader = m_blockList;
		while (pHeader)
		{
			MemBlock* curr = pHeader;
			pHeader = pHeader->pPrev;
			m_alloc.deallocate(curr);
		}
		m_begin = m_end = _null.begin();
		m_blockList = NULL;
		m_freeList.clear();
		m_freeSize = 0;
		m_destroyCount = 0;
	}

private:
	void* BOOST_MEMORY_CALL _do_allocate(const size_t cb, const size_t cbData)
	{
		if ((size_t)(m_end - m_begin) >= cbData)
		{
			MemHeader* pAlloc = (MemHeader*)m_begin - 1;
			pAlloc->dataMemHeader = (nodeAlloced | (HeaderSizeT)(m_end - (char*)pAlloc));
			m_begin = m_end = _null.begin();
			return pAlloc + 1;
		}
		
		FreeMemHeader* pNew;
		if (cb >= AllocSizeBig)
		{
			if (cb >= BlockSize)
			{
				if (cb >= AllocSizeHuge)
					return m_hugeAlloc.allocate(cbData);

				MemHeader* pAlloc = (MemHeader*)_newBlock(cb + HeaderSize);
				pAlloc->dataMemHeader |= nodeAlloced;
				return pAlloc + 1;
			}
			pNew = _newBlock(MemBlockSize);
		}
		else
		{
			try_gc();
			if (m_freeList.empty() || (pNew = m_freeList.top())->getNodeSize() < cb) {
				pNew = _newBlock(MemBlockSize);
			}
			else {
				m_freeList.pop();
			}
		}
		_commitCurrentNode();
		m_begin = pNew->begin();
		m_end = pNew->end();

		BOOST_MEMORY_ASSERT((size_t)(m_end - m_begin) >= cb);

		MemHeader* pAlloc = (MemHeader*)(m_end -= cb);
		pAlloc->dataMemHeader = (nodeAlloced | (HeaderSizeT)cb);
		return pAlloc + 1;
	}

public:
	__forceinline void* BOOST_MEMORY_CALL allocate(const size_t cbData)
	{
		const size_t cb = BOOST_MEMORY_ALIGN(cbData, AlignSize) + sizeof(MemHeader);
		if ((size_t)(m_end - m_begin) >= cb)
		{
			MemHeader* pAlloc = (MemHeader*)(m_end -= cb);
			pAlloc->dataMemHeader = (nodeAlloced | (HeaderSizeT)cb);
			return pAlloc + 1;
		}
		return _do_allocate(cb, cbData);
	}

	void* BOOST_MEMORY_CALL allocate(const size_t cbData0, destructor_t fn)
	{
		const size_t cbData = sizeof(DestroyInfo) + cbData0;
		const size_t cbAlloc = BOOST_MEMORY_ALIGN(cbData, AlignSize) + sizeof(MemHeader);
		if (cbAlloc >= AllocSizeHuge)
			return m_hugeAlloc.allocate(cbData0, fn);
		
		MemHeaderEx* pNode = (MemHeaderEx*)((char*)allocate(cbData) - sizeof(MemHeader));
		pNode->fnDestroy = fn;
		pNode->pPrev = m_destroyChain;
		pNode->dataMemHeader |= nodeAllocedWithDestructor;
		m_destroyChain = pNode;
		return pNode + 1;
	}

	void* BOOST_MEMORY_CALL allocate(size_t cb, int fnZero)
	{
		return allocate(cb);
	}

	void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cbData0, destructor_t fn)
	{
		const size_t cbData = sizeof(DestroyInfo) + cbData0;

		BOOST_MEMORY_ASSERT(
			BOOST_MEMORY_ALIGN(cbData, AlignSize) + sizeof(MemHeader) < AllocSizeHuge);

		DestroyInfo* pInfo = (DestroyInfo*)allocate(cbData);
		pInfo->fnDestroy = fn;
		return pInfo + 1;
	}

	void* BOOST_MEMORY_CALL manage(void* p, destructor_t fn)
	{
		MemHeaderEx* pNode = (MemHeaderEx*)p - 1;
		BOOST_MEMORY_ASSERT(pNode->fnDestroy == fn);
		BOOST_MEMORY_ASSERT(pNode->getNodeType() == nodeAlloced);

		pNode->pPrev = m_destroyChain;
		pNode->dataMemHeader |= nodeAllocedWithDestructor;
		m_destroyChain = pNode;
		return p;
	}

	void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, int fnZero)
	{
		return allocate(cb);
	}

	void* BOOST_MEMORY_CALL manage(void* p, int fnZero)
	{
		return p;
	}

	__forceinline void BOOST_MEMORY_CALL deallocate(void* pData, const size_t cbData)
	{
		const size_t cb = BOOST_MEMORY_ALIGN(cbData, AlignSize) + sizeof(MemHeader);
		
		if (cb < AllocSizeHuge)
		{
			MemHeader* p = (MemHeader*)pData - 1;
			BOOST_MEMORY_ASSERT(p->getNodeSize() >= cb);
			BOOST_MEMORY_ASSERT(p->getNodeType() == nodeAlloced);

			p->freeNode();
			m_freeSize += cb;
		}
		else
		{
			m_hugeAlloc.deallocate(pData, cbData);
		}
	}

	void* BOOST_MEMORY_CALL reallocate(void* p, size_t oldSize, size_t newSize)
	{
		void* p2 = allocate(newSize);
		memcpy(p2, p, oldSize);
		deallocate(p, oldSize);
		return p2;
	}

private:
	template <class Type>
	void BOOST_MEMORY_CALL _destroy(Type* obj, destructor_t)
	{
		const size_t cb = sizeof(Type) + sizeof(MemHeaderEx);
		BOOST_MEMORY_ASSERT( BOOST_MEMORY_ALIGN(cb, AlignSize) < AllocSizeHuge );
		
		obj->~Type();
		MemHeaderEx* p = (MemHeaderEx*)obj - 1;
		
		p->freeNode();
		m_freeSize += cb;
		++m_destroyCount;
	}

	template <class Type>
	void BOOST_MEMORY_CALL _destroy(Type* obj, int)
	{
		const size_t cb = sizeof(Type) + sizeof(MemHeader);
		BOOST_MEMORY_ASSERT( BOOST_MEMORY_ALIGN(cb, AlignSize) < AllocSizeHuge );
		
		MemHeader* p = (MemHeader*)obj - 1;
		
		p->freeNode();
		m_freeSize += cb;
	}

	template <class Type>
	void BOOST_MEMORY_CALL _destroyArray(Type* array, size_t count, destructor_t)
	{
		typedef destructor_traits<Type> Traits;

		const size_t cb = Traits::getArrayAllocSize(count) + sizeof(MemHeaderEx);

		Traits::destructArrayN(array, count);
		void* pData = Traits::getArrayBuffer(array);
		MemHeaderEx* p = (MemHeaderEx*)pData - 1;
		
		p->freeNode();
		m_freeSize += cb;
		++m_destroyCount;
	}

	template <class Type>
	void BOOST_MEMORY_CALL _destroyArray(Type* array, size_t count, int)
	{
		const size_t cb = sizeof(Type) * count + sizeof(MemHeader);

		MemHeader* p = (MemHeader*)array - 1;

		p->freeNode();
		m_freeSize += cb;
	}

public:
	template <class Type>
	void BOOST_MEMORY_CALL destroy(Type* obj)
	{
		BOOST_MEMORY_ASSERT( _isValid(obj) );
		_destroy(obj, destructor_traits<Type>::destruct);
	}

	template <class Type>
	Type* BOOST_MEMORY_CALL newArray(size_t count, Type* zero)
	{
		const size_t cb = sizeof(MemHeader) + 
			destructor_traits<Type>::HasDestructor * sizeof(DestroyInfo) +
			destructor_traits<Type>::getArrayAllocSize(count);

		if (BOOST_MEMORY_ALIGN(cb, AlignSize) >= AllocSizeHuge)
			return m_hugeAlloc.newArray(count, zero);

		Type* array = (Type*)destructor_traits<Type>::allocArray(*this, count);
		return constructor_traits<Type>::constructArray(array, count);
	}

	template <class Type>
	void BOOST_MEMORY_CALL destroyArray(Type* array, size_t count)
	{
		BOOST_MEMORY_ASSERT( _isValidArray(array, count) );

		const size_t cb = sizeof(MemHeader) + 
			destructor_traits<Type>::HasDestructor * sizeof(DestroyInfo) +
			destructor_traits<Type>::getArrayAllocSize(count);

		if (BOOST_MEMORY_ALIGN(cb, AlignSize) >= AllocSizeHuge)
		{
			m_hugeAlloc.destroyArray(array, count);
		}
		else
		{
			_destroyArray(array, count, destructor_traits<Type>::destruct);
		}
	}

	BOOST_MEMORY_FAKE_DBG_ALLOCATE_();
};

template <class PolicyT>
typename gen_alloc<PolicyT>::FreeMemHeader gen_alloc<PolicyT>::_null;

// -------------------------------------------------------------------------
// class gc_alloc

typedef gen_alloc<NS_BOOST_MEMORY_POLICY::pool> gc_alloc;

// -------------------------------------------------------------------------
// $Log: gc_alloc.hpp,v $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_GC_ALLOC_HPP */
