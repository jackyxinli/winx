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
// Module: stdext/text/format/put.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: printf.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_FORMAT_PUT_H
#define STDEXT_TEXT_FORMAT_PUT_H

#ifndef STDEXT_TEXT_APPEND_H
#include "../Append.h"
#endif

#ifndef STDEXT_TCHAR_H
#include "../../tchar.h"
#endif

#ifndef STDEXT_TEXT_FORMAT_PRINTYPE_H
#include "printype.h"
#endif

#ifndef NS_STDEXT_FORMAT
#define NS_STDEXT_FORMAT			NS_STDEXT::ns_format
#define NS_STDEXT_FORMAT_BEGIN		namespace NS_STDEXT { namespace ns_format {
#define NS_STDEXT_FORMAT_END		} }
#endif

NS_STDEXT_FORMAT_BEGIN

// -------------------------------------------------------------------------

struct FormatParams
{
	unsigned flags;
	unsigned specifier;
	int width;
	int precision;
};

template <class StringT, class CharT>
const CharT* winx_call getFormatParams(FormatParams& params, StringT& dest, const CharT* fmt)
{
	//
	// Flags
	//
	params.flags = 0;
	for (;;)
	{
		const unsigned flag = PrinType::is(STD_PRINTYPE_FLAGS, *fmt);
		if (flag)
		{
			params.flags |= flag;
			++fmt;
		}
		else
		{
			break;
		}
	}

	//
	// Width
	//
	params.width = 0;
	while (*fmt >= '0' && *fmt <= '9')
	{
		params.width = params.width * 10 + (*fmt - '0');
		++fmt;
	}

	//
	// .Precision
	//
	if (*fmt == '.')
	{
		++fmt;
		params.precision = 0;
		while (*fmt >= '0' && *fmt <= '9')
		{
			params.precision = params.precision * 10 + (*fmt - '0');
			++fmt;
		}
	}
	else
	{
		params.precision = 1;
	}

	//
	// Length
	//
	if (PrinType::is(STD_PRINTYPE_LENGTH, *fmt))
		++fmt;

	//
	// Specifier
	//
	params.specifier = PrinType::is(STD_PRINTYPE_SPECIFIER, *fmt);
	if (params.specifier)
		++fmt;

	return fmt;
}

// -------------------------------------------------------------------------

template <class StringT, class CharT>
const CharT* winx_call put(StringT& dest, const CharT* fmt, long val)
{
	FormatParams params;
	fmt = getFormatParams(params, dest, fmt);

	if (params.specifier & STD_PRINTYPE_I)
	{
		CharT buf[64];
		const int radix = (
			(params.specifier & (STD_PRINTYPE_I_DEC | STD_PRINTYPE_I_UDEC)) ? 10 :
			(params.specifier & STD_PRINTYPE_I_HEX) ? 16 : 8);
		if (STD_PRINTYPE_I_UDEC)
			tchar::ltoa(val, buf, radix);
		else
			tchar::ultoa(val, buf, radix);
		NS_STDEXT_TEXT::append(dest, buf);
	}
	
	return fmt;
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, unsigned long val)
{
	return put(dest, fmt, (long)val);
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, unsigned int val)
{
	return put(dest, fmt, (long)val);
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, int val)
{
	return put(dest, fmt, (long)val);
}

// -------------------------------------------------------------------------

template <class StringT, class CharT, class ValT>
const CharT* winx_call putString(StringT& dest, const CharT* fmt, const ValT& val)
{
	FormatParams params;
	fmt = getFormatParams(params, dest, fmt);

	NS_STDEXT_TEXT::append(dest, val);

	return fmt;
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, const CharT* val)
{
	return putString(dest, fmt, val);
}

template <class StringT>
inline const char* winx_call put(StringT& dest, const char* fmt, const TempString<char>& val)
{
	return putString(dest, fmt, val);
}

template <class StringT>
inline const wchar_t* winx_call put(StringT& dest, const wchar_t* fmt, const TempString<wchar_t>& val)
{
	return putString(dest, fmt, val);
}

// -------------------------------------------------------------------------

template <class StringT>
const char* winx_call put(StringT& dest, const char* fmt, const double val)
{
	FormatParams params;
	fmt = getFormatParams(params, dest, fmt);

	char buf[64];
	_gcvt(val, 12, buf);
	NS_STDEXT_TEXT::append(dest, buf);
	
	return fmt;
}

template <class StringT>
const wchar_t* winx_call put(StringT& dest, const wchar_t* fmt, const double val)
{
	FormatParams params;
	fmt = getFormatParams(params, dest, fmt);
	
	char buf[64];
	wchar_t wbuf[64];
	_gcvt(val, 12, buf);
	for (size_t i = 0; (wbuf[i] = buf[i]) != '\0'; ++i);
	NS_STDEXT_TEXT::append(dest, wbuf);
	
	return fmt;
}

// -------------------------------------------------------------------------

NS_STDEXT_FORMAT_END

#endif /* STDEXT_TEXT_FORMAT_PUT_H */
