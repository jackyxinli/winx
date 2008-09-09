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
#ifndef STDEXT_TEXT_BASICSTRING_H
#define STDEXT_TEXT_BASICSTRING_H

#ifndef STDEXT_TEXT_RANGE_H
#include "Range.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// WINX_FIND_OPS_

#define WINX_FIND_OPS_(It, findOp, findC, findS, Bgn, End, Const)			\
																			\
	It winx_call findOp(const CharT ch, It from) Const						\
		{return findC(from, End, ch); }										\
																			\
	It winx_call findOp(const CharT ch) Const								\
		{return findC(Bgn, End, ch); }										\
																			\
	It winx_call findOp(const String_& pattern, It from) Const				\
		{return findS(from, End, pattern.begin(), pattern.end()); }			\
																			\
	It winx_call findOp(const String_& pattern) Const						\
		{return findS(Bgn, End, pattern.begin(), pattern.end()); }			\
																			\
	It winx_call findOp(const CharT* pattern, size_type len, It from) Const	\
		{return findS(from, End, pattern, pattern + len); }					\
																			\
	It winx_call findOp(const CharT* pattern, size_type len) Const			\
		{return findS(Bgn, End, pattern, pattern + len); }

#define WINX_FIND_CONST_(findOp, findC, findS)								\
	WINX_FIND_OPS_(const_iterator, findOp, findC, findS, begin(), end(), const)

#define WINX_FIND_NONCONST_(findOp, findC, findS)							\
	WINX_FIND_OPS_(iterator, findOp, findC, findS, begin(), end(), WINX_NOTHING_)

#define WINX_RFIND_CONST_(findOp, findC, findS)								\
	WINX_FIND_OPS_(const_reverse_iterator, findOp, findC, findS, rbegin(), rend(), const)

#define WINX_RFIND_NONCONST_(findOp, findC, findS)							\
	WINX_FIND_OPS_(reverse_iterator, findOp, findC, findS, rbegin(), rend(), WINX_NOTHING_)

#define WINX_NOTHING_	/* nothing */

#define WINX_FIND_ALL_(findOp, findC, findS)								\
	WINX_FIND_CONST_(findOp, findC, findS)									\
	WINX_FIND_NONCONST_(findOp, findC, findS)	

#define WINX_RFIND_ALL_(findOp, findC, findS)								\
	WINX_RFIND_CONST_(findOp, findC, findS)									\
	WINX_RFIND_NONCONST_(findOp, findC, findS)	

#define WINX_BASICSTRING_USING_												\
public:																		\
	using Base::begin;														\
	using Base::end;														\
	using Base::rbegin;														\
	using Base::rend

// -------------------------------------------------------------------------
// class BasicString

#ifndef WINX_ALLOC_TYPE_
#define WINX_ALLOC_TEMPLATE_ARGS_		class PolicyT
#define WINX_ALLOC_TYPE_				RegionAllocT<PolicyT>
#endif

template <class CharT>
class BasicString : public Range<const CharT*>
{
private:
	typedef Range<const CharT*> Base;
	WINX_BASICSTRING_USING_;
	
	typedef TempString<CharT> String_;
	typedef BasicString Myt_;

public:
	typedef typename Base::size_type size_type;
	typedef typename Base::difference_type difference_type;
	typedef typename Base::value_type value_type;

