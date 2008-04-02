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
// Module: stdext/archive/Stream.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 20:06:28
// 
// $Id: Stream.h,v 1.3 2006/12/03 05:30:27 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_STREAM_H__
#define __STDEXT_ARCHIVE_STREAM_H__

#ifndef __STDEXT_ARCHIVE_WRITER_H__
#include "Writer.h"
#endif

#ifndef __STDEXT_ARCHIVE_READER_H__
#include "Reader.h"
#endif

#ifndef __STDEXT_ARCHIVE_WRITEARCHIVE_H__
#include "WriteArchive.h"
#endif

#ifndef __STDEXT_ARCHIVE_READARCHIVE_H__
#include "ReadArchive.h"
#endif

#ifndef __STDEXT_LARGEINTEGER_H__
#include "../LargeInteger.h"
#endif

#if !defined(STD_NO_WINSDK)
	#if !defined(WINX_USE_WINSDK)
		#if !defined(_MSC_VER) || (_MSC_VER <= 1200)
			STDAPI SHCreateStreamOnFileA(LPCSTR pszFile, DWORD grfMode, struct IStream **ppstm);
			STDAPI SHCreateStreamOnFileW(LPCWSTR pszFile, DWORD grfMode, struct IStream **ppstm);
		#else
			#ifndef _INC_SHLWAPI
			#include <shlwapi.h>
			#endif
		#endif
	#endif
#else
	#ifndef __STDEXT_WINAPI_SHLWAPI_H__
	#include "../winapi/shlwapi.h"
	#endif
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// toInt, toUInt

__forceinline int winx_call toInt(const LargeInteger& rhs)
{
	return (int)rhs.QuadPart;
}

__forceinline UINT winx_call toUInt(const ULargeInteger& rhs)
{
	return (UINT)rhs.QuadPart;
}

// -------------------------------------------------------------------------

#ifndef _WINX_STREAM_THROW_IO_ERROR
#define _WINX_STREAM_THROW_IO_ERROR(sc)	throw IoException(sc)
#endif

// -------------------------------------------------------------------------
// class IStreamAdapter

class IStreamAdapter
{
private:
	typedef char _E;

public:
	enum { endch = -1 };

	typedef _E				char_type;
	typedef int				int_type;
	typedef unsigned char	uchar_type;
	typedef ULargeInteger	pos_type;
	typedef LargeInteger	off_type;
	typedef unsigned 		size_type;
	
protected:
	IStream* m_pStrm;
	
public:
	IStreamAdapter()
	{
		m_pStrm = NULL;
	}
	IStreamAdapter(const IStreamAdapter& rhs, bool do_clone)
	{
		if (do_clone)
		{
			m_pStrm = NULL;
			if (rhs.m_pStrm)
			{
				rhs.m_pStrm->Clone(&m_pStrm);
			}
		}
		else
		{
			m_pStrm = rhs.m_pStrm;
			if (m_pStrm)
				m_pStrm->AddRef();
		}
	}
	~IStreamAdapter()
	{
		if (m_pStrm)
			m_pStrm->Release();
	}

	void winx_call attach(IStream* pStrm)
	{
		if (m_pStrm)
			m_pStrm->Release();
		m_pStrm = pStrm;
	}

	IStream* winx_call detach()
	{
		IStream* p = m_pStrm;
		m_pStrm = NULL;
		return p;
	}

	HRESULT winx_call open(LPCWSTR szFile, UINT nMode)
	{
		WINX_ASSERT(m_pStrm == NULL);
		return SHCreateStreamOnFileW(szFile, nMode, &m_pStrm);
	}

	void winx_call open_handle(IStream* pStrm)
	{
		WINX_ASSERT(m_pStrm == NULL);
		WINX_ASSERT(tell() == 0);

		m_pStrm = pStrm;
		if(pStrm)
			pStrm->AddRef();
	}

	void winx_call open_handle(IStream* pStrm, const pos_type& pos)
	{
		WINX_ASSERT(m_pStrm == NULL);
		WINX_ASSERT(tell() == pos);

		m_pStrm = pStrm;
		if(pStrm)
			pStrm->AddRef();
	}

	HRESULT winx_call open_to_read(LPCWSTR szFile)
	{
		WINX_ASSERT(m_pStrm == NULL);
		return SHCreateStreamOnFileW(szFile, STGM_READ, &m_pStrm);
	}

	HRESULT winx_call open_to_read(LPCSTR szFile)
	{
		WINX_ASSERT(m_pStrm == NULL);
		return SHCreateStreamOnFileA(szFile, STGM_READ, &m_pStrm);
	}

	HRESULT winx_call open_to_write(LPCWSTR szFile)
	{
		WINX_ASSERT(m_pStrm == NULL);
		return SHCreateStreamOnFileW(szFile, STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE, &m_pStrm);
	}

