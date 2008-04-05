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

#ifndef __STDEXT_TEXT_TEMPSTRING_H__
#include "TempString.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class CompareNoCase

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
// _WINX_FIND_OPS

#define _WINX_FIND_OPS(It, findOp, findC, findS, Bgn, End, Const)			\
																			\
	It winx_call findOp(const _E ch, It from) Const							\
		{return findC(from, End, ch); }										\
																			\
	It winx_call findOp(const _E ch) Const									\
		{return findC(Bgn, End, ch); }										\
																			\
	It winx_call findOp(const _String pattern, It from) Const				\
		{return findS(from, End, pattern.begin(), pattern.end()); }			\
																			\
	It winx_call findOp(const _String pattern) Const						\
		{return findS(Bgn, End, pattern.begin(), pattern.end()); }			\
																			\
	It winx_call findOp(const _E* pattern, size_type len, It from) Const	\
		{return findS(from, End, pattern, pattern + len); }					\
																			\
	It winx_call findOp(const _E* pattern, size_type len) Const				\
		{return findS(Bgn, End, pattern, pattern + len); }

#define _WINX_FIND_CONST(findOp, findC, findS)								\
	_WINX_FIND_OPS(const_iterator, findOp, findC, findS, begin(), end(), const)

#define _WINX_FIND_NONCONST(findOp, findC, findS)							\
	_WINX_FIND_OPS(iterator, findOp, findC, findS, begin(), end(), _WINX_NOTHING)

#define _WINX_RFIND_CONST(findOp, findC, findS)								\
	_WINX_FIND_OPS(const_reverse_iterator, findOp, findC, findS, rbegin(), rend(), const)

#define _WINX_RFIND_NONCONST(findOp, findC, findS)							\
	_WINX_FIND_OPS(reverse_iterator, findOp, findC, findS, rbegin(), rend(), _WINX_NOTHING)

#define _WINX_NOTHING	/* nothing */

#define _WINX_FIND_ALL(findOp, findC, findS)								\
	_WINX_FIND_CONST(findOp, findC, findS)									\
	_WINX_FIND_NONCONST(findOp, findC, findS)	

#define _WINX_RFIND_ALL(findOp, findC, findS)								\
	_WINX_RFIND_CONST(findOp, findC, findS)									\
	_WINX_RFIND_NONCONST(findOp, findC, findS)	

#define _WINX_BASICSTRING_USING												\
public:																		\
	using _Base::begin;														\
	using _Base::end;														\
	using _Base::rbegin;													\
	using _Base::rend

// -------------------------------------------------------------------------
// class BasicString

#ifndef _WINX_ALLOC_TYPE
#define _WINX_ALLOC_TEMPLATE_ARGS		class _Policy
#define _WINX_ALLOC_TYPE				std::AutoFreeAllocT<_Policy>
#endif

template <class _E>
class BasicString : public TempString<_E>
{
private:
	typedef TempString<_E> _Base;
	typedef TempString<_E> _String;
	typedef BasicString _Myt;
	_WINX_BASICSTRING_USING;
	
protected:
	using _Base::m_pszBuf;
	using _Base::m_length;

public:
	typedef typename _Base::size_type size_type;
	typedef typename _Base::difference_type difference_type;
	typedef typename _Base::value_type value_type;

