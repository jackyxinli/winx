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

#if defined(X_STL_NET)
	#ifndef _HASH_MAP_
	#include <hash_map>
	#endif
#else
	#ifndef __SGI_HASH_MAP_H__
	#include "sgi/hash_map.h"
	#endif
#endif

#ifndef __STDEXT_MEMORY_H__
#include "Memory.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class Hash

#if defined(X_STL_NET)

template <class KeyT>
class Hash
{
public:
	size_t winx_call operator()(const KeyT& v) const
	{
		return stdext::hash_value(v);
	}
};

#else

template <class KeyT>
class Hash : public stdext::hash<KeyT>
{
};

#endif

// -------------------------------------------------------------------------
// class HashCompare

template <class KeyT>
class HashCompare
{
public:
	typedef Hash<KeyT> hasher;
	typedef std::equal_to<KeyT> key_equal;
	typedef std::less<KeyT> key_pred;
};

// -------------------------------------------------------------------------
// Configuration

#if defined(X_STL_NET)

template <class KeyT, class HashCompT>
struct _HashComp : public stdext::hash_compare<KeyT, typename HashCompT::key_pred>
{
	typedef stdext::hash_compare<KeyT, typename HashCompT::key_pred> __Base;

	_HashComp() {}
	_HashComp(typename HashCompT::key_pred pr) : __Base(pr) {}

	bool winx_call operator()(const KeyT& key1, const KeyT& key2) const
	{
		return __Base::operator()(key1, key2);
	}

	size_t winx_call operator()(const KeyT& key) const
	{
		typename HashCompT::hasher hash;
		ldiv_t _Qrem = ldiv((long)hash(key), 127773);
		_Qrem.rem = 16807 * _Qrem.rem - 2836 * _Qrem.quot;
		if (_Qrem.rem < 0)
			_Qrem.rem += 2147483647;
		return ((size_t)_Qrem.rem);
	}
};

#define _WINX_BASE_HASHMAP(KeyT, DataT, HashCompT, AllocT)					\
	stdext::hash_map<														\
		KeyT, DataT,														\
		_HashComp<KeyT, HashCompT>,											\
		StlAlloc<DataT, AllocT>												\
	>

#define _WINX_BASE_HASHMULTIMAP(KeyT, DataT, HashCompT, AllocT)				\
	stdext::hash_multimap<													\
		KeyT, DataT,														\
		_HashComp<KeyT, HashCompT>,											\
		StlAlloc<DataT, AllocT>												\
	>

#else

#define _WINX_BASE_HASHMAP(KeyT, DataT, HashCompT, AllocT)					\
	stdext::hash_map<														\
		KeyT, DataT,														\
		typename HashCompT::hasher,											\
		typename HashCompT::key_equal,										\
		StlAlloc<DataT, AllocT>												\
		>

#define _WINX_BASE_HASHMULTIMAP(KeyT, DataT, HashCompT, AllocT)				\
	stdext::hash_multimap<													\
		KeyT, DataT,														\
		typename HashCompT::hasher,											\
		typename HashCompT::key_equal,										\
		StlAlloc<DataT, AllocT>												\
	>

#endif

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

#endif /* __STDEXT_HASHMAP_H__ */
