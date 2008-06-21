//
//  stl_contianers.cpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//

#pragma warning(disable:4786)
#include <boost/memory.hpp>
#include <set>
#include <map>
#include <list>
#include <deque>
#include <cstdio>

using boost::stl_allocator;

// -------------------------------------------------------------------------

enum { Count = 1000000 };

void testDeque()
{
	printf("\n===== Deque (scoped_alloc) =====\n");
	boost::scoped_alloc alloc;
	std::deque<int, stl_allocator<int> > s(alloc);
	for (int i = 0; i < Count; ++i)
		s.push_back(i);
}

void testList()
{
	printf("\n===== List (scoped_alloc) =====\n");
	boost::scoped_alloc alloc;
	std::list<int, stl_allocator<int> > s(alloc);
	for (int i = 0; i < Count; ++i)
		s.push_back(i);
}

void testSet()
{
	printf("\n===== Set (scoped_alloc) =====\n");
	boost::scoped_alloc alloc;
	std::set<int, std::less<int>, stl_allocator<int> > s(std::less<int>(), alloc);
	for (int i = 0; i < Count; ++i)
		s.insert(i);
}

void testMap()
{
	printf("\n===== Map (scoped_alloc) =====\n");
	boost::scoped_alloc alloc;
	std::map<int, int, std::less<int>, stl_allocator<int> > s(std::less<int>(), alloc);
	for (int i = 0; i < Count; ++i)
		s.insert(std::pair<int, int>(i, i));
}

void testStlContainers()
{
	testDeque();
	testList();
	testSet();
	testMap();
}
