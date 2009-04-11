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
// Module: stdext/archive/Binary.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: Binary.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_BINARY_H
#define STDEXT_ARCHIVE_BINARY_H

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "../text/BasicString.h"
#endif

#ifndef NS_STDEXT_IO_BINARY
#define NS_STDEXT_IO_BINARY			NS_STDEXT::io
#define NS_STDEXT_IO_BINARY_BEGIN	NS_STDEXT_BEGIN namespace io {
#define NS_STDEXT_IO_BINARY_END		} NS_STDEXT_END
#endif

NS_STDEXT_IO_BINARY_BEGIN

// -------------------------------------------------------------------------
// ==== BinaryWriter ====

template <class WriteArchiveT>
inline void winx_call put16i(
	IN WriteArchiveT& ar, IN UINT16 val) throw(IoException)
{
	_WinxByteSwap16(val);
	ar.put( (const char*)&val, sizeof(val) );
}

template <class WriteArchiveT>
inline void winx_call put32i(
	IN WriteArchiveT& ar, IN UINT32 val) throw(IoException)
{
	_WinxByteSwap32(val);
	ar.put( (const char*)&val, sizeof(val) );
}

template <class WriteArchiveT, class StrucType>
inline void winx_call put_struct(
	IN WriteArchiveT& ar, IN const StrucType& struc) throw(IoException)
{
	_WinxByteSwapStruct(struc);
	ar.put( (const char*)&struc, sizeof(struc) );
}

#if defined(WINX_BYTESWAP)

template <class WriteArchiveT>
inline void put16i(
	IN WriteArchiveT& ar, IN const UINT16 warray[], IN UINT count) throw(IoException)
{
	for (UINT i = 0; i < count; ++i)
		put16i(ar, warray[i]);
}

template <class WriteArchiveT>
inline void put16i(
	IN WriteArchiveT& ar, IN const INT16 warray[], IN UINT count) throw(IoException)
{
	for (UINT i = 0; i < count; ++i)
		put16i(ar, warray[i]);
}

template <class WriteArchiveT>
inline void WriteArchiveT& put32i(
	IN WriteArchiveT& ar, IN const UINT32 dwarray[], IN UINT count) throw(IoException)
{
	for (UINT i = 0; i < count; ++i)
		put32i(ar, dwarray[i]);
}

template <class WriteArchiveT>
inline void put32i(
	IN WriteArchiveT& ar, IN const INT32 dwarray[], IN UINT count) throw(IoException)
{
	for (UINT i = 0; i < count; ++i)
		put32i(ar, dwarray[i]);
}

template <class WriteArchiveT, class StrucType>
inline void put_struct(
	WriteArchiveT& ar, const StrucType* array, UINT count) throw(IoException)
{
	for (UINT i = 0; i < count; ++i)
		put_struct(ar, array[i]);
}

#else

template <class WriteArchiveT>
inline void put16i(
	WriteArchiveT& ar, IN const UINT16 warray[], IN UINT count) throw(IoException)
{
	ar.put( (const char*)warray, sizeof(UINT16)*count );
}

template <class WriteArchiveT>
inline void put16i(
	IN WriteArchiveT& ar, IN const INT16 warray[], IN UINT count) throw(IoException)
{
	ar.put( (const char*)warray, sizeof(INT16)*count );
}

template <class WriteArchiveT>
inline void put32i(
	IN WriteArchiveT& ar, IN const UINT32 dwarray[], IN UINT count) throw(IoException)
{
	ar.put( (const char*)dwarray, sizeof(UINT32)*count );
}

template <class WriteArchiveT>
inline void put32i(
	IN WriteArchiveT& ar, IN const INT32 dwarray[], IN UINT count) throw(IoException)
{
	ar.put( (const char*)dwarray, sizeof(INT32)*count );
}

template <class WriteArchiveT, class StrucType>
inline void put_struct(
	WriteArchiveT& ar, const StrucType* array, UINT count) throw(IoException)
{
	ar.put( (const char*)array, sizeof(StrucType)*count );
}

#endif // !defined(WINX_BYTESWAP)

// -------------------------------------------------------------------------
// ==== BinaryReader ====

//
// get binary data
//

template <class ReadArchiveT>
inline HRESULT winx_call get16i(ReadArchiveT& ar, UINT16& val)
{
	UINT cbRead =  ar.get( (char*)&val, sizeof(val) );
	if (cbRead != sizeof(val))
		return STG_E_READFAULT;

	_WinxByteSwap16(val);
	return S_OK;
}

