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
// Module: stdext/text/TempString.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: TempString.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_TEXT_TEMPSTRING_H__
#define __STDEXT_TEXT_TEMPSTRING_H__

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

// -------------------------------------------------------------------------
// class TempString

__NS_STD_BEGIN

inline void winx_call __Xran()
	{ throw std::out_of_range("invalid string position"); }

template <class _E>
class TempString
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

protected:
	const _E* m_pszBuf;
	size_t m_length;

private:
	typedef TempString _Myt;

public:
	TempString()
		: m_length(0)
	{
	}

	TempString(const _E& ch)
		: m_pszBuf(&ch), m_length(1)
	{
	}

	TempString(const _E* szVal)
		: m_pszBuf(szVal), m_length(std::length(szVal))
	{
	}

	TempString(const _E* szVal, size_t len)
		: m_pszBuf(szVal), m_length(len)
	{
	}

	template <class _Tr, class _Alloc>
	TempString(const std::basic_string<_E, _Tr, _Alloc>& s)
		: m_pszBuf(_ConvIt(s.begin())), m_length(s.length())
	{
	}

	template <class _Alloc>
	TempString(const std::vector<_E, _Alloc>& s)
		: m_pszBuf(_ConvIt(s.begin())), m_length(s.size())
	{
	}

public:
	void winx_call attach(const value_type* pszVal)
	{
		m_length = std::length(pszVal);
		m_pszBuf = pszVal;
	}

	void winx_call attach(const value_type* pszVal, size_type cch)
	{
		m_length = cch;
		m_pszBuf = pszVal;
	}

public:
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

	void winx_call clear() { m_length = 0; }
	void winx_call erase() { m_length = 0; }

	bool winx_call empty() const { return m_length == 0; }

	size_type winx_call length() const { return m_length; }
	size_type winx_call size() const { return m_length; }

	const _E* winx_call data() const { return m_pszBuf; }

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

public:
	// 将String转为STL string。这个操作比较费时。

	basic_string<_E> winx_call stl_str() const
		{return basic_string<_E>(m_pszBuf, m_length);	}
};

__NS_STD_END

STD_NO_DESTRUCTOR(std::TempString<char>);
STD_NO_DESTRUCTOR(std::TempString<WCHAR>);
STD_NO_DESTRUCTOR(std::TempString<int>);
STD_NO_DESTRUCTOR(std::TempString<short>);

// -------------------------------------------------------------------------
// $Log: TempString.h,v $

#endif /* __STDEXT_TEXT_TEMPSTRING_H__ */
