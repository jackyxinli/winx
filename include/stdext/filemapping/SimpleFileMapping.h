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
// Module: stdext/filemapping/SimpleFileMapping.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-31 10:11:31
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_FILEMAPPING_SIMPLEFILEMAPPING_H__
#define __STDEXT_FILEMAPPING_SIMPLEFILEMAPPING_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// struct FileMappingReadWrite

struct FileMappingReadWrite
{
	enum { PageProtect = PAGE_READWRITE };
	enum { PageAccess = FILE_MAP_READ|FILE_MAP_WRITE };

	enum { FileDesiredAccess = GENERIC_WRITE|GENERIC_READ };
	enum { FileShareMode = FILE_SHARE_READ };
	enum { FileCreationDisposition = OPEN_ALWAYS };
	enum { FileFlagsAndAttributes = FILE_FLAG_WRITE_THROUGH|FILE_ATTRIBUTE_ARCHIVE };
};

// -------------------------------------------------------------------------
// struct FileMappingReadOnly

struct FileMappingReadOnly
{
	enum { PageProtect = PAGE_READONLY };
	enum { PageAccess = FILE_MAP_READ };
	
	enum { FileDesiredAccess = GENERIC_READ };
	enum { FileShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE };
	enum { FileCreationDisposition = OPEN_EXISTING };
	enum { FileFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN };
};

// -------------------------------------------------------------------------
// class SimpleFileMappingT

template <class Config>
class SimpleFileMappingT
{
private:
	HANDLE m_hFileMapping;
	char* m_pView;
	
private:
	enum { _ShareMode = Config::FileShareMode };
	enum { _CreationDisposition = Config::FileCreationDisposition };
	enum { _FlagsAndAttributes = Config::FileFlagsAndAttributes };

public:
	SimpleFileMappingT() : m_pView(NULL)
	{
	}

	SimpleFileMappingT(
		LPCTSTR szFile, DWORD cbSize,
		DWORD dwShareMode = _ShareMode,
		DWORD dwCreationDisposition = _CreationDisposition,
		DWORD dwFlagsAndAttributes = _FlagsAndAttributes,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL) : m_pView(NULL)
	{
		open(
			szFile, cbSize,
			dwShareMode, dwCreationDisposition, dwFlagsAndAttributes,
			lpFileMappingAttributes, pvBaseAddress
			);
	}

	SimpleFileMappingT(
		DWORD cbSize, LPCTSTR lpObjectName,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL) : m_pView(NULL)
	{
		open(cbSize, lpObjectName, lpFileMappingAttributes, pvBaseAddress);
	}

	~SimpleFileMappingT()
	{
		if (m_pView)
		{
			UnmapViewOfFile(m_pView);
			CloseHandle(m_hFileMapping);
		}
	}

public:
	HRESULT openHandle(
		HANDLE hFile, DWORD cbSize,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL)
	{
		if (good())
			return E_ACCESSDENIED;

		WINX_ASSERT(hFile != INVALID_HANDLE_VALUE);

		m_hFileMapping = CreateFileMapping(
			hFile, lpFileMappingAttributes, Config::PageProtect, 0, cbSize, NULL);

		WINX_ASSERT(m_hFileMapping);

		if (m_hFileMapping == NULL)
			return E_ACCESSDENIED;

		m_pView = (char*)MapViewOfFileEx(
			m_hFileMapping, Config::PageAccess, 0, 0, cbSize, pvBaseAddress);

		WINX_ASSERT(m_pView);

		if (m_pView == NULL)
		{
			CloseHandle(m_hFileMapping);
			m_hFileMapping = NULL;
			return E_FAIL;
		}

		return S_OK;;
	}
	
	HRESULT open(
		LPCTSTR szFile, DWORD cbSize,
		DWORD dwShareMode = _ShareMode,
		DWORD dwCreationDisposition = _CreationDisposition,
		DWORD dwFlagsAndAttributes = _FlagsAndAttributes,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL)
	{
		HANDLE hFile = CreateFile(
			szFile, 
			GENERIC_WRITE|GENERIC_READ,
			dwShareMode,
			NULL,
			dwCreationDisposition, 
			dwFlagsAndAttributes,
			NULL);

		if (hFile == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_WIN32(GetLastError());

		HRESULT hr = openHandle(hFile, cbSize, lpFileMappingAttributes, pvBaseAddress);
		CloseHandle(hFile);
		return hr;
	}

	HRESULT open(
		DWORD cbSize, LPCTSTR lpObjectName,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL)
	{
		if (good())
			return E_ACCESSDENIED;

		WINX_ASSERT(lpObjectName != NULL);

		m_hFileMapping = CreateFileMapping(
			INVALID_HANDLE_VALUE, lpFileMappingAttributes, Config::PageProtect, 0, cbSize, lpObjectName);

		WINX_ASSERT(m_hFileMapping);

		if (m_hFileMapping == NULL)
			return E_ACCESSDENIED;

		m_pView = (char*)MapViewOfFileEx(
			m_hFileMapping, Config::PageAccess, 0, 0, cbSize, pvBaseAddress);

		WINX_ASSERT(m_pView);

		if (m_pView == NULL)
		{
			CloseHandle(m_hFileMapping);
			m_hFileMapping = NULL;
			return E_FAIL;
		}

		return S_OK;;
	}

	void flush()
	{
		WINX_ASSERT(m_pView);

		FlushViewOfFile(m_pView, 0);
	}

	void close()
	{
		if (m_pView)
		{
			UnmapViewOfFile(m_pView);
			CloseHandle(m_hFileMapping);
			m_pView = NULL;
			m_hFileMapping = NULL;
		}
	}

	int good() const
	{
		return m_pView != NULL;
	}
	
	char* view() const
	{
		return m_pView;
	}
};

typedef SimpleFileMappingT<FileMappingReadOnly> SimpleFileMappingRO;
typedef SimpleFileMappingT<FileMappingReadWrite> SimpleFileMapping;

// -------------------------------------------------------------------------
// class TestSimpleFileMapping

template <class LogT>
class TestSimpleFileMapping
{
	WINX_TEST_SUITE(TestSimpleFileMapping);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void test(LogT& log)
	{
		SimpleFileMapping sfm(WINX_TEXT("/__SimpleFileMapping__.txt"), 32);
		char* buf = sfm.view();
		strcpy(buf, "hello, xushiwei");

		SimpleFileMappingRO sfmro(WINX_TEXT("/__SimpleFileMapping__.txt"), 32);
		char* buf2 = sfmro.view();
		AssertExp(strcmp(buf, buf2) == 0);
		log.print(buf2).newline();
	}
};

// -------------------------------------------------------------------------
// $Log: $

__NS_STD_END

#endif /* __STDEXT_FILEMAPPING_SIMPLEFILEMAPPING_H__ */
