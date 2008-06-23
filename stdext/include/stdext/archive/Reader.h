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
// Module: stdext/archive/Reader.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: Reader.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_READER_H
#define STDEXT_ARCHIVE_READER_H

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "../text/BasicString.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class Reader

template <class BaseArchive>
class Reader : public BaseArchive
{
private:
	typedef BaseArchive _Base, BaseClass;
	typedef Reader _Myt;

public:
	typedef typename _Base::char_type char_type;
	typedef typename _Base::uchar_type uchar_type;
	typedef typename _Base::int_type int_type;

	typedef typename _Base::size_type size_type;

public:
	Reader() {}

	template <class InitArgT>
	explicit Reader(InitArgT file)
		: _Base(file) {}

	template <class InitArgT1, class InitArgT2>
	Reader(InitArgT1 arg1, InitArgT2 arg2)
		: _Base(arg1, arg2) {}

	template <class InitArgT1, class InitArgT2, class InitArgT3>
	Reader(InitArgT1 arg1, InitArgT2 arg2, InitArgT3 arg3)
		: _Base(arg1, arg2, arg3) {}

//
// ======== access binary data ========
//
public:
	// get a binary string

	template <class AllocT2>
	HRESULT winx_call gets(AllocT2& alloc, BasicString<char>& s)
	{
		OutputBasicString<char, AllocT2> s1(alloc, s);
		return gets(s1);
	}

	template <class StringT>
	HRESULT winx_call gets(StringT& s)
	{
		HRESULT hr;
		size_type cch = _Base::get();
		if (cch < 254) {
			/* nothing todo */
		}
		else if (cch == 254) {
			UINT16 cch2;
			hr = get16i(cch2);
			if (hr != S_OK)
				return hr;
			cch = cch2;
		}
		else if (cch == 255) {
			INT32 cch4;
			hr = get32i(cch4);
			if (hr != S_OK && cch4 < 0)
				return STG_E_READFAULT;
			cch = cch4;
		}
		else {
			return E_UNEXPECTED;
		}
		size_type cbRead = _Base::get(std::resize(s, cch), cch);
		if (cbRead != cch)
			return STG_E_READFAULT;
		if ( !(cch & 1) )
			_Base::get(); // padding
		return S_OK;
	}

	template <class AllocT2>
	HRESULT winx_call wgets(AllocT2& alloc, BasicString<WCHAR>& s)
	{
		OutputBasicString<WCHAR, AllocT2> s1(alloc, s);
		return wgets(s1);
	}

	template <class StringT>
	HRESULT winx_call wgets(StringT& s)
	{
		size_type cch;
		UINT16 cch2;
		HRESULT hr = get16i(cch2);
		if (hr != S_OK) {
			return hr;
		}
		else if (cch2 == 65535) {
			INT32 cch4;
			hr = get32i(cch4);
			if (hr != S_OK && cch4 < 0)
				return STG_E_READFAULT;
			cch = cch4;
		}
		else {
			cch = cch2;
		}
		return get16i(std::resize(s, cch), cch);
	}

public:
	// get binary data
	
	size_type winx_call read(void* lpBuf, size_type nMax)
	{
		return _Base::get( (char*)lpBuf, nMax );
	}

	HRESULT winx_call get16i(UINT16& val)
	{
		UINT cbRead =  _Base::get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;

		_WinxByteSwap16(val);
		return S_OK;
	}
	
	HRESULT winx_call get16i(INT16& val)
	{
		UINT cbRead =  _Base::get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;
		
		_WinxByteSwap16(val);
		return S_OK;
	}

	HRESULT winx_call get32i(UINT32& val)
	{
		UINT cbRead =  _Base::get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;
		
		_WinxByteSwap32(val);
		return S_OK;
	}

	HRESULT winx_call get32i(INT32& val)
	{
		UINT cbRead =  _Base::get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;
		
		_WinxByteSwap32(val);
		return S_OK;
	}
	
