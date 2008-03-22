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

#ifndef __STDEXT_TEXT_STRINGALGO_H__
#include "StringAlgo.h"
#endif

#ifndef __STD_VECTOR_H__
#include "../../std/vector.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------

#define _WINX_BSB_USING_VECTOR												\
public:																		\
	using _Base::insert;													\
	using _Base::erase;														\
	using _Base::clear;														\
	using _Base::begin;														\
	using _Base::end;														\
	using _Base::rbegin;													\
	using _Base::rend;														\
	using _Base::size

#define _WINX_BCB_BASE														\
	*static_cast<_Base*>(this)

// -------------------------------------------------------------------------
// class BasicStringBuilder

template <class _E>
class BasicStringBuilder : public std::vector<_E>
{
private:
	typedef std::vector<_E> _Base;
	typedef TempString<_E> _String;
	typedef BasicStringBuilder _Myt;
	_WINX_BSB_USING_VECTOR;

public:
	typedef typename _Base::size_type size_type;
	typedef typename _Base::difference_type difference_type;
	typedef typename _Base::iterator iterator;
	typedef typename _Base::const_iterator const_iterator;

public:
	BasicStringBuilder() {}

	explicit BasicStringBuilder(const _String s)
		: _Base(s.begin(), s.end()) {}

	BasicStringBuilder(size_type cch, _E ch)
		: _Base(cch, ch) {}

	BasicStringBuilder(const _E* s, size_type count) 
		: _Base(s, s+count) {} 

	template <class _InputIterator>
	BasicStringBuilder(_InputIterator first, _InputIterator last)
		: _Base(first, last) {}

public:
	BasicString<_E> winx_call cast_str() const {
		return BasicString<_E>(_ConvIt(begin()), size());
	}

	BasicString<_E> winx_call cast_substr(
		size_type from = 0, size_type cch = (size_type)-1) const
	{
		size_type cchMax = size() - from;
		if ((difference_type)cchMax < 0)
			__Xran();
		size_type cchLength = (cchMax < cch ? cchMax : cch);
		return BasicString<_E>(_ConvIt(begin() + from), cchLength);
	}

public:
	template <class AllocT>
	BasicString<_E> winx_call str(AllocT& alloc) const {
		return BasicString<_E>(alloc, _ConvIt(begin()), size());
	}

	template <class AllocT>
	BasicString<_E> winx_call substr(
		AllocT& alloc, size_type from = 0, size_type cch = (size_type)-1) const
	{
		return BasicString<_E>(alloc, cast_substr(from, cch));
	}

	basic_string<_E> winx_call stl_str() const
	{
		return basic_string<_E>(_ConvIt(begin()), size());
	}

	const _E* winx_call data() const {
		return _ConvIt(begin());
	}

	void winx_call erase() {
		clear();
	}

	size_type winx_call length() const {
		return size();
	}

public:
	template <class _InputerIterator>
	_Myt& winx_call assign(_InputerIterator first, _InputerIterator last)
	{
		_Base::assign(first, last);
		return *this;
	}

	_Myt& winx_call assign(const _String s)
    {
		_Base::assign(s.begin(), s.end());
		return *this;
	}

	_Myt& winx_call assign(const _E* s, size_type cch)
	{
		_Base::assign(s, s + cch);
		return *this;
	}

	_Myt& winx_call assign(size_type cch, _E ch)
	{
		_Base::assign(cch, ch);
		return *this;
	}

	_Myt& winx_call operator=(const _String s)
	{
		_Base::assign(s.begin(), s.end());
		return *this;
	}

public:
	template <class _InputerIterator>
	_Myt& winx_call append(_InputerIterator first, _InputerIterator last)
	{
		_Base::insert(end(), first, last);
		return *this;
	}

	_Myt& winx_call append(const _String s)
    {
		_Base::insert(end(), s.begin(), s.end());
		return *this;
	}

	_Myt& winx_call append(const _E* s, size_type cch)
	{
		_Base::insert(end(), s, s + cch);
		return *this;
	}

	_Myt& winx_call append(size_type cch, _E ch)
	{
		_Base::insert(end(), cch, ch);
		return *this;
	}

	_Myt& winx_call operator+=(const _String s)
	{
		_Base::insert(end(), s.begin(), s.end());
		return *this;
	}

public:
	template <class _RandIterator>
	_Myt& winx_call replace(
		iterator first, iterator last,
		_RandIterator bfirst, _RandIterator blast)
	{
		std::replace(_WINX_BCB_BASE, first, last, bfirst, blast);
		return *this;
	}

