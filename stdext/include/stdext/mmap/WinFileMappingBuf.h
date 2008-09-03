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
// Module: stdext/mmap/WinFileMappingBuf.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-31 10:11:31
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MMAP_WINFILEMAPPINGBUF_H
#define STDEXT_MMAP_WINFILEMAPPINGBUF_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// struct WinFileMappingReadWrite

struct WinFileMappingReadWrite
{
	enum { PageProtect = PAGE_READWRITE };
	enum { PageAccess = FILE_MAP_READ|FILE_MAP_WRITE };

	enum { FileDesiredAccess = GENERIC_WRITE|GENERIC_READ };
	enum { FileShareMode = FILE_SHARE_READ };
	enum { FileCreationDisposition = OPEN_ALWAYS };
	enum { FileFlagsAndAttributes = FILE_FLAG_WRITE_THROUGH|FILE_ATTRIBUTE_ARCHIVE };
};

// -------------------------------------------------------------------------
// struct WinFileMappingReadOnly

struct WinFileMappingReadOnly
{
	enum { PageProtect = PAGE_READONLY };
	enum { PageAccess = FILE_MAP_READ };
	
	enum { FileDesiredAccess = GENERIC_READ };
	enum { FileShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE };
	enum { FileCreationDisposition = OPEN_EXISTING };
	enum { FileFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN };
};

// -------------------------------------------------------------------------
// class WinFileMappingBuf

template <class Config>
class WinFileMappingBuf
{
public:
	typedef DWORD size_type;
	typedef HANDLE file_descriptor;
	
private:
	HANDLE m_hFileMapping;
	char* m_pView;
	
public:
	WinFileMappingBuf() : m_pView(NULL)
	{
	}

	WinFileMappingBuf(
		LPCTSTR szFile, size_type cbSize,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL) : m_pView(NULL)
	{
		open(
			szFile, cbSize,
			lpFileMappingAttributes, pvBaseAddress
			);
	}

	WinFileMappingBuf(
		size_type cbSize, LPCTSTR lpObjectName,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL) : m_pView(NULL)
	{
		open(cbSize, lpObjectName, lpFileMappingAttributes, pvBaseAddress);
	}

	~WinFileMappingBuf()
	{
		if (m_pView)
		{
			UnmapViewOfFile(m_pView);
			CloseHandle(m_hFileMapping);
		}
	}

public:
	HRESULT winx_call openHandle(
		file_descriptor hFile, size_type cbSize,
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

	HRESULT winx_call open(
		LPCTSTR szFile, size_type cbSize,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL)
	{
		file_descriptor hFile = CreateFile(
			szFile, 
			Config::FileDesiredAccess,
			Config::FileShareMode,
			NULL,
			Config::FileCreationDisposition, 
			Config::FileFlagsAndAttributes,
			NULL);

		if (hFile == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_WIN32(GetLastError());

		HRESULT hr = openHandle(hFile, cbSize, lpFileMappingAttributes, pvBaseAddress);
		CloseHandle(hFile);
		return hr;
	}

	HRESULT winx_call open(
		size_type cbSize, LPCTSTR lpObjectName,
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

	void winx_call flush()
	{
		WINX_ASSERT(m_pView);

		FlushViewOfFile(m_pView, 0);
	}

	void winx_call close()
	{
		if (m_pView)
		{
			UnmapViewOfFile(m_pView);
			CloseHandle(m_hFileMapping);
			m_pView = NULL;
			m_hFileMapping = NULL;
		}
	}

	int winx_call good() const
	{
		return m_pView != NULL;
	}
	
	char* winx_call view() const
	{
		return m_pView;
	}
};

typedef WinFileMappingBuf<WinFileMappingReadOnly> WinFileMappingBufRO;
typedef WinFileMappingBuf<WinFileMappingReadWrite> WinFileMappingBufRW;

// -------------------------------------------------------------------------
// class TestWinFileMappingBuf

#if defined(STD_UNITTEST)

#pragma warning(disable:4996) // XXX  was declared deprecated

template <class LogT>
class TestWinFileMappingBuf
{
	WINX_TEST_SUITE(TestWinFileMappingBuf);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void test(LogT& log)
	{
		WinFileMappingBufRW sfm(WINX_TEXT("/__SimpleFileMapping__.txt"), 32);
		char* buf = sfm.view();
		strcpy(buf, "hello, xushiwei");

		WinFileMappingBufRO sfmro(WINX_TEXT("/__SimpleFileMapping__.txt"), 32);
		char* buf2 = sfmro.view();
		AssertExp(strcmp(buf, buf2) == 0);
		log.print(buf2).newline();
	}
};

#pragma warning(default:4996) // XXX  was declared deprecated

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_MMAP_WINFILEMAPPINGBUF_H */
