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
#include <boost/memory/scoped_alloc.hpp>

// -------------------------------------------------------------------------

using namespace boost;

tls_block_pool_t _tls_blockPool;

STDAPI_(tls_block_pool_t*) _boost_TlsBlockPool()
{
	return &_tls_blockPool;
}

// -------------------------------------------------------------------------
// $Log: $
