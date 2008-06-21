//
//  system_alloc_perform.cpp
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
#include <boost/memory/system_pool.hpp>
#include <boost/detail/performance_counter.hpp>

using namespace NS_BOOST_MEMORY;
using namespace NS_BOOST_MEMORY;

typedef system_pool_s<NS_BOOST_MEMORY_POLICY::stdlib, normal_stack> system_pool_static;

enum { NTimes = 100 };
enum { Total = 400 };

static void* p[Total];

template <class LogT>
class TestSystemAlloc
{
public:
	template <class LogT2, class AllocT>
	void doTestAlloc(LogT2& log, AllocT& alloc, int N, int M = 16)
	{
		NS_BOOST_DETAIL::accumulator acc;
		for (int i, j = 0; j < M; ++j)
		{
			NS_BOOST_DETAIL::performance_counter counter;
			{
				for (int k = 0; k < N; ++k)
				{
					for (i = 0; i < Total; ++i)
					{
						p[i] = alloc.allocate(BOOST_MEMORY_BLOCK_SIZE);
					}
					for (i = 0; i < Total; ++i)
					{
						alloc.deallocate(p[i]);
					}
				}
			}
			acc.accumulate(counter.trace(log));
		}
		acc.trace_avg(log);
	}

	void test(LogT& log)
	{
		system_pool_dyn sysPool;
		system_pool_static sysPool2;
		stdlib_alloc stdLib;

		NS_BOOST_DETAIL::null_log nullLog;
		doTestAlloc(nullLog, sysPool, 1, 1);
		doTestAlloc(nullLog, sysPool2, 1, 1);

		log.trace("\n===== SystemPoolAlloc (Lock) =====\n");
		doTestAlloc(log, sysPool2, NTimes);

		log.trace("\n===== SystemPoolAlloc (LockFree) =====\n");
		doTestAlloc(log, sysPool, NTimes);

		log.trace("\n===== StdLibAlloc =====\n");
		doTestAlloc(log, stdLib, NTimes);
	}
};

void testSystemAlloc()
{
	typedef NS_BOOST_DETAIL::stdout_log LogT;
	LogT log;
	TestSystemAlloc<LogT> test;
	test.test(log);
}
