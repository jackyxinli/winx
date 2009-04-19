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
// Module: RopePerformance.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: RopePerformance.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <stdext/text/Rope.h>
#include <stdext/Counter.h>

#if defined(WINX_GCC)
#include <ext/rope>
#endif

// -------------------------------------------------------------------------

template <class LogT>
class TestRopePerformance
{
private:
	std::Accumulator m_acc;

public:
	enum { Count = 1000000 };
	enum { TestN = 16 };

	void doRope(LogT& log)
	{
		std::BlockPool recycle;

		log.print("\n===== Rope (ScopedAlloc) =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				std::ScopedAlloc alloc(recycle);
				std::Rope<char> s(alloc);
				for (int i = 0; i < Count; ++i)
					s.push_back('a' + (i % 26));
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}

#if defined(WINX_GCC)
	void doSgiRope(LogT& log)
	{
		log.print("\n===== rope (GNU C++) =====\n");
		m_acc.start();
		for (int j = 0; j < TestN; ++j)
		{
			std::PerformanceCounter counter;
			{
				stdext::rope<char> s;
				for (int i = 0; i < Count; ++i)
					s.push_back('a' + (i % 26));
			}
			m_acc.accumulate(counter.trace(log));
		}
		m_acc.trace_avg(log);
	}
#endif
	
	void testComparison(LogT& log)
	{
		doRope(log);
#if defined(WINX_GCC)
		doSgiRope(log);
#endif
	}
};

int main()
{
	std::ErrorLog log;
	TestRopePerformance<std::ErrorLog> test;
	test.testComparison(log);
	return 0;
}

// -------------------------------------------------------------------------
// $Log: RopePerformance.cpp,v $
//
