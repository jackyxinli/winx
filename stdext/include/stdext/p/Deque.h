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
// Module: stdext/p/PDeque.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-9-6 10:08:57
// 
// $Id: PDeque.h 2009-9-6 10:08:57 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_P_DEQUE_H
#define STDEXT_P_DEQUE_H

#ifndef STDEXT_MEMORY_H
#include "../Memory.h"
#endif

// -------------------------------------------------------------------------
// class StlDeque

#if defined(X_CC_VC6)

#ifndef _DEQUE_
#include <deque>
#endif

#ifndef AllocTLGORITHM_
#include <algorithm>
#endif

#define WINX_DEQUE_USING_BASE_												\
public:																		\
	using Base::begin;														\
	using Base::end;														\
	using Base::assign;														\
	using Base::insert;														\
	using Base::resize

template<class DataT, class AllocT = allocator<DataT> >
class StlDeque : public std::deque<DataT, AllocT>
{
private:
	typedef std::deque<DataT, AllocT> Base;
	WINX_DEQUE_USING_BASE_;

public:
	typedef typename Base::iterator iterator;

	explicit StlDeque(const AllocT& alloc = AllocT())
		: Base(alloc) {}
	
	explicit StlDeque(size_type count, const DataT& val = DataT(), const AllocT& alloc = AllocT())
		: Base(count, val, alloc) {}

	template <class Iterator>
	StlDeque(Iterator first, Iterator last, const AllocT& alloc = AllocT())
		: Base(alloc)
	{
		std::copy(first, last, std::back_insert_iterator<Base>(*this));
	}

public:
	template <class Iterator>
	void assign(Iterator first, Iterator last)
	{
		Base::resize(std::distance(first, last));
		std::copy(first, last, begin());
	}

	template <class Iterator>
	void insert(iterator it, Iterator first, Iterator last)
	{
		std::copy(first, last, std::insert_iterator<Base>(*this, it));
	}
};

#else

#if !defined(_DEQUE_) && !defined(_GLIBCXX_DEQUE) && !defined(_DEQUE)
#include <deque>
#endif

#define StlDeque	std::deque

#endif

// -------------------------------------------------------------------------
// class PDeque

NS_STDEXT_BEGIN

template <class DataT, class AllocT = ScopedPools>
class PDeque : public StlDeque<DataT, StlAlloc<DataT, AllocT> >
{
private:
	typedef StlDeque<DataT, StlAlloc<DataT, AllocT> > Base;

	PDeque(const PDeque&);
	void operator=(const PDeque&);

public:
	typedef typename Base::size_type size_type;

	explicit PDeque(AllocT& a)
		: Base(a) {}
	
	PDeque(AllocT& a, size_type n, const DataT& v = DataT())
		: Base(n, v, a) {}

	template <class Iterator>
	PDeque(AllocT& a, Iterator first, Iterator last)
		: Base(first, last, a) {}

	void winx_call copy(const Base& from) {
		Base::operator=(from);
	}
};

NS_STDEXT_END

// -------------------------------------------------------------------------

#endif /* STDEXT_P_DEQUE_H */

