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
#include <boost/memory/object_pool.hpp>

// -------------------------------------------------------------------------

#ifndef	countof
#define countof(array)						(sizeof(array)/sizeof(array[0]))
#endif

void testPool()
{
	int i;
	boost::memory::pool alloc(sizeof(int));
	boost::memory::pool alloc2(sizeof(double));

	int* p[3000];
	for (i = 0; i < countof(p); ++i)
	{
		p[i] = (int*)alloc.allocate();
		*p[i] = i;
	}
	for (i = 0; i < countof(p); ++i)
	{
		alloc.deallocate(p[i]);
	}

	void* p1 = alloc.allocate();
	void* p2 = alloc.allocate();
	void* p3 = alloc.allocate();

	alloc.deallocate(p2);
	alloc.deallocate(p1);
	alloc.deallocate(p3);

	//
	// Note: It's ok even if you forget to free allocated memory!
	//
	for (i = 0; i < 1000; ++i)
		alloc.allocate();
}

void testScopedPool()
{
	int i;
	boost::memory::block_pool recycle;
	boost::memory::scoped_pool alloc(recycle, sizeof(int));
	boost::memory::scoped_pool alloc2(alloc.get_alloc(), sizeof(double));

	void* p[3000];
	for (i = 0; i < countof(p); ++i)
		p[i] = alloc.allocate();
	for (i = 0; i < countof(p); ++i)
		alloc.deallocate(p[i]);

	void* p1 = alloc.allocate();
	void* p2 = alloc.allocate();
	void* p3 = alloc.allocate();

	alloc.deallocate(p2);
	alloc.deallocate(p1);
	alloc.deallocate(p3);

	//
	// Note: It's ok even if you forget to free allocated memory!
	//
	for (i = 0; i < 1000; ++i)
		alloc.allocate();
}

int g_nConstruct = 0;
int g_nDestruct = 0;

struct Obj
{
	char m_val[100];

	Obj()		 { ++g_nConstruct; m_val[1] = 0x23; }
	Obj(int val) { ++g_nConstruct; }
	~Obj()		 { ++g_nDestruct; }
};

void testObjectPool()
{
	boost::memory::object_pool<int> alloc;
	int* p1 = alloc.construct();
	int* p2 = alloc.construct();
	int* p3 = BOOST_NEW(alloc, int)(30);
	BOOST_MEMORY_ASSERT(*p3 == 30);

	{
		boost::memory::object_pool<Obj> alloc2;
		for (int i = 0; i < 3000; ++i)
		{
			Obj* o1 = alloc2.construct();
			Obj* o2 = alloc2.construct();
			Obj* o3 = BOOST_NEW(alloc2, Obj)(20);
			Obj* o4 = BOOST_NEW(alloc2, Obj)(25);
			alloc2.destroy(o2);
			alloc2.destroy(o3);
		}
	}
	BOOST_MEMORY_ASSERT(g_nConstruct == g_nDestruct);
	if (g_nConstruct != g_nDestruct)
		printf("ERROR: memory leaks!\n");
}

void testScopedPools()
{
	size_t i;
	boost::memory::block_pool recycle;
	boost::memory::scoped_pools alloc(recycle);

	void* p[22];
	for (i = 0; i < countof(p); ++i)
		p[i] = alloc.get_pool(i * 7).allocate();
	for (i = 3; i < countof(p) - 2; ++i)
		alloc.get_pool(i * 7).deallocate(p[i]);
}

int main()
{
	NS_BOOST_MEMORY::enableMemoryLeakCheck();

	testScopedPools();
	testPool();
	testScopedPool();
	testObjectPool();
	return 0;
}
