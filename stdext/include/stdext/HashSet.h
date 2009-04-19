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
#ifndef STDEXT_HASHSET_H
#define STDEXT_HASHSET_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STD_HASH_SET_H
#include "../std/hash_set.h"
#endif

#ifndef WINX_NO_HASH_SET_

#ifndef STDEXT_HASH_H
#include "Hash.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "Memory.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class HashSet

template <
	class ValT,
	class HashCompT = HashCompare<ValT>,
	class AllocT = DefaultAlloc
	>
class HashSet : public WINX_BASE_HASHSET_(ValT, HashCompT, AllocT)
{
public:
	typedef ValT key_type;
	typedef ValT value_type;
	typedef typename HashCompT::hasher hasher;
	typedef typename HashCompT::key_equal key_equal;
	typedef typename HashCompT::key_pred key_pred;
	typedef AllocT allocator_type;

private:
	typedef StlAlloc<ValT, AllocT> StlAllocT;
	typedef WINX_BASE_HASHSET_(ValT, HashCompT, AllocT) Base;

	HashSet(const HashSet&);
	void operator=(const HashSet&);

public:
	typedef typename Base::size_type size_type;

#if defined(X_STL_NET)
	explicit HashSet(
		allocator_type& alloc,
		size_type n = 100) : Base(HashComp_<ValT, HashCompT>(), alloc)
	{
	}

	template <class Iterator>
	HashSet(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : Base(first, last, HashComp_<ValT, HashCompT>(), alloc)
	{
	}
#else
	explicit HashSet(
		allocator_type& alloc,
		size_type n = 100) : Base(n, hasher(), key_equal(), alloc)
	{
	}

	template <class Iterator>
	HashSet(
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
// class HashMultiSet

template <
	class ValT,
	class HashCompT = HashCompare<ValT>,
	class AllocT = DefaultAlloc
	>
class HashMultiSet : public WINX_BASE_HASHMULTISET_(ValT, HashCompT, AllocT)
{
public:
	typedef ValT key_type;
	typedef ValT value_type;
	typedef typename HashCompT::hasher hasher;
	typedef typename HashCompT::key_equal key_equal;
	typedef typename HashCompT::key_pred key_pred;
	typedef AllocT allocator_type;

private:
	typedef StlAlloc<ValT, AllocT> StlAllocT;
	typedef WINX_BASE_HASHMULTISET_(ValT, HashCompT, AllocT) Base;

	HashMultiSet(const HashMultiSet&);
	void operator=(const HashMultiSet&);

public:
	typedef typename Base::size_type size_type;

#if defined(X_STL_NET)
	explicit HashMultiSet(
		allocator_type& alloc,
		size_type n = 100) : Base(HashComp_<ValT, HashCompT>(), alloc)
	{
	}

	template <class Iterator>
	HashMultiSet(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : Base(first, last, HashComp_<ValT, HashCompT>(), alloc)
	{
	}
#else
	explicit HashMultiSet(
		allocator_type& alloc,
		size_type n = 100) : Base(n, hasher(), key_equal(), alloc)
	{
	}

	template <class Iterator>
	HashMultiSet(
		allocator_type& alloc,
		Iterator first, Iterator last,
		size_type n = 100) : Base(first, last, n, hasher(), key_equal(), alloc)
	{
	}
#endif
};

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestHashSet

#if defined(STD_UNITTEST)

template <class LogT>
class TestHashSet : public TestCase
{
	WINX_TEST_SUITE(TestHashSet);
		WINX_TEST(testSet);
		WINX_TEST(testMultiSet);
	WINX_TEST_SUITE_END();

public:
	void testSet(LogT& log)
	{
		typedef NS_STDEXT::HashSet<int> SetType;

		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);
		
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
		typedef NS_STDEXT::HashMultiSet<int> SetType;

		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);
		
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
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: HashSet.h,v $

#endif // WINX_NO_HASH_SET_

#endif /* STDEXT_HASHSET_H */

