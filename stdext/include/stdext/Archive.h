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
#ifndef __STDEXT_ARCHIVE_H__
#define __STDEXT_ARCHIVE_H__

// -------------------------------------------------------------------------

#ifndef __STDEXT_ARCHIVE_BASIC_H__
#include "archive/Basic.h"
#endif

#ifndef __STDEXT_ARCHIVE_WRITER_H__
#include "archive/Writer.h"
#endif

#ifndef __STDEXT_ARCHIVE_READER_H__
#include "archive/Reader.h"
#endif

#ifndef __STDEXT_ARCHIVE_WRITEARCHIVE_H__
#include "archive/WriteArchive.h"
#endif

#ifndef __STDEXT_ARCHIVE_READARCHIVE_H__
#include "archive/ReadArchive.h"
#endif

#ifndef __STDEXT_ARCHIVE_STDIO_H__
#include "archive/Stdio.h"
#endif

#ifndef __STDEXT_ARCHIVE_STREAM_H__
#include "archive/Stream.h"
#endif

#ifndef __STDEXT_ARCHIVE_MEMARCHIVE_H__
#include "archive/MemArchive.h"
#endif

#ifndef __STDEXT_ARCHIVE_MEM_H__
#include "archive/Mem.h"
#endif

#ifndef __STDEXT_ARCHIVE_WINREG_H__
#include "archive/WinReg.h"
#endif

#ifndef __STDEXT_ARCHIVE_RECORD_H__
#include "archive/Record.h"
#endif

// -------------------------------------------------------------------------
// class TestRecord

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
			RecordWriterT ar(alloc, &stg);
			
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
			RecordReaderT reader(alloc, &stg);
			RecordReaderT::record_info info;

			AssertExp(reader.next(info));
			AssertExp(info.recId == 23);
			std::MemReader ar1(alloc, &info);
			std::String s;
			AssertExp(ar1.gets(alloc, s) == S_OK);
			AssertExp(s == "Hello, world!");
			ar1.getline(alloc, s);
			AssertExp(s == "You're welcome!");
			AssertExp(ar1.get() == std::MemReader::endch);

			AssertExp(reader.next(info));
			AssertExp(info.recId == 99);
			std::MemReader ar2(alloc, &info);
			UINT16 w;
			AssertExp(ar2.get16i(w) == S_OK);
			AssertExp(w == 32);
			AssertExp(ar2.get_uint() == 123);
			AssertExp(ar2.get() == std::MemReader::endch);
		}
	}
};

__NS_STD_END

// -------------------------------------------------------------------------
// $Log: Archive.h,v $

#endif /* __STDEXT_ARCHIVE_H__ */
