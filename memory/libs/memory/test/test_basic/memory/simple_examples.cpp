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
#define BOOST_MEMORY_DEBUG_GC
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
	NS_BOOST_MEMORY::block_pool recycle;
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

void testGCAlloc()
{
	boost::gc_alloc alloc;
	// same as: boost::gc_alloc(boost::tls_block_pool::instance());
	
	int* intObj = BOOST_NEW(alloc, int);
	int* intObjWithArg = BOOST_NEW(alloc, int)(10);
	int* intArray = BOOST_NEW_ARRAY(alloc, int, 10000); // > BlockSize
	int* intBuf = BOOST_ALLOC(alloc, int);
	int* intArrayBuf = BOOST_ALLOC_ARRAY(alloc, int, 100);

	enum { AllocSizeHuge = NS_BOOST_MEMORY_POLICY::sys::AllocSizeHuge };
	char* hugeBuf = BOOST_NEW_ARRAY(alloc, char, AllocSizeHuge); // > HugeSize
	
	boost::gc_alloc* suballoc = BOOST_NEW(alloc, boost::gc_alloc);
	int* e = BOOST_NEW(*suballoc, int);

	boost::gc_alloc* allocArray = BOOST_NEW_ARRAY(alloc, boost::gc_alloc, 10);

	alloc.destroyArray(intArray, 10000);
	alloc.destroyArray(allocArray, 10);

	int i;
	for (i = 0; i < 1000; ++i)
		BOOST_NEW_ARRAY(alloc, int, 100);
	for (i = 0; i < 8000; ++i)
		BOOST_NEW(alloc, int);

	boost::gc_alloc alloc2;
	alloc.swap(alloc2);
}

void simpleExamples()
{
	boost::enableMemoryLeakCheck();
	testAutoAlloc();
	testScopedAlloc();
	testTlsScopedAlloc();
	testGCAlloc();
}
