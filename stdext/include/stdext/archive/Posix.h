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
// Module: stdext/archive/Posix.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 21:07:06
// 
// $Id: Posix.h,v 1.3 2007/01/10 09:36:12 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_POSIX_H__
#define __STDEXT_ARCHIVE_POSIX_H__

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

#ifndef __STDEXT_ARCHIVE_POSIX_CONFIG_H__
#include "posix/Config.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class PosixAdapter

#pragma warning(disable:4996) // XXX  was declared deprecated

class PosixAdapter
{
public:
	enum { endch = -1 };
	enum { nullfd = -1 };

	typedef char			char_type;
	typedef unsigned char	uchar_type;
	typedef int				int_type;
	
	typedef size_t			size_type;
	
	typedef __off_t			pos_type;
	typedef __off_t			off_type;

private:
	typedef char_type _E;
	typedef int _Handle;

	enum { writeMode = O_TRUNC|O_CREAT|O_BINARY|O_WRONLY };
	enum { readMode = O_BINARY|O_RDONLY };
	enum { CMASK = 0644 }; // wrr

protected:
	_Handle m_fd;
	
public:
	PosixAdapter() : m_fd(nullfd) {}
	~PosixAdapter()
	{
		if (m_fd != nullfd)
		{
			::close(m_fd);
			m_fd = nullfd;
		}
	}
	
	void winx_call attach(_Handle fd)
	{
		m_fd = fd;
	}

	int winx_call detach()
	{
		_Handle fd = m_fd;
		m_fd = nullfd;
		return fd;
	}

	void winx_call open_handle(_Handle fd)
	{
		WINX_ASSERT(m_fd == nullfd);
		WINX_ASSERT(tell() == 0);
		m_fd = fd;
	}

	void winx_call open_handle(_Handle fd, const pos_type& pos)
	{
		WINX_ASSERT(m_fd == nullfd);
		WINX_ASSERT(tell() == pos);
		m_fd = fd;
	}

