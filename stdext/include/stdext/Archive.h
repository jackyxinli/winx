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
// Module: stdext/Archive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-25 9:47:19
// 
// $Id: Archive.h,v 1.2 2006/11/30 03:19:17 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_H
#define STDEXT_ARCHIVE_H

#if (0)
#define STD_STREAM_ARCHIVE
#endif

// -------------------------------------------------------------------------

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "archive/Basic.h"
#endif

#ifndef STDEXT_ARCHIVE_WRITER_H
#include "archive/Writer.h"
#endif

#ifndef STDEXT_ARCHIVE_READER_H
#include "archive/Reader.h"
#endif

#ifndef STDEXT_ARCHIVE_WRITEARCHIVE_H
#include "archive/WriteArchive.h"
#endif

#ifndef STDEXT_ARCHIVE_READARCHIVE_H
#include "archive/ReadArchive.h"
#endif

#ifndef STDEXT_ARCHIVE_STDIO_H
#include "archive/Stdio.h"
#endif

#ifndef STDEXT_ARCHIVE_POSIX_H
#include "archive/Posix.h"
#endif

#ifndef STDEXT_ARCHIVE_MEMARCHIVE_H
#include "archive/MemArchive.h"
#endif

#ifndef STDEXT_ARCHIVE_RECORD_H
#include "archive/Record.h"
#endif

#ifndef STDEXT_ARCHIVE_MEM_H
#include "archive/Mem.h"
#endif

#if defined(X_OS_WINDOWS)

#if defined(STD_STREAM_ARCHIVE)
	#ifndef STDEXT_ARCHIVE_STREAM_H
	#include "archive/Stream.h"
	#endif
#endif // defined(STD_STREAM_ARCHIVE)

#ifndef STDEXT_ARCHIVE_WINREG_H
#include "archive/WinReg.h"
#endif

#endif

// -------------------------------------------------------------------------
// class FileReader/FileWriter

__NS_STD_BEGIN

typedef PosixReadArchive FileReadArchive;
typedef Reader<FileReadArchive> FileReader;

typedef PosixWriteArchive FileWriteArchive;
typedef Writer<FileWriteArchive> FileWriter;

__NS_STD_END

// -------------------------------------------------------------------------
// class TestRecord

#if defined(STD_UNITTEST)

__NS_STD_BEGIN

template <class LogT>
class TestRecord : public TestCase
{
	WINX_TEST_SUITE(TestRecord);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		typedef std::RecordWriter<std::VectorWriter> RecordWriterT;
		typedef std::RecordReader<std::VectorReadArchive> RecordReaderT;

		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
	
		std::CharVector stg;
		{
			RecordWriterT ar(&stg);
			
			ar.beginRecord(23);
			ar.puts("Hello, world!");
			ar.put("You're welcome!\n");
			ar.endRecord();

			ar.beginRecord(99);
			ar.put16i(32);
			ar.put("123");
			ar.endRecord();
		}
		{
			RecordReaderT reader(&stg);
			RecordReaderT::record_info info;

			AssertExp(reader.next(info));
			AssertExp(info.recId == 23);
			std::MemReader ar1(&info);
			std::String s;
			AssertExp(ar1.gets(alloc, s) == S_OK);
			AssertExp(s == "Hello, world!");
			ar1.getline(alloc, s);
			AssertExp(s == "You're welcome!");
			AssertExp(ar1.get() == std::MemReader::endch);

			AssertExp(reader.next(info));
			AssertExp(info.recId == 99);
			std::MemReader ar2(&info);
			UINT16 w;
			AssertExp(ar2.get16i(w) == S_OK);
			AssertExp(w == 32);
			AssertExp(ar2.get_uint() == 123);
			AssertExp(ar2.get() == std::MemReader::endch);

			AssertExp(!reader.next(info));
		}
	}
};

__NS_STD_END

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: Archive.h,v $

#endif /* STDEXT_ARCHIVE_H */
