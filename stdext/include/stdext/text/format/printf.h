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
// Module: stdext/text/formatAppend/printf.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: printf.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_FORMAT_PRINTF_H
#define STDEXT_TEXT_FORMAT_PRINTF_H

#ifndef STDEXT_BASIC_H
#include "../../Basic.h"
#endif

#ifndef STDEXT_CHARTYPE_H
#include "../../CharType.h"
#endif

#ifndef STDEXT_TCHAR_H
#include "../../tchar.h"
#endif

#ifndef STDEXT_TEXT_FORMAT_STR_H
#include "str.h"
#endif

#ifndef STDEXT_TEXT_FORMAT_PRINTYPE_H
#include "printype.h"
#endif

// -------------------------------------------------------------------------
// NS_STDEXT_TEXT

#ifndef NS_STDEXT_TEXT
#define NS_STDEXT_TEXT				NS_STDEXT::ns_text
#define NS_STDEXT_TEXT_BEGIN		namespace NS_STDEXT { namespace ns_text {
#define NS_STDEXT_TEXT_END			} }
#endif

NS_STDEXT_TEXT_BEGIN

template <class CharT, class AllocT>
void winx_call clear(std::vector<CharT, AllocT>& dest)
{
	dest.clear();
}

template <class CharT, class Tr, class AllocT>
void winx_call clear(std::basic_string<CharT, Tr, AllocT>& dest)
{
	dest.erase();
}

template <class CharT, class AllocT>
void winx_call append(std::vector<CharT, AllocT>& dest, const CharT* val)
{
	const CharT* valEnd = NS_STDEXT::end(val);
	dest.insert(dest.end(), val, valEnd);
}

template <class CharT, class AllocT>
void winx_call append(std::vector<CharT, AllocT>& dest, const CharT* val, const CharT* valEnd)
{
	dest.insert(dest.end(), val, valEnd);
}

template <class CharT, class AllocT>
void winx_call append(std::vector<CharT, AllocT>& dest, const size_t count, const CharT val)
{
	dest.insert(dest.end(), count, val);
}

template <class CharT, class AllocT>
void winx_call append(std::vector<CharT, AllocT>& dest, const CharT val)
{
	dest.push_back(val);
}

template <class CharT, class Tr, class AllocT>
void winx_call append(std::basic_string<CharT, Tr, AllocT>& dest, const CharT* val)
{
	dest.append(val);
}

template <class CharT, class Tr, class AllocT>
void winx_call append(std::basic_string<CharT, Tr, AllocT>& dest, const CharT* val, const CharT* valEnd)
{
	dest.append(val, valEnd);
}

template <class CharT, class Tr, class AllocT>
void winx_call append(std::basic_string<CharT, Tr, AllocT>& dest, const size_t count, const CharT val)
{
	dest.append(count, val);
}

template <class CharT, class Tr, class AllocT>
void winx_call append(std::basic_string<CharT, Tr, AllocT>& dest, const CharT val)
{
	dest.append(1, val);
}

NS_STDEXT_TEXT_END

// -------------------------------------------------------------------------
// class FormatBox - http://www.cplusplus.com/reference/clibrary/cstdio/printf/

NS_STDEXT_BEGIN

template <class StringT>
class FormatBox
{
protected:
	typedef typename StringT::value_type CharT;

	StringT& m_dest;
	const CharT* m_fmt;

protected:
	unsigned m_flags;
	unsigned m_specifier;
	int m_width;
	int m_precision;

	void winx_call prepare()
	{
		const CharT* p;

Retry:	p = tchar::strchr(m_fmt, (CharT)'%');
		if (p == NULL)
		{
			static const CharT s_null = CharT();
			NS_STDEXT_TEXT::append(m_dest, m_fmt);
			m_fmt = &s_null;
			m_width = m_precision = 0;
			m_specifier = 0;
			return;
		}

		NS_STDEXT_TEXT::append(m_dest, m_fmt, p);
		++p;
			
		//
		// Flags
		//
		m_flags = 0;
		for (;;)
		{
			const unsigned flag = PrinType::is(STD_PRINTYPE_FLAGS, *p);
			if (flag)
				m_flags |= flag;
			else
				break;
		}

		//
		// Width
		//
		if (*p == '*')
		{
			m_width = -1;
			++p;
		}
		else
		{
			m_width = 0;
			while (*p >= '0' && *p <= '9') {
				m_width = m_width * 10 + (*p - '0');
				++p;
			}
		}

		//
		// .Precision
		//
		if (*p == '.')
		{
			++p;
			if (*p == '*')
			{
				m_precision = -1;
				++p;
			}
			else
			{
				m_precision = 0;
				while (*p >= '0' && *p <= '9') {
					m_precision = m_precision * 10 + (*p - '0');
					++p;
				}
			}
		}
		else
		{
			m_precision = 1;
		}

		//
		// Length
		//
		if (PrinType::is(STD_PRINTYPE_LENGTH, *p))
			++p;

		//
		// Specifier
		//
		m_specifier = PrinType::is(STD_PRINTYPE_SPECIFIER, *p);
		m_fmt = p;
		if (m_specifier)
		{
			++m_fmt;
		}
		else if (*p == '%')
		{
			++m_fmt;
			NS_STDEXT_TEXT::append(m_dest, (CharT)'%');
			goto Retry;
		}
	}

public:
	FormatBox(StringT& dest, const CharT* fmt)
		: m_dest(dest), m_fmt(fmt)
	{
		prepare();
	}

