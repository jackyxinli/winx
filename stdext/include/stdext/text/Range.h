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
// Module: stdext/text/Range.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Range.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_RANGE_H
#define STDEXT_TEXT_RANGE_H

#ifndef STDEXT_TEXT_BASICALGO_H
#include "BasicAlgo.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "../Memory.h"
#endif

#ifndef STD_VECTOR_H
#include "../../std/vector.h"
#endif

#ifndef STD_STRING_H
#include "../../std/string.h"
#endif

#ifndef STD_ITERATOR_H
#include "../../std/iterator.h"
#endif

NS_STDEXT_BEGIN

// =========================================================================
// class Range

#if defined(X_CC_VC6)
#define WINX_RG_TYPENAME_
#else
#define WINX_RG_TYPENAME_	typename
#endif

template <
	class Iterator,
	class ValueT = WINX_RG_TYPENAME_ std::iterator_traits_alter<Iterator>::value_type>
class Range // std::pair<Iterator>
{
protected:
	Iterator first;
	Iterator second;

public:
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;

	typedef ValueT value_type;
	
	typedef const value_type* pointer;
	typedef const value_type* const_pointer;
	
	typedef const value_type& reference;
	typedef const value_type& const_reference;

	typedef Iterator iterator;
	typedef Iterator const_iterator;

#if defined(WINX_NO_PARTIAL_SPECIALIZATION)
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
	typedef std::basic_string<value_type> StlString_;

public:
	Range()
		: first(), second() {}

	Range(Iterator first_, Iterator second_)
		: first(first_), second(second_) {}

	template <class ContainerT>
	explicit Range(const ContainerT& cont)
		: first(cont.begin()), second(cont.end()) {
	}

	template <class ContainerT>
	explicit Range(ContainerT& cont)
		: first(cont.begin()), second(cont.end()) {
	}

public:
	void winx_call assign(Iterator first_, Iterator second_) {
		first = first_;
		second = second_;
	}

	template <class ContainerT>
	void winx_call assign(const ContainerT& cont) {
		first = cont.begin();
		second = cont.end();
	}
	
	template <class ContainerT>
	void winx_call assign(ContainerT& cont) {
		first = cont.begin();
		second = cont.end();
	}

	void winx_call clear() {
		first = second = Iterator();
	}

	void winx_call erase() {
		first = second = Iterator();
	}

	void winx_call swap(Range& b) {
		std::swap(first, b.first);
		std::swap(second, b.second);
	}

	bool winx_call empty() const {
		return first == second;
	}

	size_type winx_call length() const {
		return std::distance(first, second);
	}

	size_type winx_call size() const {
		return std::distance(first, second);
	}

	iterator winx_call begin() const {
		return first;
	}

	iterator winx_call end() const {
		return second;
	}

	reverse_iterator winx_call rbegin() const {
		return reverse_iterator(second);
	}

	reverse_iterator winx_call rend() const {
		return reverse_iterator(first);
	}

	StlString_ winx_call stl_str() const {
		return StlString_(first, second);
	}

	template <class LogT>
	void winx_call trace(LogT& log) const {
		log.printString(first, second);
	}

public:
	template <class ContainerT>
	int winx_call compare(const ContainerT& b) const {
		return std::compare(first, second, b.begin(), b.end());
	}

	int winx_call compare(const value_type* b) const {
		return std::compare(first, second, b);
	}

	int winx_call compare(const value_type* b, size_type blen) const {
		return std::compare(first, second, b, b + blen);
	}

public:
	template <class ContainerT, class ComprT>
	int winx_call compare_by(const ContainerT& b, ComprT cmp) const {
		return std::compare_by(begin(), end(), b.begin(), b.end(), cmp);
	}

	template <class ComprT>
	int winx_call compare_by(const value_type* b, ComprT cmp) const {
		return std::compare_by(begin(), end(), b, cmp);
	}

	template <class ComprT>
	int winx_call compare_by(const value_type* b, size_type blen, ComprT cmp) const {
		return std::compare_by(begin(), end(), b, b + blen, cmp);
	}

private:
	typedef CompareNoCase<value_type> ComareNoCase_;

public:
	template <class ContainerT>
	int winx_call icompare(const ContainerT& b) const {
		return compare_by(b, ComareNoCase_());
	}

	int winx_call icompare(const value_type* b, size_type blen) const {
		return compare_by(b, blen, ComareNoCase_());
	}

	int winx_call icompare(const value_type* b) const {
		return compare_by(b, ComareNoCase_());
	}
};

// -------------------------------------------------------------------------

