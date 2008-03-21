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
// Module: stdext/text/BasicString.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: BasicString.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_TEXT_BASICSTRING_H__
#define __STDEXT_TEXT_BASICSTRING_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

#ifndef __STDEXT_MEMORY_H__
#include "../Memory.h"
#endif

#ifndef __STD_VECTOR_H__
#include "../../std/vector.h"
#endif

#ifndef __STD_STRING_H__
#include "../../std/string.h"
#endif

#ifndef __STDEXT_TEXT_BASICALGO_H__
#include "BasicAlgo.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------

template <class _E>
class CompareNoCase
{
};

template <>
class CompareNoCase<char>
{
public:
	int winx_call operator()(int c1, int c2) const {
		return toupper(c1) - toupper(c2);
	}
};

template <>
class CompareNoCase<wchar_t>
{
public:
	int winx_call operator()(wchar_t c1, wchar_t c2) const {
		return towupper(c1) - towupper(c2);
	}
};

// -------------------------------------------------------------------------

inline void winx_call __Xran()
	{ throw std::out_of_range("invalid string position"); }

#ifndef _WINX_ALLOC_TYPE
#define _WINX_ALLOC_TEMPLATE_ARGS		class _Alloc, int _MemBlockSize
#define _WINX_ALLOC_TYPE				std::AutoFreeAllocT<_Alloc, _MemBlockSize>
#endif

// -------------------------------------------------------------------------
// class BasicString

template <class _E>
class BasicString
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef const _E* pointer;
	typedef const _E* const_pointer;
	typedef const _E& reference;
	typedef const _E& const_reference;
	typedef _E value_type;

	typedef const _E* iterator;
	typedef const _E* const_iterator;

#if defined(X_CC_VC6)
	typedef std::reverse_iterator<const_iterator, value_type,
		const_reference, const_pointer, difference_type>
			const_reverse_iterator;
	typedef std::reverse_iterator<iterator, value_type,
		reference, pointer, difference_type>
			reverse_iterator;
#else
	typedef std::reverse_iterator<const_iterator>
			const_reverse_iterator;
	typedef std::reverse_iterator<iterator>
			reverse_iterator;
#endif

private:
    size_type m_length;
    const value_type* m_pszBuf;
	
private:
	typedef BasicString _Myt;
	typedef std::char_traits<value_type> _Tr;

public:
	typedef _Tr traits_type;

public:
	template <class AllocT, class Iterator>
	static const value_type* _makeBuf(AllocT& alloc, Iterator first, size_type cch)
	{
		value_type* psz = (value_type*)alloc.allocate(cch * sizeof(value_type));
		std::copy(first, first + cch, psz);
		return psz;
	}

	template <class AllocT>
	static const value_type* _makeBuf(AllocT& alloc, size_type count, value_type ch)
	{
		value_type* psz = (value_type*)alloc.allocate(count * sizeof(value_type));
		std::fill_n(psz, count, ch);
		return psz;
	}

