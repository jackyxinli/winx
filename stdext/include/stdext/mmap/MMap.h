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
// Module: stdext/mmap/MMap.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MMAP_MMAP_H
#define STDEXT_MMAP_MMAP_H

#ifndef STDEXT_MMAP_MMAPBUF_H
#include "MMapBuf.h"
#endif

#ifndef STDEXT_MMAP_ACCESSBUFFER_H
#include "AccessBuffer.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class MMapUtils

class MMapUtils
{
public:
	static void winx_call unmap(void* p, size_t cb) {
		munmap(p, cb);
	}

	static BOOL winx_call flush(void* p, size_t cb) {
		return msync(p, cb, MS_ASYNC) != 0;
	}
};

// -------------------------------------------------------------------------
// class MMap

template <class Config, class PosT = UINT32>
class MMap
{
private:
	int m_fd;
	DWORD m_nTotalPage;

	MMap(const MMap&);
	void operator=(const MMap&);

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
	MMap() : m_fd(-1) {}
	MMap(LPCTSTR szFile)
	{
		m_fd = -1;
		open(szFile);
	}
	~MMap()
	{
		close();
	}

	DWORD winx_call getTotalPages() const
	{
		return m_nTotalPage;
	}

	int winx_call good() const
	{
		return m_fd != -1;
	}

	void winx_call close()
	{
		if (m_fd != -1)
		{
			::close(m_fd);
			m_fd = -1;
		}
	}

	HRESULT winx_call reopen(DWORD nInitialTotalPage)
	{
		m_nTotalPage = nInitialTotalPage;

		ftruncate(m_fd, (off_t)m_nTotalPage << AllocationGranularityBits);

		return S_OK;
	}

	HRESULT winx_call open(LPCTSTR szFile)
	{
		WINX_ASSERT(!good());
		
		if (good())
			return E_ACCESSDENIED;

		m_fd = ::open(
			szFile,
			Config::FileDesiredAccess | Config::FileShareMode | Config::FileCreationDisposition,
			Config::FileFlagsAndAttributes);

		if (m_fd < 0)
			return E_ACCESSDENIED;

		m_nTotalPage = 0;
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

		return (char*)mmap(
			NULL, nPageCount << AllocationGranularityBits,
			Config::PageProtect, Config::PageAccess,
			m_fd, (off_t)iBasePage << AllocationGranularityBits);
	}

	char* winx_call accessSegment(DWORD iBasePage, DWORD nPageCount)
	{
		if (iBasePage + nPageCount > m_nTotalPage)
		{
			m_nTotalPage = iBasePage + nPageCount;
			
			ftruncate(m_fd, (off_t)m_nTotalPage << AllocationGranularityBits);
		}

		return (char*)mmap(
			NULL, nPageCount << AllocationGranularityBits,
			Config::PageProtect, Config::PageAccess,
			m_fd, (off_t)iBasePage << AllocationGranularityBits);
	}

	char* winx_call allocSegment(DWORD nPageCount, DWORD& iBasePage)
	{
		WINX_ASSERT(good());

		iBasePage = m_nTotalPage;

		m_nTotalPage += nPageCount;

		ftruncate(m_fd, (off_t)m_nTotalPage << AllocationGranularityBits);

		return (char*)mmap(
			NULL, nPageCount << AllocationGranularityBits,
			Config::PageProtect, Config::PageAccess,
			m_fd, (off_t)iBasePage << AllocationGranularityBits);
	}

public:
	typedef MMapUtils Utils;
	typedef HandleProxy<MMap> Handle;
};

typedef MMap<MMapReadWrite> MMapRW;
typedef MMap<MMapReadOnly> MMapRO;

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_MMAP_MMAP_H */