template <class Iterator, class T2> __forceinline
	bool winx_call operator==(const Range<Iterator>& a, const T2& b)
    {return (a.compare(b) == 0); }

template <class Iterator> __forceinline
    bool winx_call operator==(const typename Range<Iterator>::value_type* a, const Range<Iterator>& b)
    {return (b.compare(a) == 0); }

template <class Iterator, class CharT, class Tr, class AllocT> __forceinline
    bool winx_call operator==(const std::basic_string<CharT, Tr, AllocT>& a, const Range<Iterator>& b)
    {return (b.compare(a) == 0); }

template <class Iterator, class CharT, class AllocT> __forceinline
    bool winx_call operator==(const std::vector<CharT, AllocT>& a, const Range<Iterator>& b)
    {return (b.compare(a) == 0); }

// -------------------------------------------------------------------------

template <class Iterator, class T2> __forceinline
    bool winx_call operator!=(const Range<Iterator>& a, const T2& b)
    {return (a.compare(b) != 0); }

template <class Iterator> __forceinline
    bool winx_call operator!=(const typename Range<Iterator>::value_type* a, const Range<Iterator>& b)
    {return (b.compare(a) != 0); }

template <class Iterator, class CharT, class Tr, class AllocT> __forceinline
    bool winx_call operator!=(const std::basic_string<CharT, Tr, AllocT>& a, const Range<Iterator>& b)
    {return (b.compare(a) != 0); }

template <class Iterator, class CharT, class AllocT> __forceinline
    bool winx_call operator!=(const std::vector<CharT, AllocT>& a, const Range<Iterator>& b)
    {return (b.compare(a) != 0); }

// -------------------------------------------------------------------------

template <class Iterator, class T2> __forceinline
    bool winx_call operator<(const Range<Iterator>& a, const T2& b)
    {return (a.compare(b) < 0); }

template <class Iterator> __forceinline
    bool winx_call operator<(const typename Range<Iterator>::value_type* a, const Range<Iterator>& b)
    {return (b.compare(a) > 0); }

template <class Iterator, class CharT, class Tr, class AllocT> __forceinline
    bool winx_call operator<(const std::basic_string<CharT, Tr, AllocT>& a, const Range<Iterator>& b)
    {return (b.compare(a) > 0); }

template <class Iterator, class CharT, class AllocT> __forceinline
    bool winx_call operator<(const std::vector<CharT, AllocT>& a, const Range<Iterator>& b)
    {return (b.compare(a) > 0); }

// -------------------------------------------------------------------------

template <class Iterator, class T2> __forceinline
    bool winx_call operator>(const Range<Iterator>& a, const T2& b)
    {return (b < a); }

template <class Iterator> __forceinline
    bool winx_call operator>(const typename Range<Iterator>::value_type* a, const Range<Iterator>& b)
    {return (b < a); }

template <class Iterator, class CharT, class Tr, class AllocT> __forceinline
    bool winx_call operator>(const std::basic_string<CharT, Tr, AllocT>& a, const Range<Iterator>& b)
    {return (b < a); }

template <class Iterator, class CharT, class AllocT> __forceinline
    bool winx_call operator>(const std::vector<CharT, AllocT>& a, const Range<Iterator>& b)
    {return (b < a); }

// -------------------------------------------------------------------------

template <class Iterator, class T2> __forceinline
	bool winx_call operator<=(const Range<Iterator>& a, const T2& b)
	{return (!(b < a)); }

template <class Iterator> __forceinline
	bool winx_call operator<=(const typename Range<Iterator>::value_type* a, const Range<Iterator>& b)
	{return (!(b < a)); }

template <class Iterator, class CharT, class Tr, class AllocT> __forceinline
	bool winx_call operator<=(const std::basic_string<CharT, Tr, AllocT>& a, const Range<Iterator>& b)
	{return (!(b < a)); }

template <class Iterator, class CharT, class AllocT> __forceinline
	bool winx_call operator<=(const std::vector<CharT, AllocT>& a, const Range<Iterator>& b)
	{return (!(b < a)); }

// -------------------------------------------------------------------------

template <class Iterator, class T2> __forceinline
	bool winx_call operator>=(const Range<Iterator>& a, const T2& b)
	{return (!(a < b)); }

template <class Iterator> __forceinline
	bool winx_call operator>=(const typename Range<Iterator>::value_type* a, const Range<Iterator>& b)
	{return (!(a < b)); }

template <class Iterator, class CharT, class Tr, class AllocT> __forceinline
	bool winx_call operator>=(const std::basic_string<CharT, Tr, AllocT>& a, const Range<Iterator>& b)
	{return (!(a < b)); }

