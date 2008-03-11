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

#ifndef __SGI_HASH_MAP_H__
#include "sgi/hash_map.h"
#endif

#ifndef __STDEXT_MEMORY_H__
#include "Memory.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class Hash

template <class KeyT>
class Hash : public stdext::hash<KeyT>
{
};

// -------------------------------------------------------------------------
// class HashMap

template <
	class KeyT, class DataT,
	class HashT = Hash<KeyT>,
	class EqualT = std::equal_to<KeyT>,
	class AllocT = ScopeAlloc
	>
class HashMap : public 
	stdext::hash_map< KeyT, DataT, HashT, EqualT, StlAlloc<DataT, AllocT> >
{
private:
	typedef StlAlloc<DataT, AllocT> _Alloc;
	typedef stdext::hash_map< KeyT, DataT, HashT, EqualT, StlAlloc<DataT, AllocT> > _Base;

public:
	typedef typename _Base::size_type size_type;
	typedef typename _Base::hasher hasher;
	typedef typename _Base::key_equal key_equal;
	typedef AllocT allocator_type;

public:
	explicit HashMap(
		allocator_type& alloc,
		size_type n = 100,
		const hasher& hf = hasher(),
		const key_equal& eql = key_equal()) : _Base(n, hf, eql, alloc)
	{
	}

	template <class Iterator>
	HashMap(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100,
		const hasher& hf = hasher(),
		const key_equal& eql = key_equal()) : _Base(first, last, n, hf, eql, alloc)
	{
	}
};

// -------------------------------------------------------------------------
// class HashMultiMap

template <
	class KeyT, class DataT,
	class HashT = Hash<KeyT>,
	class EqualT = std::equal_to<KeyT>,
	class AllocT = ScopeAlloc
	>
class HashMultiMap : public 
	stdext::hash_multimap< KeyT, DataT, HashT, EqualT, StlAlloc<DataT, AllocT> >
{
private:
	typedef StlAlloc<DataT, AllocT> _Alloc;
	typedef stdext::hash_multimap< KeyT, DataT, HashT, EqualT, StlAlloc<DataT, AllocT> > _Base;

public:
	typedef typename _Base::size_type size_type;
	typedef typename _Base::hasher hasher;
	typedef typename _Base::key_equal key_equal;
	typedef AllocT allocator_type;

public:
	explicit HashMultiMap(
		allocator_type& alloc,
		size_type n = 100,
		const hasher& hf = hasher(),
		const key_equal& eql = key_equal()) : _Base(n, hf, eql, alloc)
	{
	}

	template <class Iterator>
	HashMultiMap(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100,
		const hasher& hf = hasher(),
		const key_equal& eql = key_equal()) : _Base(first, last, n, hf, eql, alloc)
	{
	}
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

#endif /* __STDEXT_HASHMAP_H__ */
