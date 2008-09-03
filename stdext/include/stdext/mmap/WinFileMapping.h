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
// Module: stdext/mmap/WinFileMapping.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MMAP_WINFILEMAPPING_H
#define STDEXT_MMAP_WINFILEMAPPING_H

#ifndef STDEXT_MMAP_WINFILEMAPPINGBUF_H
#include "WinFileMappingBuf.h"
#endif

#ifndef STDEXT_MMAP_ACCESSBUFFER_H
#include "AccessBuffer.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class WinFileMappingUtils

class WinFileMappingUtils
{
public:
	static void winx_call unmap(void* p, size_t) {
		UnmapViewOfFile(p);
	}

	static BOOL winx_call flush(void* p, size_t cb) {
		return FlushViewOfFile(p, cb);
	}
};

// -------------------------------------------------------------------------
// class WinFileMapping

template <class Config, class PosT = UINT32>
class WinFileMapping
{
private:
	HANDLE m_hFile;
	HANDLE m_hFileMapping;
	DWORD m_nTotalPage;

	WinFileMapping(const WinFileMapping&);
	void operator=(const WinFileMapping&);

public:
	enum { _nAGBits = 16 };
	enum { _AllocationGranularityInvBits = sizeof(DWORD)*8 - _nAGBits };

public:
	enum { AllocationGranularityBits = _nAGBits };
	enum { AllocationGranularity = (1 << _nAGBits) };
	enum { AllocationGranularityMask = (AllocationGranularity - 1) };

public:
	typedef UINT size_type;
	typedef PosT pos_type;
	typedef PosT pos_argtype;

public:
	WinFileMapping() : m_hFile(INVALID_HANDLE_VALUE) {}
	WinFileMapping(LPCTSTR szFile)
	{
		m_hFile = INVALID_HANDLE_VALUE;
		open(szFile);
	}
	~WinFileMapping()
	{
		close();
	}

	DWORD winx_call getTotalPages() const
	{
		return m_nTotalPage;
	}

	int winx_call good() const
	{
		return m_hFile != INVALID_HANDLE_VALUE;
	}

	void winx_call close()
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			if (m_hFileMapping) {
				WINX_VERIFY(CloseHandle(m_hFileMapping));
				m_hFileMapping = NULL;
			}
			WINX_VERIFY(CloseHandle(m_hFile));
			m_hFile = INVALID_HANDLE_VALUE;
		}
	}

	HRESULT winx_call reopen(DWORD nInitialTotalPage)
	{
		WINX_ASSERT(nInitialTotalPage);

		if (m_hFileMapping)
			WINX_VERIFY(CloseHandle(m_hFileMapping));

		m_nTotalPage = nInitialTotalPage;
		m_hFileMapping = CreateFileMapping(
			m_hFile, NULL, Config::PageProtect,
			m_nTotalPage >> _AllocationGranularityInvBits,
			m_nTotalPage << AllocationGranularityBits,
			NULL);

		return m_hFileMapping ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open(LPCTSTR szFile)
	{
		WINX_ASSERT(!good());
		WINX_ASSERT(
			Config::FileCreationDisposition >= CREATE_NEW &&
			Config::FileCreationDisposition <= TRUNCATE_EXISTING);
		WINX_ASSERT(
			FILE_SHARE_READ == Config::FileShareMode ||
			(FILE_SHARE_READ|FILE_SHARE_WRITE) == Config::FileShareMode);

		if (good())
			return E_ACCESSDENIED;

		m_hFile = CreateFile(
			szFile, 
			Config::FileDesiredAccess,
			Config::FileShareMode,
			NULL,
			Config::FileCreationDisposition, 
			Config::FileFlagsAndAttributes,
			NULL);

		if (m_hFile == INVALID_HANDLE_VALUE)
			return E_ACCESSDENIED;

		m_nTotalPage = 0;
		m_hFileMapping = NULL;
		return S_OK;
	}

	char* winx_call viewSegment(DWORD iBasePage, DWORD nPageCount)
	{
		if (iBasePage + nPageCount > m_nTotalPage)
		{
			if (iBasePage >= m_nTotalPage)
				return NULL;
			else
				nPageCount = m_nTotalPage - iBasePage;
		}
		return (char*)MapViewOfFileEx(
			m_hFileMapping, Config::PageAccess,
			iBasePage >> _AllocationGranularityInvBits,
			iBasePage << AllocationGranularityBits,
			nPageCount << AllocationGranularityBits,
			NULL);
	}

	char* winx_call accessSegment(DWORD iBasePage, DWORD nPageCount)
	{
		if (iBasePage + nPageCount > m_nTotalPage)
		{
			if (m_hFileMapping)
				WINX_VERIFY(CloseHandle(m_hFileMapping));

			m_nTotalPage = iBasePage + nPageCount;

			m_hFileMapping = CreateFileMapping(
				m_hFile, NULL, Config::PageProtect,
				m_nTotalPage >> _AllocationGranularityInvBits,
				m_nTotalPage << AllocationGranularityBits,
				NULL);
		}

		return (char*)MapViewOfFileEx(
			m_hFileMapping, Config::PageAccess,
			iBasePage >> _AllocationGranularityInvBits,
			iBasePage << AllocationGranularityBits,
			nPageCount << AllocationGranularityBits,
			NULL);
	}

	char* winx_call allocSegment(DWORD nPageCount, DWORD& iBasePage)
	{
		WINX_ASSERT(good());

		if (m_hFileMapping)
			WINX_VERIFY(CloseHandle(m_hFileMapping));

		iBasePage = m_nTotalPage;

		m_nTotalPage += nPageCount;

		m_hFileMapping = CreateFileMapping(
			m_hFile, NULL, Config::PageProtect,
			m_nTotalPage >> _AllocationGranularityInvBits,
			m_nTotalPage << AllocationGranularityBits,
			NULL);

		return (char*)MapViewOfFileEx(
			m_hFileMapping, Config::PageAccess,
			iBasePage >> _AllocationGranularityInvBits,
			iBasePage << AllocationGranularityBits,
			nPageCount << AllocationGranularityBits,
			NULL);
	}

public:
	typedef WinFileMappingUtils Utils;
	typedef HandleProxy<WinFileMapping> Handle;
};

typedef WinFileMapping<WinFileMappingReadWrite> WinFileMappingRW;
typedef WinFileMapping<WinFileMappingReadOnly> WinFileMappingRO;

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_MMAP_WINFILEMAPPING_H */
