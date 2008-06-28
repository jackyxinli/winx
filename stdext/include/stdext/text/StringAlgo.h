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

#ifndef STDEXT_TEXT_RANGE_H
#include "Range.h"
#endif

#ifndef STDEXT_CHARTYPE_H
#include "../CharType.h"
#endif

#ifndef STDEXT_MSVCRT_VARGS_H
#include "../msvcrt/vargs.h"
#endif

NS_STDEXT_BEGIN

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
	wchar_t* str2 = std::resize(dest, cch2);
	MultiByteToWideChar(from, 0, str, cch, str2, cch2);
}

template <class StringT>
inline void winx_call iconv(
	const wchar_t* str, size_t cch, codepage_t to, StringT& dest)
{
	int cch2 = WideCharToMultiByte(to, 0, str, cch, NULL, 0, NULL, NULL);
	char* str2 = std::resize(dest, cch2);
	WideCharToMultiByte(to, 0, str, cch, str2, cch2, NULL, NULL);
}

template <class AllocT>
inline BasicString<wchar_t> winx_call iconv(
	AllocT& alloc, codepage_t from, const char* str, size_t cch)
{
	int cch2 = MultiByteToWideChar(from, 0, str, cch, NULL, 0);
	wchar_t* str2 = STD_ALLOC_ARRAY(alloc, wchar_t, cch2);
	MultiByteToWideChar(from, 0, str, cch, str2, cch2);
	return BasicString<wchar_t>(str2, cch2);
}

template <class AllocT>
inline BasicString<char> winx_call iconv(
	AllocT& alloc, const wchar_t* str, size_t cch, codepage_t to)
{
	int cch2 = WideCharToMultiByte(to, 0, str, cch, NULL, 0, NULL, NULL);
	char* str2 = STD_ALLOC_ARRAY(alloc, char, cch2);
	WideCharToMultiByte(to, 0, str, cch, str2, cch2, NULL, NULL);
	return BasicString<char>(str2, cch2);
}

template <class AllocT>
__forceinline BasicString<wchar_t> winx_call iconv(
	AllocT& alloc, codepage_t from, const TempString<char>& str)
{
	return iconv(alloc, from, str.data(), str.size());
}

template <class AllocT>
__forceinline BasicString<char> winx_call iconv(
	AllocT& alloc, const TempString<wchar_t>& str, codepage_t to)
{
	return iconv(alloc, str.data(), str.size(), to);
}

// -------------------------------------------------------------------------
// upper/lower

template <class CharT, class AllocT, class TransT>
inline BasicString<CharT> winx_call transform(
	AllocT& alloc, const TempString<CharT>& str, const TransT& trans)
{
	size_t n = str.size();
	CharT* p = STD_NEW_ARRAY(alloc, CharT, n);
	std::transform(str.begin(), str.end(), p, trans);
	return BasicString<CharT>(p, n);
}

template <class AllocT>
__forceinline BasicString<char> winx_call upper(AllocT& alloc, const TempString<char>& str) {
	return std::transform(alloc, str, ToUpper<char>());
}

template <class AllocT>
__forceinline BasicString<wchar_t> winx_call upper(AllocT& alloc, const TempString<wchar_t>& str) {
	return std::transform(alloc, str, ToUpper<wchar_t>());
}

template <class AllocT>
__forceinline BasicString<char> winx_call lower(AllocT& alloc, const TempString<char>& str) {
	return std::transform(alloc, str, ToLower<char>());
}

template <class AllocT>
__forceinline BasicString<wchar_t> winx_call lower(AllocT& alloc, const TempString<wchar_t>& str) {
	return std::transform(alloc, str, ToLower<wchar_t>());
}

// -------------------------------------------------------------------------
// trim/trimLeft/trimRight

template <class CharT>
inline BasicString<CharT> winx_call trimLeft(const BasicString<CharT>& s)
{
	typedef typename BasicString<CharT>::const_iterator iterator;
	const iterator last = s.end();
	const iterator first = std::find_if(s.begin(), last, CharType::NotIsSpace());
	return BasicString<CharT>(first, last-first);
}

