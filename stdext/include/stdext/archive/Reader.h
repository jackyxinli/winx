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
#ifndef __STDEXT_ARCHIVE_READER_H__
#define __STDEXT_ARCHIVE_READER_H__

#ifndef __STDEXT_CHARTYPE_H__
#include "../CharType.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class Reader

template <class BaseArchive>
class Reader : public BaseArchive
{
private:
	typedef BaseArchive _Base;
	typedef Reader _Myt;

public:
	typedef typename _Base::size_type size_type;
	typedef typename _Base::char_type char_type;
	typedef typename _Base::int_type int_type;

public:
	template <class AllocT>
	explicit Reader(AllocT& alloc)
		: _Base(alloc) {}

	template <class AllocT, class InitArgT>
	Reader(AllocT& alloc, const InitArgT& file)
		: _Base(alloc, file) {}

	template <class AllocT, class InitArgT1, class InitArgT2>
	Reader(AllocT& alloc, const InitArgT1& arg1, const InitArgT2& arg2)
		: _Base(alloc, arg1, arg2) {}

public:
	size_type winx_call read(void* lpBuf, size_type nMax)
	{
		return get( (char*)lpBuf, nMax );
	}

	HRESULT winx_call get16i(UINT16& val)
	{
		UINT cbRead = get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;

		_WinxByteSwap16(val);
		return S_OK;
	}
	
	HRESULT winx_call get16i(INT16& val)
	{
		UINT cbRead = get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;
		
		_WinxByteSwap16(val);
		return S_OK;
	}

	HRESULT winx_call get32i(UINT32& val)
	{
		UINT cbRead = get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;
		
		_WinxByteSwap32(val);
		return S_OK;
	}

	HRESULT winx_call get32i(INT32& val)
	{
		UINT cbRead = get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;
		
		_WinxByteSwap32(val);
		return S_OK;
	}
	
	template <class StrucType>
	HRESULT winx_call get_struct(StrucType& val)
	{
		UINT cbRead = get( (char*)&val, sizeof(val) );
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
		UINT cbRead = get( (char*)warray, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
	HRESULT winx_call get16i(IN INT16 warray[], IN UINT count)
	{
		UINT cbToRead = sizeof(INT16) * count;
		UINT cbRead = get( (char*)warray, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
	HRESULT get32i(IN UINT32 dwarray[], IN UINT count)
	{
		UINT cbToRead = sizeof(UINT32) * count;
		UINT cbRead = get( (char*)dwarray, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
	HRESULT get32i(IN INT32 dwarray[], IN UINT count)
	{
		UINT cbToRead = sizeof(INT32) * count;
		UINT cbRead = get( (char*)dwarray, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
	template <class StrucType>
	HRESULT get_struct(StrucType* array, UINT count)
	{
		UINT cbToRead = sizeof(StrucType) * count;
		UINT cbRead = get( (char*)array, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
#endif // !defined(WINX_BYTESWAP)

public:
	HRESULT winx_call scan_uint(unsigned& val, unsigned radix = 10)
	{
		int_type ch = getnws();
		unsigned dig = DigitTable::toDigit(ch);
		if (dig < radix)
		{
			val = get_uint(radix, dig);
			return S_OK;
		}
		else
		{
			unget(ch);
			return STG_E_READFAULT;
		}
	}

	unsigned winx_call get_uint(unsigned radix = 10, unsigned preval = 0)
	{
		unsigned dig;
		int_type ch;
		while ( (dig = DigitTable::toDigit(ch = get())) < radix )
			preval = preval * radix + dig;
		unget(ch);
		return preval;
	}

	template <class _Cond>
	int_type winx_call get_not_if(const _Cond& cond)
	{
		int_type ch;
		while (	cond(ch = get()) );
		return ch;
	}

	int_type winx_call getnws()
	{
		int_type ch;
		while (	::isspace(ch = get()) );
		return ch;
	}

	template <class _Cond>
	size_type winx_call skip_if(const _Cond& cond)
	{
		WINX_ASSERT( !cond(endch) );

		size_type count = 0;
		int_type ch;
		while ( cond(ch = get()) )
			++count;
		unget(ch);
		return count;
	}

	void winx_call skipws()
	{
		int_type ch;
		while (	::isspace(ch = get()) );
		unget(ch);
	}

	void winx_call skip_eol()
	{
		int_type ch = get();
		if (ch == 0x0a)
		{
			ch = get();
			if (ch != 0x0d)
				unget(ch);
		}
		else if (ch == 0x0d)
		{
			ch = get();
			if (ch != 0x0a)
				unget(ch);
		}
	}

	template <class StringT>
	void winx_call getline(StringT& str)
	{
		size_type count = skip_if(CharType::NotIsEOL());
		str.resize(count);
		reget(count, &str[0], count);
		skip_eol();
	}
};

// -------------------------------------------------------------------------
// $Log: Reader.h,v $

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_READER_H__ */
