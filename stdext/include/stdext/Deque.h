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
#ifndef __STDEXT_DEQUE_H__
#define __STDEXT_DEQUE_H__

#ifndef __STDEXT_MEMORY_H__
#include "Memory.h"
#endif

#if defined(X_CC_VC6)
	#ifndef __SGI_DEQUE_H__
	#include "sgi/deque.h"
	#endif
	#define _WINX_DEQUE_BASE(DataT, AllocT) \
		stdext::deque<DataT, StlAlloc<DataT, AllocT> >
#else
	#if !defined(_DEQUE)
	#include <deque>
	#endif
	#define _WINX_DEQUE_BASE(DataT, AllocT) \
		std::deque<DataT, StlAlloc<DataT, AllocT> >
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class Deque

template <class DataT, class AllocT = ScopeAlloc>
class Deque : public _WINX_DEQUE_BASE(DataT, AllocT)
{
private:
	typedef _WINX_DEQUE_BASE(DataT, AllocT) _Base;

	Deque(const Deque&);
	void operator=(const Deque&);

public:
	typedef typename _Base::size_type size_type;

public:
	explicit Deque(AllocT& _Al)
		: _Base(_Al) {}
	
	Deque(AllocT& _Al, size_type _N, const DataT& _V = DataT())
		: _Base(_N, _V, _Al) {}

	template <class _Iterator>
	Deque(AllocT& _Al, _Iterator _F, _Iterator _L)
		: _Base(_F, _L, _Al) {}

public:
	void winx_call copy(const Deque& _From) {
		_Base::operator=(_From);
	}
};

// -------------------------------------------------------------------------
// class TestDeque

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
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		std::Deque<int> coll(alloc);
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
		std::PerformanceCounter counter;
		{
			DequeT coll;
			for (int i = 0; i < N; ++i)
				coll.push_back(i);
		}
		counter.trace(log);
	}

	void doDeque(LogT& log)
	{
		typedef std::Deque<int> DequeT;
		log.print("===== std::Deque (ScopeAlloc) =====\n");
		std::PerformanceCounter counter;
		{
			std::BlockPool recycle;
			std::ScopeAlloc alloc(recycle);
			DequeT coll(alloc);
			for (int i = 0; i < N; ++i)
				coll.push_back(i);
		}
		counter.trace(log);
	}

	void doShareAllocDeque(LogT& log)
	{
		typedef std::Deque<int> DequeT;
		std::BlockPool recycle;
		log.newline();
		for (int i = 0; i < 5; ++i)
		{
			log.print("===== doShareAllocDeque =====\n");
			std::PerformanceCounter counter;
			{
				std::ScopeAlloc alloc(recycle);
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

// -------------------------------------------------------------------------
// $Log: Deque.h,v $

__NS_STD_END

#endif /* __STDEXT_DEQUE_H__ */