template <class ReadArchiveT>
inline HRESULT winx_call get16i(ReadArchiveT& ar, INT16& val)
{
	UINT cbRead =  ar.get( (char*)&val, sizeof(val) );
	if (cbRead != sizeof(val))
		return STG_E_READFAULT;

	_WinxByteSwap16(val);
	return S_OK;
}

template <class ReadArchiveT>
inline HRESULT winx_call get32i(ReadArchiveT& ar, UINT32& val)
{
	UINT cbRead =  ar.get( (char*)&val, sizeof(val) );
	if (cbRead != sizeof(val))
		return STG_E_READFAULT;
	
	_WinxByteSwap32(val);
	return S_OK;
}

template <class ReadArchiveT>
inline HRESULT winx_call get32i(ReadArchiveT& ar, INT32& val)
{
	UINT cbRead =  ar.get( (char*)&val, sizeof(val) );
	if (cbRead != sizeof(val))
		return STG_E_READFAULT;
	
	_WinxByteSwap32(val);
	return S_OK;
}

template <class ReadArchiveT, class StrucType>
inline HRESULT winx_call get_struct(ReadArchiveT& ar, StrucType& val)
{
	UINT cbRead =  ar.get( (char*)&val, sizeof(val) );
	if (cbRead != sizeof(val))
		return STG_E_READFAULT;
	
	_WinxByteSwapStruct(val);
	return S_OK;
}

//
// get binary array
//

#if defined(WINX_BYTESWAP)

template <class ReadArchiveT>
inline HRESULT winx_call get16i(ReadArchiveT& ar, UINT16 warray[], UINT count)
{
	HRESULT hr = S_OK;
	for (UINT i = 0; i < count; ++i)
		hr = get16i(ar, warray[i]);
	return hr;
}

template <class ReadArchiveT>
inline HRESULT winx_call get16i(ReadArchiveT& ar, INT16 warray[], UINT count)
{
	HRESULT hr = S_OK;
	for (UINT i = 0; i < count; ++i)
		hr = get16i(ar, warray[i]);
	return hr;
}

template <class ReadArchiveT>
inline HRESULT winx_call get32i(ReadArchiveT& ar, UINT32 dwarray[], UINT count)
{
	HRESULT hr = S_OK;
	for (UINT i = 0; i < count; ++i)
		hr = get32i(ar, warray[i]);
	return hr;
}

template <class ReadArchiveT>
inline HRESULT winx_call get32i(ReadArchiveT& ar, INT32 dwarray[], UINT count)
{
	HRESULT hr = S_OK;
	for (UINT i = 0; i < count; ++i)
		hr = get32i(ar, warray[i]);
	return hr;
}

template <class ReadArchiveT, class StrucType>
inline HRESULT winx_call get_struct(ReadArchiveT& ar, StrucType array[], UINT count)
{
	HRESULT hr = S_OK;
	for (UINT i = 0; i < count; ++i)
		hr = get_struct(ar, warray[i]);
	return hr;
}

#else

template <class ReadArchiveT>
inline HRESULT winx_call get16i(ReadArchiveT& ar, UINT16 warray[], UINT count)
{
	UINT cbToRead = sizeof(UINT16) * count;
	UINT cbRead =  ar.get( (char*)warray, cbToRead );
	if (cbRead != cbToRead)
		return STG_E_READFAULT;
	return S_OK;
}

template <class ReadArchiveT>
inline HRESULT winx_call get16i(ReadArchiveT& ar, INT16 warray[], UINT count)
{
	UINT cbToRead = sizeof(INT16) * count;
	UINT cbRead =  ar.get( (char*)warray, cbToRead );
	if (cbRead != cbToRead)
		return STG_E_READFAULT;
	return S_OK;
}

template <class ReadArchiveT>
inline HRESULT get32i(ReadArchiveT& ar, UINT32 dwarray[], UINT count)
{
	UINT cbToRead = sizeof(UINT32) * count;
	UINT cbRead =  ar.get( (char*)dwarray, cbToRead );
	if (cbRead != cbToRead)
		return STG_E_READFAULT;
	return S_OK;
}

template <class ReadArchiveT>
inline HRESULT get32i(ReadArchiveT& ar, INT32 dwarray[], UINT count)
{
	UINT cbToRead = sizeof(INT32) * count;
	UINT cbRead =  ar.get( (char*)dwarray, cbToRead );
	if (cbRead != cbToRead)
		return STG_E_READFAULT;
	return S_OK;
}

template <class ReadArchiveT, class StrucType>
inline HRESULT get_struct(ReadArchiveT& ar, StrucType array[], UINT count)
{
	UINT cbToRead = sizeof(StrucType) * count;
	UINT cbRead =  ar.get( (char*)array, cbToRead );
	if (cbRead != cbToRead)
		return STG_E_READFAULT;
	return S_OK;
}

