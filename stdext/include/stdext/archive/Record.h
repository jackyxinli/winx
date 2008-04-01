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
// Module: stdext/archive/Record.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-26 0:19:33
// 
// $Id: Record.h,v 1.2 2006/11/30 03:19:24 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_RECORD_H__
#define __STDEXT_ARCHIVE_RECORD_H__

#ifndef __STDEXT_ARCHIVE_DIRECTBUFFER_H__
#include "DirectBuffer.h"
#endif

__NS_STD_BEGIN

#pragma pack(1)

// -------------------------------------------------------------------------
// DirectReadBuffer Algorithms

template <class DirectReadBufferT, class HeaderT>
inline typename DirectReadBufferT::const_pointer
	winx_call next_record(DirectReadBufferT& ar, HeaderT& hdr)
{
	const HeaderT* hdr0 = (const HeaderT*)ar.read(sizeof(HeaderT));
	if (hdr0 == NULL)
		return NULL;

	hdr = *hdr0;
	_WinxByteSwapStruct(hdr);

	return ar.read(hdr.cbData);
}

// =========================================================================
// class MemRecordArchive

struct RecordHeaderType
{
	UINT32 recId;
	UINT32 cbData;
};

template <class BaseWriter, class HeaderT = RecordHeaderType, int recIdEOF = 0>
class RecordWriter : public BaseWriter
{
private:
	typedef BaseWriter _Base;

public:
	typedef HeaderT header_type;
	typedef typename _Base::pos_type pos_type;

private:
	UINT m_recId;
	pos_type m_offset;
	
public:
	template <class AllocT>
	explicit RecordWriter(AllocT& alloc)
		: _Base(alloc)
	{
		m_recId = recIdEOF;
	}

	template <class AllocT, class InitArgT>
	RecordWriter(AllocT& alloc, InitArgT file)
		: _Base(alloc, file)
	{
		m_recId = recIdEOF;
	}

	HRESULT winx_call beginRecord(UINT recId)
	{
		WINX_ASSERT(m_recId == recIdEOF);

		header_type hdr = { recIdEOF, 0 };
		_WinxByteSwapStruct(hdr);
		_Base::put((const char*)&hdr, sizeof(header_type));
		m_recId = recId;
		m_offset = _Base::tell();
		return S_OK;
	}
	
	HRESULT winx_call endRecord()
	{
		const UINT32 cbData = _Base::tell() - m_offset;
		header_type hdr = { m_recId, cbData };
		_WinxByteSwapStruct(hdr);
		_Base::reput(
			cbData + sizeof(header_type), (const char*)&hdr, sizeof(header_type));
		m_recId = recIdEOF;
		return S_OK;
	}
};

// =========================================================================
// class RecordReader

template <class DirectReadBufferT, class HeaderT = RecordHeaderType>
class RecordReader : private DirectReadBufferT
{
private:
	typedef DirectReadBufferT _Base, BaseClass;

public:
	typedef HeaderT header_type;

	typedef typename DirectReadBufferT::char_type char_type;
	typedef typename DirectReadBufferT::const_pointer const_pointer;

public:
	RecordReader()
	{
	}

	template <class InitArgT1, class InitArgT2>
	RecordReader(InitArgT1 arg1, InitArgT2 arg2)
		: _Base(arg1, arg2)
	{
	}

public:
	const_pointer winx_call next(OUT HeaderT& hdr)
	{
		return std::next_record(WINX_BASE, hdr);
	}
};

// =========================================================================
// class TestRecord

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

		std::CharVector stg;
		{
			RecordWriterT ar(&stg);
			//@@todo
		}
	}
};

// =========================================================================
// $Log: Record.h,v $

#pragma pack()

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_RECORD_H__ */
