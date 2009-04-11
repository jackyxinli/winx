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

#include <boost/memory.hpp>
#include <boost/memory/linklib.hpp>

void testPerformance();
void testSystemAlloc();
void testStlContainers();
void testExceptionSemantics();
void simpleExamples();

int main()
{
	testPerformance();
//	testExceptionSemantics();
//	testSystemAlloc();
//	testPerformance();
//	simpleExamples();
//	testStlContainers();
	return 0;
}
