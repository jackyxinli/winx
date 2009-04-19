#include <iostream>
#include <stdext/text/Cast.h>

// -------------------------------------------------------------------------
// format

int main1()
{
	int ival = NS_STDEXT::cast<int>("12");
	std::cout << ival << '\n';
	
	unsigned uval = NS_STDEXT::cast<unsigned>(8, "12");
	std::cout << uval << '\n';
	
	uval = NS_STDEXT::cast<unsigned>(16, "12");
	std::cout << uval << '\n';

	__int64 ival64 = NS_STDEXT::cast<__int64>("-12");
	std::cout << ival64 << '\n';

	double dblval = NS_STDEXT::cast<double>("1.2e10");
	std::cout << dblval << '\n';
		
	float fval = NS_STDEXT::cast<float>("1.2e10");
	std::cout << fval << '\n';

	NS_STDEXT::TempString<wchar_t> s(L"123.4");
	std::cout << NS_STDEXT::cast<double>(s) << '\n';

	NS_STDEXT::String s2("123.4", 3);
	std::cout << NS_STDEXT::cast<float>(s2) << '\n';
	
	std::string s3 = "123.4";
	std::cout << NS_STDEXT::cast<double>(s3) << '\n';
	return 0;
}

// -------------------------------------------------------------------------

