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
// Module: stdext/memory/apr_pools.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: TestBoostRegEx.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_APR_POOLS_H__
#define __STDEXT_MEMORY_APR_POOLS_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

#if (0)
#define WINX_USE_DEFAPR
#define APR_DECLARE_STATIC
#endif

// -------------------------------------------------------------------------

#if defined(WINX_USE_DEFAPR)

#ifndef APR_POOLS_H
#include <apr_pools.h>
#endif

#else

#ifndef APR_POOLS_H
#include "../../apr/include/apr_pools.h"
#endif

#endif

// -------------------------------------------------------------------------
// Link apr lib

#if defined(APR_DECLARE_STATIC)

#pragma comment(lib, "apr-1.lib")
#pragma comment(lib, "wsock32.lib")

#else

#pragma comment(lib, "libapr-1.lib")

#endif

// -------------------------------------------------------------------------
// class TestAprPools

#ifndef __STDEXT_COUNTER_H__
#include "Counter.h"
#endif

#ifndef __STDEXT_MEMORY_H__
#include "Memory.h"
#endif

#if defined(STD_UNITTEST)

template <class LogT>
class TestAprPools : public TestCase
{
	WINX_TEST_SUITE(TestAprPools);
		WINX_TEST(testComparison1);
		WINX_TEST(testComparison2);
	WINX_TEST_SUITE_END();

private:
	apr_pool_t* m_pool;

	void setUp()
	{
		apr_pool_initialize();
		apr_pool_create(&m_pool, NULL);
#if !defined(_MT)
		apr_allocator_mutex_set(apr_pool_allocator_get(m_pool), NULL);
#endif
	}

	void tearDown()
	{
		apr_pool_destroy(m_pool);
		apr_pool_terminate();
	}

public:
	enum { N = 60000 };

	void doNewDelete1(LogT& log)
	{
		log.print("===== NewDelete =====\n");
		std::PerformanceCounter counter;
		for (int i = 0; i < N; ++i)
		{
			int* p = new int;
			delete p;
		}
		counter.trace(log);
	}

	void doAprPools1(LogT& log)
	{
		log.print("===== APR Pools =====\n");
		std::PerformanceCounter counter;
		for (int i = 0; i < N; ++i)
		{
			apr_pool_t* alloc;
			apr_pool_create(&alloc, m_pool);
			int* p = (int*)apr_palloc(alloc, sizeof(int));
			apr_pool_destroy(alloc);
		}
		counter.trace(log);
	}

	void doAutoFreeAlloc1(LogT& log)
	{
		log.print("===== AutoFreeAlloc =====\n");
		std::PerformanceCounter counter;
		for (int i = 0; i < N; ++i)
		{
			std::AutoFreeAlloc alloc;
			int* p = STD_NEW(alloc, int);
		}
		counter.trace(log);
	}

	void doScopeAlloc1(LogT& log)
	{
		log.print("===== ScopeAlloc =====\n");
		std::BlockPool recycle;
		std::PerformanceCounter counter;
		for (int i = 0; i < N; ++i)
		{
			std::ScopeAlloc alloc(recycle);
			int* p = STD_NEW(alloc, int);
		}
		counter.trace(log);
	}

	void testComparison1(LogT& log)
	{
		for (int i = 0; i < 4; ++i)
		{
			log.newline();
			doAutoFreeAlloc1(log);
			doAprPools1(log);
			doNewDelete1(log);
			doScopeAlloc1(log);
		}
	}

public:
	void doNewDelete2(LogT& log)
	{
		int i, *p[N];
		log.print("===== NewDelete =====\n");
		std::PerformanceCounter counter;
		for (i = 0; i < N; ++i)
		{
			p[i] = new int;
		}
		for (i = 0; i < N; ++i)
		{
			delete p[i];
		}
		counter.trace(log);
	}

	void doAprPools2(LogT& log)
	{
		log.print("===== APR Pools =====\n");
		std::PerformanceCounter counter;
		{
			apr_pool_t* alloc;
			apr_pool_create(&alloc, m_pool);
			for (int i = 0; i < N; ++i)
			{
				int* p = (int*)apr_palloc(alloc, sizeof(int));
			}
			apr_pool_destroy(alloc);
		}
		counter.trace(log);
	}

	void doAutoFreeAlloc2(LogT& log)
	{
		log.print("===== AutoFreeAlloc =====\n");
		std::PerformanceCounter counter;
		{
			std::AutoFreeAlloc alloc;
			for (int i = 0; i < N; ++i)
			{
				int* p = STD_NEW(alloc, int);
			}
		}
		counter.trace(log);
	}

	void doScopeAlloc2(LogT& log)
	{
		log.print("===== ScopeAlloc =====\n");
		std::BlockPool recycle;
		std::PerformanceCounter counter;
		{
			std::ScopeAlloc alloc(recycle);
			for (int i = 0; i < N; ++i)
			{
				int* p = STD_NEW(alloc, int);
			}
		}
		counter.trace(log);
	}

	void testComparison2(LogT& log)
	{
		for (int i = 0; i < 4; ++i)
		{
			log.newline();
			doAutoFreeAlloc2(log);
			doAprPools2(log);
			doNewDelete2(log);
			doScopeAlloc2(log);
		}
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: apr_pools.h,v $
//

#endif /* __STDEXT_MEMORY_APR_POOLS_H__ */