	typedef typename Base::iterator iterator;
	typedef typename Base::const_iterator const_iterator;
	typedef typename Base::reverse_iterator reverse_iterator;
	typedef typename Base::const_reverse_iterator const_reverse_iterator;

private:
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
	BasicString(const CharT* pszVal, size_type cch)
		: Base(pszVal, pszVal + cch) {
	}
	BasicString(const CharT* first, const CharT* last)
		: Base(first, last) {
	}
	BasicString(const BasicArray<CharT>& s)
		: Base(s) {
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_>
	BasicString(WINX_ALLOC_TYPE_& alloc, const String_& s) {
		assign(alloc, s);
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_>
	BasicString(WINX_ALLOC_TYPE_& alloc, const value_type* pszVal, size_type cch) {
		assign(alloc, pszVal, cch);
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_>
	BasicString(WINX_ALLOC_TYPE_& alloc, size_type count, value_type ch) {
		assign(alloc, count, ch);
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_, class Iterator>
	BasicString(WINX_ALLOC_TYPE_& alloc, Iterator first, Iterator last) {
		assign(alloc, first, last);
	}

public:
	template <WINX_ALLOC_TEMPLATE_ARGS_>
	Myt_& winx_call assign(WINX_ALLOC_TYPE_& alloc, const String_& s) {
		const size_type cch = s.size();
		const value_type* psz = _makeBuf(alloc, s.begin(), cch);
		Base::assign(psz, psz+cch);
		return *this;
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_>
	Myt_& winx_call assign(WINX_ALLOC_TYPE_& alloc, const CharT* pszVal, size_type cch) {
		const value_type* psz = _makeBuf(alloc, pszVal, cch);
		Base::assign(psz, psz+cch);
		return *this;
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_>
	Myt_& winx_call assign(WINX_ALLOC_TYPE_& alloc, size_type count, value_type ch) {
		const value_type* psz = _makeBuf(alloc, count, ch);
		Base::assign(psz, psz+count);
		return *this;
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_, class Iterator>
	Myt_& winx_call assign(WINX_ALLOC_TYPE_& alloc, Iterator first, Iterator last) {
		const size_type cch = std::distance(first, last);
		const value_type* psz = _makeBuf(alloc, first, cch);
		Base::assign(psz, psz+cch);
		return *this;
	}

	void winx_call attach(const CharT* szVal) {
		Base::assign(szVal, std::end(szVal));
	}

	void winx_call attach(const CharT* szVal, size_type cch) {
		Base::assign(szVal, szVal+cch);
	}

	const Myt_& winx_call operator=(const BasicArray<CharT>& s) {
		Base::operator=(s);
		return *this;
	}

public:
	const CharT* winx_call data() const {
		return Base::first;
	}

	const CharT& winx_call at(size_type i) const {
		if (i <= Base::size())
			throw_out_of_range_();
		return Base::first[i];
	}

	const CharT& winx_call operator[](size_type i) const {
		return Base::first[i];
	}

	operator const TempString<CharT>&() const {
		return *(const TempString<CharT>*)this;
	}

	operator const BasicArray<CharT>&() const {
		return *(const BasicArray<CharT>*)this;
	}

	template <class _Tr, class _Alloc2>
	static Myt_ winx_call cast(const basic_string<CharT, _Tr, _Alloc2>& src) {
		return Myt_(src.data(), src.length());
	}

	template <class _Alloc2>
	static Myt_ winx_call cast(const vector<CharT, _Alloc2>& builder) {
		return Myt_(_ConvIt(builder.begin()), builder.size());
	}
	
	Myt_ winx_call substr(size_type from) const
	{
		const size_type cch = Base::size() - from;
		if ((difference_type)cch < 0)
			throw_out_of_range_();
		return Myt_(Base::first + from, cch);
	}

	Myt_ winx_call substr(size_type from, size_type cch) const
	{
		const size_type cchMax = Base::size() - from;
		if ((difference_type)cchMax < 0)
			throw_out_of_range_();
		return Myt_(Base::first + from, cchMax < cch ? cchMax : cch);
	}

public:
	WINX_FIND_CONST_(find, std::find, std::search)
	WINX_FIND_CONST_(rfind, std::rfind, std::find_end)

	WINX_FIND_CONST_(find_first_of, std::find, std::find_first_of)
	WINX_RFIND_CONST_(find_last_of, std::find, std::find_first_of)

	WINX_FIND_CONST_(find_first_not_of, std::find_not, std::find_first_not_of)
	WINX_RFIND_CONST_(find_last_not_of, std::find_not, std::find_first_not_of)

public:
	bool winx_call contains(const String_& b) const {
		return find(b) != end();
	}

	bool winx_call contains(const CharT* b, size_type blen) const {
		return find(b, blen) != end();
	}
};

typedef BasicString<char> String;
typedef BasicString<wchar_t> WString;

#if defined(UNICODE) || defined(_UNICODE)
typedef WString TString;
#else
typedef String TString;
#endif

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
// function g_str

template <class CharT>
inline BasicString<CharT> winx_call g_str(const CharT* s)
{
	return BasicString<CharT>(s, end(s));
}

// -------------------------------------------------------------------------

NS_STDEXT_END

STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<char>);
STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<wchar_t>);
STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<int>);
STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<long>);
STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<short>);
STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<unsigned int>);
STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<unsigned long>);

// -------------------------------------------------------------------------
// class TestBasicString

#if defined(STD_UNITTEST)

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
		std::ScopedAlloc alloc(recycle);

		const std::String a1("Hello", 4);
		AssertExp(a1.compare("Hell") == 0);

		std::String a2 = a1.substr(3);
		AssertExp(a2.compare("l") == 0);

		std::String a3 = a1.substr(1, 2);
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

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: BasicString.h,v $

#endif /* STDEXT_TEXT_BASICSTRING_H */

