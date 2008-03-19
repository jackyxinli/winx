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
// Module: stdext/text/StringAlgo.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: StringAlgo.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_TEXT_STRINGALGO_H__
#define __STDEXT_TEXT_STRINGALGO_H__

#ifndef __STDEXT_TEXT_BASICSTRING_H__
#include "BasicString.h"
#endif

#ifndef __STDEXT_MSVCRT_VARGS_H__
#include "../msvcrt/vargs.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class TempString

template <class _E>
class TempString : public BasicString<_E>
{
private:
	typedef BasicString<_E> _Base;

public:
	TempString(const _E* szVal)
		: _Base(szVal, std::length(szVal))
	{
	}

	TempString(const _E* szVal, size_t len)
		: _Base(szVal, len)
	{
	}

	TempString(const std::basic_string<_E>& s)
		: _Base(_ConvIt(s.begin()), s.length())
	{
	}

	TempString(const std::vector<_E>& s)
		: _Base(_ConvIt(s.begin()), s.size())
	{
	}

	TempString(const BasicString<_E>& s)
		: _Base(s)
	{
	}
};

// -------------------------------------------------------------------------
// concat

template <class AllocT, class StringT>
inline StringT winx_call concat(AllocT& alloc, const StringT val[], size_t count)
{
	typedef typename StringT::value_type CharT;
	
	size_t i, len = 0;
	for (i = 0; i < count; ++i)
		len += val[i].size();
	
	CharT* buf = STD_NEW_ARRAY(alloc, CharT, len);
	for (i = 0; i < count; ++i)
		buf = std::copy(val[i].begin(), val[i].end(), buf);

	return StringT(buf-len, len);
}

template <class AllocT, class CharT>
inline BasicString<CharT> winx_call __concatString(
	AllocT& alloc, const TempString<CharT>* val[], size_t count)
{
	size_t i, len = 0;
	for (i = 0; i < count; ++i)
		len += val[i]->size();

	CharT* buf = STD_NEW_ARRAY(alloc, CharT, len);
	for (i = 0; i < count; ++i)
		buf = std::copy(val[i]->begin(), val[i]->end(), buf);

	return BasicString<CharT>(buf-len, len);
}

WINX_VARGS_TFUNC_EP1_REF(
	BasicString<char>, concat, AllocT, const TempString<char>,
	__concatString, WINX_CB_PTR);

WINX_VARGS_TFUNC_EP1_REF(
	BasicString<WCHAR>, concat, AllocT, const TempString<WCHAR>,
	__concatString, WINX_CB_PTR);

// -------------------------------------------------------------------------
// class TestStringAlgo

template <class LogT>
class TestStringAlgo : public TestCase
{
	WINX_TEST_SUITE(TestStringAlgo);
		WINX_TEST(testConcat);
	WINX_TEST_SUITE_END();

public:
	void testConcat(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::String s[4];
		s[0].attach("Hello");
		s[1].assign(alloc, ", ");
		s[2] = std::String(alloc, "world");
		s[3].assign(alloc, 3, '!');

		std::String result = std::concat(alloc, s, countof(s));
		AssertExp(result == "Hello, world!!!");

		std::String result2 = std::concat(alloc, "Hello", "!!!");
		AssertExp(result2 == "Hello!!!");

		std::String result3 = std::concat(alloc, s[0], s[1], s[2], s[3]);
		AssertExp(result == result3);

		char s2buf[] = "world";
		std::vector<char> s2(s2buf, std::end(s2buf));
		std::string s1 = result2.strSTL();
		std::String result4 = std::concat(alloc, s1, " ", s2, "!!", "!");
		AssertExp(result4 == "Hello!!! world!!!");
	}
};

// -------------------------------------------------------------------------
// $Log: StringAlgo.h,v $

__NS_STD_END

#endif /* __STDEXT_TEXT_STRINGALGO_H__ */
