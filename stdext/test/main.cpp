#define STD_UNITTEST
#include <stdext/Basic.h>

// Event:
#define WINX_USE_DEFINE_IID
#include <stdext/AOP.h>

// Containers:
#include <stdext/StaticAlgo.h>
#include <stdext/HashMap.h>
#include <stdext/HashSet.h>
#include <stdext/Relation.h>

// String:
#include <stdext/text/BasicString.h>
#include <stdext/text/StringBuilder.h>
#include <stdext/text/TextPool.h>
#include <stdext/text/Rope.h>
#include <stdext/text/StringAlgo.h>
#include <stdext/text/Format.h>
#include <stdext/text/Cast.h>

// Archive:
#include "archive.h"

int main()
{
	WINX_TEST_APP(NS_STDEXT::ErrorLog, "", "");

	// Archive:
	WINX_TEST_CLASS(TestRecord);
	WINX_TEST_CLASS(TestMemArchive);
	WINX_TEST_CLASS(TestStdioArchive);
	WINX_TEST_CLASS(TestPosixArchive);

	// Event:
	WINX_TEST_CLASS(TestEvent);
	WINX_TEST_CLASS(TestEventContainer);

	// Containers:
	WINX_TEST_CLASS(TestStaticAlgo);
	WINX_TEST_CLASS(TestHashMap);
	WINX_TEST_CLASS(TestHashSet);
	WINX_TEST_CLASS(TestRelation);

	// String:
	WINX_TEST_CLASS(TestBasicString);
	WINX_TEST_CLASS(TestStringBuilder);
	WINX_TEST_CLASS(TestStringAlgo);
	WINX_TEST_CLASS(TestTextPool);
	WINX_TEST_CLASS(TestRope);
	WINX_TEST_CLASS(TestFormat);
	WINX_TEST_CLASS(TestCast);
}
