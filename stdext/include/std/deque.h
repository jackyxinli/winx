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
// Module: std/deque.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: deque.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STD_DEQUE_H
#define STD_DEQUE_H

#ifndef STDEXT_PLATFORM_H
#include "../stdext/Platform.h"
#endif

// -------------------------------------------------------------------------

#if defined(X_STL_SGI)
	#ifndef __SGI_STL_DEQUE_H
	#include "../../../stl/deque.h"
	#endif
#else
	#if !defined(_DEQUE_) && !defined(_GLIBCXX_DEQUE) && !defined(_DEQUE)
	#include <deque>
	#endif
#endif

#ifndef STD_ALGORITHM_H
#include "algorithm.h"
#endif

// -------------------------------------------------------------------------

#if defined(X_STL_VC6)

#define _WINX_DEQUE_USING_BASE												\
public:																		\
	using _Base::begin;														\
	using _Base::end;														\
	using _Base::assign;													\
	using _Base::insert;													\
	using _Base::resize

namespace std {

template<class _Ty, class _A = allocator<_Ty> >
class _Deque : public std::deque<_Ty, _A>
{
private:
	typedef std::deque<_Ty, _A> _Base;
	_WINX_DEQUE_USING_BASE;

public:
	typedef typename _Base::iterator iterator;

public:
	explicit _Deque(const _A& _Al = _A())
		: _Base(_Al) {}
	
	explicit _Deque(size_type _N, const _Ty& _V = _Ty(), const _A& _Al = _A())
		: _Base(_N, _V, _Al) {}

	template <class _Iterator>
	_Deque(_Iterator _F, _Iterator _L, const _A& _Al = _A())
		: _Base(_Al)
	{
		std::copy(_F, _L, std::back_insert_iterator<_Base>(*this));
	}

public:
	template <class _Iterator>
	void assign(_Iterator first, _Iterator last)
	{
		_Base::resize(std::distance(first, last));
		std::copy(first, last, begin());
	}

	template <class _Iterator>
	void insert(iterator it, _Iterator first, _Iterator last)
	{
		std::copy(first, last, std::insert_iterator<_Base>(*this, it));
	}
};

}; // namespace std

#else

#define _Deque	deque

#endif

// -------------------------------------------------------------------------
// $Log: deque.h,v $

#endif /* STD_DEQUE_H */
