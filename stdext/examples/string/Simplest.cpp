#include <iostream>
#include <list>
#include <vector>
#include <stdext/String.h>

// -------------------------------------------------------------------------
// log

int main()
{
	std::AutoFreeAlloc alloc;

	std::list<std::string> lst;
	lst.push_back("Hello,");
	lst.push_back(" ");
	lst.push_back("world!\n");

	std::String s = std::flatten(alloc, lst);
	std::cout << s;

	std::vector<std::string> vec;
	vec.push_back("I");
	vec.push_back(" am");
	vec.push_back(" xushiwei!\n");

	s = std::flatten(alloc, vec);
	std::cout << s;
	return 0;
}

// -------------------------------------------------------------------------

