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
// Module: TestStdExt.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: TestStdExt.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

#define SGI_DEQUE_UNITTEST
#define STD_UNITTEST
#define STD_FILTER_TEST_CASE
//#define STD_AUTORUN_SETUP
//#define STD_NO_MSVCRT
//#define STD_NO_WINSDK
#define USES_APR
#define USES_BOOST
#include <stdext.h>
#include <stdext/LinkLib.h>
#include <stdext/FileMapping.h>
#include <stdext/ScopeDebug.h>
#include <stdext/Registry.h>
#include <stdext/Map.h>
#include <stdext/HashMap.h>
#include <stdext/HashSet.h>
#include <stdext/AOP.h>
#include <stdext/Set.h>
#include <stdext/List.h>
#include <stdext/Deque.h>
#include <stdext/String.h>
#include <stdext/text/TextPool.h>
#include <stdext/text/Rope.h>
#include <stdext/codepage/CP1252.h>
#include <stdext/Rand.h>
#include <stdext/Archive.h>
#include <stdext/archive/Stream.h>
#include <stdext/kmp/TestFinder.h>
#include <stdext/tchar.h>
#pragma comment(lib, "shlwapi.lib")

// -------------------------------------------------------------------------
// testBoost

#if defined(USES_BOOST)

#include "../../../wrapper/boost/include/wrapper/boost/pool.h"

WINX_AUTORUN_CLASS(TestPool, std::ErrorLog);

#endif

// -------------------------------------------------------------------------
// testApr

#if defined(USES_APR)

#include "../../../wrapper/apr/include/wrapper/apr/apr_pools.h"

WINX_AUTORUN_CLASS(TestAprPools, std::ErrorLog);

#endif

// -------------------------------------------------------------------------
// testStdExt

void testSgiHashMap();

void testStdExt()
{
	WINX_TEST_APP(std::ErrorLog, "TestFinder", "");
//	testSgiHashMap();

	WINX_TEST_CLASS(TestFinder);
#if !defined(STD_NO_WINSDK)
	WINX_TEST_CLASS(std::TestWinRegKey);
	WINX_TEST_CLASS(std::TestWinReg);
#endif
	WINX_TEST_CLASS(std::TestErrorGuard);
	WINX_TEST_CLASS(std::TestScopeLog);
	WINX_TEST_CLASS(std::TestCheckException);
	WINX_TEST_CLASS(std::TestSimpleFileMapping);
	WINX_TEST_CLASS(std::TestFileMapping);
	WINX_TEST_CLASS(std::TestStreamArchive);
	WINX_TEST_CLASS(std::TestStdioArchive);
	WINX_TEST_CLASS(std::TestPosixArchive);
	WINX_TEST_CLASS(std::TestMemArchive);
	WINX_TEST_CLASS(std::TestRecord);
	WINX_TEST_CLASS(std::TestLog);
	WINX_TEST_CLASS(std::TestFileBuf);

	//String
	WINX_TEST_CLASS(std::TestCharType);
	WINX_TEST_CLASS(std::TestCP1252);
	WINX_TEST_CLASS(std::TestString);
	WINX_TEST_CLASS(std::TestStringAlgo);
	WINX_TEST_CLASS(std::TestBasicString);
	WINX_TEST_CLASS(std::TestStringBuilder);
	WINX_TEST_CLASS(std::TestTextPool);
	WINX_TEST_CLASS(std::TestRope);
	WINX_TEST_CLASS(std::TestRand);

	//Collection
	WINX_TEST_CLASS(std::TestPriorityArray);
	WINX_TEST_CLASS(std::TestMap);
	WINX_TEST_CLASS(std::TestSet);
#ifndef _WINX_NO_HASH_MAP
	WINX_TEST_CLASS(std::TestHashMap);
	WINX_TEST_CLASS(std::TestHashSet);
#endif
#ifndef _WINX_NO_SLIST
	WINX_TEST_CLASS(std::TestSlist);
#endif
#if defined(X_CC_VC6) && !defined(NO_SGISTL)
	WINX_TEST_CLASS(TestSgiDeque);
#endif
	WINX_TEST_CLASS(std::TestList);
	WINX_TEST_CLASS(std::TestDclList);
	WINX_TEST_CLASS(std::TestDeque);

	//AOP.h
	WINX_TEST_CLASS(std::TestEvent);
	WINX_TEST_CLASS(std::TestEventContainer);
}

int main()
{
	WINX_SELECT_RUN("testStdExt");
	WINX_AUTORUN_ALL();
	return 0;
}

WINX_AUTORUN(testStdExt);

// -------------------------------------------------------------------------
// $Log: TestStdExt.cpp,v $
