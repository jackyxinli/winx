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
// Module: stdext/mmap/MMapBuf.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-31 10:11:31
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MMAP_MMAPBUF_H
#define STDEXT_MMAP_MMAPBUF_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

#ifndef _SYS_STAT_H
#include <sys/stat.h>
#endif

#ifndef _SYS_MMAN_H
#include <sys/mman.h>
#endif

#ifndef	_FCNTL_H
#include <fcntl.h>
#endif

#ifndef	_UNISTD_H
#include <unistd.h>
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// struct MMapReadWrite

struct MMapReadWrite
{
	enum { PageProtect = PROT_READ | PROT_WRITE };
	enum { PageAccess = MAP_SHARED };

	enum { FileDesiredAccess = O_RDWR };
	enum { FileShareMode = 0 };
	enum { FileCreationDisposition = O_CREAT|O_TRUNC };
	enum { FileFlagsAndAttributes = S_IRUSR|S_IWUSR | S_IRGRP | S_IROTH };
};

// -------------------------------------------------------------------------
// struct MMapReadOnly

struct MMapReadOnly
{
	enum { PageProtect = PROT_READ };
	enum { PageAccess = MAP_SHARED };

	enum { FileDesiredAccess = O_RDONLY };
	enum { FileShareMode = 0 };
	enum { FileCreationDisposition = 0 };
	enum { FileFlagsAndAttributes = 0 };
};

// -------------------------------------------------------------------------
// class MMapBuf

template <class Config>
class MMapBuf
{
public:
	typedef size_t size_type;
	typedef int file_descriptor;
	
private:
	file_descriptor m_fd;
	char* m_pView;
	size_type m_cbSize;
	
public:
	MMapBuf() : m_pView(NULL)
	{
	}

	MMapBuf(
		LPCSTR szFile, size_type cbSize,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL) : m_pView(NULL)
	{
		open(
			szFile, cbSize,
			lpFileMappingAttributes, pvBaseAddress
			);
	}

	MMapBuf(
		size_type cbSize, LPCSTR lpObjectName,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL) : m_pView(NULL)
	{
		open(cbSize, lpObjectName, lpFileMappingAttributes, pvBaseAddress);
	}

	~MMapBuf()
	{
		if (m_pView)
		{
			munmap(m_pView, m_cbSize);
			::close(m_fd);
		}
	}

public:
	HRESULT winx_call attach(
		file_descriptor fd, size_type cbSize,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL)
	{
		if (good())
			return E_ACCESSDENIED;
			
		m_pView = (char*)mmap(
			pvBaseAddress, cbSize, Config::PageProtect, Config::PageAccess, fd, 0);
		
		WINX_ASSERT(m_pView != MAP_FAILED);
		
		if (m_pView == MAP_FAILED) {
			m_pView = NULL;
			return E_FAIL;
		}

		m_fd = fd;
		m_cbSize = cbSize;
		return S_OK;
	}

	HRESULT winx_call open(
		LPCSTR szFile, size_type cbSize,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL)
	{
		int fd = open(
			szFile,
			Config::FileDesiredAccess | Config::FileShareMode | Config::FileCreationDisposition,
			Config::FileFlagsAndAttributes);

		if (fd < 0)
			return E_FAIL;

		if (Config::FileCreationDisposition)
			ftruncate(fd, cbSize);

		HRESULT hr = attach(fd, cbSize, lpFileMappingAttributes, pvBaseAddress);
		if (hr != S_OK)
			close(fd);
		return hr;
	}

	HRESULT winx_call open(
		size_type cbSize, LPCSTR lpObjectName,
		LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL,
		PVOID pvBaseAddress = NULL)
	{
		if (good())
			return E_ACCESSDENIED;

		return E_NOTIMPL;
	}

	void winx_call flush(int flags = MS_ASYNC)
	{
		WINX_ASSERT(m_pView);

		msync(m_pView, m_cbSize, flags);
	}

	void winx_call close()
	{
		if (m_pView)
		{
			munmap(m_pView, m_cbSize);
			::close(m_fd);
			m_pView = NULL;
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
	
	size_type winx_call size() const
	{
		return m_cbSize;
	}
};

typedef MMapBuf<MMapReadOnly> MMapBufRO;
typedef MMapBuf<MMapReadWrite> MMapBufRW;

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_MMAP_MMAPBUF_H */
