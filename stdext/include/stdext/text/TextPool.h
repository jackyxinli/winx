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
// Module: stdext/text/TextPool.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: TextPool.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_TEXT_TEXTPOOL_H__
#define __STDEXT_TEXT_TEXTPOOL_H__

#ifndef __STDEXT_TEXT_BASICSTRING_H__
#include "BasicString.h"
#endif

#ifndef __STDEXT_TEXT_STRINGALGO_H__
#include "StringAlgo.h"
#endif

#ifndef __STDEXT_DEQUE_H__
#include "Deque.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------

#define _WINX_TP_USING_DEQUE												\
public:																		\
	using _Base::insert;													\
	using _Base::erase;														\
	using _Base::clear;														\
	using _Base::begin;														\
	using _Base::end;														\
	using _Base::rbegin;													\
	using _Base::rend;														\
	using _Base::size

#define _WINX_TP_BASE														\
	*static_cast<_Base*>(this)

// -------------------------------------------------------------------------
// class TextPool

template <class _E, class _Alloc = ScopeAlloc>
class TextPool : public std::Deque<_E, _Alloc>
{
private:
	typedef std::Deque<_E, _Alloc> _Base;
	typedef TempString<_E> _String;
	typedef TextPool _Myt;
	_WINX_TP_USING_DEQUE;

public:
	typedef typename _Base::size_type size_type;
	typedef typename _Base::difference_type difference_type;
	typedef typename _Base::iterator iterator;
	typedef typename _Base::const_iterator const_iterator;

public:
	TextPool(_Alloc& alloc)
		: _Base(alloc) {}

	explicit TextPool(_Alloc& alloc, const _String s)
		: _Base(alloc, s.begin(), s.end()) {}

	TextPool(_Alloc& alloc, size_type cch, _E ch)
		: _Base(alloc, cch, ch) {}

	TextPool(_Alloc& alloc, const _E* s, size_type count) 
		: _Base(alloc, s, s+count) {} 

	template <class _InputIterator>
	TextPool(_Alloc& alloc, _InputIterator first, _InputIterator last)
		: _Base(alloc, first, last) {}

public:
	template <class AllocT>
	BasicString<_E> winx_call str(AllocT& alloc) const {
		return BasicString<_E>(alloc, begin(), end());
	}

	template <class AllocT>
	BasicString<_E> winx_call substr(
		AllocT& alloc, size_type from = 0, size_type cch = (size_type)-1) const
	{
		size_type cchMax = size() - from;
		if ((difference_type)cchMax < 0)
			__Xran();
		size_type cchLength = (cchMax < cch ? cchMax : cch);
		const_iterator it = begin() + from;
		return BasicString<_E>(alloc, it, it + cchLength);
	}

