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
// Module: stdext/filebuf/WinFileMapping.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-31 10:11:31
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_FILEBUF_WINFILEMAPPING_H
#define STDEXT_FILEBUF_WINFILEMAPPING_H

#ifndef STDEXT_MMAP_WINFILEMAPPINGBUF_H
#include "../mmap/WinFileMappingBuf.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class WinFileMappingFileBuf

class WinFileMappingFileBuf
{
private:
	typedef WinFileMappingReadOnly Config;
	typedef WinFileMappingBufRO MMap;
	typedef MMap::file_descriptor file_descriptor;
	
	MMap m_filemapping;
	DWORD m_dwFileSize;
	
public:
	WinFileMappingFileBuf() {}

	WinFileMappingFileBuf(LPCSTR file) {
		read(file);
	}

	WinFileMappingFileBuf(LPCWSTR file) {
		read(file);
	}

public:
	HRESULT winx_call readRand(file_descriptor hFile)
	{
		DWORD dwFileSizeHigh = 0;
		m_dwFileSize = ::GetFileSize(hFile, &dwFileSizeHigh);
		if (dwFileSizeHigh > 0)
			return E_OUTOFMEMORY;

		return m_filemapping.openHandle(hFile, m_dwFileSize);
	}

	HRESULT winx_call read(LPCSTR file)
	{
		file_descriptor hFile = CreateFile(
			file,
			GENERIC_READ,
			Config::FileShareMode,
			NULL,
			Config::FileCreationDisposition, 
			Config::FileFlagsAndAttributes,
			NULL);

		if (hFile == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_WIN32(GetLastError());

		HRESULT hr = readRand(hFile);
		CloseHandle(hFile);
		return hr;
	}

	HRESULT winx_call read(LPCWSTR file)
	{
		WINX_USES_CONVERSION;
		return read(WINX_W2CA(file));
	}

public:
	void winx_call clear() {
		m_filemapping.close();
	}

	const char* winx_call data() const {
		return m_filemapping.view();
	}

	const char* winx_call begin() const {
		return m_filemapping.view();
	}

	const char* winx_call end() const {
		return m_filemapping.view() + m_dwFileSize;
	}

	int winx_call good() const {
		return m_filemapping.good();
	}

	const size_t winx_call size() const {
		return m_dwFileSize;
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_FILEBUF_WINFILEMAPPING_H */
