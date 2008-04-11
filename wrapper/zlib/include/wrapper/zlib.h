/* -------------------------------------------------------------------------
//	文件名		：	wrapper/zlib.h
//	创建者		：	许式伟
//	创建时间	：	2003-10-14 17:57:09
//	功能描述	：	
//
//	$Id: zlib.h,v 1.12 2006/09/01 00:55:56 zhujunhang Exp $

// -----------------------------------------------------------------------*/
#ifndef __WRAPPER_ZLIB_H__
#define __WRAPPER_ZLIB_H__

#ifndef __STDEXT_BASIC_H__
#include "../../../../stdext/include/stdext/Basic.h"
#endif

#ifndef ZLIB_H
#include "../../../../zlib/zlib.h"
#endif

#ifndef S_ZIP_NONEED_COMPRESSION
#define S_ZIP_NONEED_COMPRESSION	0x80
#endif

// -------------------------------------------------------------------------
// ZlibCompress

template <class AllocT>
inline HRESULT winx_call ZlibCompress(
	AllocT& alloc, const void* pSrc, size_t cbSize,
	void** ppBuf, size_t* pcbAfterCompress, int method = Z_DEFAULT_COMPRESSION)
{
	WINX_ASSERT(Z_OK == S_OK && sizeof(size_t) >= sizeof(uLongf));

	uLongf const cbAfterCompress0 = cbSize + cbSize/1000 + (12 + 4); // 4 是额外加的
	Bytef* const pDest = (Bytef*)alloc.allocate(cbAfterCompress);
	
	HRESULT hr;
	uLongf cbAfterCompress = cbAfterCompress0;
	try
	{
		hr = compress2(pDest, &cbAfterCompress, (const Bytef*)pSrc, cbSize, method);
	}
	catch (...)
	{
		hr = E_UNEXPECTED;
	}
	
	if (hr == Z_OK)
	{
		if (cbAfterCompress >= cbSize)
		{
			hr = S_ZIP_NONEED_COMPRESSION;
			// 就算返回了S_ZIP_NONEED_COMPRESSION，仍然进行压缩，返回压缩结果。
			//--> goto lzExit;
		}
		*ppBuf = alloc.reallocate(pDest, cbAfterCompress0, cbAfterCompress);
		*pcbAfterCompress = cbAfterCompress;
		return hr;
	}
	
	alloc.deallocate(pDest, cbAfterCompress0);
	return hr;
}

// -------------------------------------------------------------------------
// ZlibDecompress

template <class AllocT>
inline HRESULT winx_call ZlibDecompress(
	AllocT& alloc, const void* pSrc, size_t cbSize, size_t cbOrgSize,
	void** ppBuf, UINT nErrLevel = 0)
{
	WINX_ASSERT(Z_OK == S_OK && sizeof(size_t) >= sizeof(uLongf));

	Bytef* const pDest = (Bytef*)alloc.allocate(cbOrgSize);
	uLongf cbDecodeSize = cbOrgSize;
	
	HRESULT hr;
	try
	{
		hr = uncompress(pDest, &cbDecodeSize, (const Bytef*)pSrc, cbSize, nErrLevel);
		WINX_ASSERT(hr == Z_OK);
		if (cbDecodeSize !=	cbOrgSize)
		{
			hr = E_FAIL;
			goto lzExit;
		}
	}
	catch (...)
	{
		WINX_ASSERT( !"ZlibDecompress failed!" );
		hr = E_UNEXPECTED;
	}
	
	if (hr == Z_OK)	// if success!!
	{
		*ppBuf = pDest;
		return S_OK;
	}
	
lzExit:
	alloc.deallocate(pDest);
	return hr;
}

// -------------------------------------------------------------------------
// $Log: zlib.h,v $

#endif /* __WRAPPER_ZLIB_H__ */
