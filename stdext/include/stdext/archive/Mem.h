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
// Module: stdext/archive/Mem.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 21:07:06
// 
// $Id: Mem.h,v 1.3 2007/01/10 09:36:12 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_MEM_H__
#define __STDEXT_ARCHIVE_MEM_H__

#ifndef __STDEXT_ARCHIVE_WRITER_H__
#include "Writer.h"
#endif

#ifndef __STDEXT_ARCHIVE_READER_H__
#include "Reader.h"
#endif

#ifndef __STDEXT_ARCHIVE_MEMARCHIVE_H__
#include "MemArchive.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class MemReader

typedef MemReadArchive<const char*> PointerReadArchive;
typedef Reader<PointerReadArchive> MemReader;

// -------------------------------------------------------------------------
// class VectorReader/VectorWriter

typedef std::vector<char> CharVector;

typedef MemWriteArchive<CharVector> VectorWriteArchive;
typedef Writer<VectorWriteArchive> VectorWriter;

typedef MemReadArchive<CharVector::const_iterator> VectorReadArchive;
typedef Reader<VectorReadArchive> VectorReader;

// -------------------------------------------------------------------------
// class StringBuilderReader/StringBuilderWriter

typedef VectorWriter StringBuilderWriter;
typedef VectorReader StringBuilderReader;

// -------------------------------------------------------------------------
// class StlStringReader/StlStringWriter

typedef MemWriteArchive<std::string> StlStringWriteArchive;
typedef Writer<StlStringWriteArchive> StlStringWriter;

typedef MemReadArchive<std::string::const_iterator> StlStringReadArchive;
typedef Reader<StlStringReadArchive> StlStringReader;

// -------------------------------------------------------------------------
// class DequeReader/DequeWriter, TextPoolReader/TextPoolWriter

#ifdef __STDEXT_DEQUE_H__

typedef std::Deque<char> CharDeque;

typedef MemWriteArchive<CharDeque> DequeWriteArchive;
typedef Writer<DequeWriteArchive> DequeWriter;

typedef MemReadArchive<CharDeque::const_iterator> DequeReadArchive;
typedef Reader<DequeReadArchive> DequeReader;

typedef DequeWriter TextPoolWriter;
typedef DequeReader TextPoolReader;

#endif

// -------------------------------------------------------------------------
// class TestMemArchive

#if defined(STD_UNITTEST)

template <class LogT>
class TestMemArchive : public TestCase
{
	WINX_TEST_SUITE(TestMemArchive);
		WINX_TEST(testVector);
		WINX_TEST(testStlString);
#ifdef __STDEXT_DEQUE_H__
		WINX_TEST(testDeque);
#endif
	WINX_TEST_SUITE_END();

#ifdef __STDEXT_DEQUE_H__
	void testDeque(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		std::CharDeque stg(alloc);
		std::DequeWriter* w = NULL;
		std::DequeReader* r = NULL;
		test(log, w, r, stg);
	}
#endif

	void testVector(LogT& log)
	{
		std::CharVector stg;
		std::VectorWriter* w = NULL;
		std::VectorReader* r = NULL;
		test(log, w, r, stg);
	}

	void testStlString(LogT& log)
	{
		std::string stg;
		std::StlStringWriter* w = NULL;
		std::StlStringReader* r = NULL;
		test(log, w, r, stg);
	}

	template <class WriterT, class ReaderT, class StorageT>
	void test(LogT& log, WriterT*, ReaderT*, StorageT& stg)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		{
			WriterT ar(&stg);
			ar.put("hello\n");
			ar.put('\n');
		}
		{
			char szBuf[100];
			ReaderT ar(&stg);
			size_t cch = ar.get(szBuf, countof(szBuf));
			szBuf[cch] = '\0';
			AssertExp(strcmp(szBuf, "hello\n\n") == 0);
		}
		stg.erase(stg.begin(), stg.end());
		{
			WriterT ar;
			ar.open(&stg);
			ar.put("you're welcome!\n");
		}
		{
			char szBuf[100];
			ReaderT ar(stg.begin(), stg.end());
			size_t cch = ar.get(szBuf, countof(szBuf));
			szBuf[cch] = '\0';
			AssertExp(strcmp(szBuf, "you're welcome!\n") == 0);
		}
		stg.erase(stg.begin(), stg.end());
		{
			char szBuf[100];
			WriterT ar(&stg);
			ar.put(_itoa(13242, szBuf, 10));
			ar.put(' ');
			ar.put(_itoa(1111, szBuf, 10));
		}
		{
			ReaderT ar(&stg);
			unsigned val;
			ar.scan_uint(val);
			AssertExp(val == 13242);
			ar.scan_uint(val, 2);
			AssertExp(val == 15);
		}
		stg.erase(stg.begin(), stg.end());
		{
			WriterT ar(&stg);
			ar.puts("Hello");
			ar.puts(std::string("World!"));
			ar.puts(std::vector<char>(256, '!'));
			ar.puts(std::vector<char>(65537, '?'));
		}
		{
			ReaderT ar(&stg);
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

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: Mem.h,v $

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_MEM_H__ */
