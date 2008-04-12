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
// Module: cppunit/basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-14 17:57:09
// 
// $Id: basic.h,v 1.1 2006/11/30 08:45:42 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __CPPUNIT_BASIC_H__
#define __CPPUNIT_BASIC_H__

// -------------------------------------------------------------------------

#ifndef __CPPUNIT_PLATAPI_H__
#include "platapi.h"
#endif

#if !defined(W2A)
#include <atlbase.h>
#endif

#ifndef _STRING_
#include <string>
#endif

#ifndef _INC_CRTDBG
#include <crtdbg.h>
#endif

// -------------------------------------------------------------------------
// class TestCase

#if defined(STD_NO_GLOBAL_TESTCASE)

class TestCase : public TestFixture {};

#else

class CppUnitTestCase : public TestFixture {};

#define TestCase CppUnitTestCase

#endif

// -------------------------------------------------------------------------
// _CppUnit_GetModuleHandleEx

inline HMODULE _CppUnit_GetModuleHandleEx(LPCVOID lpAddress)
{
	MEMORY_BASIC_INFORMATION mInfo;
	VirtualQuery(lpAddress, &mInfo, sizeof(mInfo));
	return (HMODULE)mInfo.AllocationBase;
}

// -------------------------------------------------------------------------
// _CppUnit_GetModuleName

inline VOID _DummyFunction() {}

inline LPCWSTR _CppUnit_GetModuleName()
{
	static WCHAR szModule[_MAX_PATH];
	static int cch = GetModuleFileNameW(
		_CppUnit_GetModuleHandleEx((void*)_DummyFunction), szModule, _MAX_PATH);
	return szModule;
}

// -------------------------------------------------------------------------
// _CppUnit_Abort

#if defined(_DEBUG)
#define _CppUnit_Abort()	_ASSERTE(!"CppUnit Error: Program abort!!!")
#else
#define _CppUnit_Abort()	_CppUnit_TestRunnerError("CppUnit Error: Program abort!!!")
#endif

// =========================================================================
//
//	TestApp app(rununitConsole);
//	app.AddUnit(szDllName);
//

// -------------------------------------------------------------------------
// class TestApp

class TestApp
{
private:
	LPCSTR m_name;

public:
	TestApp(EnumRunUnitType runtype = rununitNone, LPCSTR name = NULL)
	{
		m_name = name;
		_CppUnit_Initialize(runtype);
		_CppUnit_FilterCase(_CppUnit_GetModuleName(), NULL, NULL);
	}
	~TestApp()
	{
		try
		{
			_CppUnit_RunTests_ByName(__argc, __wargv, m_name);
		}
		catch (...)
		{
			_CppUnit_Abort();
		}
		_CppUnit_Terminate();
	}

	static HRESULT AddUnit(LPCSTR szDll)
	{
		return LoadLibraryA(szDll) ? S_OK : E_ACCESSDENIED;
	}

	static HRESULT AddUnit(LPCWSTR szDll)
	{		
		USES_CONVERSION;
		return LoadLibraryA(W2A(szDll)) ? S_OK : E_ACCESSDENIED;
	}
};

// =========================================================================
//
//	CPPUNIT_TEST_SUITE(TestCaseClass);
//		CPPUNIT_TEST(testMethod1);
//		CPPUNIT_TEST(testMethod2);
//		...
//	CPPUNIT_TEST_SUITE_END();
//

// -------------------------------------------------------------------------
// class TestFixtureCreator

template <class TestFixtureType>
class TestFixtureCreator
{
public:
	static void makeFixture(TestFixture** ppv)
	{
		*ppv = new TestFixtureType();
	}
};

// -------------------------------------------------------------------------

#define CPPUNIT_TEST_SUITE(ATestFixtureType)                           		\
public:                                                                		\
	typedef ATestFixtureType TestFixtureType;								\
	static const char* __getFixtureName()									\
	{																		\
		return #ATestFixtureType;											\
	}																		\
    static void addTestsToSuite(TestSuiteBuilderContextBase* baseContext )	\
    {																		\
		TestSuiteBuilderContextType* context;								\
		_CppUnit_CreateTestSuiteBuilderContext(baseContext, &context)

#define CPPUNIT_TEST(testMethod)											\
		_CppUnit_AddTest(													\
			context, 														\
			__getFixtureName(),												\
			#testMethod,													\
			(FnTestMethod)&TestFixtureType::testMethod						\
			)

#define CPPUNIT_TEST_EXCEPTION(testMethod, ExceptionType)

#define CPPUNIT_TEST_SUITE_END()                                           	\
		_CppUnit_DeleteTestSuiteBuilderContext(context);					\
    }																		\
																			\
    static void suite(TestSuite** ppSuite)                                  \
    {																		\
		_CppUnit_SuiteEx(													\
			TestFixtureCreator<TestFixtureType>::makeFixture,				\
			TestFixtureType::addTestsToSuite,								\
			__getFixtureName(),												\
			_CppUnit_GetModuleName(),										\
			ppSuite);														\
	}																		\
private: /* dummy typedef so that the macro can still end with ';'*/		\
	typedef int CppUnitDummyTypedefForSemiColonEnding__

// =========================================================================
//
//	CPPUNIT_REPORT(msg); or
//	CPPUNIT_ASSERT(cond); or
//	CPPUNIT_ASSERT_OK(hr); or
//	CPPUNIT_ASSERT_EQ(x, y);
//

