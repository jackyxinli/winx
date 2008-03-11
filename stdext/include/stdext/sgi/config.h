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
// Module: sgi/config.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: config.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __SGI_CONFIG_H__
#define __SGI_CONFIG_H__

// -------------------------------------------------------------------------
// std::allocator

#define __STL_USE_STD_ALLOCATORS
#define __SGI_STL_INTERNAL_ALLOC_H

// -------------------------------------------------------------------------

#ifndef __STL_CONFIG_H
#include "../../../../stl/stl_config.h"
#endif

// -------------------------------------------------------------------------

#ifndef __STDEXT_MEMORY_H__
#include "../Memory.h"
#endif

namespace stdext
{
#if defined(X_CC_VC6)
	template <class _Tp, class _Allocator>
	struct _Alloc_traits
	{
		typedef std::StlAlloc<_Tp, std::ScopeAlloc> allocator_type;
	};
#else
	template <class _Tp, class _Allocator>
	struct _Alloc_traits
	{
		typedef typename _Allocator::rebind<_Tp>::other allocator_type;
	};
#endif
}

// -------------------------------------------------------------------------
// namespace stdext

#undef __STD 
#undef __STL_BEGIN_NAMESPACE 
#undef __STL_END_NAMESPACE 
#undef __STL_USE_NAMESPACE_FOR_RELOPS
#undef __STL_BEGIN_RELOPS_NAMESPACE 
#undef __STL_END_RELOPS_NAMESPACE 
#undef __STD_RELOPS 
#undef  __STL_USE_NAMESPACES

#define __STL_USE_NAMESPACES
#define __STD stdext
#define __STL_BEGIN_NAMESPACE namespace stdext {
#define __STL_END_NAMESPACE }
#define __STL_USE_NAMESPACE_FOR_RELOPS
#define __STL_BEGIN_RELOPS_NAMESPACE namespace stdext { namespace rel_ops {
#define __STL_END_RELOPS_NAMESPACE } }
#define __STD_RELOPS stdext::rel_ops

// -------------------------------------------------------------------------
// $Log: config.h,v $

#endif /* __SGI_CONFIG_H__ */
