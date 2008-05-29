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
// Module: stdext/archive/Writer.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: Writer.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_WRITER_H
#define STDEXT_ARCHIVE_WRITER_H

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_TEXT_TEMPSTRING_H
#include "../text/TempString.h"
#endif

NS_STD_BEGIN

// -------------------------------------------------------------------------
// class Writer

template <class BaseArchive>
class Writer : public BaseArchive
{
private:
	typedef BaseArchive _Base;
	typedef Writer _Myt;

public:
	typedef typename _Base::char_type char_type;
	typedef typename _Base::uchar_type uchar_type;

	typedef typename _Base::size_type size_type;

private:
	typedef TempString<char> _String;
	typedef TempString<WCHAR> _StringW;
	
public:
	Writer() {}

	template <class InitArgT>
	explicit Writer(InitArgT file)
		: _Base(file) {}

	template <class InitArgT1, class InitArgT2>
	Writer(InitArgT1 arg1, InitArgT2 arg2)
		: _Base(arg1, arg2) {}

	template <class InitArgT1, class InitArgT2, class InitArgT3>
	Writer(InitArgT1 arg1, InitArgT2 arg2, InitArgT3 arg3)
		: _Base(arg1, arg2, arg3) {}

public:
	__forceinline size_type winx_call put(
		const char_type* lpBuf, size_type cch) throw(IoException)
	{
		return _Base::put(lpBuf, cch);
	}

	__forceinline size_type winx_call put(_String s) throw(IoException)
	{
		return _Base::put(s.data(), s.size());
	}

	__forceinline size_type winx_call put(char_type ch) throw(IoException)
	{
		return _Base::put(ch);
	}

	__forceinline size_type winx_call put(uchar_type ch) throw(IoException)
	{
		return _Base::put(ch);
	}

public:
	//
	// paddingIf(Fpad) => if Fpad -> 0:byte end.
	//
	// (Buf,Len):string0 => Len:byte test(Len < 254) Buf:char[Len];
	// (Buf,Len):string0 => 254:byte Len:word Buf:char[Len];
	// (Buf,Len):string0 => 255:byte Len:dword Buf:char[Len].
	//
	// string => (Buf,Len):string0 paddingIf(isEven(Len)).
	//
	void winx_call puts(
		const char* lpBuf, size_type cch) throw(IoException)
	{
		if (cch < 254) {
			_Base::put( (UINT8)cch );
		}
		else if (cch < 65536) {
			_Base::put( (UINT8)254 );
			put16i( (UINT16)cch );
		}
		else {
			_Base::put( (UINT8)255 );
			put32i( (UINT32)cch );
		}
		put( lpBuf, cch );
		if ( !(cch & 1) )
			_Base::put( (UINT8)0 );
	}

	void winx_call puts(_String s) throw(IoException)
	{
		puts(s.data(), s.size());
	}

	//
	// string => Len:word test(Len < 65535) Buf:word[Len];
	// string => 65535:word Len:dword Buf:word[Len];
	//
	void winx_call wputs(const WCHAR* s, size_type cch) throw(IoException)
	{
		if (cch < 65535) {
			put16i( (UINT16)cch );
		}
		else {
			put16i( (UINT16)65535 );
			put32i( (UINT32)cch );
		}
		put16i( s, cch );
	}

	void winx_call wputs(_StringW s) throw(IoException)
	{
		wputs(s.data(), s.size());
	}

public:
	__forceinline size_type winx_call write(
		const void* lpBuf, size_type nMax) throw(IoException)
	{
		return _Base::put( (const char*)lpBuf, nMax );
	}

public:
	_Myt& winx_call put16i(IN UINT16 val) throw(IoException)
	{
		_WinxByteSwap16(val);
		_Base::put( (const char*)&val, sizeof(val) );
		return *this;
	}
	_Myt& winx_call put32i(IN UINT32 val) throw(IoException)
	{
		_WinxByteSwap32(val);
		_Base::put( (const char*)&val, sizeof(val) );
		return *this;
	}
	
	template <class StrucType>
	_Myt& winx_call put_struct(IN const StrucType& struc) throw(IoException)
	{
		_WinxByteSwapStruct(struc);
		_Base::put( (const char*)&struc, sizeof(struc) );
		return *this;
	}

#if defined(WINX_BYTESWAP)
	_Myt& put16i(IN const UINT16 warray[], IN UINT count) throw(IoException)
	{
		for (UINT i = 0; i < count; ++i)
			put16i(warray[i]);
		return *this;
	}
	_Myt& put16i(IN const INT16 warray[], IN UINT count) throw(IoException)
	{
		for (UINT i = 0; i < count; ++i)
			put16i(warray[i]);
		return *this;
	}
	_Myt& put32i(IN const UINT32 dwarray[], IN UINT count) throw(IoException)
	{
		for (UINT i = 0; i < count; ++i)
			put32i(dwarray[i]);
		return *this;
	}
	_Myt& put32i(IN const INT32 dwarray[], IN UINT count) throw(IoException)
	{
		for (UINT i = 0; i < count; ++i)
			put32i(dwarray[i]);
		return *this;
	}
	template <class StrucType>
	_Myt& put_struct(const StrucType* array, UINT count) throw(IoException)
	{
		for (UINT i = 0; i < count; ++i)
			put_struct(array[i]);
		return *this;
	}
#else
	_Myt& put16i(IN const UINT16 warray[], IN UINT count) throw(IoException)
	{
		_Base::put( (const char*)warray, sizeof(UINT16)*count );
		return *this;
	}
	_Myt& put16i(IN const INT16 warray[], IN UINT count) throw(IoException)
	{
		_Base::put( (const char*)warray, sizeof(INT16)*count );
		return *this;
	}
	_Myt& put32i(IN const UINT32 dwarray[], IN UINT count) throw(IoException)
	{
		_Base::put( (const char*)dwarray, sizeof(UINT32)*count );
		return *this;
	}
	_Myt& put32i(IN const INT32 dwarray[], IN UINT count) throw(IoException)
	{
		_Base::put( (const char*)dwarray, sizeof(INT32)*count );
		return *this;
	}
	template <class StrucType>
	_Myt& put_struct(const StrucType* array, UINT count) throw(IoException)
	{
		_Base::put( (const char*)array, sizeof(StrucType)*count );
		return *this;
	}
#endif // !defined(WINX_BYTESWAP)
};

// -------------------------------------------------------------------------
// $Log: Writer.h,v $

NS_STD_END

#endif /* STDEXT_ARCHIVE_WRITER_H */
