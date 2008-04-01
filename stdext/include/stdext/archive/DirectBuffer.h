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
// Module: stdext/archive/DirectBuffer.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: DirectBuffer.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_DIRECTBUFFER_H__
#define __STDEXT_ARCHIVE_DIRECTBUFFER_H__

#ifndef __STDEXT_ARCHIVE_BASIC_H__
#include "Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class DirectReadBuffer

class DirectReadBuffer
{
public:
	enum { SuggestBufSize = 0x2400 };

	typedef UINT32 off_type;
	typedef UINT size_type;
	
private:
	typedef unsigned char UINT8;

	UINT8* m_lpBufStart;
	UINT8* m_lpBufCur;
	UINT8* m_lpBufMax;
	UINT32 m_lbPhyPos; // 对应的磁盘当前位置
	UINT32 m_cbBufSize;
	IStream* m_pStrm;
	
private:
	DirectReadBuffer(const DirectReadBuffer&);
	void operator=(const DirectReadBuffer&);
	
	void winx_call _initBuffer(UINT32 nBufSize)
	{
		//
		// 读状态：m_lpBufMax - m_lpBufCur 为已经读入预读的数据！
		//
		m_cbBufSize = nBufSize;
		m_lpBufStart = (UINT8*)malloc(nBufSize);
		m_lpBufCur = m_lpBufMax = m_lpBufStart;
	}

public:
	explicit DirectReadBuffer(UINT32 nBufSize = SuggestBufSize)
		: m_pStrm(NULL)
	{
		_initBuffer(nBufSize);
	}
	DirectReadBuffer(IStream* pStream, off_type lbPhyPos = 0, UINT32 nBufSize = SuggestBufSize)
		: m_pStrm(NULL)
	{
		_initBuffer(nBufSize);
		open(pStream, lbPhyPos);
	}
	~DirectReadBuffer()
	{
		free(m_lpBufStart);
		if (m_pStrm)
			m_pStrm->Release();
	}
	
	void winx_call close()
	{
		if (m_pStrm)
		{
			m_pStrm->Release();
			m_pStrm = NULL;
		}
	}
	
	HRESULT winx_call open(IStream* pStream, off_type lbPhyPos = 0)
	{
		if (pStream == NULL)
			return E_INVALIDARG;
		
		if (m_pStrm != NULL)
			return E_ACCESSDENIED;
		
#if defined(_DEBUG)
		LARGE_INTEGER dlibMove;
		ULARGE_INTEGER libCur;
		LISet32(dlibMove, 0);
		pStream->Seek(dlibMove, SEEK_CUR, &libCur);
		WINX_ASSERT(
			libCur.HighPart == 0 && libCur.LowPart == lbPhyPos);
#endif
		m_lbPhyPos = lbPhyPos;
		m_pStrm = pStream;
		m_pStrm->AddRef();
		return S_OK;
	}
	
	int winx_call good() const
	{
		return m_pStrm != NULL;
	}
	
	int winx_call bad() const
	{
		return m_pStrm == NULL;
	}
	
	off_type winx_call tell() const
	{
		return m_lbPhyPos - (m_lpBufMax - m_lpBufCur);
	}
	
	HRESULT winx_call seek(IN off_type lbPlyPos)
	{
		if (
			lbPlyPos <= m_lbPhyPos &&
			(m_lbPhyPos - lbPlyPos) <= (UINT32)(m_lpBufMax - m_lpBufStart)
			)
		{
			m_lpBufCur = m_lpBufMax - (m_lbPhyPos - lbPlyPos);
			return S_OK;
		}
		else
		{
			//clear cache
			m_lpBufCur = m_lpBufMax = m_lpBufStart;
			
			//seek
			LARGE_INTEGER dlibMove;
			ULISet32(dlibMove, lbPlyPos);
			HRESULT hr = m_pStrm->Seek(dlibMove, SEEK_SET, (ULARGE_INTEGER*)&dlibMove);
			m_lbPhyPos = dlibMove.LowPart;
			return hr;
		}
	}

	const UINT8* winx_call read(size_type cb)
	{
		if (m_lpBufCur + cb <= m_lpBufMax)
		{
			m_lpBufCur += cb;
			return m_lpBufCur - cb;
		}
		else
		{
			ULONG cbRead;
			size_type cbRest = m_lpBufMax - m_lpBufCur;
			if (cb > m_cbBufSize)
			{
				m_cbBufSize = ROUND(cb, 0x200);
				UINT8* lpNewBuf = (UINT8*)malloc(m_cbBufSize);
				CopyMemory(lpNewBuf, m_lpBufCur, cbRest);
				free(m_lpBufStart);
				m_lpBufStart = lpNewBuf;
			}
			else
			{
				CopyMemory(m_lpBufStart, m_lpBufCur, cbRest);
			}

			m_lpBufCur = m_lpBufStart + cbRest;
			cb -= cbRest;

			m_pStrm->Read(m_lpBufCur, m_cbBufSize - cbRest, &cbRead);
			m_lbPhyPos += cbRead;
			if (cbRead < cb)
			{
				m_lpBufCur = m_lpBufMax;
				return NULL; // 失败！
			}

			m_lpBufMax = m_lpBufCur + cbRead;
			m_lpBufCur += cb;
			return m_lpBufStart;
		}
	}
};

// -------------------------------------------------------------------------
// $Log: DirectBuffer.h,v $

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_DIRECTBUFFER_H__ */
