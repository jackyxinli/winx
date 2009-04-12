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
#ifndef STDEXT_HASHMAP_H
#define STDEXT_HASHMAP_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STD_HASH_MAP_H
#include "../std/hash_map.h"
#endif

#ifndef WINX_NO_HASH_MAP_

#ifndef STDEXT_HASH_H
#include "Hash.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "Memory.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class HashMap

template <
	class KeyT, class DataT,
	class HashCompT = HashCompare<KeyT>,
	class AllocT = DefaultAlloc
	>
class HashMap : public WINX_BASE_HASHMAP_(KeyT, DataT, HashCompT, AllocT)
{
public:
	typedef KeyT key_type;
	typedef typename HashCompT::hasher hasher;
	typedef typename HashCompT::key_equal key_equal;
	typedef typename HashCompT::key_pred key_pred;
	typedef AllocT allocator_type;

private:
	typedef StlAlloc<DataT, AllocT> StlAllocT;
	typedef WINX_BASE_HASHMAP_(KeyT, DataT, HashCompT, AllocT) Base;
	
	HashMap(const HashMap&);
	void operator=(const HashMap&);

public:
	typedef typename Base::size_type size_type;

#if defined(X_STL_NET)
	explicit HashMap(
		allocator_type& alloc,
		size_type n = 100) : Base(HashComp_<KeyT, HashCompT>(), alloc)
	{
	}

	template <class Iterator>
	HashMap(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : Base(first, last, HashComp_<KeyT, HashCompT>(), alloc)
	{
	}
#else
	explicit HashMap(
		allocator_type& alloc,
		size_type n = 100) : Base(n, hasher(), key_equal(), alloc)
	{
	}

	template <class Iterator>
	HashMap(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : Base(first, last, n, hasher(), key_equal(), alloc)
	{
	}
#endif

	void winx_call copy(const Base& from) {
		Base::operator=(from);
	}
};

// -------------------------------------------------------------------------
// class HashMultiMap

template <
	class KeyT, class DataT,
	class HashCompT = HashCompare<KeyT>,
	class AllocT = DefaultAlloc
>
class HashMultiMap : public WINX_BASE_HASHMULTIMAP_(KeyT, DataT, HashCompT, AllocT)
{
public:
	typedef KeyT key_type;
	typedef typename HashCompT::hasher hasher;
	typedef typename HashCompT::key_equal key_equal;
	typedef typename HashCompT::key_pred key_pred;
	typedef AllocT allocator_type;

private:
	typedef StlAlloc<DataT, AllocT> StlAllocT;
	typedef WINX_BASE_HASHMULTIMAP_(KeyT, DataT, HashCompT, AllocT) Base;

	HashMultiMap(const HashMultiMap&);
	void operator=(const HashMultiMap&);

public:
	typedef typename Base::size_type size_type;

#if defined(X_STL_NET)
	explicit HashMultiMap(
		allocator_type& alloc,
		size_type n = 100) : Base(HashComp_<KeyT, HashCompT>(), alloc)
	{
	}

	template <class Iterator>
	HashMultiMap(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : Base(first, last, HashComp_<KeyT, HashCompT>(), alloc)
	{
	}
#else
	explicit HashMultiMap(
		allocator_type& alloc,
		size_type n = 100) : Base(n, hasher(), key_equal(), alloc)
	{
	}

	template <class Iterator>
	HashMultiMap(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : Base(first, last, n, hasher(), key_equal(), alloc)
	{
	}
#endif

	void winx_call copy(const Base& from) {
		Base::operator=(from);
	}
};

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestHashMap

#if defined(STD_UNITTEST)

template <class LogT>
class TestHashMap : public TestCase
{
	WINX_TEST_SUITE(TestHashMap);
		WINX_TEST(testMap);
		WINX_TEST(testMultiMap);
	WINX_TEST_SUITE_END();

public:
	void testMap(LogT& log)
	{
		typedef std::HashMap<int, int> MapType;

		std::BlockPool recycle;
		std::ScopedAlloc alloc(recycle);
		
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
		std::ScopedAlloc alloc(recycle);
		
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
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: HashMap.h,v $

#endif // WINX_NO_HASH_MAP_

#endif /* STDEXT_HASHMAP_H */
