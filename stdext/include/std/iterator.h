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
// Module: std/iterator.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: iterator.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STD_ITERATOR_H
#define STD_ITERATOR_H

#ifndef STDEXT_PLATFORM_H
#include "../stdext/Platform.h"
#endif

// -------------------------------------------------------------------------
// iterator.h

#if defined(X_STL_SGI)
	#ifndef __SGI_STL_ITERATOR
	#include "../../../stl/iterator"
	#endif
#else
	#if !defined(_ITERATOR_) && !defined(_ITERATOR)
	#include <iterator>
	#endif
#endif

// -------------------------------------------------------------------------
// iterator_traits

#if defined(X_CC_VC6)

namespace std {

	template <class Iterator>
	struct iterator_traits_alter {
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type        value_type;
		typedef typename Iterator::difference_type   difference_type;
		typedef typename Iterator::pointer           pointer;
		typedef typename Iterator::reference         reference;
	};

	template <>
	struct iterator_traits_alter<char*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef char						value_type;
		typedef ptrdiff_t					difference_type;
		typedef char*						pointer;
		typedef char&						reference;
	};

	template <>
	struct iterator_traits_alter<const char*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef char						value_type;
		typedef ptrdiff_t					difference_type;
		typedef const char*					pointer;
		typedef const char&					reference;
	};

	template <>
	struct iterator_traits_alter<wchar_t*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef wchar_t						value_type;
		typedef ptrdiff_t					difference_type;
		typedef wchar_t*					pointer;
		typedef wchar_t&					reference;
	};

	template <>
	struct iterator_traits_alter<const wchar_t*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef wchar_t						value_type;
		typedef ptrdiff_t					difference_type;
		typedef const wchar_t*				pointer;
		typedef const wchar_t&				reference;
	};

	template <>
	struct iterator_traits_alter<int*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef int							value_type;
		typedef ptrdiff_t					difference_type;
		typedef int*						pointer;
		typedef int&						reference;
	};

	template <>
	struct iterator_traits_alter<const int*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef int							value_type;
		typedef ptrdiff_t					difference_type;
		typedef const int*					pointer;
		typedef const int&					reference;
	};

	template <>
	struct iterator_traits_alter<short*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef short						value_type;
		typedef ptrdiff_t					difference_type;
		typedef short*						pointer;
		typedef short&						reference;
	};

	template <>
	struct iterator_traits_alter<const short*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef short						value_type;
		typedef ptrdiff_t					difference_type;
		typedef const short*				pointer;
		typedef const short&				reference;
	};

	template <>
	struct iterator_traits_alter<long*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef long						value_type;
		typedef ptrdiff_t					difference_type;
		typedef long*						pointer;
		typedef long&						reference;
	};

	template <>
	struct iterator_traits_alter<const long*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef long						value_type;
		typedef ptrdiff_t					difference_type;
		typedef const long*				pointer;
		typedef const long&				reference;
	};
};

#else

#define iterator_traits_alter	iterator_traits

#endif

// -------------------------------------------------------------------------
// $Log: iterator.h,v $

#endif /* STD_ITERATOR_H */
