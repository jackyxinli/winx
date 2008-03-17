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
// Module: stdext/String.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:48:49
// 
// $Id: String.h,v 1.3 2007/01/10 09:35:16 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_STRING_H__
#define __STDEXT_STRING_H__

#ifndef __STDEXT_BASIC_H__
#include "Basic.h"
#endif

#ifndef __STDEXT_TEXT_BASICALGO_H__
#include "text/BasicAlgo.h"
#endif

#ifndef __STDEXT_TEXT_BASICSTRING_H__
#include "text/BasicString.h"
#endif

#ifndef __STDEXT_TEXT_BASICSTRINGBUILDER_H__
#include "text/BasicStringBuilder.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class tstring

#if defined(UNICODE)
typedef std::basic_string<WCHAR> tstring;
#else
typedef std::basic_string<char> tstring;
#endif

// -------------------------------------------------------------------------
// trim

template <class _E>
inline void winx_call trim(std::basic_string<_E>& str, const _E* blanks)
{
	typedef std::basic_string<_E> StringT;
	typedef typename StringT::size_type size_type;

	size_type pos1 = str.find_first_not_of(blanks);
	if (pos1 == StringT::npos)
	{
		str.erase();
		return;
	}

	size_type count = str.find_last_not_of(blanks) - pos1 + 1;
	if (str.length() != count)
		str = str.substr(pos1, count);
}

inline void winx_call trim(std::basic_string<char>& str)
{
	const char blanks[] = { ' ', '\t', '\r', '\n', '\0' };
	trim(str, blanks);
}

inline void winx_call trim(std::basic_string<WCHAR>& str)
{
	const WCHAR blanks[] =
	{
		/* 支持中文空格 */
		' ', 12288, '\t', '\r', '\n', '\0'
	};
	trim(str, blanks);
}

// -------------------------------------------------------------------------
// strupper, strlower

template <class StringT>
inline void winx_call strupper(StringT& str)
{
    typename StringT::iterator itEnd = str.end();
    for (typename StringT::iterator it = str.begin(); it != itEnd; ++it)
    {
		*it = ::toupper(*it);
    }
}

template <class StringT>
inline void winx_call strlower(StringT& str)
{
    typename StringT::iterator itEnd = str.end();
    for (typename StringT::iterator it = str.begin(); it != itEnd; ++it)
    {
		*it = ::tolower(*it);
    }
}

// -------------------------------------------------------------------------
// replaceText

template <class StringT>
inline void winx_call replaceText(
	StringT& szText, const StringT& szFindText, const StringT& szReplaceText)
{
	typedef typename StringT::size_type size_type;
	size_type pos = 0;
	while (
		(pos = szText.find(szFindText, pos)) != StringT::npos
		)
	{
		szText.replace(pos, szFindText.size(), szReplaceText);
		pos += szReplaceText.size();
	}
}

// -------------------------------------------------------------------------
// class TestString

#if defined(X_CC_VC)

template <class LogT>
class TestString : public TestCase
{
public:
	WINX_TEST_SUITE(TestString);
		WINX_TEST(testTrim);
		WINX_TEST(testUpperLower);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void testTrim(LogT& log)
	{
		std::string s = " \t  abc 123  \r\n";
		std::trim(s);
		AssertExp(s == "abc 123");
	}

	void testUpperLower(LogT& log)
	{
		std::string s = "abc123";
		std::strupper(s);
		AssertExp(s == "ABC123");
		std::strlower(s);
		AssertExp(s == "abc123");
	}
};

#endif

// -------------------------------------------------------------------------
// $Log: String.h,v $

__NS_STD_END

#endif /* __STDEXT_STRING_H__ */
