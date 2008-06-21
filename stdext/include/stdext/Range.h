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
// Module: stdext/Range.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Range.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_RANGE_H
#define STDEXT_RANGE_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STD_ITERATOR_H
#include "../std/iterator.h"
#endif

NS_STD_BEGIN

// =========================================================================
// class Range

template <class Iterator>
class Range // std::pair<Iterator>
{
public:
	Iterator first;
	Iterator second;

private:
	typedef std::iterator_traits_alter<Iterator> Tr_;

public:
	typedef Iterator iterator;
	typedef Iterator const_iterator;

	typedef ptrdiff_t difference_type;
	typedef size_t size_type;

	typedef typename Tr_::value_type value_type;
	typedef typename Tr_::pointer pointer;
	typedef typename Tr_::reference reference;

private:
	typedef std::basic_string<value_type> StlString_;

public:
	Range()
		: first(), second() {}

	Range(Iterator first_, Iterator second_)
		: first(first_), second(second_) {}

	template <class ContainerT>
	Range(const ContainerT& cont)
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

	StlString_ winx_call stl_str() const {
		return StlString_(first, second);
	}
};

// -------------------------------------------------------------------------
// class BasicArray

template <class Type>
class BasicArray : public Range<const Type*>
{
private:
	typedef Range<const Type*> Base;
	
public:
	BasicArray() {}
	BasicArray(const Type* arr, size_t n)
		: Base(arr, arr+n) {}
	BasicArray(const Type* first_, const Type* second_)
		: Base(first_, second_) {}

	template <class ContainerT>
	explicit BasicArray(ContainerT& cont)
		: Base(cont) {}
};

// =========================================================================
// $Log: $

NS_STD_END

#endif /* STDEXT_RANGE_H */

