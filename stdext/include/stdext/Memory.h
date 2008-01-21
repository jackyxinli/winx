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
// Module: stdext/Memory.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 15:43:17
// 
// $Id: Memory.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_H__
#define __STDEXT_MEMORY_H__

// -------------------------------------------------------------------------

#ifndef __STDEXT_MEMORY_BASIC_H__
#include "memory/Basic.h"
#endif

#ifndef __STDEXT_MEMORY_AUTOFREEALLOC_H__
#include "memory/AutoFreeAlloc.h"
#endif

#ifndef __STDEXT_MEMORY_AUTOARRAY_H__
#include "memory/AutoArray.h"
#endif

#ifndef __STDEXT_MEMORY_RECYCLEBUFFER_H__
#include "memory/RecycleBuffer.h"
#endif

#ifndef __STDEXT_MEMORY_SCOPEALLOC_H__
#include "memory/ScopeAlloc.h"
#endif

// -------------------------------------------------------------------------
// class TestCompareAllocators

#ifndef __STDEXT_COUNTER_H__
#include "Counter.h"
#endif

__NS_STD_BEGIN

template <class LogT>
class TestCompareAllocators : public TestCase
{
	WINX_TEST_SUITE(TestCompareAllocators);
		WINX_TEST(testComparison1);
		WINX_TEST(testComparison2);
	WINX_TEST_SUITE_END();

public:
	enum { N = 60000 };

	void doNewDelete(LogT& log)
	{
		log.print("===== NewDelete =====\n");
		PerformanceCounter counter;
		for (int i = 0; i < N; ++i)
		{
			int* p = new int;
			delete p;
		}
		counter.trace(log);
	}

	void doAutoFreeAlloc1(LogT& log)
	{
		log.print("===== AutoFreeAlloc =====\n");
		PerformanceCounter counter;
		for (int i = 0; i < N; ++i)
		{
			AutoFreeAlloc alloc;
			int* p = STD_NEW(alloc, int);
		}
		counter.trace(log);
	}

	void doScopeAlloc1(LogT& log)
	{
		log.print("===== ScopeAlloc =====\n");
		BlockPool recyle;
		PerformanceCounter counter;
		for (int i = 0; i < N; ++i)
		{
			ScopeAlloc alloc(recyle);
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
			doNewDelete(log);
			doScopeAlloc1(log);
		}
	}

public:
	void doAutoFreeAlloc2(LogT& log)
	{
		log.print("===== AutoFreeAlloc =====\n");
		PerformanceCounter counter;
		{
			AutoFreeAlloc alloc;
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
		BlockPool recyle;
		PerformanceCounter counter;
		{
			ScopeAlloc alloc(recyle);
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
			doNewDelete(log);
			doScopeAlloc2(log);
		}
	}
};

__NS_STD_END

// -------------------------------------------------------------------------
// $Log: Memory.h,v $
// Revision 1.1  2006/10/18 12:13:39  xushiwei
// stdext as independent component
//
// Revision 1.2  2006/08/19 04:40:48  xushiwei
// STL-Extension:
//   Memory(AutoFreeAlloc, RecycleBuffer, AutoArray, etc)
//   String Algorithm(trim, match, compare, etc), Container(SimpleMultiMap), CharType(isCSymbolFirstChar, etc)
//   Log(OutputLog, ErrorLog, FileLog, StringLog), PerformanceCounter, Diagnost(WINX_ASSERT, WINX_RUN_TEST, etc)
//

#endif /* __STDEXT_MEMORY_H__ */
