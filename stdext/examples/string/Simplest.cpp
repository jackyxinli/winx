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

void testExplode()
{
	std::AutoFreeAlloc alloc;

	std::String s(alloc, "Hello, world!  I am xushiwei!");

	std::BasicArray<std::String> arr = std::explode(alloc, ' ', s);
	for (size_t i = 0; i < arr.size(); ++i)
		std::cout << arr[i] << '\n';
		
	std::cout << "+++++++++++++++++++++\n";
		
	arr = std::explode<false>(alloc, ' ', s);
	for (size_t i = 0; i < arr.size(); ++i)
		std::cout << arr[i] << '\n';
}

int main()
{
	std::cout << "----------------------------------\n";
	testConcat();
	std::cout << "----------------------------------\n";
	testImplode();
	std::cout << "----------------------------------\n";
	testExplode();
	std::cout << "----------------------------------\n";
	return 0;
}

// -------------------------------------------------------------------------

