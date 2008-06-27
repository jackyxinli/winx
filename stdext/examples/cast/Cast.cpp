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

	return 0;
}

// -------------------------------------------------------------------------