#endif // !defined(WINX_BYTESWAP)

// -------------------------------------------------------------------------
// ==== Binary String Writer ====

//
// paddingIf(Fpad) => if Fpad -> 0:byte end.
//
// (Buf,Len):string0 => Len:byte test(Len < 254) Buf:char[Len];
// (Buf,Len):string0 => 254:byte Len:word Buf:char[Len];
// (Buf,Len):string0 => 255:byte Len:dword Buf:char[Len].
//
// string => (Buf,Len):string0 paddingIf(isEven(Len)).
//
template <class WriterArchiveT>
inline void winx_call puts(
	WriterArchiveT& ar, const char* lpBuf, size_t cch) throw(IoException)
{
	if (cch < 254) {
		ar.put( (UINT8)cch );
	}
	else if (cch < 65536) {
		ar.put( (UINT8)254 );
		put16i( ar, (UINT16)cch );
	}
	else {
		ar.put( (UINT8)255 );
		put32i( ar, (UINT32)cch );
	}
	ar.put( lpBuf, cch );
	if ( !(cch & 1) )
		ar.put( (UINT8)0 );
}

template <class WriterArchiveT>
inline void winx_call puts(
	WriterArchiveT& ar, const TempString<char> s) throw(IoException)
{
	puts(ar, s.data(), s.size());
}

//
// string => Len:word test(Len < 65535) Buf:word[Len];
// string => 65535:word Len:dword Buf:word[Len];
//
template <class WriterArchiveT>
inline void winx_call wputs(
	WriterArchiveT& ar, const UINT16* s, size_t cch) throw(IoException)
{
	if (cch < 65535) {
		put16i( ar, (UINT16)cch );
	}
	else {
		put16i( ar, (UINT16)65535 );
		put32i( ar, (UINT32)cch );
	}
	put16i( ar, s, cch );
}

template <class WriterArchiveT>
inline void winx_call wputs(
	WriterArchiveT& ar, const TempString<UINT16> s) throw(IoException)
{
	wputs(ar, s.data(), s.size());
}

// -------------------------------------------------------------------------
// ==== Binary String Reader ====

//
// get a binary string
//

template <class ReadArchiveT, class StringT>
inline HRESULT winx_call gets(ReadArchiveT& ar, StringT& s)
{
	typedef typename ReadArchiveT::size_type size_type;
	
	HRESULT hr;
	size_type cch = ar.get();
	if (cch < 254) {
		/* nothing todo */
	}
	else if (cch == 254) {
		UINT16 cch2;
		hr = get16i(ar, cch2);
		if (hr != S_OK)
			return hr;
		cch = cch2;
	}
	else if (cch == 255) {
		INT32 cch4;
		hr = get32i(ar, cch4);
		if (hr != S_OK && cch4 < 0)
			return STG_E_READFAULT;
		cch = cch4;
	}
	else {
		return E_UNEXPECTED;
	}
	size_type cbRead = ar.get(std::resize(s, cch), cch);
	if (cbRead != cch)
		return STG_E_READFAULT;
	if ( !(cch & 1) )
		ar.get(); // padding
	return S_OK;
}

template <class ReadArchiveT, class AllocT>
inline HRESULT winx_call gets(ReadArchiveT& ar, AllocT& alloc, BasicString<char>& s)
{
	OutputBasicString<char, AllocT> s1(alloc, s);
	return gets(ar, s1);
}

template <class ReadArchiveT, class StringT>
inline HRESULT winx_call wgets(ReadArchiveT& ar, StringT& s)
{
	typedef typename ReadArchiveT::size_type size_type;

	size_type cch;
	UINT16 cch2;
	HRESULT hr = get16i(ar, cch2);
	if (hr != S_OK) {
		return hr;
	}
	else if (cch2 == 65535) {
		INT32 cch4;
		hr = get32i(ar, cch4);
		if (hr != S_OK && cch4 < 0)
			return STG_E_READFAULT;
		cch = cch4;
	}
	else {
		cch = cch2;
	}
	return get16i(ar, std::resize(s, cch), cch);
}

template <class ReadArchiveT, class AllocT>
inline HRESULT winx_call wgets(ReadArchiveT& ar, AllocT& alloc, BasicString<UINT16>& s)
{
	OutputBasicString<UINT16, AllocT> s1(alloc, s);
	return wgets(ar, s1);
}

// -------------------------------------------------------------------------
// $Log: Binary.h,v $

NS_STDEXT_IO_BINARY_END

#endif /* STDEXT_ARCHIVE_BINARY_H */

