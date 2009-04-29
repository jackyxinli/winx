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
// Module: AllocatorPerformance.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: AllocatorPerformance.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <stdext/Memory.h>
#include <stdext/Counter.h>
#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>

#if !defined(X_OS_WINDOWS)
#include <apr/apr_pools.h>
#include <ext/mt_allocator.h>
#endif

#if defined(TEST_DLMALLOC)
#include "../../wrapper/dlmalloc/include/wrapper/dlmalloc.h"
#endif

// -------------------------------------------------------------------------

template <class LogT>
class TestAllocatorPerformance
{
private:
	apr_pool_t* m_pool;
	NS_STDEXT::Accumulator m_acc;
	NS_STDEXT::BlockPool m_recycle;

	void __setUp()
	{
		apr_pool_initialize();
		apr_pool_create(&m_pool, NULL);
		apr_allocator_mutex_set(apr_pool_allocator_get(m_pool), NULL);
	}

	void __tearDown()
	{
		apr_pool_destroy(m_pool);
		apr_pool_terminate();
	}

public:
	void doNewDelete(LogT& log, int NAlloc, int PerAlloc)
	{
		int i, **p;
		p = new int*[PerAlloc];
		NS_STDEXT::PerformanceCounter counter;
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

#if defined(TEST_DLMALLOC)
	void doDLMalloc(LogT& log, int NAlloc, int PerAlloc)
	{
		int i, **p;
		p = (int**)dlmalloc(sizeof(int*)*PerAlloc);
		NS_STDEXT::PerformanceCounter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				for (i = 0; i < PerAlloc; ++i)
				{
					p[i] = (int*)dlmalloc(sizeof(int));
				}
				for (i = 0; i < PerAlloc; ++i)
				{
					dlfree(p[i]);
				}
			}
		}
		m_acc.accumulate(counter.trace(log));
		dlfree(p);
	}
#endif

#if !defined(X_OS_WINDOWS)
	void doMtAllocator(LogT& log, int NAlloc, int PerAlloc)
	{
		typedef __gnu_cxx::__mt_alloc<int> allocator_type;
		typedef __gnu_cxx::__pool_base::_Tune tune_type;
		//tune_type tune(16, 5120, 32, 5120, 20, 10, false);
		
		int i, **p;
		p = new int*[PerAlloc];
		NS_STDEXT::PerformanceCounter counter;
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
#endif // !defined(X_OS_WINDOWS)

	void doBoostPool(LogT& log, int NAlloc, int PerAlloc)
	{
		NS_STDEXT::PerformanceCounter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				boost::pool<> alloc(sizeof(int));
				for (int i = 0; i < PerAlloc; ++i)
				{
					int* p;
					p = (int*)alloc.malloc();
				}
			}
		}
		m_acc.accumulate(counter.trace(log));
	}

	void doBoostObjectPool(LogT& log, int NAlloc, int PerAlloc)
	{
		NS_STDEXT::PerformanceCounter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				boost::object_pool<int> alloc;
				for (int i = 0; i < PerAlloc; ++i)
				{
					int* p;
					p = alloc.construct();
				}
			}
		}
		m_acc.accumulate(counter.trace(log));
	}

	template <class LogT2>
	void doAprPools(LogT2& log, int NAlloc, int PerAlloc)
	{
		NS_STDEXT::PerformanceCounter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				apr_pool_t* alloc;
				apr_pool_create(&alloc, m_pool);
				for (int i = 0; i < PerAlloc; ++i)
				{
					int* p;
					p = (int*)apr_palloc(alloc, sizeof(int));
				}
				apr_pool_destroy(alloc);
			}
		}
		m_acc.accumulate(counter.trace(log));
	}

	template <class LogT2>
	void doAutoFreeAlloc(LogT2& log, int NAlloc, int PerAlloc)
	{
		NS_STDEXT::PerformanceCounter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				NS_STDEXT::AutoAlloc alloc;
				for (int i = 0; i < PerAlloc; ++i)
				{
					int* p;
					p = STD_NEW(alloc, int);
				}
			}
		}
		m_acc.accumulate(counter.trace(log));
	}

	template <class LogT2>
	void doTlsScopedAlloc(LogT2& log, int NAlloc, int PerAlloc)
	{
		NS_STDEXT::PerformanceCounter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				NS_STDEXT::ScopedAlloc alloc;
				for (int i = 0; i < PerAlloc; ++i)
				{
					int* p;
					p = STD_NEW(alloc, int);
				}
			}
		}
		m_acc.accumulate(counter.trace(log));
	}

	template <class LogT2>
	void doScopedAlloc(LogT2& log, int NAlloc, int PerAlloc)
	{
		NS_STDEXT::PerformanceCounter counter;
		{
			for (int j = 0; j < NAlloc; ++j)
			{
				NS_STDEXT::ScopedAlloc alloc(m_recycle);
				for (int i = 0; i < PerAlloc; ++i)
				{
					int* p;
					p = STD_NEW(alloc, int);
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
		log.print(PerAlloc, "\n===== AutoAlloc(%d) =====\n");
		for (i = 0; i < Count; ++i)
			doAutoFreeAlloc(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);

		m_acc.start();
		log.print(PerAlloc, "\n===== APR Pools(%d) =====\n");
		for (i = 0; i < Count; ++i)
			doAprPools(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);

		m_acc.start();
		log.print(PerAlloc, "\n===== TlsScopedAlloc(%d) =====\n");
		for (i = 0; i < Count; ++i)
			doTlsScopedAlloc(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);

		m_acc.start();
		log.print(PerAlloc, "\n===== ScopedAlloc(%d) =====\n");
		for (i = 0; i < Count; ++i)
			doScopedAlloc(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);

#if !defined(X_OS_WINDOWS)
		m_acc.start();
		log.print(PerAlloc, "\n===== MtAllocator(%d) =====\n");
		for (i = 0; i < Count; ++i)
			doMtAllocator(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);
#endif

#if defined(TEST_DLMALLOC)
		m_acc.start();
		log.print(PerAlloc, "\n===== DLMalloc(%d) =====\n");
		for (i = 0; i < Count; ++i)
			doDLMalloc(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);
#endif
		
		m_acc.start();
		log.print(PerAlloc, "\n===== BoostPool(%d) =====\n");
		for (i = 0; i < Count; ++i)
			doBoostPool(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);

		m_acc.start();
		log.print(PerAlloc, "\n===== BoostObjectPool(%d) =====\n");
		for (i = 0; i < Count; ++i)
			doBoostObjectPool(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);

		m_acc.start();
		log.print(PerAlloc, "\n===== NewDelete(%d) =====\n");
		for (i = 0; i < Count; ++i)
			doNewDelete(log, NAlloc, PerAlloc);
		m_acc.trace_avg(log);
	}
	
	void testComparison(LogT& log)
	{
		NS_STDEXT::NullLog nullLog;
		const int Total = 1000000;
		__setUp();
		doAutoFreeAlloc(nullLog, Total, 1);
		doTlsScopedAlloc(nullLog, Total, 1);
		doScopedAlloc(nullLog, Total, 1);
		doAprPools(nullLog, Total, 1);
		doComparison(log, Total, Total);
		doComparison(log, Total, 1000);
		doComparison(log, Total, 1);
		__tearDown();
	}
};

int main()
{
	NS_STDEXT::ErrorLog log;
	TestAllocatorPerformance<NS_STDEXT::ErrorLog> test;
	test.testComparison(log);
	return 0;
}

// -------------------------------------------------------------------------
// $Log: AllocatorPerformance.cpp,v $
//
