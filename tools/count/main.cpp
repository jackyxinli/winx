#include <iostream>

int main()
{
	int nTotal = 0;
	for (;;)
	{
		std::string strLine;
		if (!std::getline(std::cin, strLine)) {
			std::cout << nTotal << '\n';
			return 0;
		}
		++nTotal;
	}
}

