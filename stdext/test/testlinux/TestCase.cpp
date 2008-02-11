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
// Module: TestCase.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: TestCase.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <stdext/Basic.h>

// -------------------------------------------------------------------------
// TestCase

template <class LogT>
class DoTest : public TestCase
{
	WINX_TEST_SUITE(DoTest);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void test(LogT& log)
	{
		int a[] = {1, 2, 3};
		int b[] = {1, 2, 3};
		AssertEq(a[0], b[0]);
		AssertExp(a[1] == b[1]);
		AssertEqBuf(a, b, countof(a));
	}
};

void testTestCase()
{
	WINX_TEST_APP(std::ErrorLog, "", "");
	WINX_TEST_CLASS(DoTest);
}

WINX_AUTORUN(testTestCase);

// -------------------------------------------------------------------------
// $Log: TestCase.cpp,v $
//
