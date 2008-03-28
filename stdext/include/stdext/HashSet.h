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
// Module: stdext/HashSet.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: HashSet.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_HASHSET_H__
#define __STDEXT_HASHSET_H__

#ifndef __STDEXT_BASIC_H__
#include "Basic.h"
#endif

#ifndef __STD_HASH_SET_H__
#include "../std/hash_set.h"
#endif

#ifndef _WINX_NO_HASH_SET

#ifndef __STDEXT_HASH_H__
#include "Hash.h"
#endif

#ifndef __STDEXT_MEMORY_H__
#include "Memory.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class HashSet

template <
	class ValT,
	class HashCompT = HashCompare<ValT>,
	class AllocT = ScopeAlloc
	>
class HashSet : public _WINX_BASE_HASHSET(ValT, HashCompT, AllocT)
{
public:
	typedef ValT key_type;
	typedef ValT value_type;
	typedef typename HashCompT::hasher hasher;
	typedef typename HashCompT::key_equal key_equal;
	typedef typename HashCompT::key_pred key_pred;
	typedef AllocT allocator_type;

private:
	typedef StlAlloc<ValT, AllocT> _Alloc;
	typedef _WINX_BASE_HASHSET(ValT, HashCompT, AllocT) _Base;

	HashSet(const HashSet&);
	void operator=(const HashSet&);

public:
	typedef typename _Base::size_type size_type;

#if defined(X_STL_NET)
	explicit HashSet(
		allocator_type& alloc,
		size_type n = 100) : _Base(_HashComp<ValT, HashCompT>(), alloc)
	{
	}

	template <class Iterator>
	HashSet(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : _Base(first, last, _HashComp<ValT, HashCompT>(), alloc)
	{
	}
#else
	explicit HashSet(
		allocator_type& alloc,
		size_type n = 100) : _Base(n, hasher(), key_equal(), alloc)
	{
	}

	template <class Iterator>
	HashSet(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : _Base(first, last, n, hasher(), key_equal(), alloc)
	{
	}
#endif
};

// -------------------------------------------------------------------------
// class HashMultiSet

template <
	class ValT,
	class HashCompT = HashCompare<ValT>,
	class AllocT = ScopeAlloc
	>
class HashMultiSet : public _WINX_BASE_HASHMULTISET(ValT, HashCompT, AllocT)
{
public:
	typedef ValT key_type;
	typedef ValT value_type;
	typedef typename HashCompT::hasher hasher;
	typedef typename HashCompT::key_equal key_equal;
	typedef typename HashCompT::key_pred key_pred;
	typedef AllocT allocator_type;

private:
	typedef StlAlloc<ValT, AllocT> _Alloc;
	typedef _WINX_BASE_HASHMULTISET(ValT, HashCompT, AllocT) _Base;

	HashMultiSet(const HashMultiSet&);
	void operator=(const HashMultiSet&);

public:
	typedef typename _Base::size_type size_type;

#if defined(X_STL_NET)
	explicit HashMultiSet(
		allocator_type& alloc,
		size_type n = 100) : _Base(_HashComp<ValT, HashCompT>(), alloc)
	{
	}

	template <class Iterator>
	HashMultiSet(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : _Base(first, last, _HashComp<ValT, HashCompT>(), alloc)
	{
	}
#else
	explicit HashMultiSet(
		allocator_type& alloc,
		size_type n = 100) : _Base(n, hasher(), key_equal(), alloc)
	{
	}

	template <class Iterator>
	HashMultiSet(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : _Base(first, last, n, hasher(), key_equal(), alloc)
	{
	}
#endif
};

// -------------------------------------------------------------------------
// class TestHashSet

template <class LogT>
class TestHashSet : public TestCase
{
	WINX_TEST_SUITE(TestHashSet);
		WINX_TEST(testSet);
		WINX_TEST(testMultiSet);
		WINX_TEST(testCompare);
	WINX_TEST_SUITE_END();

public:
	void testSet(LogT& log)
	{
		typedef std::HashSet<int> SetType;

		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		
		SetType simp(alloc);

		simp.insert(1);
		simp.insert(1);
		simp.insert(192);
		simp.insert(194);
		for (SetType::const_iterator it = simp.begin(); it != simp.end(); ++it)
		{
			log.print(*it).newline();
		}
	}

	void testMultiSet(LogT& log)
	{
		typedef std::HashMultiSet<int> SetType;

		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		
		SetType simp(alloc);

		simp.insert(1);
		simp.insert(1);
		simp.insert(192);
		simp.insert(194);
		for (SetType::const_iterator it = simp.begin(); it != simp.end(); ++it)
		{
			log.print(*it).newline();
		}
	}

public:
	enum { N = 20000 };

	void doSet(LogT& log)
	{
		typedef std::HashSet<int> SetType;
		log.print("===== std::HashSet (ScopeAlloc) =====\n");
		std::PerformanceCounter counter;
		{
			std::BlockPool recycle;
			std::ScopeAlloc alloc(recycle);
			SetType coll(alloc);
			for (int i = 0; i < N; ++i)
				coll.insert(i);
		}
		counter.trace(log);
	}

	void doShareAllocSet(LogT& log)
	{
		typedef std::HashSet<int> SetType;
		std::BlockPool recycle;
		log.newline();
		for (int i = 0; i < 5; ++i)
		{
			log.print("===== doShareAllocSet =====\n");
			std::PerformanceCounter counter;
			{
				std::ScopeAlloc alloc(recycle);
				SetType coll(alloc);
				for (int i = 0; i < N; ++i)
					coll.insert(i);
			}
			counter.trace(log);
		}
	}

	void testCompare(LogT& log)
	{
		for (int i = 0; i < 5; ++i)
		{
			log.newline();
			doSet(log);
		}
		doShareAllocSet(log);
	}
};

// -------------------------------------------------------------------------
// $Log: HashSet.h,v $

__NS_STD_END

#endif // _WINX_NO_HASH_SET

#endif /* __STDEXT_HASHSET_H__ */
