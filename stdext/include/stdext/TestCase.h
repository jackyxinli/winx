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
// Module: stdext/TestCase.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-25 16:52:33
// 
// $Id: TestCase.h,v 1.4 2007/01/10 09:32:25 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_TESTCASE_H__
#define __STDEXT_TESTCASE_H__

#ifndef __STDEXT_BASIC_H__
#include "Basic.h"
#endif

#ifndef __STD_ITERATOR_H__
#include "../std/iterator.h"
#endif

#if !defined(_INC_STDIO) && !defined(_STDIO_H)
#include <stdio.h>
#endif

#if defined(_DEBUG)
#define STD_FILTER_TEST_CASE
#endif

__NS_STD_BEGIN

// =========================================================================
// class TestRunner

template <class LogT>
class TestRunner : public LogT
{
private:
	LPCSTR szTestClass;
	LPCSTR szTestCase;

	LPCSTR szTestClassSel;
	LPCSTR szTestCaseSel;
	
public:
	TestRunner() : 
	  szTestClass("$UnknownTestClass"), szTestCase("$UnknownTestCase"),
	  szTestClassSel(""), szTestCaseSel("") {
	}

	void winx_call select(LPCSTR testClass = "", LPCSTR testCase = "")
	{
#if defined(STD_FILTER_TEST_CASE)
		WINX_ASSERT(testClass != NULL);
		WINX_ASSERT(testCase != NULL);
		
		szTestClassSel = testClass;
		szTestCaseSel = testCase;
#endif
	}

	void winx_call setTestCase(LPCSTR testCase)
	{
		szTestCase = testCase;
	}

public:
	bool winx_call runableTestClass(LPCSTR testClass)
	{
#if defined(STD_FILTER_TEST_CASE)
		if (strstr(testClass, szTestClassSel) == NULL)
			return false;
#endif
		szTestClass = testClass;
		LogT::step('*');
		return true;
	}

	bool winx_call runableTestCase(LPCSTR testCase)
	{
#if defined(STD_FILTER_TEST_CASE)
		if (*szTestCaseSel != 0 && strcmp(testCase, szTestCaseSel) != 0)
			return false;
#endif
		szTestCase = testCase;
		LogT::step();
		return true;
	}

public:
	void winx_call reportError(LPCSTR msg, const char* szFile, int nLine)
	{
		LogT::reportTestCaseError(szTestClass, szTestCase, msg, szFile, nLine);
	}
};

// -------------------------------------------------------------------------
// WINX_DBG_PAUSE

inline void pause()
{
	printf("Press <return> key to continue");
	getchar();
}

struct PauseOnExit {
	~PauseOnExit() { pause(); }
};

#if defined(_DEBUG) && defined(WINX_GCC)
#define WINX_DBG_PAUSE_ON_EXIT()	std::PauseOnExit _winx_pause_on_exit
#define WINX_DBG_PAUSE()			std::pause()
#else
#define WINX_DBG_PAUSE_ON_EXIT()
#define WINX_DBG_PAUSE()
#endif

// -------------------------------------------------------------------------
// TestApp

#define WINX_TESTCASE_LOG(LogT)												\
	_CrtSetDbgFlag(															\
		_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|_CRTDBG_LEAK_CHECK_DF);			\
	typedef std::TestRunner< LogT > TestCaseLog;							\
	TestCaseLog log

#define WINX_TEST_APP(LogT, Test, Case)										\
	WINX_DBG_PAUSE_ON_EXIT();												\
	WINX_TESTCASE_LOG(LogT);												\
	log.select(Test, Case)

// -------------------------------------------------------------------------
// TestClass

