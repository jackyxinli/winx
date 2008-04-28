//
//  simple_examples.cpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#include <boost/memory.hpp>

// -------------------------------------------------------------------------
// TestCase

void testAutoAlloc()
{
	boost::auto_alloc alloc;
    int* intObj = BOOST_NEW(alloc, int);
	int* intObjWithArg = BOOST_NEW(alloc, int)(10);
    int* intArray = BOOST_NEW_ARRAY(alloc, int, 100);
    int* intBuf = BOOST_ALLOC(alloc, int);
    int* intArrayBuf = BOOST_ALLOC_ARRAY(alloc, int, 100);
    
    boost::auto_alloc* subAlloc = BOOST_NEW(alloc, boost::auto_alloc);
    int* e = BOOST_NEW(*subAlloc, int);
}

void testScopedAlloc()
{
    boost::block_pool recycle;
    boost::scoped_alloc alloc(recycle);

    int* intObj = BOOST_NEW(alloc, int);
	int* intObjWithArg = BOOST_NEW(alloc, int)(10);
    int* intArray = BOOST_NEW_ARRAY(alloc, int, 100);
    int* intBuf = BOOST_ALLOC(alloc, int);
    int* intArrayBuf = BOOST_ALLOC_ARRAY(alloc, int, 100);
    
    boost::scoped_alloc* suballoc = BOOST_NEW(alloc, boost::scoped_alloc)(alloc);
    int* e = BOOST_NEW(*suballoc, int);
}

void testTlsScopedAlloc()
{
    boost::scoped_alloc alloc;
	// same as: boost::scoped_alloc(boost::tls_block_pool::instance());

    int* intObj = BOOST_NEW(alloc, int);
	int* intObjWithArg = BOOST_NEW(alloc, int)(10);
    int* intArray = BOOST_NEW_ARRAY(alloc, int, 100);
    int* intBuf = BOOST_ALLOC(alloc, int);
    int* intArrayBuf = BOOST_ALLOC_ARRAY(alloc, int, 100);

    boost::scoped_alloc* suballoc = BOOST_NEW(alloc, boost::scoped_alloc);
    int* e = BOOST_NEW(*suballoc, int);
}

void simpleExamples()
{
    testAutoAlloc();
    testScopedAlloc();
	testTlsScopedAlloc();
}