template <class CharT>
inline BasicString<CharT> winx_call trimRight(const BasicString<CharT>& s)
{
	typedef typename BasicString<CharT>::const_iterator iterator;
	const CharType::NotIsSpace notSpace = CharType::NotIsSpace();
	const iterator first = s.begin();
	iterator last = s.end();
	while (first != last) {
		if (notSpace(*--last)) {
			++last;
			break;
		}
	}
	return BasicString<CharT>(first, last-first);
}

template <class CharT>
inline BasicString<CharT> winx_call trim(const BasicString<CharT>& s)
{
	typedef typename BasicString<CharT>::const_iterator iterator;
	const CharType::NotIsSpace notSpace = CharType::NotIsSpace();
	iterator last = s.end();
	const iterator first = std::find_if(s.begin(), last, notSpace);
	while (first != last) {
		if (notSpace(*--last)) {
			++last;
			break;
		}
	}
	return BasicString<CharT>(first, last-first);
}

// -------------------------------------------------------------------------
// flatten

template <class AllocT, class Iterator>
inline
BasicArray<typename iterator_traits_alter<Iterator>::value_type::value_type>
winx_call flatten(AllocT& alloc, const Iterator first, const Iterator last)
{
	typedef typename iterator_traits_alter<Iterator>::value_type::value_type ValueT;

	Iterator it;
	size_t len = 0;
	for (it = first; it != last; ++it)
		len += (*it).size();

	ValueT* buf = STD_NEW_ARRAY(alloc, ValueT, len);
	for (it = first; it != last; ++it)
		buf = std::copy((*it).begin(), (*it).end(), buf);

	return BasicArray<ValueT>(buf-len, buf);
}

template <class AllocT, class ContainerT>
inline
BasicArray<typename ContainerT::value_type::value_type>
winx_call flatten(AllocT& alloc, const ContainerT& cont)
{
	return flatten(alloc, cont.begin(), cont.end());
}

// -------------------------------------------------------------------------
// concat

template <class AllocT, class StringT>
__forceinline
BasicString<typename StringT::value_type>
winx_call concat(AllocT& alloc, const StringT val[], size_t count) {
	return flatten(alloc, val, val + count);
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
	BasicString<wchar_t>, concat, AllocT, const TempString<wchar_t>, __concatString);

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestStringAlgo

#if defined(STD_UNITTEST)

template <class LogT>
class TestStringAlgo : public TestCase
{
	WINX_TEST_SUITE(TestStringAlgo);
		WINX_TEST(testConcat);
		WINX_TEST(testConv);
		WINX_TEST(testFlatten);
		WINX_TEST(testTrim);
//		WINX_TEST(testIconv);
	WINX_TEST_SUITE_END();

public:
	void testIconv(LogT& log)
	{
		std::ScopeAlloc alloc;

		std::WString s1 = std::iconv(alloc, std::cp_auto, "Hello, world!");
		AssertExp(s1 == L"Hello, world!");

		std::String s2 = std::iconv(alloc, s1, std::cp_utf8);
		AssertExp(s2 == "Hello, world!");
	}
	
	void testConv(LogT& log)
	{
		std::ScopeAlloc alloc;

		std::string s1 = "ABC";
		AssertExp(std::lower(alloc, s1) == "abc");
		
		std::String s2(alloc, "abc");
		AssertExp(std::upper(alloc, s2) == "ABC");
	}
	
	void testTrim(LogT& log)
	{
		std::ScopeAlloc alloc;
		std::String s(alloc, " \t Hello, world! \t\n");
		
		AssertExp(std::trim(s) == "Hello, world!");
		AssertExp(std::trimLeft(s) == "Hello, world! \t\n");
		AssertExp(std::trimRight(s) == " \t Hello, world!");
	}

	void testFlatten(LogT& log)
	{
		std::AutoFreeAlloc alloc;
		{
			std::list<std::string> lst;
			lst.push_back("Hello,");
			lst.push_back(" ");
			lst.push_back("world!");
		
			std::String s = std::flatten(alloc, lst);
			AssertExp(s == "Hello, world!");
		}
		{
			std::vector<std::string> vec;
			vec.push_back("Hello,");
			vec.push_back(" ");
			vec.push_back("world!");
		
			std::String s = std::flatten(alloc, vec);
			AssertExp(s == "Hello, world!");
		}
	}

	void testConcat(LogT& log)
	{
		std::ScopeAlloc alloc;

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

#endif /* STDEXT_TEXT_STRINGALGO_H */