#define WINX_TEST_CLASS(Test)												\
	try {																	\
		if (log.runableTestClass(#Test))									\
			Test<TestCaseLog>().doTest(log);								\
	} catch(...) {															\
		log.setTestCase("doTest");											\
		log.reportError(													\
			"Failed: Unexpected exception!!!", __FILE__, __LINE__);			\
	}

// -------------------------------------------------------------------------
// TestCase

#define WINX_DEFINE_TEST(Test)												\
	typedef Test ThisTestClass												\

#define WINX_TEST_CASE(Case)												\
	try {																	\
		if (log.runableTestCase(#Case))										\
			ThisTestClass().Case(log);										\
	} catch(...) {															\
		log.reportError(													\
			"Failed: Unexpected exception!!!", __FILE__, __LINE__);			\
	}

// -------------------------------------------------------------------------
// TestCase - CppUnit Compatibility

class TestCase
{
public:
	void setUp() {}
	void tearDown() {}
};

typedef TestCase TestFixture;

//@see CPPUNIT_TEST_SUITE
#define WINX_TEST_SUITE(Test)												\
public:																		\
	void doTest(LogT& log)													\
	{																		\
		WINX_DEFINE_TEST(Test);

//@see CPPUNIT_TEST
#define WINX_TEST(Case)														\
		try {																\
			if (log.runableTestCase(#Case))	{								\
				ThisTestClass obj;											\
				obj.setUp();												\
				obj.Case(log);												\
				obj.tearDown();												\
			}																\
		} catch(...) {														\
			log.reportError(												\
				"Failed: Unexpected exception!!!", __FILE__, __LINE__);		\
		}

//@see CPPUNIT_TEST_SUITE_END
#define WINX_TEST_SUITE_END()												\
	}

// =========================================================================
// isEqBuf

template <class _It1, class _It2>
bool winx_call isEqBuf(_It1 a1, _It2 a2, size_t count)
{
	_It1 last = a1;
	std::advance(last, count);
	return std::equal(a1, last, a2);
}

// -------------------------------------------------------------------------
// AssertExp, AssertEq, AssertEqBuf, AssertFail, ReportErrorMsgIf

#ifndef ReportErrorMsgIf
#define ReportErrorMsgIf(msg, exp) 											\
	do {																	\
		if (!(exp)) {														\
			log.reportError(msg, __FILE__, __LINE__);						\
			WINX_DBG_BREAK();												\
		}																	\
	} while (0)
#endif

#define AssertFail(msg)														\
	ReportErrorMsgIf(msg, 0)

#define AssertExp(exp)														\
	ReportErrorMsgIf("Failed: AssertExp(" #exp ");", exp)

#define AssertEq(a, b)														\
	ReportErrorMsgIf("Failed: AssertEq(" #a "," #b ");", (a) == (b))

#define AssertEqBuf(a1, a2, count)											\
	ReportErrorMsgIf("Failed: AssertEqBuf(" #a1 "," #a2 "," #count ");", std::isEqBuf(a1, a2, count))

// =========================================================================
// WINX_SELECT_RUN, WINX_AUTORUN, WINX_AUTORUN_CLASS

template <int n>
struct __SelectFun
{
	static const char* _g_name;
};

#define WINX_SELECT_RUN(szFun)												\
	template <> const char* std::__SelectFun<0>::_g_name = szFun

template <class FunT>
int __autoRun(FunT Fun, const char* szFun)
{
	const char* szSelFun = __SelectFun<0>::_g_name;
	if (szSelFun == NULL || strstr(szFun, szSelFun))
	{
		printf("------------------------- %s -------------------------\n", szFun);
		Fun();
	}
	return 0;
}

#define WINX_AUTORUN(Fun)													\
	static int __g_autoRun_ ## Fun = std::__autoRun(Fun, #Fun)

#define WINX_AUTORUN_CLASS(Test, LogT)										\
	inline void __autoRun_##Test()											\
	{																		\
		WINX_TESTCASE_LOG(LogT);											\
		WINX_TEST_CLASS(Test);												\
	}																		\
	WINX_AUTORUN(__autoRun_##Test)

// =========================================================================
// $Log: TestCase.h,v $
// Revision 1.1  2006/09/26 07:51:00  xushiwei
// STL-Extension:
//  TestCase(WINX_TEST_APP, WINX_TEST_CLASS, WINX_TEST_SUITE, WINX_TEST, WINX_TEST_SUITE_END)
//  UnitTestAssert(AssertExp, AssertEq, AssertEqBuf)
//

__NS_STD_END

#endif /* __STDEXT_TESTCASE_H__ */
