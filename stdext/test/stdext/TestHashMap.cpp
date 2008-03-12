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
// Module: TestHashMap.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: TestHashMap.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

#if defined(_MSC_VER) && (_MSC_VER <= 1200)

#pragma warning(disable:4010)

#define USE_SGISTL
#include <stdext/Basic.h>
#include <stdext/Counter.h>

#include "../../stl/hash_map.h"

// -------------------------------------------------------------------------

template <class LogT>
class TestSgiHashMap : public TestCase
{
	WINX_TEST_SUITE(TestSgiHashMap);
		WINX_TEST(testBasic);
		WINX_TEST(testCompare);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		typedef std::hash_map<int, int> MapType;
		MapType simp;

		simp.insert(MapType::value_type(1, 2));
		simp.insert(MapType::value_type(1, 2));
		simp.insert(MapType::value_type(192, 4));
		simp.insert(MapType::value_type(194, 8));
		for (MapType::const_iterator it = simp.begin(); it != simp.end(); ++it)
		{
			log.print((*it).first)
				.print(", ")
				.print((*it).second)
				.newline();
		}
	}

public:
	enum { N = 20000 };

	void doMap(LogT& log)
	{
		typedef std::hash_map<int, int> MapT;
		log.print("===== std::hash_map =====\n");
		std::PerformanceCounter counter;
		{
			MapT coll;
			for (int i = 0; i < N; ++i)
				coll.insert(MapT::value_type(i, i));
		}
		counter.trace(log);
	}

	void testCompare(LogT& log)
	{
		for (int i = 0; i < 5; ++i)
		{
			log.newline();
			doMap(log);
		}
	}
};

WINX_AUTORUN_CLASS(TestSgiHashMap, std::ErrorLog);

// -------------------------------------------------------------------------

void testSgiHashMap()
{
	__autoRun_TestSgiHashMap();
}

#else

void testSgiHashMap()
{
}

// -------------------------------------------------------------------------
// $Log: TestHashMap.cpp,v $
//

#endif // defined(_MSC_VER)
