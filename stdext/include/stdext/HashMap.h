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
// Module: stdext/HashMap.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: HashMap.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_HASHMAP_H__
#define __STDEXT_HASHMAP_H__

#ifndef __STDEXT_BASIC_H__
#include "Basic.h"
#endif

#ifndef __STD_HASH_MAP_H__
#include "../std/hash_map.h"
#endif

#ifndef _WINX_NO_HASH_MAP

#ifndef __STDEXT_HASH_H__
#include "Hash.h"
#endif

#ifndef __STDEXT_MEMORY_H__
#include "Memory.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class HashMap

template <
	class KeyT, class DataT,
	class HashCompT = HashCompare<KeyT>,
	class AllocT = ScopeAlloc
	>
class HashMap : public _WINX_BASE_HASHMAP(KeyT, DataT, HashCompT, AllocT)
{
public:
	typedef KeyT key_type;
	typedef typename HashCompT::hasher hasher;
	typedef typename HashCompT::key_equal key_equal;
	typedef typename HashCompT::key_pred key_pred;
	typedef AllocT allocator_type;

private:
	typedef StlAlloc<DataT, AllocT> _Alloc;
	typedef _WINX_BASE_HASHMAP(KeyT, DataT, HashCompT, AllocT) _Base;
	
	HashMap(const HashMap&);
	void operator=(const HashMap&);

public:
	typedef typename _Base::size_type size_type;

#if defined(X_STL_NET)
	explicit HashMap(
		allocator_type& alloc,
		size_type n = 100) : _Base(_HashComp<KeyT, HashCompT>(), alloc)
	{
	}

	template <class Iterator>
	HashMap(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : _Base(first, last, _HashComp<KeyT, HashCompT>(), alloc)
	{
	}
#else
	explicit HashMap(
		allocator_type& alloc,
		size_type n = 100) : _Base(n, hasher(), key_equal(), alloc)
	{
	}

	template <class Iterator>
	HashMap(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : _Base(first, last, n, hasher(), key_equal(), alloc)
	{
	}
#endif
};

// -------------------------------------------------------------------------
// class HashMultiMap

template <
	class KeyT, class DataT,
	class HashCompT = HashCompare<KeyT>,
	class AllocT = ScopeAlloc
>
class HashMultiMap : public _WINX_BASE_HASHMULTIMAP(KeyT, DataT, HashCompT, AllocT)
{
public:
	typedef KeyT key_type;
	typedef typename HashCompT::hasher hasher;
	typedef typename HashCompT::key_equal key_equal;
	typedef typename HashCompT::key_pred key_pred;
	typedef AllocT allocator_type;

private:
	typedef StlAlloc<DataT, AllocT> _Alloc;
	typedef _WINX_BASE_HASHMULTIMAP(KeyT, DataT, HashCompT, AllocT) _Base;

	HashMultiMap(const HashMultiMap&);
	void operator=(const HashMultiMap&);

public:
	typedef typename _Base::size_type size_type;

#if defined(X_STL_NET)
	explicit HashMultiMap(
		allocator_type& alloc,
		size_type n = 100) : _Base(_HashComp<KeyT, HashCompT>(), alloc)
	{
	}

	template <class Iterator>
	HashMultiMap(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : _Base(first, last, _HashComp<KeyT, HashCompT>(), alloc)
	{
	}
#else
	explicit HashMultiMap(
		allocator_type& alloc,
		size_type n = 100) : _Base(n, hasher(), key_equal(), alloc)
	{
	}

	template <class Iterator>
	HashMultiMap(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : _Base(first, last, n, hasher(), key_equal(), alloc)
	{
	}
#endif
};

// -------------------------------------------------------------------------
// class TestHashMap

template <class LogT>
class TestHashMap : public TestCase
{
	WINX_TEST_SUITE(TestHashMap);
		WINX_TEST(testMap);
		WINX_TEST(testMultiMap);
		WINX_TEST(testCompare);
	WINX_TEST_SUITE_END();

public:
	void testMap(LogT& log)
	{
		typedef std::HashMap<int, int> MapType;

		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		
		MapType simp(alloc);

		simp.insert(MapType::value_type(1, 2));
		simp.insert(MapType::value_type(1, 2));
		simp.insert(MapType::value_type(192, 4));
		simp.insert(MapType::value_type(194, 8));
		for (MapType::const_iterator it = simp.begin(); it != simp.end(); ++it)
		{
			log.print((*it).first)
				.print(", ")
				.print((*it).second)
				.newline();
		}
	}

	void testMultiMap(LogT& log)
	{
		typedef std::HashMultiMap<int, int> MapType;

		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		
		MapType simp(alloc);

		simp.insert(MapType::value_type(1, 2));
		simp.insert(MapType::value_type(1, 2));
		simp.insert(MapType::value_type(192, 4));
		simp.insert(MapType::value_type(194, 8));
		for (MapType::const_iterator it = simp.begin(); it != simp.end(); ++it)
		{
			log.print((*it).first)
				.print(", ")
				.print((*it).second)
				.newline();
		}
	}

public:
	enum { N = 20000 };

	void doMap(LogT& log)
	{
		typedef std::HashMap<int, int> MapT;
		log.print("===== std::HashMap (ScopeAlloc) =====\n");
		std::PerformanceCounter counter;
		{
			std::BlockPool recycle;
			std::ScopeAlloc alloc(recycle);
			MapT coll(alloc);
			for (int i = 0; i < N; ++i)
				coll.insert(MapT::value_type(i, i));
		}
		counter.trace(log);
	}

	void doShareAllocMap(LogT& log)
	{
		typedef std::HashMap<int, int> MapT;
		std::BlockPool recycle;
		log.newline();
		for (int i = 0; i < 5; ++i)
		{
			log.print("===== doShareAllocMap =====\n");
			std::PerformanceCounter counter;
			{
				std::ScopeAlloc alloc(recycle);
				MapT coll(alloc);
				for (int i = 0; i < N; ++i)
					coll.insert(MapT::value_type(i, i));
			}
			counter.trace(log);
		}
	}

	void testCompare(LogT& log)
	{
		for (int i = 0; i < 5; ++i)
		{
			log.newline();
			doMap(log);
		}
		doShareAllocMap(log);
	}
};

// -------------------------------------------------------------------------
// $Log: HashMap.h,v $

__NS_STD_END

#endif // _WINX_NO_HASH_MAP

#endif /* __STDEXT_HASHMAP_H__ */
