#include <iostream>

int main()
{
	int nVal, nTotal = 0;
	for (;;)
	{
		std::string strLine;
		if (!(std::cin >> nVal) || !std::getline(std::cin, strLine)) {
			std::cout << nTotal << '\n';
			return 0;
		}
		nTotal += nVal;
	}
}

