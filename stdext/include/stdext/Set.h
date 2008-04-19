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
// Module: stdext/Set.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Set.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_SET_H__
#define __STDEXT_SET_H__

#ifndef __STDEXT_MEMORY_H__
#include "Memory.h"
#endif

#ifndef __STD_SET_H__
#include "../std/set.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class Set

template <
	class KeyT,
	class PredT = std::less<KeyT>,
	class AllocT = ScopeAlloc
	>
class Set : public std::set< KeyT, PredT, StlAlloc<KeyT, AllocT> >
{
private:
	typedef StlAlloc<KeyT, AllocT> _Alloc;
	typedef std::set<KeyT, PredT, _Alloc> _Base;

	Set(const Set&);
	void operator=(const Set&);

public:
	explicit Set(AllocT& alloc, const PredT& pred = PredT())
		: _Base(pred, alloc)
	{
	}

	template <class Iterator>
	Set(AllocT& alloc, Iterator first, Iterator last, const PredT& pred = PredT())
		: _Base(first, last, pred, alloc)
	{
	}

	void winx_call copy(const _Base& from) {
		_Base::operator=(from);
	}
};

// -------------------------------------------------------------------------
// class MultiSet

template <
	class KeyT,
	class PredT = std::less<KeyT>,
	class AllocT = ScopeAlloc
	>
class MultiSet : public std::multiset< KeyT, PredT, StlAlloc<KeyT, AllocT> >
{
private:
	typedef StlAlloc<KeyT, AllocT> _Alloc;
	typedef std::multiset<KeyT, PredT, _Alloc> _Base;

	MultiSet(const MultiSet&);
	void operator=(const MultiSet&);

public:
	explicit MultiSet(AllocT& alloc, const PredT& pred = PredT())
		: _Base(pred, alloc)
	{
	}

	template <class Iterator>
	MultiSet(AllocT& alloc, Iterator first, Iterator last, const PredT& pred = PredT())
		: _Base(first, last, pred, alloc)
	{
	}

	void winx_call copy(const _Base& from) {
		_Base::operator=(from);
	}
};

// -------------------------------------------------------------------------
// class TestSet

#if defined(STD_UNITTEST)

template <class LogT>
class TestSet : public TestCase
{
	WINX_TEST_SUITE(TestSet);
		WINX_TEST(testSet);
		WINX_TEST(testMultiSet);
		WINX_TEST(testCompare);
	WINX_TEST_SUITE_END();

public:
	class Obj
	{
		WINX_DECLARE_COUNT(Obj);
	private:
		int m_val;
	public:
		Obj(int arg = 0) {
			m_val = arg;
			printf("construct Obj: %d\n", m_val);
		}
		Obj(const Obj& o) {
			m_val = o.m_val;
			printf("copy constructor: %d\n", m_val);
		}
		~Obj() {
			printf("destruct Obj: %d\n", m_val);
		}
		bool operator<(const Obj& o) const {
			return m_val < o.m_val;
		}
	};

	void testSet(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		std::Set<Obj> coll(alloc);
		coll.insert(1);
		coll.insert(1);
		coll.insert(2);
		coll.insert(4);
		AssertExp(coll.size() == 3);
	}

	void testMultiSet(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		std::MultiSet<Obj> coll(alloc);
		coll.insert(1);
		coll.insert(1);
		coll.insert(2);
		coll.insert(4);
		AssertExp(coll.size() == 4);
	}

public:
	enum { N = 20000 };

	void doStlSet(LogT& log)
	{
		typedef std::set<int> SetT;
		log.print("===== std::set =====\n");
		std::PerformanceCounter counter;
		{
			SetT coll;
			for (int i = 0; i < N; ++i)
				coll.insert(i);
		}
		counter.trace(log);
	}

	void doSet1(LogT& log)
	{
		typedef std::Set<int, std::less<int>, std::AutoFreeAlloc> SetT;
		log.print("===== std::Set (AutoFreeAlloc) =====\n");
		std::PerformanceCounter counter;
		{
			std::AutoFreeAlloc alloc;
			SetT coll(alloc);
			for (int i = 0; i < N; ++i)
				coll.insert(i);
		}
		counter.trace(log);
	}

	void doSet2(LogT& log)
	{
		typedef std::Set<int> SetT;
		log.print("===== std::Set (ScopeAlloc) =====\n");
		std::PerformanceCounter counter;
		{
			std::BlockPool recycle;
			std::ScopeAlloc alloc(recycle);
			SetT coll(alloc);
			for (int i = 0; i < N; ++i)
				coll.insert(i);
		}
		counter.trace(log);
	}

	void doShareAllocSet(LogT& log)
	{
		typedef std::Set<int> SetT;
		std::BlockPool recycle;
		log.newline();
		for (int i = 0; i < 5; ++i)
		{
			log.print("===== doShareAllocSet =====\n");
			std::PerformanceCounter counter;
			{
				std::ScopeAlloc alloc(recycle);
				SetT coll(alloc);
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
			doStlSet(log);
			doSet2(log);
			doSet1(log);
		}
		doShareAllocSet(log);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: Set.h,v $

__NS_STD_END

#endif /* __STDEXT_SET_H__ */
