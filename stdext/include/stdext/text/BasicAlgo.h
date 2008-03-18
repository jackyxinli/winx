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
// Module: stdext/text/BasicAlgo.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: BasicString.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_TEXT_BASICALGO_H__
#define __STDEXT_TEXT_BASICALGO_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

#ifndef __STDEXT_CHARTYPE_H__
#include "../CharType.h"
#endif

#if defined(X_STL_SGI)
	#ifndef __SGI_STL_ALGORITHM
	#include "../../../../stl/algorithm"
	#endif
#else
	#ifndef _ALGORITHM_
	#include <algorithm>
	#endif
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// end, length

template <class _It>
inline _It winx_call end(_It it)
{
	while (*it)
		++it;
	return it;
}

template <class _It>
inline size_t winx_call length(_It it)
{
	_It first = it;
	while (*it)
		++it;
	return it - first;
}

// -------------------------------------------------------------------------
// find_first_of, find_first_not_of

template <class _InputIt, class _E>
inline _InputIt winx_call find_first_of(
	_InputIt first, _InputIt last, const _E* cset)
{
	const _E* str;
	for (; first != last; ++first)
	{
		for (str = cset; *str; ++str)
		{
			if (*str == *first)
				return first;
		}
	}
	return first;
}

template <class _InputIt, class _E>
inline _InputIt winx_call find_first_not_of(
	_InputIt first, _InputIt last, const _E* cset)
{
	const _E* str;
	for (; first != last; ++first)
	{
		for (str = cset; *str; ++str)
		{
			if (*str == *first)
				break;
		}
		if (!*str)
			break;
	}
	return first;
}

template <class _InputIt, class _ForwardIt>
inline _InputIt winx_call find_first_not_of(
	_InputIt first, _InputIt last, _ForwardIt set_first, _ForwardIt set_last)
{
	_ForwardIt set_it;
	for (; first != last; ++first)
	{
		for (set_it = set_first; set_it != set_last; ++set_it)
		{
			if (*set_it == *first)
				break;
		}
		if (set_it == set_last)
			break;
	}
	return first;
}

template <class _InputIt, class _ValT>
inline _InputIt winx_call find_not(
	_InputIt first, _InputIt last, const _ValT& val)
{
	for (; first != last; ++first)
	{
		if (*first != val)
			break;
	}
	return first;
}

template <class _BidiIt, class _ValT>
inline _BidiIt winx_call rfind(
	_BidiIt first, _BidiIt last, const _ValT& val)
{
	_BidiIt last0 = last;
	while (first != last)
	{
		if (*--last == val)
			return last;
	}
	return last0;
}

// -------------------------------------------------------------------------
// search

template <class _ForwardIt, class _E>
inline _ForwardIt winx_call search(
	_ForwardIt first, _ForwardIt last, const _E* pattern)
{
	return std::search(
		first, last,
		pattern, end(pattern)
		);
}

// -------------------------------------------------------------------------
// match, match_symbol, match_csymbol

template<class _InputIt1, class _InputIt2>
inline _InputIt1 winx_call match(
	_InputIt1 first1, _InputIt1 last1, _InputIt2 first2, _InputIt2 last2)
{
	_InputIt1 first = first1;
	for (;; ++first1, ++first2)
	{
		if (first2 == last2)
			return first1;
		if (first1 == last1)
			return first;
		if (*first1 != *first2)
			return first;
	}
}

template<class _InputIt, class _E>
inline _InputIt winx_call match(
	_InputIt first, _InputIt last, const _E* str)
{
	_InputIt first1 = first;
	for (;; ++first, ++str)
	{
		if (!*str)
			return first;
		if (first == last)
			return first1;
		if (*first != *str)
			return first1;
	}
}

template <class _InputIt, class _ForwardIt>
inline _InputIt winx_call match_symbol(
	_InputIt first, _InputIt last,
	_ForwardIt set_first, _ForwardIt set_last1, _ForwardIt set_last2)
{
	if (first == last)
		return first;

	if (std::find(set_first, set_last1, *first) == set_last1)
		return first;

	return find_first_not_of(++first, last, set_first, set_last2);
}

template <class _InputIt>
inline _InputIt winx_call match_csymbol(
	_InputIt first, _InputIt last)
{
	if (first == last)
		return first;

	if (!CharType::isCSymbolFirstChar(*first))
		return first;

	return std::find_if(++first, last, CharType::NotIsCSymbolNextChar());
}

// -------------------------------------------------------------------------
// find_not_csymbol, find_digit_or_csymbol

template <class _InputIt>
inline _InputIt winx_call find_not_csymbol(
	_InputIt first, _InputIt last)
{
	return std::find_if(first, last, CharType::NotIsCSymbolNextChar());
}

template <class _InputIt>
inline _InputIt winx_call find_digit_or_csymbol(
	_InputIt first, _InputIt last)
{
	return std::find_if(first, last, CharType::IsCSymbolNextChar());
}

// -------------------------------------------------------------------------
// compare

template<class _InputIt1, class _InputIt2>
inline int winx_call compare(
	_InputIt1 first1, _InputIt1 last1, _InputIt2 first2, _InputIt2 last2)
{
	for (;; ++first1, ++first2)
	{
		if (first1 == last1)
			return first2 == last2 ? 0 : -1;
		if (first2 == last2)
			return 1;
		if (*first1 != *first2)
			return (int)*first1 - (int)*first2;
	}
}

template<class _InputIt, class _E>
inline int winx_call compare(
	_InputIt first, _InputIt last, const _E* str)
{
	for (;; ++first, ++str)
	{
		if (first == last)
			return *str == 0 ? 0 : -1;
		if (*first != *str)
			return (int)*first - (int)*str;
		if (*str == 0)
			return 1;
	}
}

__forceinline int compare(const char* src, const char* dst)
{
	return strcmp(src, dst);
}

__forceinline int compare(const wchar_t* src, const wchar_t* dst)
{
	return wcscmp(src, dst);
}

// -------------------------------------------------------------------------
// strecpy

template <class xchar>
inline xchar* winx_call strecpy(xchar* pszDest, const xchar* pszSrc)
{
	while ((*pszDest = *pszSrc) != 0)
	{
		++pszDest;
		++pszSrc;
	}
	return pszDest;
}

// -------------------------------------------------------------------------
// $Log: BasicAlgo.h,v $

__NS_STD_END

#endif /* __STDEXT_TEXT_BASICALGO_H__ */