	HRESULT winx_call open_to_write(LPCSTR szFile)
	{
		WINX_ASSERT(m_pStrm == NULL);
		return SHCreateStreamOnFileA(szFile, STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE, &m_pStrm);
	}

	void winx_call close()
	{
		if (m_pStrm)
		{
			m_pStrm->Release();
			m_pStrm = NULL;
		}
	}

	IStream** winx_call operator&()
	{
		WINX_ASSERT(m_pStrm == NULL);
		return &m_pStrm;
	}

	operator IStream*() const
	{
		return m_pStrm;
	}
	
	int winx_call operator!() const
	{
		return m_pStrm == NULL;
	}

	int winx_call bad() const
	{
		return m_pStrm == NULL;
	}

	int winx_call good() const
	{
		return m_pStrm != NULL;
	}
	
	void winx_call seek(IN const off_type& pos, IN int dir = SEEK_SET) throw(IoException)
	{
		HRESULT hr = m_pStrm->Seek(pos, dir, NULL);
		if (FAILED(hr))
		{
			_WINX_STREAM_THROW_IO_ERROR(hr);
		}
	}

	pos_type winx_call tell() const
	{
		ULARGE_INTEGER dlibPos;
		ULISet32(dlibPos, 0);
		m_pStrm->Seek(*(LARGE_INTEGER*)&dlibPos, SEEK_CUR, &dlibPos);
		return dlibPos;
	}

	pos_type winx_call size() const
	{
		STATSTG statstg;
		ULISet32(statstg.cbSize, 0);
		m_pStrm->Stat(&statstg, STATFLAG_NONAME);
		return statstg.cbSize;
	}

	int_type winx_call get()
	{
		int_type ch = 0;
		ULONG stRead = 0;
		m_pStrm->Read((void*)&ch, sizeof(char_type), &stRead);
		if (!stRead)
			return endch;
		return ch;
	}
	
	size_type winx_call get(char_type* buf, size_type cch)
	{
		ULONG stRead = 0;
		m_pStrm->Read((void*)buf, sizeof(char_type)*cch , &stRead);
		return stRead;
	}
	
	size_type winx_call read(void* buf, size_type cb)
	{
		ULONG stRead = 0L;
		m_pStrm->Read(buf, sizeof(char_type)*cb, &stRead);
		return stRead;
	}

	void winx_call flush() throw(IoException)
	{
		HRESULT hr = m_pStrm->Commit(0);
		if (FAILED(hr))
		{
			_WINX_STREAM_THROW_IO_ERROR(hr);
		}
	}
	
	void winx_call put(const _E* s, size_type cch) throw(IoException)
	{
		ULONG uWrite = 0L;
		HRESULT hr = m_pStrm->Write((void*)s, (ULONG)cch, &uWrite);
		if (SUCCEEDED(hr))
		{
			if (uWrite == cch)
				return;
			//
			//The caller does not have enough permissions for writing to this stream object. 
			//Note:
			//	磁盘写失败，通常出于两种原因：
			//	1）介质满（DiskFull: STG_E_MEDIUMFULL）。
			//	2）介质只读（Permissions: STG_E_ACCESSDENIED）。
			//
			hr = STG_E_ACCESSDENIED;
		}
		_WINX_STREAM_THROW_IO_ERROR(hr);
	}

	void winx_call put(const _E* s)
	{
		const _E* ps = s;
		while(*ps)
			++ps;
		put(s, (size_type)(ps - s));
	}
	
	void winx_call put(size_type cch, _E ch)
	{
		_E* p0 = new _E[cch];
		_E* p = p0;
		while (cch--)
			*p++ = ch;
		put(p0, (size_type)(p - p0));
		delete[] p0;
	}
	
	void winx_call put(_E ch)
	{
		put(&ch, 1);
	}

	size_type winx_call write(const void* buf, size_type cb)
	{
		ULONG uWrite = 0L;
		m_pStrm->Write((void*)buf, (ULONG)cb, &uWrite);
		return uWrite;
	}
};

// -------------------------------------------------------------------------

typedef WriteArchive<IStream*, IStreamAdapter> StreamWriteArchive;
typedef Writer<StreamWriteArchive> StreamWriter;

typedef ReadArchive<IStream*, IStreamAdapter> StreamReadArchive;
typedef Reader<StreamReadArchive> StreamReader;

// -------------------------------------------------------------------------
// class MemStreamWriter

