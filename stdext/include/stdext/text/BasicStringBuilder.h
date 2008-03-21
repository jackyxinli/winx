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
// Module: stdext/text/BasicStringBuilder.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: BasicStringBuilder.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_TEXT_BASICSTRINGBUILDER_H__
#define __STDEXT_TEXT_BASICSTRINGBUILDER_H__

#ifndef __STDEXT_TEXT_BASICSTRING_H__
#include "BasicString.h"
#endif

#ifndef __STD_VECTOR_H__
#include "../../std/vector.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------

#define _WINX_BSB_USING_VECTOR												\
public:																		\
	using _Base::assign;													\
	using _Base::insert;													\
	using _Base::erase;														\
	using _Base::clear;														\
	using _Base::begin;														\
	using _Base::size

// -------------------------------------------------------------------------
// class BasicStringBuilder

template <class _E>
class BasicStringBuilder : public std::vector<_E>
{
private:
	typedef std::vector<_E> _Base;
	typedef BasicString<_E> _String;
	typedef BasicStringBuilder _Myt;
	_WINX_BSB_USING_VECTOR;

public:
	typedef typename _Base::size_type size_type;
	
public:
	BasicStringBuilder() {}

	template <class _Tr, class _Alloc>
	explicit BasicStringBuilder(const basic_string<_E, _Tr, _Alloc>& s)
		: _Base(s.begin(), s.end()) {}

	explicit BasicStringBuilder(const _String& s)
		: _Base(s.begin(), s.end()) {}
	
	explicit BasicStringBuilder(size_type count, const _E& val = _E())
		: _Base(count, val) {}

public:
	_String winx_call cast_str() const {
		return _String(_ConvIt(begin()), size());
	}

	template <class AllocT>
	_String winx_call str(AllocT& alloc) const {
		return _String(_ConvIt(begin()), size(), alloc);
	}

	void winx_call erase() {
		clear();
	}
};

typedef BasicStringBuilder<char> StringBuilder;
typedef BasicStringBuilder<WCHAR> WStringBuilder;

// -------------------------------------------------------------------------
// class TestStringBuilder

template <class LogT>
class TestStringBuilder : public TestCase
{
	WINX_TEST_SUITE(TestStringBuilder);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void test(LogT& log)
	{
	}
};

// -------------------------------------------------------------------------
// $Log: BasicStringBuilder.h,v $

__NS_STD_END

#endif /* __STDEXT_TEXT_BASICSTRINGBUILDER_H__ */
