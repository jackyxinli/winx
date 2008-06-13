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
// Module: tpl/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Basic.h 638 2008-06-11 19:52:41Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_BASIC_H
#define TPL_BASIC_H

#ifndef STDEXT_BASIC_H
#include "../../../stdext/include/stdext/Basic.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "../../../stdext/include/stdext/Memory.h"
#endif

#ifndef TPL_UNMANAGED_NEW
#define TPL_UNMANAGED_NEW(alloc, Type)	new((alloc).allocate(sizeof(Type))) Type
#endif

#ifndef TPL_ALLOC_ARRAY
#define TPL_ALLOC_ARRAY(alloc, Type, n)	((Type*)(alloc).allocate((n)*sizeof(Type)))
#endif

#ifndef TPL_ALLOC
#define TPL_ALLOC(alloc, Type)	((Type*)(alloc).allocate(sizeof(Type)))
#endif

#ifndef TPL_NEW
#if defined(BOOST_NEW)
#define TPL_NEW(alloc, Type)	BOOST_NEW(alloc, Type)
#else
#define TPL_NEW(alloc, Type)	STD_NEW(alloc, Type)
#endif
#endif

#ifndef TPL_CALL
#if defined(winx_call)
#define TPL_CALL winx_call
#else
#define TPL_CALL
#endif
#endif

#ifndef TPL_ASSERT
#if defined(BOOST_MEMORY_ASSERT)
#define TPL_ASSERT(e)	BOOST_MEMORY_ASSERT(e)
#elif defined(_ASSERTE)
#define TPL_ASSERT(e)	_ASSERTE(e)
#else
#define TPL_ASSERT(e)	0
#endif
#endif

#ifndef TPL_TRACE
#if defined(WINX_TRACE)
#define TPL_TRACE	WINX_TRACE
#elif defined(_DEBUG)
#define TPL_TRACE	printf
#else
#define TPL_TRACE	__noop
#endif
#endif

#ifndef TPL_TRACEW
#if defined(WINX_TRACEW)
#define TPL_TRACEW	WINX_TRACEW
#elif defined(_DEBUG)
#define TPL_TRACEW	wprintf
#else
#define TPL_TRACEW	__noop
#endif
#endif

#ifndef NS_TPL_BEGIN
#define NS_TPL_BEGIN			namespace tpl {
#define NS_TPL_END				}
#define NS_TPL					tpl
#endif

#if !defined(__forceinline) && !defined(_MSC_VER)
#define __forceinline inline
#endif

NS_TPL_BEGIN

// =========================================================================
// class DefaultAllocator

#if defined(TPL_USE_AUTO_ALLOC)
typedef std::AutoFreeAlloc DefaultAllocator;
#else
typedef std::ScopedAlloc DefaultAllocator;
#endif

// =========================================================================
// TPL_REQUIRE(e, Require) -- see BOOST_STATIC_ASSERT(e)

template <bool bOk>
struct RequireFeatureTraits {
	typedef struct RequireFeatureOk {} feature_type;
};

template <>
struct RequireFeatureTraits<false> {
	struct RequireFeatureFailed {};
};

template <class T1, class T2>
struct RequireClassTraits {
	struct RequireClassFailed {};
};

template <class ClassT>
struct RequireClassTraits<ClassT, ClassT> {
	typedef ClassT class_type;
};

#ifndef TPL_REQUIRE
#define TPL_REQUIRE(e, RequireFeature_)										\
	typedef typename tpl::RequireFeatureTraits<(e) != 0>::feature_type RequireFeature_;
#endif

#ifndef TPL_REQUIRE_CLASS
#define TPL_REQUIRE_CLASS(T1, T2, RequireFeature_)							\
	typedef typename tpl::RequireClassTraits<T1, T2>::class_type RequireFeature_;
#endif

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_BASIC_H */