	HRESULT winx_call open_to_write(LPCWSTR szFile)
	{
		WINX_ASSERT(m_fd == nullfd);
		WINX_USES_CONVERSION;
		m_fd = ::open(WINX_W2CA(szFile), writeMode, CMASK);
		return good() ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_write(LPCSTR szFile)
	{
		WINX_ASSERT(m_fd == nullfd);
		m_fd = ::open(szFile, writeMode, CMASK);
		return good() ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_read(LPCWSTR szFile)
	{
		WINX_ASSERT(m_fd == nullfd);
		WINX_USES_CONVERSION;
		m_fd = ::open(WINX_W2CA(szFile), readMode, CMASK);
		return good() ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_read(LPCSTR szFile)
	{
		WINX_ASSERT(m_fd == nullfd);
		m_fd = ::open(szFile, readMode, CMASK);
		return good() ? S_OK : E_ACCESSDENIED;
	}

	void winx_call seek(const off_type& pos, int dir = SEEK_SET)
	{
		::lseek(m_fd, pos, dir);
	}
	
	pos_type winx_call tell() const
	{
		return ::tell(m_fd);
	}

	pos_type winx_call size() const
	{
		return _filelength(m_fd);
	}
	
	size_type winx_call get(_E* buf, size_type cch)
	{	
		return ::read(m_fd, buf, cch);
	}

	size_type winx_call put(const _E* s, size_type cch)
	{
		return ::write(m_fd, s, cch);
	}

	void winx_call flush()
	{
		::_commit(m_fd);
	}

	void winx_call close()
	{
		if (m_fd != nullfd)
		{
			::close(m_fd);
			m_fd = nullfd;
		}
	}

	operator _Handle() const
	{
		return m_fd;
	}

	int winx_call operator!() const
	{
		return m_fd == nullfd;
	}

	int winx_call good() const
	{
		return m_fd != nullfd;
	}

	int winx_call bad() const
	{
		return m_fd == nullfd;
	}
};

#pragma warning(default:4996) // XXX  was declared deprecated

// -------------------------------------------------------------------------

typedef WriteArchive<int, PosixAdapter> PosixWriteArchive;
typedef Writer<PosixWriteArchive> PosixWriter;

typedef ReadArchive<int, PosixAdapter> PosixReadArchive;
typedef Reader<PosixReadArchive> PosixReader;

// -------------------------------------------------------------------------
// class TestPosixArchive

#if defined(STD_UNITTEST)

#pragma warning(disable:4996) // XXX  was declared deprecated

template <class LogT>
class TestPosixArchive : public TestCase
{
	WINX_TEST_SUITE(TestPosixArchive);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		typedef std::PosixReader ReaderT;
		typedef std::PosixWriter WriterT;

		const char stg[] = "/__testposix__.txt";

		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		WINX_USES_CONVERSION;
		{
			WriterT ar(stg);
			AssertExp(ar.good());
			ar.put("hello\n");
			ar.put('\n');
		}
		{
			char szBuf[100];
			ReaderT ar(stg);
			AssertExp(ar.good());
			size_t cch = ar.get(szBuf, countof(szBuf));
			szBuf[cch] = '\0';
			AssertExp(strcmp(szBuf, "hello\n\n") == 0);
		}
		// ------------------------------------
		{
			WriterT ar;
			ar.open(WINX_A2W(stg));
			AssertExp(ar.good());
			ar.put("you're welcome!\n");
		}
		{
			char szBuf[100];
			ReaderT ar;
			ar.open(WINX_A2W(stg));
			AssertExp(ar.good());
			size_t cch = ar.get(szBuf, countof(szBuf));
			szBuf[cch] = '\0';
			AssertExp(strcmp(szBuf, "you're welcome!\n") == 0);
		}
		{
			ReaderT ar(stg);
			std::String sym;
			AssertExp(ar.get_csymbol(alloc, sym) == sym.size());
			AssertExp(sym == "you");
			AssertExp(ar.get() == '\'');
			AssertExp(ar.get_csymbol(alloc, sym) == sym.size());
			AssertExp(sym == "re");
			AssertExp(ar.scan_csymbol(alloc, sym) == S_OK);
			AssertExp(sym == "welcome");
		}
		// ------------------------------------
		{
			char szBuf[100];
			WriterT ar(stg);
			ar.put(_itoa(13242, szBuf, 10));
			ar.put(' ');
			ar.put(_itoa(1111, szBuf, 10));
		}
		{
			ReaderT ar(stg);
			unsigned val;
			ar.scan_uint(val);
			AssertExp(val == 13242);
			ar.scan_uint(val, 2);
			AssertExp(val == 15);
		}
		// ------------------------------------
		{
			WriterT ar(stg);
			ar.puts("Hello");
			ar.puts(std::string("World"));
			ar.puts(std::vector<char>(255, '!'));
			ar.puts(std::vector<char>(65538, '?'));
		}
		{
			ReaderT ar(stg);
			std::string s1;
			AssertExp(ar.gets(s1) == S_OK);
			AssertExp(s1 == "Hello");
			std::vector<char> s2;
			AssertExp(ar.gets(s2) == S_OK);
			AssertExp(std::compare(s2.begin(), s2.end(), "World") == 0);
			std::String s3;
			AssertExp(ar.gets(alloc, s3) == S_OK);
			AssertExp(s3 == std::String(alloc, 255, '!'));
			std::String s4;
			AssertExp(ar.gets(alloc, s4) == S_OK);
			AssertExp(s4 == std::String(alloc, 65538, '?'));
			std::String s5;
			AssertExp(ar.gets(alloc, s5) != S_OK);
		}
	}
};

#pragma warning(default:4996) // XXX  was declared deprecated

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: Posix.h,v $

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_POSIX_H__ */
