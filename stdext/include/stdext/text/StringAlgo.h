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
#ifndef STDEXT_TEXT_STRINGALGO_H
#define STDEXT_TEXT_STRINGALGO_H

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "BasicString.h"
#endif

#ifndef STDEXT_MSVCRT_VARGS_H
#include "../msvcrt/vargs.h"
#endif

NS_STD_BEGIN

// -------------------------------------------------------------------------
// codepage_t

enum codepage_t
{
	cp_auto		= 0,
	cp_utf7		= 65000,
	cp_utf8		= 65001,
	cp_shiftjis	= 932,
	cp_gbk		= 936,
	cp_big5		= 950,
	cp_1252		= 1252,
};

// -------------------------------------------------------------------------
// iconv

template <class StringT>
inline void winx_call iconv(
	codepage_t from, const char* str, size_t cch, StringT& dest)
{
	int cch2 = MultiByteToWideChar(from, 0, str, cch, NULL, 0);
	WCHAR* str2 = std::resize(dest, cch2);
	MultiByteToWideChar(from, 0, str, cch, str2, cch2);
}

template <class StringT>
inline void winx_call iconv(
	const WCHAR* str, size_t cch, codepage_t to, StringT& dest)
{
	int cch2 = WideCharToMultiByte(to, 0, str, cch, NULL, 0, NULL, NULL);
	char* str2 = std::resize(dest, cch2);
	WideCharToMultiByte(to, 0, str, cch, str2, cch2, NULL, NULL);
}

template <class AllocT>
inline BasicString<WCHAR> winx_call iconv(
	AllocT& alloc, codepage_t from, const char* str, size_t cch)
{
	int cch2 = MultiByteToWideChar(from, 0, str, cch, NULL, 0);
	WCHAR* str2 = STD_ALLOC_ARRAY(alloc, WCHAR, cch2);
	MultiByteToWideChar(from, 0, str, cch, str2, cch2);
	return BasicString<WCHAR>(str2, cch2);
}

template <class AllocT>
inline BasicString<char> winx_call iconv(
	AllocT& alloc, const WCHAR* str, size_t cch, codepage_t to)
{
	int cch2 = WideCharToMultiByte(to, 0, str, cch, NULL, 0, NULL, NULL);
	char* str2 = STD_ALLOC_ARRAY(alloc, char, cch2);
	WideCharToMultiByte(to, 0, str, cch, str2, cch2, NULL, NULL);
	return BasicString<char>(str2, cch2);
}

template <class AllocT>
__forceinline BasicString<WCHAR> winx_call iconv(
	AllocT& alloc, codepage_t from, const TempString<char>& str)
{
	return iconv(alloc, from, str.data(), str.size());
}

template <class AllocT>
__forceinline BasicString<char> winx_call iconv(
	AllocT& alloc, const TempString<WCHAR>& str, codepage_t to)
{
	return iconv(alloc, str.data(), str.size(), to);
}

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
	BasicString<char>, concat, AllocT, const TempString<char>, __concatString);

WINX_VARGS_TFUNC_EP1_REF(
	BasicString<WCHAR>, concat, AllocT, const TempString<WCHAR>, __concatString);

// -------------------------------------------------------------------------
// class TestStringAlgo

#if defined(STD_UNITTEST)

template <class LogT>
class TestStringAlgo : public TestCase
{
	WINX_TEST_SUITE(TestStringAlgo);
		WINX_TEST(testConcat);
		WINX_TEST(testIconv);
	WINX_TEST_SUITE_END();

public:
	void testIconv(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::WString s1 = std::iconv(alloc, std::cp_auto, "Hello, world!");
		AssertExp(s1 == L"Hello, world!");

		std::String s2 = std::iconv(alloc, s1, std::cp_utf8);
		AssertExp(s2 == "Hello, world!");
	}

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
		std::string s1 = result2.stl_str();
		std::String result4 = std::concat(alloc, s1, " ", s2, "!!", "!");
		AssertExp(result4 == "Hello!!! world!!!");
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: StringAlgo.h,v $

NS_STD_END

#endif /* STDEXT_TEXT_STRINGALGO_H */