	template <class _RandIterator>
	_Myt& winx_call replace(
		iterator first, iterator last, size_type count, _E ch)
	{
		std::replace(_WINX_BCB_BASE, first, last, count, ch);
		return *this;
	}

	_Myt& winx_call replace(
		iterator first, iterator last, const _E* s, size_type cch)
	{
		std::replace(_WINX_BCB_BASE, first, last, s, s + cch);
		return *this;
	}

	_Myt winx_call replace(iterator first, iterator last, const _String s)
	{
		std::replace(_WINX_BCB_BASE, first, last, s.begin(), s.end());
		return *this;
	}

public:
	_Myt& winx_call insert(iterator it, const _String s)
    {
		_Base::insert(it, s.begin(), s.end());
		return *this;
	}

	_Myt& winx_call insert(iterator it, const _E* s, size_type cch)
	{
		_Base::insert(it, s, s + cch);
		return *this;
	}

public:
	// 在字符串中查找子串（正向查找）。
	_WINX_FIND_ALL(find, std::find, std::search)

	// 在字符串中查找子串（反向查找）。
	_WINX_FIND_ALL(rfind, std::rfind, std::find_end)

	// 查找某个集合中的字符在字符串中第一次出现的位置（正向查找）。
	_WINX_FIND_ALL(find_first_of, std::find, std::find_first_of)

	// 查找某个集合中的字符在字符串中第一次出现的位置（反向查找）。
	_WINX_RFIND_ALL(find_last_of, std::find, std::find_first_of)

	// 在字符串中查找不在集合中出现的第一个字符的位置（正向查找）。
	_WINX_FIND_ALL(find_first_not_of, std::find_not, std::find_first_not_of)

	// 在字符串中查找不在集合中出现的第一个字符的位置（反向查找）。
	_WINX_RFIND_ALL(find_last_not_of, std::find_not, std::find_first_not_of)

public:
	// 比较两个字符串。
	
	int winx_call compare(const _String& b) const
		{return std::compare(begin(), end(), b.begin(), b.end()); }

	int winx_call compare(const _E* b, size_type blen) const
		{return std::compare(begin(), end(), b, b + blen); }

	int winx_call compare(const _E* b) const
		{return std::compare(begin(), end(), b); }

	int winx_call compare(size_type from, size_type count, const _String b) const
		{return cast_substr(from, count).compare(b); }

	int winx_call compare(size_type from, size_type count, const _E* b, size_type blen) const
		{return cast_substr(from, count).compare(b, blen); }

	int winx_call compare(size_type from, size_type count, const _E* b) const
		{return cast_substr(from, count).compare(b); }

public:
	// 比较两个字符串（传入单字符的比较函数）。
	
	template <class _Compr>
	int winx_call compare_by(const _String& b, _Compr cmp) const
		{return std::compare_by(begin(), end(), b.begin(), b.end(), cmp); }

	template <class _Compr>
	int winx_call compare_by(const _E* b, size_type blen, _Compr cmp) const
		{return std::compare_by(begin(), end(), b, b + blen, cmp); }

	template <class _Compr>
	int winx_call compare_by(const _E* b, _Compr cmp) const
		{return std::compare_by(begin(), end(), b, cmp); }

private:
	typedef CompareNoCase<_E> _ComareNoCase;

public:
	// 比较两个字符串（忽略大小写）。

	int winx_call icompare(const _String b) const
		{return compare_by(b, _ComareNoCase()); }

	int winx_call icompare(const _E* b, size_type blen) const
		{return compare_by(b, blen, _ComareNoCase()); }

	int winx_call icompare(const _E* b) const
		{return compare_by(b, _ComareNoCase()); }

public:
	// 判断是否包含指定的串。

	bool winx_call contains(const _String b) const
		{return find(b) != end(); }

	bool winx_call contains(const _E* b, size_type blen) const
		{return find(b, blen) != end(); }
};

typedef BasicStringBuilder<char> StringBuilder;
typedef BasicStringBuilder<WCHAR> WStringBuilder;

// -------------------------------------------------------------------------

template <class _E, class _T2> __forceinline
bool winx_call operator==(const BasicStringBuilder<_E>& a, const _T2& b)
	{return a.compare(b) == 0; }