template <class Iterator, class CharT, class AllocT> __forceinline
	bool winx_call operator>=(const std::vector<CharT, AllocT>& a, const Range<Iterator>& b)
	{return (!(a < b)); }

// -------------------------------------------------------------------------

#if defined(WINX_HAS_OSTREAM)

template <class CharT, class Tr, class Iterator>
inline std::basic_ostream<CharT, Tr>& 
winx_call operator<<(std::basic_ostream<CharT, Tr>& os, const Range<Iterator>& v) {
	typedef typename Range<Iterator>::value_type ValueT;
	std::copy(v.begin(), v.end(), std::ostream_iterator<ValueT, CharT, Tr>(os));
	return os;
}

#endif

// =========================================================================
// class TempString

inline void winx_call throw_out_of_range_()
{
	throw std::out_of_range("invalid string position");
}

template <class CharT>
class TempString : public Range<const CharT*, CharT>
{
private:
	typedef Range<const CharT*, CharT> Base;

public:	
	typedef typename Base::size_type size_type;

public:
	TempString() {}
	TempString(const CharT& ch)
		: Base(&ch, &ch + 1) {
	}

	TempString(const CharT* szVal)
		: Base(szVal, std::end(szVal)) {
	}

	TempString(const CharT* szVal, size_t len)
		: Base(szVal, szVal+len) {
	}
	
	template <class Tr, class AllocT>
	TempString(const std::basic_string<CharT, Tr, AllocT>& s)
		: Base(_ConvIt(s.begin()), _ConvIt(s.end())) {
	}

	template <class AllocT>
	TempString(const std::vector<CharT, AllocT>& s)
		: Base(_ConvIt(s.begin()), _ConvIt(s.end())) {
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
};

// =========================================================================
// class BasicArray

template <class Type>
class BasicArray : public Range<const Type*, Type>
{
private:
	typedef const Type* Iterator;
	typedef Range<Iterator, Type> Base;
	
public:	
	typedef typename Base::size_type size_type;

public:
	BasicArray() {}
	BasicArray(Iterator arr, size_t n)
		: Base(arr, arr+n) {}
	BasicArray(Iterator first_, Iterator second_)
		: Base(first_, second_) {}

public:
	const Type* winx_call data() const {
		return Base::first;
	}

	const Type& winx_call at(size_type i) const {
		if (i <= Base::size())
			throw_out_of_range_();
		return Base::first[i];
	}

	const Type& winx_call operator[](size_type i) const {
		return Base::first[i];
	}
};

// =========================================================================
// rangeof

template <class Type>
inline Range<const Type*, Type> winx_call rangeof_(const Type v[], size_t n)
{
	return Range<const Type*, Type>(v, v+n);
}

#define rangeof(array)		NS_STDEXT::rangeof_(array, countof(array))

// =========================================================================
// $Log: $

NS_STDEXT_END

STD_NO_DESTRUCTOR(std::Range<const char*>);
STD_NO_DESTRUCTOR(std::Range<const wchar_t*>);
STD_NO_DESTRUCTOR(std::Range<const int*>);
STD_NO_DESTRUCTOR(std::Range<const long*>);
STD_NO_DESTRUCTOR(std::Range<const short*>);
STD_NO_DESTRUCTOR(std::Range<const unsigned int*>);
STD_NO_DESTRUCTOR(std::Range<const unsigned long*>);

STD_NO_DESTRUCTOR(std::TempString<char>);
STD_NO_DESTRUCTOR(std::TempString<wchar_t>);
STD_NO_DESTRUCTOR(std::TempString<int>);
STD_NO_DESTRUCTOR(std::TempString<long>);
STD_NO_DESTRUCTOR(std::TempString<short>);
STD_NO_DESTRUCTOR(std::TempString<unsigned int>);
STD_NO_DESTRUCTOR(std::TempString<unsigned long>);

STD_NO_DESTRUCTOR(std::BasicArray<char>);
STD_NO_DESTRUCTOR(std::BasicArray<wchar_t>);
STD_NO_DESTRUCTOR(std::BasicArray<int>);
STD_NO_DESTRUCTOR(std::BasicArray<long>);
STD_NO_DESTRUCTOR(std::BasicArray<short>);
STD_NO_DESTRUCTOR(std::BasicArray<unsigned int>);
STD_NO_DESTRUCTOR(std::BasicArray<unsigned long>);

#endif /* STDEXT_TEXT_RANGE_H */

