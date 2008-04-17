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
// Module: wrapper/boost/config.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-21 14:04:45
// 
// $Id: config.h,v 1.2 2006/12/14 09:15:33 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WRAPPER_BOOST_CONFIG_H__
#define __WRAPPER_BOOST_CONFIG_H__

#if (0)
#define WINX_USE_DEFBOOST
#endif

// -------------------------------------------------------------------------

#if defined(_MSC_VER) && (_MSC_VER <= 1200)
#define BOOST_NO_STDC_NAMESPACE
#endif

#if defined(WINX_USE_DEFBOOST)

#ifndef BOOST_POOL_HPP
#include <boost/config/config.hpp>
#endif

#else

#ifndef BOOST_CONFIG_HPP
#include "../../../../../boost/boost/config.hpp"
#endif

#endif

// -------------------------------------------------------------------------
// disable malloc/free macro

inline void* __cxx_malloc(size_t cb) {
	return malloc(cb);
}

inline void __cxx_free(void* p) {
	free(p);
}

#if defined(malloc)

#undef malloc
#undef free

#define malloc __cxx_malloc
#define free __cxx_free

#endif

namespace std
{ 
	using ::__cxx_malloc;
	using ::__cxx_free;
}

// -------------------------------------------------------------------------
// $Log: config.h,v $

#endif /* __WRAPPER_BOOST_CONFIG_H__ */
