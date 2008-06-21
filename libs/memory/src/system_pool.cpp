//
//  boost/memory/system_pool.cpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#include "stdafx.h"

// -------------------------------------------------------------------------

#if 1 // defined(_WIN32)

#include <boost/memory/system_pool.hpp>

using namespace NS_BOOST_MEMORY;

STDAPI_(void*) _boost_SystemPoolAlloc(size_t cb)
{
	return system_pool::allocate(cb);
}

STDAPI_(void) _boost_SystemPoolFree(void* p)
{
	system_pool::deallocate(p);
}

STDAPI_(size_t) _boost_SystemPoolSize(void* p)
{
	return system_pool::alloc_size(p);
}

#endif

// -------------------------------------------------------------------------
// $Log: $