// -------------------------------------------------------------------------
// CPPUNIT_REPORT

#define CPPUNIT_REPORT(szMsg)												\
	if (_CppUnit_IsDebugMode())												\
		_RPTF0(_CRT_ASSERT, szMsg);											\
	else																	\
		_CppUnit_Fail("Expression: " szMsg, __FILE__, __LINE__)

// -------------------------------------------------------------------------
// CPPUNIT_ASSERT

#define CPPUNIT_ASSERT(condition)											\
do {																		\
	if ( !(condition) )														\
		CPPUNIT_REPORT("CPPUNIT_ASSERT(" #condition ");");					\
} while (0)

#define	CPPUNIT_ASSERT_OK(hr)												\
do {																		\
	if ( !(condition) )														\
		CPPUNIT_REPORT("CPPUNIT_ASSERT_OK(" #hr ");");						\
} while (0)

// -------------------------------------------------------------------------
// CPPUNIT_ASSERT_EQUAL

template <class T>
inline STDMETHODIMP_(BOOL) _CppUnit_IsEqualDbg(const T& x, const T& y) {
	return x == y;
}

inline STDMETHODIMP_(BOOL) _CppUnit_IsEqualDbg(const char* x, const char* y) {
	return strcmp(x, y) == 0;
}

inline STDMETHODIMP_(BOOL) _CppUnit_IsEqualDbg(const WCHAR* x, const WCHAR* y) {
	return wcscmp(x, y) == 0;
}

#define CPPUNIT_ASSERT_EQUAL(expected, actual)								\
do {																		\
	if ( !_CppUnit_IsEqualDbg(expected, actual) )							\
		CPPUNIT_REPORT("CPPUNIT_ASSERT_EQUAL(" #expected "," #actual ");");	\
} while (0)

#define CPPUNIT_ASSERT_EQ(expected, actual)	CPPUNIT_ASSERT_EQUAL(expected, actual)

// =========================================================================
//
//	CPPUNIT_PROC_REGISTRATION(ModuleInit, ModuleTerm);
//

#define CPPUNIT_PROC_REGISTRATION(fnInit, fnTerm)							\
	static HRESULT _CppUnit_RegisterProcedure__ ## fnInit =					\
			_CppUnit_RegisterProcedure(fnInit, fnTerm)

// =========================================================================
//
//	CPPUNIT_TEST_SUITE_REGISTRATION_DBG(TestCaseClass); or
//	CPPUNIT_TEST_SUITE_REGISTRATION(TestCaseClass); or
//	CPPUNIT_TEST_SUITE_REGISTRATION_BYNAME(TestCaseClass, SuiteName);
//

// -------------------------------------------------------------------------
// class AutoRegisterTestSuite

template <class TestFixtureType>
class AutoRegisterTestSuite
{
private:
	TestFactory* m_factory;

public:
	AutoRegisterTestSuite()
	{
		// TestFixtureType::suite是一个创建suite的函数指针，
		// 被m_factory 保存并被其 makeTest 函数调用
		_CppUnit_CreateTestSuiteFactory(TestFixtureType::suite, &m_factory);
		_CppUnit_RegisterFactory1(m_factory);
	}
	~AutoRegisterTestSuite()
	{
		_CppUnit_UnregisterFactory1(m_factory);
		_CppUnit_DeleteTestSuiteFactory(m_factory);
	}
};

template <class TestFixtureType>
class AutoRegisterTestSuite_ByName
{
private:
	TestFactory* m_factory;
	
public:
	AutoRegisterTestSuite_ByName(const char* strName) : m_strName(strName)
	{	
		_CppUnit_CreateTestSuiteFactory(TestFixtureType::suite, &m_factory);
		_CppUnit_RegisterFactory_ByName(m_factory, m_strName);
	}
	~AutoRegisterTestSuite_ByName()
	{
		_CppUnit_UnregisterFactory_ByName(m_factory, m_strName);
		_CppUnit_DeleteTestSuiteFactory(m_factory);
	}

private:
	std::string m_strName;
};

#define CPPUNIT_TEST_SUITE_REGISTRATION(TestFixtureType)					\
	static AutoRegisterTestSuite<TestFixtureType>							\
			TestFixtureType ## AutoRegister__ ## __LINE__

#define CPPUNIT_TEST_SUITE_REGISTRATION_BYNAME(TestFixtureType, SuiteName)	\
	static AutoRegisterTestSuite_ByName<TestFixtureType>					\
		TestFixtureType ## AutoRegister__ ## __LINE__ ## __ByName__(SuiteName)

// -------------------------------------------------------------------------

#if defined(_DEBUG) || defined(X_RELEASE_CASE)
#define CPPUNIT_TEST_SUITE_REGISTRATION_DBG(TestFixtureType)				\
		CPPUNIT_TEST_SUITE_REGISTRATION(TestFixtureType)
#else
#define CPPUNIT_TEST_SUITE_REGISTRATION_DBG(TestFixtureType)
#endif

#if defined(_DEBUG) || defined(X_RELEASE_CASE)
#define CPPUNIT_PROC_REGISTRATION_DBG(fnInit, fnTerm)						\
		CPPUNIT_PROC_REGISTRATION(fnInit, fnTerm)
#else
#define CPPUNIT_PROC_REGISTRATION_DBG(fnInit, fnTerm)
#endif

// =========================================================================
// $Log: basic.h,v $

#endif /* __CPPUNIT_BASIC_H__ */
