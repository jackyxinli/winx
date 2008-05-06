//
//  performance.cpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#include <boost/memory.hpp>
#include <boost/detail/performance_counter.hpp>

#if defined(__GNUG__)
#include <ext/mt_allocator.h>
#endif

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>

// -------------------------------------------------------------------------

template <class LogT>
class TestAllocatorPerformance
{
private:
	NS_BOOST_DETAIL::accumulator m_acc;
	NS_BOOST_MEMORY::block_pool m_recycle;

public:
	void doNewDelete(LogT& log, int NAlloc, int PerAlloc)
	{
		int i, **p;
		p = new int*[PerAlloc];
		NS_BOOST_DETAIL::performance_counter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				for (i = 0; i < PerAlloc; ++i)
				{
					p[i] = new int;
				}
				for (i = 0; i < PerAlloc; ++i)
				{
					delete p[i];
				}
			}
		}
		m_acc.accumulate(counter.trace(log));
		delete[] p;
	}

#if defined(__GNUG__)
	void doMtAllocator(LogT& log, int NAlloc, int PerAlloc)
	{
		typedef __gnu_cxx::__mt_alloc<int> allocator_type;
		typedef __gnu_cxx::__pool_base::_Tune tune_type;
		//tune_type tune(16, 5120, 32, 5120, 20, 10, false);
		
		int i, **p;
		p = new int*[PerAlloc];
		NS_BOOST_DETAIL::performance_counter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				allocator_type alloc;
				//alloc._M_set_options(tune);
				for (i = 0; i < PerAlloc; ++i)
				{
					p[i] = alloc.allocate(1);
				}
				for (i = 0; i < PerAlloc; ++i)
				{
					alloc.deallocate(p[i], 1);
				}
			}
		}
		m_acc.accumulate(counter.trace(log));
		delete[] p;
	}
#endif

	void doBoostPool(LogT& log, int NAlloc, int PerAlloc)
	{
		NS_BOOST_DETAIL::performance_counter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				boost::pool<> alloc(sizeof(int));
				for (int i = 0; i < PerAlloc; ++i)
				{
					int* p = (int*)alloc.malloc();
				}
			}
		}
		m_acc.accumulate(counter.trace(log));
	}

	void doBoostObjectPool(LogT& log, int NAlloc, int PerAlloc)
	{
		NS_BOOST_DETAIL::performance_counter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				boost::object_pool<int> alloc;
				for (int i = 0; i < PerAlloc; ++i)
				{
					int* p = alloc.construct();
				}
			}
		}
		m_acc.accumulate(counter.trace(log));
	}

	template <class LogT2>
	void doAutoAlloc(LogT2& log, int NAlloc, int PerAlloc)
	{
		NS_BOOST_DETAIL::performance_counter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				boost::auto_alloc alloc;
				for (int i = 0; i < PerAlloc; ++i)
				{
					int* p = BOOST_NEW(alloc, int);
				}
			}
		}
		m_acc.accumulate(counter.trace(log));
	}

	template <class LogT2>
	void doTlsScopedAlloc(LogT2& log, int NAlloc, int PerAlloc)
	{
		NS_BOOST_DETAIL::performance_counter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				boost::scoped_alloc alloc;
				for (int i = 0; i < PerAlloc; ++i)
				{
					int* p = BOOST_NEW(alloc, int);
				}
			}
		}
		m_acc.accumulate(counter.trace(log));
	}

	template <class LogT2>
	void doScopedAlloc(LogT2& log, int NAlloc, int PerAlloc)
	{
		NS_BOOST_DETAIL::performance_counter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				boost::scoped_alloc alloc(m_recycle);
				for (int i = 0; i < PerAlloc; ++i)
				{
					int* p = BOOST_NEW(alloc, int);
				}
			}
		}
		m_acc.accumulate(counter.trace(log));
	}

	void doComparison(LogT& log, int Total, int NAlloc)
	{
		int i;
		const int Count = 16;
		const int PerAlloc = Total / NAlloc;
		
		m_acc.start();
		log.trace("\n===== boost::auto_alloc(%d) =====\n", PerAlloc);
		for (i = 0; i < Count; ++i)
			doAutoAlloc(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);

		m_acc.start();
		log.trace("\n===== TLS boost::scoped_alloc(%d) =====\n", PerAlloc);
		for (i = 0; i < Count; ++i)
			doTlsScopedAlloc(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);

		m_acc.start();
		log.trace("\n===== boost::scoped_alloc(%d) =====\n", PerAlloc);
		for (i = 0; i < Count; ++i)
			doScopedAlloc(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);

#if defined(__GNUG__)
		m_acc.start();
		log.trace("\n===== MtAllocator(%d) =====\n", PerAlloc);
		for (i = 0; i < Count; ++i)
			doMtAllocator(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);
#endif

		m_acc.start();
		log.trace("\n===== BoostPool(%d) =====\n", PerAlloc);
		for (i = 0; i < Count; ++i)
			doBoostPool(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);

		m_acc.start();
		log.trace("\n===== BoostObjectPool(%d) =====\n", PerAlloc);
		for (i = 0; i < Count; ++i)
			doBoostObjectPool(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);

		m_acc.start();
		log.trace("\n===== NewDelete(%d) =====\n", PerAlloc);
		for (i = 0; i < Count; ++i)
			doNewDelete(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);
	}
	
	void testComparison(LogT& log)
	{
		NS_BOOST_DETAIL::null_log nullLog;
		
		const int Total = 1000000;
		doAutoAlloc(nullLog, Total, 1);
		doTlsScopedAlloc(nullLog, Total, 1);
		doScopedAlloc(nullLog, Total, 1);

		doComparison(log, Total, Total);
		doComparison(log, Total, 1000);
		doComparison(log, Total, 1);
	}
};

// -------------------------------------------------------------------------

void testPerformance()
{
	typedef NS_BOOST_DETAIL::stdout_log LogT;
	LogT log;
	TestAllocatorPerformance<LogT> test;
	test.testComparison(log);
}

// -------------------------------------------------------------------------
// $Log: performance.cpp,v $
//
