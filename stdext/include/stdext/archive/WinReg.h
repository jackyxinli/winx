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
// Module: stdext/archive/WinReg.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-26 0:34:01
// 
// $Id: WinReg.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_WINREG_H__
#define __STDEXT_ARCHIVE_WINREG_H__

#ifndef __STDEXT_REGISTRY_WINREGISTRY_H__
#include "../registry/WinRegistry.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// RegWriteString
// --> 千万注意 (对于ATL低版本而言): 
//	由于使用了A2W/W2A，RegReadString/RegWriteString这两个函数不能inline

#define _WINX_REG_STRING_PREFIX	'N'

template <class RegKeyType, class StringT>
HRESULT winx_call RegWriteString(
	RegKeyType& regKey, UINT index, const StringT& str)
{
	TCHAR szName[16];
	szName[0] = _WINX_REG_STRING_PREFIX;

#if defined(UNICODE) || defined(_UNICODE)
	_itow(index, szName+1, 10);
#else
	_itoa(index, szName+1, 10);
#endif

	LONG lRes = regKey.putString(szName, str);
	return HRESULT_FROM_WIN32(lRes);
}

// -------------------------------------------------------------------------
// RegReadString

template <class RegKeyType, class StringT>
HRESULT winx_call RegReadString(
	RegKeyType& regKey, UINT index, StringT& str)
{
	TCHAR szName[16];
	szName[0] = _WINX_REG_STRING_PREFIX;

#if defined(UNICODE) || defined(_UNICODE)
	_itow(index, szName+1, 10);
#else
	_itoa(index, szName+1, 10);
#endif

	LONG lRes = regKey.getString(szName, str);
	return HRESULT_FROM_WIN32(lRes);
}

// -------------------------------------------------------------------------
// class WinRegWriter

class WinRegWriter
{
private:
	UINT m_nCount;
	WinRegKeyHandle m_regKey;

public:
	WinRegWriter()
		: m_nCount(0) {
	}
	~WinRegWriter() {
		m_regKey.close();
	}

	WinRegWriter(
		HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPTSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		REGSAM samDesired = KEY_ALL_ACCESS,
		LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
		LPDWORD lpdwDisposition = NULL) : m_nCount(0)
	{
		::RegDeleteKey(hKeyParent, lpszKeyName);
		m_regKey.create(
			hKeyParent, lpszKeyName, 
			lpszClass, dwOptions, samDesired, lpSecAttr, lpdwDisposition);
	}

	void winx_call close()
	{
		m_regKey.close();
	}
	
	HRESULT winx_call open(
		HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPTSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		REGSAM samDesired = KEY_ALL_ACCESS,
		LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
		LPDWORD lpdwDisposition = NULL)
	{
		if (good())
			return E_ACCESSDENIED;
		m_nCount = 0;
		::RegDeleteKey(hKeyParent, lpszKeyName);
		return HRESULT_FROM_WIN32(
			m_regKey.create(hKeyParent, lpszKeyName, 
			lpszClass, dwOptions, samDesired, lpSecAttr, lpdwDisposition));
	}

	template <class StringT>
	HRESULT winx_call puts(const StringT& str)
	{
		return RegWriteString(m_regKey, ++m_nCount, str);
	}

	int winx_call good() const
	{
		return m_regKey != NULL;
	}
};

// -------------------------------------------------------------------------
// class WinRegReader

class WinRegReader
{
private:
	UINT m_nCount;
	WinRegKeyHandle m_regKey;
	
public:
	WinRegReader()
		: m_nCount(0) {
	}
	~WinRegReader() {
		m_regKey.close();
	}
	
	WinRegReader(
		HKEY hKeyParent, LPCTSTR lpszKeyName,
		REGSAM samDesired = KEY_ALL_ACCESS) : m_nCount(0)
	{
		m_regKey.open(hKeyParent, lpszKeyName, samDesired);
	}
	
	void winx_call close()
	{
		m_regKey.close();
	}
	
	HRESULT winx_call open(
		HKEY hKeyParent, LPCTSTR lpszKeyName,
		REGSAM samDesired = KEY_ALL_ACCESS)
	{
		if (good())
			return E_ACCESSDENIED;
		m_nCount = 0;
		return HRESULT_FROM_WIN32(m_regKey.open(hKeyParent, lpszKeyName));
	}

	template <class AllocT>
	HRESULT winx_call gets(AllocT& alloc, BasicString<TCHAR>& s)
	{
		OutputBasicString<TCHAR, AllocT> s1(alloc, s);
		return gets(s1);
	}

	template <class StringT>
	HRESULT winx_call gets(StringT& str)
	{
		return RegReadString(m_regKey, ++m_nCount, str);
	}
	
	int winx_call good() const {
		return m_regKey != NULL;
	}
};

// -------------------------------------------------------------------------
// class TestWinReg

#define _WINX_TEST_WINREG_KEY	WINX_TEXT("Software\\winx\\TestStdExt\\TestWinReg")

template <class LogT>
class TestWinReg : public TestCase
{
	WINX_TEST_SUITE(TestWinReg);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		{
			std::WinRegWriter ar(HKEY_CURRENT_USER, _WINX_TEST_WINREG_KEY);
			ar.puts("Hello");
			ar.puts(std::string("World!"));
			ar.puts(std::vector<char>(256, '!'));
			ar.puts(std::vector<char>(65537, '?'));
		}
		{
			std::WinRegReader ar(HKEY_CURRENT_USER, _WINX_TEST_WINREG_KEY);
			std::string s1;
			AssertExp(ar.gets(s1) == S_OK);
			AssertExp(s1 == "Hello");
			std::vector<char> s2;
			AssertExp(ar.gets(s2) == S_OK);
			AssertExp(std::compare(s2.begin(), s2.end(), "World!") == 0);
			std::String s3;
			AssertExp(ar.gets(alloc, s3) == S_OK);
			AssertExp(s3 == std::String(alloc, 256, '!'));
			std::String s4;
			AssertExp(ar.gets(alloc, s4) == S_OK);
			AssertExp(s4 == std::String(alloc, 65537, '?'));
			std::String s5;
			AssertExp(ar.gets(alloc, s5) != S_OK);
		}
	}
};

// -------------------------------------------------------------------------
// $Log: WinReg.h,v $

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_WINREG_H__ */