	template <class StrucType>
	HRESULT winx_call get_struct(StrucType& val)
	{
		UINT cbRead =  _Base::get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;
		
		_WinxByteSwapStruct(val);
		return S_OK;
	}

#if defined(WINX_BYTESWAP)
	HRESULT winx_call get16i(IN UINT16 warray[], IN UINT count)
	{
		HRESULT hr = S_OK;
		for (UINT i = 0; i < count; ++i)
			hr = get16u(warray[i]);
		return hr;
	}
	HRESULT winx_call get16i(IN INT16 warray[], IN UINT count)
	{
		HRESULT hr = S_OK;
		for (UINT i = 0; i < count; ++i)
			hr = get16i(warray[i]);
		return hr;
	}
	HRESULT winx_call get32i(IN UINT32 dwarray[], IN UINT count)
	{
		HRESULT hr = S_OK;
		for (UINT i = 0; i < count; ++i)
			hr = get32u(warray[i]);
		return hr;
	}
	HRESULT winx_call get32i(IN INT32 dwarray[], IN UINT count)
	{
		HRESULT hr = S_OK;
		for (UINT i = 0; i < count; ++i)
			hr = get32i(warray[i]);
		return hr;
	}
	template <class StrucType>
	HRESULT winx_call get_struct(StrucType* array, UINT count)
	{
		HRESULT hr = S_OK;
		for (UINT i = 0; i < count; ++i)
			hr = get_struct(warray[i]);
		return hr;
	}
#else
	HRESULT winx_call get16i(IN UINT16 warray[], IN UINT count)
	{
		UINT cbToRead = sizeof(UINT16) * count;
		UINT cbRead =  _Base::get( (char*)warray, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
	HRESULT winx_call get16i(IN INT16 warray[], IN UINT count)
	{
		UINT cbToRead = sizeof(INT16) * count;
		UINT cbRead =  _Base::get( (char*)warray, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
	HRESULT get32i(IN UINT32 dwarray[], IN UINT count)
	{
		UINT cbToRead = sizeof(UINT32) * count;
		UINT cbRead =  _Base::get( (char*)dwarray, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
	HRESULT get32i(IN INT32 dwarray[], IN UINT count)
	{
		UINT cbToRead = sizeof(INT32) * count;
		UINT cbRead =  _Base::get( (char*)dwarray, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
	template <class StrucType>
	HRESULT get_struct(StrucType* array, UINT count)
	{
		UINT cbToRead = sizeof(StrucType) * count;
		UINT cbRead =  _Base::get( (char*)array, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
#endif // !defined(WINX_BYTESWAP)

//
// ======== access text data ========
//
public:
	template <class ConditionT>
	int_type winx_call get_first_not_of(ConditionT cond)
		{return std::get_first_not_of(WINX_BASE, cond); }

	int_type winx_call getnws()
		{return std::get_nws(WINX_BASE); }

	size_type winx_call skip_eol()
		{return std::skip_eol(WINX_BASE); }

	template <class ConditionT>
	size_type winx_call skip_while(ConditionT cond)
		{return std::skip_while(WINX_BASE, cond); }

	void winx_call skipws()
		{std::skip_ws(WINX_BASE); }

	unsigned winx_call get_uint(unsigned preval = 0, unsigned radix = 10)
		{return std::get_uint(WINX_BASE, preval, radix); }

	HRESULT winx_call scan_uint(unsigned& val, unsigned radix = 10)
		{return std::scan_uint(WINX_BASE, val, radix); }

	template <class StringT>
	size_type winx_call get_csymbol(StringT& s)
		{return std::get_csymbol(WINX_BASE, s); }

	template <class AllocT2>
	size_type winx_call get_csymbol(AllocT2& alloc, BasicString<char_type>& s)
	{
		OutputBasicString<char_type, AllocT2> s1(alloc, s);
		return std::get_csymbol(WINX_BASE, s1);
	}

	template <class StringT>
	HRESULT winx_call scan_csymbol(StringT& s)
		{return std::scan_csymbol(WINX_BASE, s); }

	template <class AllocT2>
	HRESULT winx_call scan_csymbol(AllocT2& alloc, BasicString<char_type>& s)
	{
		OutputBasicString<char_type, AllocT2> s1(alloc, s);
		return std::scan_csymbol(WINX_BASE, s1);
	}

	template <class StringT, class ConditionT>
	size_type winx_call get_while(StringT& s, ConditionT cond)
		{return std::get_while(WINX_BASE, s, cond); }

	template <class AllocT2, class ConditionT>
	size_type winx_call get_while(
		AllocT2& alloc, BasicString<char_type>& s, ConditionT cond)
	{
		OutputBasicString<char_type, AllocT2> s1(alloc, s);
		return std::get_while(WINX_BASE, s1, cond);
	}

	template <class StringT>
	size_type winx_call getline(StringT& s)
		{return std::get_line(WINX_BASE, s); }

	template <class AllocT2>
	size_type winx_call getline(AllocT2& alloc, BasicString<char_type>& s)
	{
		OutputBasicString<char_type, AllocT2> s1(alloc, s);
		return std::get_line(WINX_BASE, s1);
	}
};

// -------------------------------------------------------------------------
// $Log: Reader.h,v $

NS_STDEXT_END

#endif /* STDEXT_ARCHIVE_READER_H */