	void winx_call add(const CharT* val)
	{
		NS_STDEXT_TEXT::append(m_dest, val);
		prepare();
	}

	void winx_call add(const TempString<CharT>& val)
	{
		NS_STDEXT_TEXT::append(m_dest, val.begin(), val.end());
		prepare();
	}

	void winx_call add(long val)
	{
		if (m_width < 0)
		{
			m_width = val;
		}
		else if (m_precision < 0)
		{
			m_precision = val;
		}
		else if (m_specifier & STD_PRINTYPE_I)
		{
			CharT buf[64];
			const int radix = (
				(m_specifier & (STD_PRINTYPE_I_DEC | STD_PRINTYPE_I_UDEC)) ? 10 :
				(m_specifier & STD_PRINTYPE_I_HEX) ? 16 : 8);
			if (STD_PRINTYPE_I_UDEC)
				tchar::ltoa(val, buf, radix);
			else
				tchar::ultoa(val, buf, radix);
			NS_STDEXT_TEXT::append(m_dest, buf);
			prepare();
		}
	}

	void winx_call add(unsigned long val)
	{
		add((long)val);
	}

	void winx_call add(unsigned int val)
	{
		add((long)val);
	}

	void winx_call add(int val)
	{
		add((long)val);
	}
};

template <class StringT, class CharT, class ArgT1>
void winx_call formatAppend(StringT& dest, const CharT* fmt, const ArgT1& arg1)
{
	FormatBox<StringT> box(dest, fmt);
	box.add(arg1);
}

template <class StringT, class CharT, class ArgT1, class ArgT2>
void winx_call formatAppend(StringT& dest, const CharT* fmt, const ArgT1& arg1, const ArgT2& arg2)
{
	FormatBox<StringT> box(dest, fmt);
	box.add(arg1);
	box.add(arg2);
}

template <class StringT, class CharT, class ArgT1, class ArgT2, class ArgT3>
void winx_call formatAppend(StringT& dest, const CharT* fmt, const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3)
{
	FormatBox<StringT> box(dest, fmt);
	box.add(arg1);
	box.add(arg2);
	box.add(arg3);
}

template <class StringT, class CharT, class ArgT1, class ArgT2, class ArgT3, class ArgT4>
void winx_call formatAppend(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3, const ArgT4& arg4)
{
	FormatBox<StringT> box(dest, fmt);
	box.add(arg1);
	box.add(arg2);
	box.add(arg3);
	box.add(arg4);
}

template <class StringT, class CharT, class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5>
void winx_call formatAppend(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2,
	const ArgT3& arg3, const ArgT4& arg4, const ArgT5& arg5)
{
	FormatBox<StringT> box(dest, fmt);
	box.add(arg1);
	box.add(arg2);
	box.add(arg3);
	box.add(arg4);
	box.add(arg5);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6>
void winx_call formatAppend(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2,
	const ArgT3& arg3, const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6)
{
	FormatBox<StringT> box(dest, fmt);
	box.add(arg1);
	box.add(arg2);
	box.add(arg3);
	box.add(arg4);
	box.add(arg5);
	box.add(arg6);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7>
void winx_call formatAppend(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3,
	const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7)
{
	FormatBox<StringT> box(dest, fmt);
	box.add(arg1);
	box.add(arg2);
	box.add(arg3);
	box.add(arg4);
	box.add(arg5);
	box.add(arg6);
	box.add(arg7);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7, class ArgT8>
void winx_call formatAppend(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3,
	const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7, const ArgT8& arg8)
{
	FormatBox<StringT> box(dest, fmt);
	box.add(arg1);
	box.add(arg2);
	box.add(arg3);
	box.add(arg4);
	box.add(arg5);
	box.add(arg6);
	box.add(arg7);
	box.add(arg8);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7, class ArgT8, class ArgT9>
void winx_call formatAppend(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3, const ArgT4& arg4,
	const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7, const ArgT8& arg8, const ArgT9& arg9)
{
	FormatBox<StringT> box(dest, fmt);
	box.add(arg1);
	box.add(arg2);
	box.add(arg3);
	box.add(arg4);
	box.add(arg5);
	box.add(arg6);
	box.add(arg7);
	box.add(arg8);
	box.add(arg9);
}

template <class StringT, class CharT, class ArgT1>
void winx_call format(StringT& dest, const CharT* fmt, const ArgT1& arg1)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1);
}

template <class StringT, class CharT, class ArgT1, class ArgT2>
void winx_call format(StringT& dest, const CharT* fmt, const ArgT1& arg1, const ArgT2& arg2)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2);
}

template <class StringT, class CharT, class ArgT1, class ArgT2, class ArgT3>
void winx_call format(StringT& dest, const CharT* fmt, const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3);
}

template <class StringT, class CharT, class ArgT1, class ArgT2, class ArgT3, class ArgT4>
void winx_call format(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3, const ArgT4& arg4)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3, arg4);
}

template <class StringT, class CharT, class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5>
void winx_call format(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2,
	const ArgT3& arg3, const ArgT4& arg4, const ArgT5& arg5)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6>
void winx_call format(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2,
	const ArgT3& arg3, const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5, arg6);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7>
void winx_call format(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3,
	const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7, class ArgT8>
void winx_call format(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3,
	const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7, const ArgT8& arg8)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7, class ArgT8, class ArgT9>
void winx_call format(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3, const ArgT4& arg4,
	const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7, const ArgT8& arg8, const ArgT9& arg9)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

NS_STDEXT_END

// -------------------------------------------------------------------------

#endif /* STDEXT_TEXT_FORMAT_PRINTF_H */