public:
	BasicString()
		: m_length(0)
	{
	}

	BasicString(const value_type* pszVal, size_type cch)
		: m_length(cch), m_pszBuf(pszVal)
	{
	}

	BasicString(const _Myt& src, size_type from, size_type cch = (size_type)-1)
	{
		assign(src, from, cch);
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	BasicString(_WINX_ALLOC_TYPE& alloc, const value_type* pszVal, size_type cch)
		: m_length(cch), m_pszBuf(_makeBuf(alloc, pszVal, length))
	{
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	BasicString(_WINX_ALLOC_TYPE& alloc, const value_type* pszVal)
		: m_length(_Tr::length(pszVal)), m_pszBuf(_makeBuf(alloc, pszVal, m_length))
	{
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	BasicString(_WINX_ALLOC_TYPE& alloc, const _Myt& b)
		: m_length(b.size()), m_pszBuf(_makeBuf(alloc, b.data(), m_length))
	{
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS, class _Tr, class _Alloc2>
	BasicString(_WINX_ALLOC_TYPE& alloc, const basic_string<_E, _Tr, _Alloc2>& b)
		: m_length(b.length()), m_pszBuf(_makeBuf(alloc, b.data(), m_length))
	{
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	BasicString(_WINX_ALLOC_TYPE& alloc, size_type count, value_type ch)
		: m_length(count), m_pszBuf(_makeBuf(alloc, count, ch))
	{
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS, class Iterator>
	BasicString(_WINX_ALLOC_TYPE& alloc, Iterator first, Iterator last)
		: m_length(std::distance(first, last)), m_pszBuf(_makeBuf(alloc, first, m_length))
	{
	}

public:
	_Myt& winx_call attach(const value_type* pszVal)
	{
		m_length = _Tr::length(pszVal);
		m_pszBuf = pszVal;
		return *this;
	}

	_Myt& winx_call attach(const value_type* pszVal, size_type cch)
	{
		m_length = cch;
		m_pszBuf = pszVal;
		return *this;
	}

	_Myt& winx_call assign(const _Myt& src, size_type from, size_type cch = (size_type)-1)
	{
		if (src.size() < from)
			__Xran();
		size_type cchMax = src.size() - from;
		m_length = (cchMax < cch ? cchMax : cch);
		m_pszBuf = src.m_pszBuf + from;
		return *this;
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	_Myt& winx_call assign(_WINX_ALLOC_TYPE& alloc, const value_type* pszVal, size_type cch)
	{
		m_length = cch;
		m_pszBuf = _makeBuf(alloc, pszVal, length);
		return *this;
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	_Myt& winx_call assign(_WINX_ALLOC_TYPE& alloc, const value_type* pszVal)
	{
		m_length = _Tr::length(pszVal);
		m_pszBuf = _makeBuf(alloc, pszVal, m_length);
		return *this;
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	_Myt& winx_call assign(_WINX_ALLOC_TYPE& alloc, const _Myt& b)
	{
		m_length = b.size();
		m_pszBuf = _makeBuf(alloc, b.data(), m_length);
		return *this;
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS, class _Tr, class _Alloc2>
	_Myt& winx_call assign(_WINX_ALLOC_TYPE& alloc, const basic_string<_E, _Tr, _Alloc2>& b)
	{
		m_length = b.length();
		m_pszBuf = _makeBuf(alloc, b.data(), m_length);
		return *this;
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	_Myt& winx_call assign(_WINX_ALLOC_TYPE& alloc, size_type count, value_type ch)
	{
		m_length = count;
		m_pszBuf = _makeBuf(alloc, count, ch);
		return *this;
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS, class Iterator>
	_Myt& winx_call assign(_WINX_ALLOC_TYPE& alloc, Iterator first, Iterator last)
	{
		m_length = std::distance(first, last);
		m_pszBuf = _makeBuf(alloc, first, m_length);
		return *this;
	}

public:
	_Myt winx_call substr(size_type from = 0, size_type count = (size_type)-1) const
	{
		return _Myt(*this, from, count);
	}

	template <class LogT>
	void winx_call trace(LogT& log) const
	{
		log.printString(m_pszBuf, m_pszBuf + m_length);
	}

	void winx_call swap(_Myt& b)
	{
		std::swap(m_length, b.m_length);
		std::swap(m_pszBuf, b.m_pszBuf);
	}

	void winx_call clear()
	{
		m_length = 0;
	}

	void winx_call erase()
	{
		m_length = 0;
	}

	template <class _Tr, class _Alloc2>
	static _Myt winx_call cast(const basic_string<_E, _Tr, _Alloc2>& src)
	{
		return _Myt(src.data(), src.length());
	}

	template <class _Alloc2>
	static _Myt winx_call cast(const vector<_E, _Alloc2>& builder)
	{
		return _Myt(_ConvIt(builder.begin()), builder.size());
	}

public:
	const_iterator winx_call begin() const
		{return m_pszBuf; }

	const_iterator winx_call end() const
		{return m_pszBuf + m_length; }
	
	const_reverse_iterator winx_call rbegin() const
		{return (const_reverse_iterator(end())); }
	
	const_reverse_iterator winx_call rend() const
		{return (const_reverse_iterator(begin())); }
	
	const_reference winx_call at(size_type i) const
		{if (i <= m_length)
			__Xran();
		return m_pszBuf[i]; }
	
	const_reference winx_call operator[](size_type i) const
		{return m_pszBuf[i]; }
	
	const value_type* winx_call data() const
		{return m_pszBuf; }
	
	size_type winx_call length() const
		{return m_length; }
	
	size_type winx_call size() const
		{return m_length; }
	
	bool winx_call empty() const
		{return m_length == 0; }

public:
	// 将String转为STL string。这个操作比较费时。

	basic_string<_E> winx_call stl_str() const
		{return basic_string<_E>(m_pszBuf, m_length);	}

public:
	// 在字符串中查找子串（正向查找）。

	const_iterator winx_call find(const _E ch, const_iterator from) const
	{
		return std::find(from, end(), ch);
	}

	const_iterator winx_call find(const _E ch) const
	{
		return std::find(begin(), end(), ch);
	}

	const_iterator winx_call find(const _Myt& pattern, const_iterator from) const
	{
		return std::search(from, end(), pattern.begin(), pattern.end());
	}
	
	const_iterator winx_call find(const _Myt& pattern) const
	{
		return std::search(begin(), end(), pattern.begin(), pattern.end());
	}

	const_iterator winx_call find(const _E* pattern, const_iterator from) const
	{
		return std::search(from, end(), pattern, std::end(pattern));
	}

	const_iterator winx_call find(const _E* pattern) const
	{
		return std::search(begin(), end(), pattern, pattern + _Tr::length(pattern));
	}

	const_iterator winx_call find(const _E* pattern, size_type len, const_iterator from) const
	{
		return std::search(from, end(), pattern, pattern + len);
	}

	const_iterator winx_call find(const _E* pattern, size_type len) const
	{
		return std::search(begin(), end(), pattern, pattern + len);
	}

public:
	// 在字符串中查找子串（反向查找）。

	const_iterator winx_call rfind(const _E ch, const_iterator from) const
	{
		return std::rfind(from, rend(), ch);
	}

	const_iterator winx_call rfind(const _E ch) const
	{
		return std::rfind(begin(), end(), ch);
	}

	const_iterator winx_call rfind(const _Myt& pattern, const_iterator from) const
	{
		return std::find_end(from, end(), pattern.begin(), pattern.end());
	}
	
	const_iterator winx_call rfind(const _Myt& pattern) const
	{
		return std::find_end(begin(), end(), pattern.begin(), pattern.end());
	}

	const_iterator winx_call rfind(const _E* pattern, const_iterator from) const
	{
		return std::find_end(from, end(), pattern, std::end(pattern));
	}

	const_iterator winx_call rfind(const _E* pattern) const
	{
		return std::find_end(begin(), end(), pattern, std::end(pattern));
	}

	const_iterator winx_call rfind(const _E* pattern, size_type len, const_iterator from) const
	{
		return std::find_end(from, end(), pattern, pattern+len);
	}

	const_iterator winx_call rfind(const _E* pattern, size_type len) const
	{
		return std::find_end(begin(), end(), pattern, pattern+len);
	}

public:
	// 查找某个集合中的字符在字符串中第一次出现的位置（正向查找）。

	const_iterator winx_call find_first_of(const _E ch, const_iterator from) const
	{
		return std::find(from, end(), ch);
	}

	const_iterator winx_call find_first_of(const _E ch) const
	{
		return std::find(begin(), end(), ch);
	}

	const_iterator winx_call find_first_of(const _Myt& pattern, const_iterator from) const
	{
		return std::find_first_of(from, end(), pattern.begin(), pattern.end());
	}
	
	const_iterator winx_call find_first_of(const _Myt& pattern) const
	{
		return std::find_first_of(begin(), end(), pattern.begin(), pattern.end());
	}

	const_iterator winx_call find_first_of(const _E* pattern, const_iterator from) const
	{
		return std::find_first_of(from, end(), pattern, pattern + _Tr::length(pattern));
	}

	const_iterator winx_call find_first_of(const _E* pattern) const
	{
		return std::find_first_of(begin(), end(), pattern, pattern + _Tr::length(pattern));
	}

	const_iterator winx_call find_first_of(const _E* pattern, size_type len, const_iterator from) const
	{
		return std::find_first_of(from, end(), pattern, pattern + len);
	}

	const_iterator winx_call find_first_of(const _E* pattern, size_type len) const
	{
		return std::find_first_of(begin(), end(), pattern, pattern + len);
	}

public:
	// 查找某个集合中的字符在字符串中第一次出现的位置（反向查找）。

	const_reverse_iterator winx_call find_last_of(const _E ch, const_reverse_iterator from) const
	{
		return std::find(from, rend(), ch);
	}

	const_reverse_iterator winx_call find_last_of(const _E ch) const
	{
		return std::find(rbegin(), rend(), ch);
	}

	const_reverse_iterator winx_call find_last_of(const _Myt& pattern, const_reverse_iterator from) const
	{
		return std::find_first_of(from, rend(), pattern.begin(), pattern.end());
	}
	
	const_reverse_iterator winx_call find_last_of(const _Myt& pattern) const
	{
		return std::find_first_of(rbegin(), rend(), pattern.begin(), pattern.end());
	}

	const_reverse_iterator winx_call find_last_of(const _E* pattern, const_reverse_iterator from) const
	{
		return std::find_first_of(from, rend(), pattern, pattern + _Tr::length(pattern));
	}

	const_reverse_iterator winx_call find_last_of(const _E* pattern) const
	{
		return std::find_first_of(rbegin(), rend(), pattern, pattern + _Tr::length(pattern));
	}

	const_reverse_iterator winx_call find_last_of(const _E* pattern, size_type len, const_reverse_iterator from) const
	{
		return std::find_first_of(from, rend(), pattern, pattern + len);
	}

	const_reverse_iterator winx_call find_last_of(const _E* pattern, size_type len) const
	{
		return std::find_first_of(rbegin(), rend(), pattern, pattern + len);
	}

public:
	// 在字符串中查找不在集合中出现的第一个字符的位置（正向查找）。

	const_iterator winx_call find_first_not_of(const _E ch, const_iterator from) const
	{
		return std::find_not(from, end(), ch);
	}

	const_iterator winx_call find_first_not_of(const _E ch) const
	{
		return std::find_not(begin(), end(), ch);
	}

	const_iterator winx_call find_first_not_of(const _Myt& pattern, const_iterator from) const
	{
		return std::find_first_not_of(from, end(), pattern.begin(), pattern.end());
	}
	
	const_iterator winx_call find_first_not_of(const _Myt& pattern) const
	{
		return std::find_first_not_of(begin(), end(), pattern.begin(), pattern.end());
	}

	const_iterator winx_call find_first_not_of(const _E* pattern, const_iterator from) const
	{
		return std::find_first_not_of(from, end(), pattern, pattern + _Tr::length(pattern));
	}

	const_iterator winx_call find_first_not_of(const _E* pattern) const
	{
		return std::find_first_not_of(begin(), end(), pattern, pattern + _Tr::length(pattern));
	}

	const_iterator winx_call find_first_not_of(const _E* pattern, size_type len, const_iterator from) const
	{
		return std::find_first_not_of(from, end(), pattern, pattern + len);
	}

	const_iterator winx_call find_first_not_of(const _E* pattern, size_type len) const
	{
		return std::find_first_not_of(begin(), end(), pattern, pattern + len);
	}

public:
	// 在字符串中查找不在集合中出现的第一个字符的位置（反向查找）。

	const_reverse_iterator winx_call find_last_not_of(const _E ch, const_reverse_iterator from) const
	{
		return std::find_not(from, rend(), ch);
	}

	const_reverse_iterator winx_call find_last_not_of(const _E ch) const
	{
		return std::find_not(rbegin(), rend(), ch);
	}

	const_reverse_iterator winx_call find_last_not_of(const _Myt& pattern, const_reverse_iterator from) const
	{
		return std::find_first_not_of(from, rend(), pattern.begin(), pattern.end());
	}
	
	const_reverse_iterator winx_call find_last_not_of(const _Myt& pattern) const
	{
		return std::find_first_not_of(rbegin(), rend(), pattern.begin(), pattern.end());
	}

	const_reverse_iterator winx_call find_last_not_of(const _E* pattern, const_reverse_iterator from) const
	{
		return std::find_first_not_of(from, rend(), pattern, pattern + _Tr::length(pattern));
	}

	const_reverse_iterator winx_call find_last_not_of(const _E* pattern) const
	{
		return std::find_first_not_of(rbegin(), rend(), pattern, pattern + _Tr::length(pattern));
	}

	const_reverse_iterator winx_call find_last_not_of(const _E* pattern, size_type len, const_reverse_iterator from) const
	{
		return std::find_first_not_of(from, rend(), pattern, pattern + len);
	}

	const_reverse_iterator winx_call find_last_not_of(const _E* pattern, size_type len) const
	{
		return std::find_first_not_of(rbegin(), rend(), pattern, pattern + len);
	}

public:
	// 比较两个字符串。
	
	int winx_call compare(const _Myt& b) const
	{
		return std::compare(begin(), end(), b.begin(), b.end());
	}

	template <class _Tr, class _Alloc2>
	int winx_call compare(const basic_string<_E, _Tr, _Alloc2>& b) const
	{
		return std::compare(begin(), end(), b.begin(), b.end());
	}

	int winx_call compare(const _E* b, size_type blen) const
	{
		return std::compare(begin(), end(), b, b + blen);
	}

	int winx_call compare(const _E* b) const
	{
		return std::compare(begin(), end(), b);
	}

	int winx_call compare(size_type from, size_type count, const _Myt& b) const
	{
		return substr(from, count).compare(b);
	}

	int winx_call compare(size_type from, size_type count, const _Myt& b, size_type bfrom, size_type bcount) const
	{
		return substr(from, count).compare(b.substr(bfrom, bcount));
	}

	int winx_call compare(size_type from, size_type count, const _E* b, size_type blen) const
	{
		return substr(from, count).compare(b, blen);
	}

	int winx_call compare(size_type from, size_type count, const _E* b) const
	{
		return substr(from, count).compare(b);
	}

public:
	// 比较两个字符串（传入单字符的比较函数）。
	
	template <class _Compr>
	int winx_call compare_by(const _Myt& b, _Compr cmp) const
	{
		return std::compare(begin(), end(), b.begin(), b.end(), cmp);
	}

	template <class _Tr, class _Alloc2, class _Compr>
	int winx_call compare_by(const basic_string<_E, _Tr, _Alloc2>& b, _Compr cmp) const
	{
		return std::compare(begin(), end(), b.begin(), b.end(), cmp);
	}

	template <class _Compr>
	int winx_call compare_by(const _E* b, size_type blen, _Compr cmp) const
	{
		return std::compare(begin(), end(), b, b + blen, cmp);
	}

	template <class _Compr>
	int winx_call compare_by(const _E* b, _Compr cmp) const
	{
		return std::compare(begin(), end(), b, std::end(b), cmp);
	}

private:
	typedef CompareNoCase<_E> _ComareNoCase;

public:
	// 比较两个字符串（忽略大小写）。

	int winx_call icompare(const _Myt& b) const
	{
		return compare_by(b, _ComareNoCase());
	}

	template <class _Tr, class _Alloc2>
	int winx_call icompare(const basic_string<_E, _Tr, _Alloc2>& b) const
	{
		return compare_by(b, _ComareNoCase());
	}

	int winx_call icompare(const _E* b, size_type blen) const
	{
		return compare_by(b, blen, _ComareNoCase());
	}

	int winx_call icompare(const _E* b) const
	{
		return compare_by(b, _ComareNoCase());
	}

public:
	// 判断是否包含指定的串。

	bool winx_call contains(const _Myt& b) const
	{
		return find(b) != end();
	}

	template <class _Tr, class _Alloc2>
	bool winx_call contains(const basic_string<_E, _Tr, _Alloc2>& b) const
	{
		return find(b) != end();
	}

	bool winx_call contains(const _E* b, size_type blen) const
	{
		return find(b, blen) != end();
	}

	bool winx_call contains(const _E* b) const
	{
		return find(b) != end();
	}
};

typedef BasicString<char> String;
typedef BasicString<WCHAR> WString;

__NS_STD_END

STD_NO_DESTRUCTOR(String);
STD_NO_DESTRUCTOR(WString);

// -------------------------------------------------------------------------

__NS_STD_BEGIN

template <class _E, class _T2> __forceinline
	bool winx_call operator==(const BasicString<_E>& a, const _T2& b)
    {return (a.compare(b) == 0); }

template <class _E> __forceinline
    bool winx_call operator==(const _E* a, const BasicString<_E>& b)
    {return (b.compare(a) == 0); }

template <class _E, class _Tr, class _Alloc> __forceinline
    bool winx_call operator==(const basic_string<_E, _Tr, _Alloc>& a, const BasicString<_E>& b)
    {return (b.compare(a) == 0); }

template <class _E, class _T2> __forceinline
    bool winx_call operator!=(const BasicString<_E>& a, const _T2& b)
    {return (a.compare(b) != 0); }

template <class _E> __forceinline
    bool winx_call operator!=(const _E* a, const BasicString<_E>& b)
    {return (b.compare(a) != 0); }

template <class _E, class _Tr, class _Alloc> __forceinline
    bool winx_call operator!=(const basic_string<_E, _Tr, _Alloc>& a, const BasicString<_E>& b)
    {return (b.compare(a) != 0); }

template <class _E, class _T2> __forceinline
    bool winx_call operator<(const BasicString<_E>& a, const _T2& b)
    {return (a.compare(b) < 0); }

template <class _E> __forceinline
    bool winx_call operator<(const _E* a, const BasicString<_E>& b)
    {return (b.compare(a) > 0); }

template <class _E, class _Tr, class _Alloc> __forceinline
    bool winx_call operator<(const basic_string<_E, _Tr, _Alloc>& a, const BasicString<_E>& b)
    {return (b.compare(a) > 0); }

template <class _E, class _T2> __forceinline
    bool winx_call operator>(const BasicString<_E>& a, const _T2& b)
    {return (b < a); }

template <class _E> __forceinline
    bool winx_call operator>(const _E* a, const BasicString<_E>& b)
    {return (b < a); }

template <class _E, class _Tr, class _Alloc> __forceinline
    bool winx_call operator>(const basic_string<_E, _Tr, _Alloc>& a, const BasicString<_E>& b)
    {return (b < a); }

template <class _E, class _T2> __forceinline
    bool winx_call operator<=(const BasicString<_E>& a, const _T2& b)
    {return (!(b < a)); }

template <class _E> __forceinline
    bool winx_call operator<=(const _E* a, const BasicString<_E>& b)
    {return (!(b < a)); }

template <class _E, class _Tr, class _Alloc> __forceinline
    bool winx_call operator<=(const basic_string<_E, _Tr, _Alloc>& a, const BasicString<_E>& b)
    {return (!(b < a)); }

template <class _E, class _T2> __forceinline
    bool winx_call operator>=(const BasicString<_E>& a, const _T2& b)
    {return (!(a < b)); }

template <class _E> __forceinline
    bool winx_call operator>=(const _E* a, const BasicString<_E>& b)
    {return (!(a < b)); }

template <class _E, class _Tr, class _Alloc> __forceinline
    bool winx_call operator>=(const basic_string<_E, _Tr, _Alloc>& a, const BasicString<_E>& b)
    {return (!(a < b)); }

// -------------------------------------------------------------------------
// class TestBasicString

template <class LogT>
class TestBasicString : public TestCase
{
	WINX_TEST_SUITE(TestBasicString);
		WINX_TEST(testConstructor);
		WINX_TEST(testFind);
		WINX_TEST(testICompare);
	WINX_TEST_SUITE_END();

public:
	void doTestFind(LogT& log)
	{
	}
	
	void testFind(LogT& log)
	{
		const std::String a1("Hello", 5);

		std::String::iterator it = a1.find("ell");
		AssertExp(it != a1.end());
		AssertExp(*it == 'e');

		doTestFind(log);
	}

	void testICompare(LogT& log)
	{
		std::String a("Hello", 5);
		std::String b("hEllO", 5);
		AssertExp(a.icompare(b) == 0);
	}

	void testConstructor(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		const std::String a1("Hello", 4);
		AssertExp(a1.compare("Hell") == 0);

		std::String a2(a1, 3);
		AssertExp(a2.compare("l") == 0);

		std::String a3(a1, 1, 2);
		AssertExp(a3.compare("el") == 0);

		const std::String a4(alloc, a1);
		AssertExp(a4 == "Hell");
		AssertExp(a1 == a4);

		std::string stl("1234");
		AssertExp(std::String::cast(stl) == "1234");
		AssertExp(std::String::cast(stl).size() == 4);

		std::String a5(alloc, stl);
		AssertExp(a5 == stl && stl == a5);

		std::String a6 = a5;
		AssertExp(a5.data() == a6.data() && a5.size() == a6.length());
		AssertExp(a6.substr(1) == "234");
		AssertExp(a6.substr(1, 1) == "2");
	}
};

// -------------------------------------------------------------------------
// $Log: BasicString.h,v $

__NS_STD_END

#endif /* __STDEXT_TEXT_BASICSTRING_H__ */
