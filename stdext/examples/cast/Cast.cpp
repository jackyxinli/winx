#include <iostream>
#include <stdext/text/Cast.h>

// -------------------------------------------------------------------------
// format

int main()
{
	int ival = std::cast<int>("12");
	std::cout << ival << '\n';
	
	unsigned uval = std::cast<unsigned>("12", 8);
	std::cout << uval << '\n';
	
	uval = std::cast<unsigned>("12", 16);
	std::cout << uval << '\n';

	__int64 ival64 = std::cast<__int64>("-12");
	std::cout << ival64 << '\n';

	double dblval = std::cast<double>("1.2e10");
	std::cout << dblval << '\n';
		
	float fval = std::cast<float>("1.2e10");
	std::cout << fval << '\n';

	std::TempString<wchar_t> s(L"123.4");
	std::cout << std::cast<double>(s) << '\n';

	std::String s2("123.4", 3);
	std::cout << std::cast<float>(s2) << '\n';
	
	std::string s3 = "123.4";
	std::cout << std::cast<double>(s3) << '\n';
	return 0;
}

// -------------------------------------------------------------------------