	basic_string<_E> winx_call stl_str() const
	{
		return basic_string<_E>(begin(), end());
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

	_Myt& winx_call assign(const _Base& s)
    {
		_Base::copy(s);
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

	_Myt& winx_call append(const _Base& s)
	{
		_Base::insert(end(), s.begin(), s.end());
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
		std::replace(_WINX_TP_BASE, first, last, bfirst, blast);
		return *this;
	}

	template <class _RandIterator>
	_Myt& winx_call replace(
		iterator first, iterator last, size_type count, _E ch)
	{
		std::replace(_WINX_TP_BASE, first, last, count, ch);
		return *this;
	}

#if !defined(X_CC_VC6) // bug: vc++ 6.0 can't compile this function!?
	_Myt& winx_call replace(
		iterator first, iterator last, const _E* s, size_type cch)
	{
		std::replace(_WINX_TP_BASE, first, last, s, s + cch);
		return *this;
	}
#endif

	_Myt& winx_call replace(iterator first, iterator last, const _String s)
	{
		std::replace(_WINX_TP_BASE, first, last, s.begin(), s.end());
		return *this;
	}

public:
	_Myt& winx_call insert(iterator it, const _Base& s)
	{
		_Base::insert(it, s.begin(), s.end());
		return *this;
	}

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
	//_WINX_RFIND_ALL(find_last_of, std::find, std::find_first_of)

	// 在字符串中查找不在集合中出现的第一个字符的位置（正向查找）。
	_WINX_FIND_ALL(find_first_not_of, std::find_not, std::find_first_not_of)

	// 在字符串中查找不在集合中出现的第一个字符的位置（反向查找）。
	//_WINX_RFIND_ALL(find_last_not_of, std::find_not, std::find_first_not_of)

public:
	// 比较两个字符串。
	
	int winx_call compare(const _Base& b) const
		{return std::compare(begin(), end(), b.begin(), b.end()); }

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
	int winx_call compare_by(const _Base& b, _Compr cmp) const
		{return std::compare_by(begin(), end(), b.begin(), b.end(), cmp); }

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

	int winx_call icompare(const _Base& b) const
		{return compare_by(b, _ComareNoCase()); }

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

// -------------------------------------------------------------------------

template <class _E, class _Alloc, class _T2> __forceinline
bool winx_call operator==(const TextPool<_E, _Alloc>& a, const _T2& b)
	{return a.compare(b) == 0; }

template <class _E, class _Alloc> __forceinline
bool winx_call operator==(const BasicString<_E>& a, const TextPool<_E, _Alloc>& b)
	{return b.compare(a) == 0; }

template <class _E, class _Alloc> __forceinline
bool winx_call operator==(const BasicString<_E>& a, TextPool<_E, _Alloc>& b)
	{return b.compare(a) == 0; }

template <class _E, class _Alloc, class _T2> __forceinline
bool winx_call operator!=(const TextPool<_E, _Alloc>& a, const _T2& b)
	{return a.compare(b) != 0; }

template <class _E, class _Alloc, class _T2> __forceinline
bool winx_call operator<(const TextPool<_E, _Alloc>& a, const _T2& b)
	{return a.compare(b) < 0; }

template <class _E, class _Alloc, class _T2> __forceinline
bool winx_call operator>(const TextPool<_E, _Alloc>& a, const _T2& b)
	{return a.compare(b) > 0; }

template <class _E, class _Alloc, class _T2> __forceinline
bool winx_call operator<=(const TextPool<_E, _Alloc>& a, const _T2& b)
	{return a.compare(b) <= 0; }

template <class _E, class _Alloc, class _T2> __forceinline
bool winx_call operator>=(const TextPool<_E, _Alloc>& a, const _T2& b)
	{return a.compare(b) >= 0; }

// -------------------------------------------------------------------------
// class TestTextPool

template <class LogT>
class TestTextPool : public TestCase
{
	WINX_TEST_SUITE(TestTextPool);
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

		std::TextPool<char> a1(alloc, 'a');
		AssertExp(a1.size() == 1 && a1[0] == 'a');

		std::TextPool<char> a2(alloc, 3, 'a');
		AssertExp(a2 == "aaa");

		std::TextPool<char> a(alloc, "Hello");
		AssertExp(a == "Hello");

		std::TextPool<char> b(alloc, "Hello", 4);
		AssertExp(b == "Hell");

		std::TextPool<char> c(alloc, b.begin(), b.end());
		std::String d = c.str(alloc);
		AssertExp(d == b && b == d);
	}

	void testSubstr(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::TextPool<char> a(alloc, "Hello");
		AssertExp(a.substr(alloc, 1, 3) == "ell");
		AssertExp(a.substr(alloc, 1) == "ello");
		AssertExp(a.substr(alloc, 5) == "");
	}

	void testAssign(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::TextPool<char> a(alloc);
		
		a.assign('a');
		AssertExp(a == "a");

		a.assign(3, 'a');
		AssertExp(a == "aaa");

		a.assign("Hello");
		AssertExp(a == "Hello");

		std::TextPool<char> b(alloc);
		b.assign("Hello", 2);
		AssertExp(b == "He");

		a.assign(b);
		AssertExp(a == "He");

		a.assign(a);
		AssertExp(a == "He");

		std::string t = "Hello, world!";
		a.assign(t);
		AssertExp(a == t);
	}

	void testAppend(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::TextPool<char> a(alloc, "Hello");
		a.append(3, '!');
		AssertExp(a == "Hello!!!");

		a.append('!');
		AssertExp(a == "Hello!!!!");

		a.append(' ').append(std::string("world"));
		AssertExp(a == "Hello!!!! world");
	
		a.append(std::TextPool<char>(alloc, '!'));
		AssertExp(a == "Hello!!!! world!");
		
		a.append(std::vector<char>(2, '!'));
		AssertExp(a == "Hello!!!! world!!!");
	
		a.append(std::String("!", 1));
		AssertExp(a == "Hello!!!! world!!!!");
	}

	void testReplace(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);

		std::TextPool<char> a(alloc, "Hello!!!! world! Good!");
		std::TextPool<char>::iterator it = a.find("!!!!");
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
// $Log: TextPool.h,v $

__NS_STD_END

#endif /* __STDEXT_TEXT_TEXTPOOL_H__ */
