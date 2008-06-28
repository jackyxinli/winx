#include <iostream>
#include <list>
#include <vector>
#include <stdext/String.h>

// -------------------------------------------------------------------------

void testConcat()
{
	std::AutoFreeAlloc alloc;

	std::list<std::string> lst;
	lst.push_back("Hello,");
	lst.push_back(" ");
	lst.push_back("world!\n");

	std::String s = std::concat(alloc, lst);
	std::cout << s;

	std::vector<std::string> vec;
	vec.push_back("I");
	vec.push_back(" am");
	vec.push_back(" xushiwei!\n");

	s = std::concat(alloc, vec);
	std::cout << s;
}

void testImplode()
{
	std::AutoFreeAlloc alloc;

	std::list<std::string> lst;
	lst.push_back("Hello,");
	lst.push_back("world!");
	lst.push_back("I");
	lst.push_back("am");
	lst.push_back("xushiwei!\n");

	std::String s = std::implode(alloc, ' ', lst);
	std::cout << s;

	std::vector<std::string> vec;
	vec.push_back("Hello,");
	vec.push_back("world!");
	vec.push_back("I");
	vec.push_back("am");
	vec.push_back("xushiwei!\n");

	s = std::implode(alloc, " -> ", vec);
	std::cout << s;
}

int main()
{
	testConcat();
	testImplode();
	return 0;
}

// -------------------------------------------------------------------------