template <class _E, class _T2> __forceinline
bool winx_call operator!=(const BasicStringBuilder<_E>& a, const _T2& b)
	{return a.compare(b) != 0; }

template <class _E, class _T2> __forceinline
bool winx_call operator<(const BasicStringBuilder<_E>& a, const _T2& b)
	{return a.compare(b) < 0; }

template <class _E, class _T2> __forceinline
bool winx_call operator>(const BasicStringBuilder<_E>& a, const _T2& b)
	{return a.compare(b) > 0; }

template <class _E, class _T2> __forceinline
bool winx_call operator<=(const BasicStringBuilder<_E>& a, const _T2& b)
	{return a.compare(b) <= 0; }

template <class _E, class _T2> __forceinline
bool winx_call operator>=(const BasicStringBuilder<_E>& a, const _T2& b)
	{return a.compare(b) >= 0; }

// -------------------------------------------------------------------------
// class TestStringBuilder

template <class LogT>
class TestStringBuilder : public TestCase
{
	WINX_TEST_SUITE(TestStringBuilder);
		WINX_TEST(testConstruct);
		WINX_TEST(testSubstr);
		WINX_TEST(testAssign);
		WINX_TEST(testAppend);
		WINX_TEST(testReplace);
	WINX_TEST_SUITE_END();

public:
	void testConstruct(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::StringBuilder a1('a');
		AssertExp(a1.size() == 1 && a1[0] == 'a');

		std::StringBuilder a2(3, 'a');
		AssertExp(a2.cast_str() == "aaa");

		std::StringBuilder a("Hello");
		AssertExp(a.cast_str() == "Hello");

		std::StringBuilder b("Hello", 4);
		AssertExp(b == "Hell");

		std::StringBuilder c(b.begin(), b.end());
		std::String d = c.str(alloc);
		AssertExp(d == b && b == d);
	}

	void testSubstr(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::StringBuilder a("Hello");
		AssertExp(a.cast_substr(1, 3) == "ell");
		AssertExp(a.substr(alloc, 1, 3) == "ell");
		AssertExp(a.substr(alloc, 1) == "ello");
		AssertExp(a.substr(alloc, 5) == "");
	}

	void testAssign(LogT& log)
	{
		std::StringBuilder a;
		
		a.assign('a');
		AssertExp(a.cast_str() == "a");

		a.assign(3, 'a');
		AssertExp(a.cast_str() == "aaa");

		a.assign("Hello");
		AssertExp(a.cast_str() == "Hello");

		std::StringBuilder b;
		b.assign("Hello", 2);
		AssertExp(b.cast_str() == "He");

		a.assign(b);
		AssertExp(a.cast_str() == "He");

		a = a;
		AssertExp(a.cast_str() == "He");

		std::string t = "Hello, world!";
		a.assign(t);
		AssertExp(a.cast_str() == t);
	}

	void testAppend(LogT& log)
	{
		std::StringBuilder a("Hello");
		a.append(3, '!');
		AssertExp(a == "Hello!!!");

		a.append('!');
		AssertExp(a == "Hello!!!!");

		a.append(' ').append(std::string("world"));
		AssertExp(a == "Hello!!!! world");
	
		a.append(std::StringBuilder('!'));
		AssertExp(a == "Hello!!!! world!");
		
		a.append(std::vector<char>(2, '!'));
		AssertExp(a == "Hello!!!! world!!!");
	
		a.append(std::String("!", 1));
		AssertExp(a == "Hello!!!! world!!!!");
	}

	void testReplace(LogT& log)
	{
		std::StringBuilder a("Hello!!!! world! Good!");
		std::StringBuilder::iterator it = a.find("!!!!");
		AssertExp(it != a.end());
		AssertExp(*(it-1) == 'o' && *it == '!');

		a.replace(it, it+4, ',');
		AssertExp(a == "Hello, world! Good!");

		it = a.find('!');
		AssertExp(it != a.end());
		AssertExp(*it == '!');

		a.replace(it, it+1, "!!!");
		AssertExp(a == "Hello, world!!! Good!");

		it = a.rfind("Good!");
		AssertExp(*it == 'G');

		a.replace(it, it+5, '#');
		AssertExp(a == "Hello, world!!! #");
	}
};

// -------------------------------------------------------------------------
// $Log: BasicStringBuilder.h,v $

__NS_STD_END

#endif /* __STDEXT_TEXT_BASICSTRINGBUILDER_H__ */
