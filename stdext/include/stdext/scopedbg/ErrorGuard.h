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
// Module: stdext/scopedbg/ErrorGuard.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-2-1 13:56:50
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_SCOPEDBG_ERRORGUARD_H__
#define __STDEXT_SCOPEDBG_ERRORGUARD_H__

#if defined(_MSC_VER)
	#ifndef __STDEXT_SCOPEDBG_CHECKEXCEPTION_H__
	#include "CheckException.h"
	#endif
#else
	#error "todo"
#endif

#ifndef __STDEXT_SCOPEDBG_SCOPELOG_H__
#include "ScopeLog.h"
#endif

__NS_STD_BEGIN

// =========================================================================
// WINX_GUARD_LOG

typedef ThreadLog<FileScopeLog>	ThreadFileScopeLog;
typedef ThreadLogInit<FileScopeLog>	ThreadFileScopeLogInit;

#define WINX_THREAD_LOG				std::ThreadFileScopeLog::instance(true)
#define WINX_THREAD_LOG_TERM()		std::ThreadFileScopeLog::term()

// =========================================================================
// class ExceptionGuard

class _ExceptionGuardBase
{
private:
	const char* m_func;
	const char* m_file;
	const int m_line;
	FileScopeLog& m_log;

public:
	_ExceptionGuardBase(const char* func, const char* file, int line)
		: m_func(func), m_file(file), m_line(line), m_log(WINX_THREAD_LOG)
	{
		m_log.enterScope();
		m_log.trace("%s(%d): Enter '%s'\n", file, line, func);
	}

	void winx_call onLeave(bool isNormal)
	{
		if (!isNormal)
		{
			char msg[1024];
			sprintf(msg, "Exception in '%s'.", m_func);
			m_log.reportGuardError("Exception", -1, msg, m_file, m_line);
		}
		m_log.leaveScope(!isNormal);
	}
};

typedef CheckException<_ExceptionGuardBase> ExceptionGuard;

#ifndef WINX_EXCEPTION_GUARD
#define WINX_EXCEPTION_GUARD(func)											\
	std::ExceptionGuard _winx_guard_exception(func, __FILE__, __LINE__)
#endif

#ifndef WINX_FUNCTION_GUARD
#define WINX_FUNCTION_GUARD		WINX_EXCEPTION_GUARD(__FUNCTION__)
#endif

// =========================================================================
// reportGuardWin32Error

template <class LogT>
inline void winx_call reportGuardWin32Error(
	LogT& log, LONG error, const char* file, int line, LPCSTR general = "Win32Error")
{
	LPSTR lpMsgBuf;
	::FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPSTR)&lpMsgBuf,
		0, NULL
		);
	log.enterScope();
	log.reportGuardError(general, error, lpMsgBuf, file, line);
	log.leaveScope();
	::LocalFree(lpMsgBuf);
}

// -------------------------------------------------------------------------
// class Win32ErrorGuard

class Win32ErrorGuard
{
private:
	const LONG& m_result;
	const char* m_file;
	const int m_line;

public:
	Win32ErrorGuard(const LONG& result, const char* file, int line)
		: m_result(result), m_file(file), m_line(line) {
	}
	~Win32ErrorGuard() {
		if (m_result != 0) {
			reportGuardWin32Error(WINX_THREAD_LOG, m_result, m_file, m_line);
		}
	}
};

#ifndef WINX_WIN32_ERROR_GUARD
#define WINX_WIN32_ERROR_GUARD(result)										\
	std::Win32ErrorGuard _winx_guard_win32err(result, __FILE__, __LINE__)
#endif

// -------------------------------------------------------------------------
// class HResultErrorGuard

class HResultErrorGuard
{
private:
	const LONG& m_result;
	const char* m_file;
	const int m_line;

public:
	HResultErrorGuard(const LONG& result, const char* file, int line)
		: m_result(result), m_file(file), m_line(line) {
	}
	~HResultErrorGuard() {
		if (FAILED(m_result)) {
			reportGuardWin32Error(WINX_THREAD_LOG, m_result, m_file, m_line, "HResultError");
		}
	}
};

#ifndef WINX_HRESULT_ERROR_GUARD
#define WINX_HRESULT_ERROR_GUARD(result)									\
	std::HResultErrorGuard _winx_guard_hresult(result, __FILE__, __LINE__)
#endif

// =========================================================================
// class TestErrorGuard

template <class LogT>
class TestErrorGuard
{
	WINX_TEST_SUITE(TestErrorGuard);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void test(LogT& log)
	{
		ThreadFileScopeLogInit logInit("/__ErrorGuard__.log");
		{
			try
			{
				LONG result;
				WINX_EXCEPTION_GUARD("TestErrorGuard::test");
				WINX_WIN32_ERROR_GUARD(result);
				::CreateDirectoryA("C:\\", NULL);
				result = ::GetLastError();
				throw 0;
			}
			catch (...) {
			}
		}
	}
};

// =========================================================================
// $Log: $

__NS_STD_END

#endif /* __STDEXT_SCOPEDBG_ERRORGUARD_H__ */