	typedef typename _Base::iterator iterator;
	typedef typename _Base::const_iterator const_iterator;
	typedef typename _Base::reverse_iterator reverse_iterator;
	typedef typename _Base::const_reverse_iterator const_reverse_iterator;

private:
	template <class AllocT>
	static const value_type* _makeBuf(AllocT& alloc, const _String& s)
	{
		value_type* psz = (value_type*)alloc.allocate(s.size() * sizeof(value_type));
		std::copy(s.begin(), s.end(), psz);
		return psz;
	}

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
	BasicString() {}
	BasicString(const _E* pszVal, size_type cch) : _Base(pszVal, cch) {}
	BasicString(const _Myt& src, size_type from, size_type cch = (size_type)-1) {
		assign(src, from, cch);
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	BasicString(_WINX_ALLOC_TYPE& alloc, const _String s) {
		assign(alloc, s);
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	BasicString(_WINX_ALLOC_TYPE& alloc, const value_type* pszVal, size_type cch) {
		assign(alloc, pszVal, cch);
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	BasicString(_WINX_ALLOC_TYPE& alloc, size_type count, value_type ch) {
		assign(alloc, count, ch);
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS, class Iterator>
	BasicString(_WINX_ALLOC_TYPE& alloc, Iterator first, Iterator last) {
		assign(alloc, first, last);
	}

public:
	_Myt& winx_call assign(const _Myt& src, size_type from, size_type cch = (size_type)-1)
	{
		size_type cchMax = src.size() - from;
		if ((difference_type)cchMax < 0)
			__Xran();
		m_length = (cchMax < cch ? cchMax : cch);
		m_pszBuf = src.m_pszBuf + from;
		return *this;
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	_Myt& winx_call assign(_WINX_ALLOC_TYPE& alloc, const _String s)
	{
		m_length = s.size();
		m_pszBuf = _makeBuf(alloc, s);
		return *this;
	}

	template <_WINX_ALLOC_TEMPLATE_ARGS>
	_Myt& winx_call assign(_WINX_ALLOC_TYPE& alloc, const _E* pszVal, size_type cch)
	{
		m_length = cch;
		m_pszBuf = _makeBuf(alloc, pszVal, cch);
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
	// 在字符串中查找子串（正向查找）。
	_WINX_FIND_CONST(find, std::find, std::search)

	// 在字符串中查找子串（反向查找）。
	_WINX_FIND_CONST(rfind, std::rfind, std::find_end)

	// 查找某个集合中的字符在字符串中第一次出现的位置（正向查找）。
	_WINX_FIND_CONST(find_first_of, std::find, std::find_first_of)

	// 查找某个集合中的字符在字符串中第一次出现的位置（反向查找）。
	_WINX_RFIND_CONST(find_last_of, std::find, std::find_first_of)

	// 在字符串中查找不在集合中出现的第一个字符的位置（正向查找）。
	_WINX_FIND_CONST(find_first_not_of, std::find_not, std::find_first_not_of)

	// 在字符串中查找不在集合中出现的第一个字符的位置（反向查找）。
	_WINX_RFIND_CONST(find_last_not_of, std::find_not, std::find_first_not_of)

public:
	// 比较两个字符串。
	
	int winx_call compare(const _String& b) const
		{return std::compare(begin(), end(), b.begin(), b.end()); }

	int winx_call compare(const _E* b, size_type blen) const
		{return std::compare(begin(), end(), b, b + blen); }

	int winx_call compare(const _E* b) const
		{return std::compare(begin(), end(), b); }

	int winx_call compare(size_type from, size_type count, const _String b) const
		{return substr(from, count).compare(b); }

	int winx_call compare(size_type from, size_type count, const _E* b, size_type blen) const
		{return substr(from, count).compare(b, blen); }

	int winx_call compare(size_type from, size_type count, const _E* b) const
		{return substr(from, count).compare(b); }

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

typedef BasicString<char> String;
typedef BasicString<WCHAR> WString;

#if defined(UNICODE) || defined(_UNICODE)
typedef WString TString;
#else
typedef String TString;
#endif

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

template <class _E, class _Alloc> __forceinline
    bool winx_call operator==(const vector<_E, _Alloc>& a, const BasicString<_E>& b)
    {return (b.compare(a) == 0); }

// -------------------------------------------------------------------------

template <class _E, class _T2> __forceinline
    bool winx_call operator!=(const BasicString<_E>& a, const _T2& b)
    {return (a.compare(b) != 0); }

template <class _E> __forceinline
    bool winx_call operator!=(const _E* a, const BasicString<_E>& b)
    {return (b.compare(a) != 0); }

template <class _E, class _Tr, class _Alloc> __forceinline
    bool winx_call operator!=(const basic_string<_E, _Tr, _Alloc>& a, const BasicString<_E>& b)
    {return (b.compare(a) != 0); }

template <class _E, class _Alloc> __forceinline
    bool winx_call operator!=(const vector<_E, _Alloc>& a, const BasicString<_E>& b)
    {return (b.compare(a) != 0); }

// -------------------------------------------------------------------------

template <class _E, class _T2> __forceinline
    bool winx_call operator<(const BasicString<_E>& a, const _T2& b)
    {return (a.compare(b) < 0); }

template <class _E> __forceinline
    bool winx_call operator<(const _E* a, const BasicString<_E>& b)
    {return (b.compare(a) > 0); }

template <class _E, class _Tr, class _Alloc> __forceinline
    bool winx_call operator<(const basic_string<_E, _Tr, _Alloc>& a, const BasicString<_E>& b)
    {return (b.compare(a) > 0); }

template <class _E, class _Alloc> __forceinline
    bool winx_call operator<(const vector<_E, _Alloc>& a, const BasicString<_E>& b)
    {return (b.compare(a) > 0); }

// -------------------------------------------------------------------------

template <class _E, class _T2> __forceinline
    bool winx_call operator>(const BasicString<_E>& a, const _T2& b)
    {return (b < a); }

template <class _E> __forceinline
    bool winx_call operator>(const _E* a, const BasicString<_E>& b)
    {return (b < a); }

template <class _E, class _Tr, class _Alloc> __forceinline
    bool winx_call operator>(const basic_string<_E, _Tr, _Alloc>& a, const BasicString<_E>& b)
    {return (b < a); }

template <class _E, class _Alloc> __forceinline
    bool winx_call operator>(const vector<_E, _Alloc>& a, const BasicString<_E>& b)
    {return (b < a); }

// -------------------------------------------------------------------------

template <class _E, class _T2> __forceinline
	bool winx_call operator<=(const BasicString<_E>& a, const _T2& b)
	{return (!(b < a)); }

template <class _E> __forceinline
	bool winx_call operator<=(const _E* a, const BasicString<_E>& b)
	{return (!(b < a)); }

template <class _E, class _Tr, class _Alloc> __forceinline
	bool winx_call operator<=(const basic_string<_E, _Tr, _Alloc>& a, const BasicString<_E>& b)
	{return (!(b < a)); }

template <class _E, class _Alloc> __forceinline
	bool winx_call operator<=(const vector<_E, _Alloc>& a, const BasicString<_E>& b)
	{return (!(b < a)); }

// -------------------------------------------------------------------------

template <class _E, class _T2> __forceinline
	bool winx_call operator>=(const BasicString<_E>& a, const _T2& b)
	{return (!(a < b)); }

template <class _E> __forceinline
	bool winx_call operator>=(const _E* a, const BasicString<_E>& b)
	{return (!(a < b)); }

template <class _E, class _Tr, class _Alloc> __forceinline
	bool winx_call operator>=(const basic_string<_E, _Tr, _Alloc>& a, const BasicString<_E>& b)
	{return (!(a < b)); }

template <class _E, class _Alloc> __forceinline
	bool winx_call operator>=(const vector<_E, _Alloc>& a, const BasicString<_E>& b)
	{return (!(a < b)); }

// -------------------------------------------------------------------------
// class OutputBasicString

template <class DataT, class AllocT>
class OutputBasicString
{
private:
	AllocT& m_alloc;
	BasicString<DataT>& m_str;

public:
	typedef DataT* pointer;
	typedef DataT* iterator;
	typedef DataT value_type;

public:
	OutputBasicString(AllocT& alloc, BasicString<DataT>& s)
		: m_alloc(alloc), m_str(s) {
	}

	void winx_call resize(size_t newSize) {
		m_str.attach(STD_NEW_ARRAY(m_alloc, DataT, newSize), newSize);
	}

	iterator winx_call begin() {
		return (iterator)m_str.data();
	}

	iterator winx_call end() {
		return (iterator)m_str.end();
	}

	iterator winx_call erase(iterator it) {
		std::copy(it + 1, end(), it);
		m_str.attach(m_str.data(), m_str.size() - 1);
		return it;
	}
};

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
	void testFind(LogT& log)
	{
		const std::String a1("Hello", 5);

		std::String::iterator it = a1.find("ell");
		AssertExp(it != a1.end());
		AssertExp(*it == 'e');

		it = a1.find('e');
		AssertExp(it != a1.end());
		AssertExp(*it == 'e');

		std::vector<char> pattern(2, 'l');
		it = a1.find(pattern);
		AssertExp(it != a1.end());
		AssertExp(*it == 'l');
		AssertExp(*(it+1) == 'l');
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
