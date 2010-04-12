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

#define STD_PRINTYPE_FLAGS_NEGATIVE STD_PRINTYPE_RESERVED

template <class StringT, class CharT>
void winx_call format(StringT& dest, const FormatParams& params, const CharT* text, const CharT* textEnd)
{
    CharT prefix[2];
	/* numeric prefix -- up to two characters */
    
	int prefixlen = 0;
	/* length of prefix -- 0 means no prefix */

    int padding;
	/* amount of padding, negative means zero */

	const unsigned flags = params.flags;

    if (params.specifier & (STD_PRINTYPE_F | STD_PRINTYPE_I_DEC))
	{
        if (flags & STD_PRINTYPE_FLAGS_NEGATIVE)
		{
            /* prefix is a '-' */
            prefix[0] = '-';
            prefixlen = 1;
        }
        else if (flags & STD_PRINTYPE_FLAGS_ADD)
		{
            /* prefix is '+' */
            prefix[0] = '+';
            prefixlen = 1;
        }
        else if (flags & STD_PRINTYPE_FLAGS_SPACE)
		{
            /* prefix is ' ' */
            prefix[0] = ' ';
            prefixlen = 1;
        }
    }
	else if (params.specifier & STD_PRINTYPE_I_HEX)
	{
		if (flags & STD_PRINTYPE_FLAGS_SHARP)
		{
			/* alternate form means '0x' prefix */
			prefix[0] = '0';
			prefix[1] = (params.specifier & STD_PRINTYPE_I_HEX_UPPER) ? 'X' : 'x'; /* 'x' or 'X' */
			prefixlen = 2;
		}
	}

    /*
	 *  calculate amount of padding
	 *  -- might be negative
     *  but this will just mean zero
	 */
    padding = params.width - (textEnd - text) - prefixlen;

    /* put out the padding, prefix, and text */

    if (!(flags & (STD_PRINTYPE_FLAGS_SUB | STD_PRINTYPE_FLAGS_ZERO))) 
	{
        /* pad on left with blanks */
		if (padding > 0)
			NS_STDEXT_TEXT::append(dest, padding, (CharT)' ');
	}

    /* write prefix */
	if (prefixlen > 0)
		NS_STDEXT_TEXT::append(dest, prefix, prefix + prefixlen);

    if ((flags & STD_PRINTYPE_FLAGS_ZERO) && !(flags & STD_PRINTYPE_FLAGS_SUB))
	{
        /* write leading zeros */
		if (padding > 0)
			NS_STDEXT_TEXT::append(dest, padding, (CharT)'0');
    }

    /* write text */
	NS_STDEXT_TEXT::append(dest, text, textEnd);

    if (flags & STD_PRINTYPE_FLAGS_SUB)
	{
        /* pad on right with blanks */
		if (padding > 0)
			NS_STDEXT_TEXT::append(dest, padding, (CharT)' ');
    }
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
		CharT* text = buf + countof(buf);
		
		int hexadd, radix;
	
		if (params.specifier & (STD_PRINTYPE_I_DEC | STD_PRINTYPE_I_UDEC))
		{
			radix = 10;
		}
		else if (params.specifier & STD_PRINTYPE_I_HEX)
		{
			radix = 16;
			hexadd = (params.specifier & STD_PRINTYPE_I_HEX_UPPER) ? ('A' - '9' - 1) : ('a' - '9' - 1);
		}
		else
		{
			radix = 8;
		}
		
		if (params.specifier & STD_PRINTYPE_I_DEC)
		{
			if (val < 0)
			{
				params.flags |= STD_PRINTYPE_FLAGS_NEGATIVE;
				val = -val;
			}
		}
		
		if (params.precision < 0)
		{
			params.precision = 1;
		}
		else
		{
			params.flags &= ~STD_PRINTYPE_FLAGS_ZERO;
		}

		while (params.precision-- > 0 || val != 0)
		{
			int digit = (int)(val % radix) + '0';
			val /= radix;
			if (digit > '9')
			{
				/* a hex digit, make it a letter */
				digit += hexadd;
			}
			*--text = (CharT)digit;
		}

		if ((params.flags & STD_PRINTYPE_FLAGS_SHARP) && radix == 8)
		{
			*--text = '0';
		}

		format(dest, params, text, buf + countof(buf));
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
