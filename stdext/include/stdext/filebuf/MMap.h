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
// Module: stdext/filebuf/MMap.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-31 10:11:31
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_FILEBUF_MMAP_H
#define STDEXT_FILEBUF_MMAP_H

#ifndef STDEXT_MMAP_MMAPBUF_H
#include "../mmap/MMapBuf.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class MMapFileBuf

class MMapFileBuf
{
private:
	typedef MMapReadOnly Config;
	typedef MMapBufRO MMap;
	typedef MMap::file_descriptor file_descriptor;
	
	MMap m_mmap;
	
public:
	MMapFileBuf() {}

	MMapFileBuf(LPCSTR file) {
		read(file);
	}

	MMapFileBuf(LPCWSTR file) {
		read(file);
	}

public:
	HRESULT winx_call attach(file_descriptor fd)
	{
		struct stat fi;
		if (fstat(fd, &fi) < 0)
			return E_FAIL;
		else
			return m_mmap.attach(fd, fi.st_size);
	}

	HRESULT winx_call read(LPCSTR file)
	{
		file_descriptor fd = open(
			file,
			Config::FileDesiredAccess | Config::FileShareMode | Config::FileCreationDisposition,
			Config::FileFlagsAndAttributes);

		if (fd < 0)
			return E_FAIL;

		HRESULT hr = attach(fd);
		if (hr != S_OK)
			close(fd);
		return hr;
	}

	HRESULT winx_call read(LPCWSTR file)
	{
		WINX_USES_CONVERSION;
		return read(WINX_W2CA(file));
	}

public:
	void winx_call clear() {
		m_mmap.close();
	}

	const char* winx_call data() const {
		return m_mmap.view();
	}

	const char* winx_call begin() const {
		return m_mmap.view();
	}

	const char* winx_call end() const {
		return m_mmap.view() + m_mmap.size();
	}

	int winx_call good() const {
		return m_mmap.good();
	}

	const size_t winx_call size() const {
		return m_mmap.size();
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_FILEBUF_MMAP_H */
