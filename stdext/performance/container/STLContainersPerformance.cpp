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
// Module: STLContainersPerformance.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: STLContainersPerformance.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <stdext/Counter.h>
#include <stdext/Set.h>
#include <stdext/HashSet.h>
#include <stdext/Map.h>
#include <stdext/HashMap.h>
#include <stdext/List.h>
#include <stdext/Deque.h>
#include <deque>

// -------------------------------------------------------------------------

template <class LogT>
class TestSTLContainersPerformance
{
private:
	std::Accumulator m_acc;

public:
	enum { Count = 1000000 };
	enum { TestN = 16 };

	void doDeque(LogT& log)
	{
		std::BlockPool recycle;

		log.print("\n===== Deque (ScopedAlloc) =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				std::ScopedAlloc alloc(recycle);
				std::Deque<int> s(alloc);
				for (int i = 0; i < Count; ++i)
					s.push_back(i);
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}

	void doStlDeque(LogT& log)
	{
		log.print("\n===== std::deque =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				std::deque<int> s;
				for (int i = 0; i < Count; ++i)
					s.push_back(i);
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}

	void doList(LogT& log)
	{
		std::BlockPool recycle;

		log.print("\n===== List (ScopedAlloc) =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				std::ScopedAlloc alloc(recycle);
				std::List<int> s(alloc);
				for (int i = 0; i < Count; ++i)
					s.push_back(i);
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}

	void doStlList(LogT& log)
	{
		log.print("\n===== std::list =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				std::list<int> s;
				for (int i = 0; i < Count; ++i)
					s.push_back(i);
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}

	void doSet(LogT& log)
	{
		std::BlockPool recycle;

		log.print("\n===== Set (ScopedAlloc) =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				std::ScopedAlloc alloc(recycle);
				std::Set<int> s(alloc);
				for (int i = 0; i < Count; ++i)
					s.insert(i);
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}

	void doStlSet(LogT& log)
	{
		log.print("\n===== std::set =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				std::set<int> s;
				for (int i = 0; i < Count; ++i)
					s.insert(i);
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}
	
	void doMap(LogT& log)
	{
		std::BlockPool recycle;

		log.print("\n===== Map (ScopedAlloc) =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				std::ScopedAlloc alloc(recycle);
				std::Map<int, int> s(alloc);
				for (int i = 0; i < Count; ++i)
					s.insert(std::Map<int, int>::value_type(i, i));
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}

	void doStlMap(LogT& log)
	{
		log.print("\n===== std::map =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				std::map<int, int> s;
				for (int i = 0; i < Count; ++i)
					s.insert(std::map<int, int>::value_type(i, i));
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}

	void doHashSet(LogT& log)
	{
		std::BlockPool recycle;

		log.print("\n===== HashSet (ScopedAlloc) =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				std::ScopedAlloc alloc(recycle);
				std::HashSet<int> s(alloc);
				for (int i = 0; i < Count; ++i)
					s.insert(i);
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}

#if !defined(WINX_VC6)
	void doStlHashSet(LogT& log)
	{
		log.print("\n===== stdext::hash_set =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				stdext::hash_set<int> s;
				for (int i = 0; i < Count; ++i)
					s.insert(i);
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}
#endif

	void doHashMap(LogT& log)
	{
		std::BlockPool recycle;

		log.print("\n===== HashMap (ScopedAlloc) =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				std::ScopedAlloc alloc(recycle);
				std::HashMap<int, int> s(alloc);
				for (int i = 0; i < Count; ++i)
					s.insert(std::HashMap<int, int>::value_type(i, i));
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}

#if !defined(WINX_VC6)
	void doStlHashMap(LogT& log)
	{
		log.print("\n===== stdext::hash_map =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				stdext::hash_map<int, int> s;
				for (int i = 0; i < Count; ++i)
					s.insert(stdext::hash_map<int, int>::value_type(i, i));
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}
#endif

	void testComparison(LogT& log)
	{
		doDeque(log);
		doStlDeque(log);
		doList(log);
		doStlList(log);
		doSet(log);
		doStlSet(log);
		doHashSet(log);
#if !defined(WINX_VC6)
		doStlHashSet(log);
#endif
		doMap(log);
		doStlMap(log);
		doHashMap(log);
#if !defined(WINX_VC6)
		doStlHashMap(log);
#endif
	}
};

int main()
{
	std::ErrorLog log;
	TestSTLContainersPerformance<std::ErrorLog> test;
	test.testComparison(log);
	return 0;
}

// -------------------------------------------------------------------------
// $Log: STLContainersPerformance.cpp,v $
//
