//
//  boost/memory/block_pool.cpp
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
#include <boost/memory/tls_gc_alloc.hpp>

// -------------------------------------------------------------------------

using namespace NS_BOOST_MEMORY;

tls_gc_alloc_t g_tls_gcAlloc;

STDAPI_(tls_gc_alloc_t*) _boost_TlsGcAlloc()
{
	return &g_tls_gcAlloc;
}

// -------------------------------------------------------------------------
// class tls_gc_alloc_init

class tls_gc_alloc_init
{
public:
	tls_gc_alloc_init() { g_tls_gcAlloc.init(); }
	~tls_gc_alloc_init() { g_tls_gcAlloc.term(); }
};

tls_gc_alloc_init g_tls_gcAllocInit;

// -------------------------------------------------------------------------
// $Log: $