class MemStreamWriter : public StreamWriter
{
private:
	typedef StreamWriter _Base;

public:
	template <class AllocT>
	explicit MemStreamWriter(AllocT& alloc)
		: _Base(alloc)
	{
		CreateStreamOnHGlobal(NULL, FALSE, &m_handle);
	}
	~MemStreamWriter()
	{
		close();
	}

public:
	void winx_call close()
	{
		if (m_handle.good())
		{
			HGLOBAL hgbl = detach();
			if (hgbl)
				GlobalFree(hgbl);
		}
	}

	HGLOBAL winx_call detach()
	{
		HGLOBAL hgbl = NULL;
		clear_cache();
		GetHGlobalFromStream(m_handle, &hgbl);
		m_handle.close();
		return hgbl;
	}

	HGLOBAL winx_call get_handle()
	{
		HGLOBAL hgbl = NULL;
		clear_cache();
		GetHGlobalFromStream(m_handle, &hgbl);
		return hgbl;
	}
};

// -------------------------------------------------------------------------
// class MemStreamReader

class MemStreamReader : public StreamReader
{
private:
	typedef StreamReader _Base;

public:
	template <class AllocT>
	explicit MemStreamReader(AllocT& alloc)
		: _Base(alloc) {}

	template <class AllocT>
	MemStreamReader(AllocT& alloc, HGLOBAL hgbl)
		: _Base(alloc)
	{
		CreateStreamOnHGlobal(hgbl, FALSE, &m_handle);
	}

public:
	HRESULT winx_call open(HGLOBAL hgbl, BOOL fDeleteOnRelease = FALSE)
	{
		if (good())
			return E_ACCESSDENIED;
		return CreateStreamOnHGlobal(hgbl, fDeleteOnRelease, &m_handle);
	}
};

// -------------------------------------------------------------------------
// class TestStreamArchive

template <class LogT>
class TestStreamArchive : public TestCase
{
	WINX_TEST_SUITE(TestStreamArchive);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		typedef std::StreamReader ReaderT;
		typedef std::StreamWriter WriterT;

		const char stg[] = "/__teststream__.txt";

		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		WINX_USES_CONVERSION;
		{
			WriterT ar(alloc, WINX_A2W(stg));
			ar.put("hello\n");
			ar.put('!');
			ar.put('\n');
		}
		{
			char szBuf[100];
			ReaderT ar(alloc, stg);
			size_t cch = ar.get(szBuf, countof(szBuf));
			szBuf[cch] = '\0';
			AssertExp(strcmp(szBuf, "hello\n!\n") == 0);
		}
		{
			ReaderT ar(alloc, stg);
			std::string s1;
			ar.getline(s1);
			AssertExp(s1 == "hello");
			std::vector<char> s2;
			ar.getline(s2);
			AssertExp(s2 == std::String(alloc, "!"));
			std::String s3;
			ar.getline(alloc, s3);
			AssertExp(s3.empty());
		}
		// ------------------------------------
		{
			WriterT ar(alloc);
			ar.open(stg);
			ar.put("you're welcome!\n");
		}
		{
			char szBuf[100];
			ReaderT ar(alloc);
			ar.open(WINX_A2W(stg));
			size_t cch = ar.get(szBuf, countof(szBuf));
			szBuf[cch] = '\0';
			AssertExp(strcmp(szBuf, "you're welcome!\n") == 0);
		}
		// ------------------------------------
		{
			char szBuf[100];
			WriterT ar(alloc, stg);
			ar.put(_itoa(13242, szBuf, 10));
			ar.put(' ');
			ar.put(_itoa(1111, szBuf, 10));
		}
		{
			ReaderT ar(alloc, stg);
			unsigned val;
			ar.scan_uint(val);
			AssertExp(val == 13242);
			ar.scan_uint(val, 2);
			AssertExp(val == 15);
		}
		// ------------------------------------
		{
			WriterT ar(alloc, stg);
			ar.puts("Hello!");
			ar.puts(std::string("World"));
			ar.puts(std::vector<char>(255, '!'));
			ar.puts(std::vector<char>(65537, '?'));
		}
		{
			ReaderT ar(alloc, stg);
			std::string s1;
			AssertExp(ar.gets(s1) == S_OK);
			AssertExp(s1 == "Hello!");
			std::vector<char> s2;
			AssertExp(ar.gets(s2) == S_OK);
			AssertExp(std::compare(s2.begin(), s2.end(), "World") == 0);
			std::String s3;
			AssertExp(ar.gets(alloc, s3) == S_OK);
			AssertExp(s3 == std::String(alloc, 255, '!'));
			std::String s4;
			AssertExp(ar.gets(alloc, s4) == S_OK);
			AssertExp(s4 == std::String(alloc, 65537, '?'));
			std::String s5;
			AssertExp(ar.gets(alloc, s5) != S_OK);
		}
	}
};

// -------------------------------------------------------------------------
// $Log: Stream.h,v $

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_STREAM_H__ */
