#define STD_UNITTEST
#include <stdext/Basic.h>

// Archive:
#include <stdext/Archive.h>

// Event:
#define WINX_USE_DEFINE_IID
#include <stdext/AOP.h>

// Hash:
#include <stdext/HashMap.h>
#include <stdext/HashSet.h>

// String:
#include <stdext/text/BasicString.h>
#include <stdext/text/BasicStringBuilder.h>
#include <stdext/text/TextPool.h>
#include <stdext/text/Rope.h>
#include <stdext/text/StringAlgo.h>
#include <stdext/text/Format.h>

int main()
{
	WINX_TEST_APP(std::ErrorLog, "", "");

	// Archive:
	WINX_TEST_CLASS(TestRecord);
	WINX_TEST_CLASS(TestMemArchive);
	WINX_TEST_CLASS(TestStdioArchive);
	WINX_TEST_CLASS(TestPosixArchive);

	// Event:
	WINX_TEST_CLASS(TestEvent);
	WINX_TEST_CLASS(TestEventContainer);

	// Hash:
	WINX_TEST_CLASS(TestHashMap);
	WINX_TEST_CLASS(TestHashSet);

	// String:
	WINX_TEST_CLASS(TestBasicString);
	WINX_TEST_CLASS(TestStringBuilder);
	WINX_TEST_CLASS(TestStringAlgo);
	WINX_TEST_CLASS(TestTextPool);
	WINX_TEST_CLASS(TestRope);
	WINX_TEST_CLASS(TestFormat);
}

