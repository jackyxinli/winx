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
// Module: stdext/Deque.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Deque.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_DEQUE_H
#define STDEXT_DEQUE_H

#ifndef STDEXT_MEMORY_H
#include "Memory.h"
#endif

#if defined(X_CC_VC6)
	#if defined(NO_SGISTL)
		#ifndef STD_DEQUE_H
		#include "../std/deque.h"
		#endif
		#define WINX_DEQUE_BASE_(DataT, AllocT) \
			std::_Deque<DataT, NS_STDEXT::StlAlloc<DataT, AllocT> >
	#else
		#ifndef __SGI_DEQUE_H__
		#include "sgi/deque.h"
		#endif
		#define WINX_DEQUE_BASE_(DataT, AllocT) \
			stdext::deque<DataT, NS_STDEXT::StlAlloc<DataT, AllocT> >
	#endif
#else
	#if !defined(_DEQUE) && !defined(_GLIBCXX_DEQUE) && !defined(_DEQUE_)
	#include <deque>
	#endif
	#define WINX_DEQUE_BASE_(DataT, AllocT) \
		std::deque<DataT, NS_STDEXT::StlAlloc<DataT, AllocT> >
#endif

#if defined(STD_UNITTEST)
	#ifndef STDEXT_COUNTER_H
	#include "Counter.h"
	#endif
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class Deque

template <class DataT, class AllocT = DefaultAlloc>
class Deque : public WINX_DEQUE_BASE_(DataT, AllocT)
{
private:
	typedef WINX_DEQUE_BASE_(DataT, AllocT) Base;

	Deque(const Deque&);
	void operator=(const Deque&);

public:
	typedef typename Base::size_type size_type;

public:
	explicit Deque(AllocT& a)
		: Base(a) {}
	
	Deque(AllocT& a, size_type n, const DataT& v = DataT())
		: Base(n, v, a) {}

	template <class Iterator>
	Deque(AllocT& a, Iterator first, Iterator last)
		: Base(first, last, a) {}

public:
	void winx_call copy(const Base& from) {
		Base::operator=(from);
	}

public:
	typedef AllocT alloc_type;
	
	AllocT& get_alloc() const {
		return Base::get_allocator().get_alloc();
	}
};

// -------------------------------------------------------------------------
// class TestDeque

#if defined(STD_UNITTEST)

template <class LogT>
class TestDeque : public TestCase
{
	WINX_TEST_SUITE(TestDeque);
		WINX_TEST(testDeque);
		WINX_TEST(testCompare);
	WINX_TEST_SUITE_END();

public:
	void testDeque(LogT& log)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);
		NS_STDEXT::Deque<int> coll(alloc);
		coll.push_back(1);
		coll.push_back(2);
		coll.push_back(3);
		AssertExp(coll.size() == 3);
	}

public:
	enum { N = 40000 };

	void doStlDeque(LogT& log)
	{
		typedef std::deque<int> DequeT;
		log.print("===== std::deque =====\n");
		NS_STDEXT::PerformanceCounter counter;
		{
			DequeT coll;
			for (int i = 0; i < N; ++i)
				coll.push_back(i);
		}
		counter.trace(log);
	}

	void doDeque(LogT& log)
	{
		typedef NS_STDEXT::Deque<int> DequeT;
		log.print("===== NS_STDEXT::Deque (ScopedAlloc) =====\n");
		NS_STDEXT::PerformanceCounter counter;
		{
			NS_STDEXT::BlockPool recycle;
			NS_STDEXT::ScopedAlloc alloc(recycle);
			DequeT coll(alloc);
			for (int i = 0; i < N; ++i)
				coll.push_back(i);
		}
		counter.trace(log);
	}

	void doShareAllocDeque(LogT& log)
	{
		typedef NS_STDEXT::Deque<int> DequeT;
		NS_STDEXT::BlockPool recycle;
		log.newline();
		for (int i = 0; i < 5; ++i)
		{
			log.print("===== doShareAllocDeque =====\n");
			NS_STDEXT::PerformanceCounter counter;
			{
				NS_STDEXT::ScopedAlloc alloc(recycle);
				DequeT coll(alloc);
				for (int i = 0; i < N; ++i)
					coll.push_back(i);
			}
			counter.trace(log);
		}
	}

	void testCompare(LogT& log)
	{
		for (int i = 0; i < 5; ++i)
		{
			log.newline();
			doStlDeque(log);
			doDeque(log);
		}
		doShareAllocDeque(log);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: Deque.h,v $

NS_STDEXT_END

#endif /* STDEXT_DEQUE_H */

