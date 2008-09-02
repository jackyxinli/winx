#include <stdext/Basic.h>

// -------------------------------------------------------------------------
// testcase

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

int main()
{
	WINX_TEST_APP(NS_STDEXT::ErrorLog, "", "");
	WINX_TEST_CLASS(DoTest);
	return 0;
}

// -------------------------------------------------------------------------

