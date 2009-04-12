#include <iostream>
#include <list>
#include <vector>
#include <stdext/String.h>

// -------------------------------------------------------------------------

void testConcat()
{
	NS_STDEXT::AutoAlloc alloc;

	std::list<std::string> lst;
	lst.push_back("Hello,");
	lst.push_back(" ");
	lst.push_back("world!\n");

	NS_STDEXT::String s = NS_STDEXT::concat(alloc, lst);
	std::cout << s;

	std::vector<std::string> vec;
	vec.push_back("I");
	vec.push_back(" am");
	vec.push_back(" xushiwei!\n");

	s = NS_STDEXT::concat(alloc, vec);
	std::cout << s;
	
	s = NS_STDEXT::concat(alloc, "Hello", " ", "world!", " I", " am", " xushiwei!\n");
	std::cout << s;
}

void testImplode()
{
	NS_STDEXT::AutoAlloc alloc;

	std::list<std::string> lst;
	lst.push_back("Hello,");
	lst.push_back("world!");
	lst.push_back("I");
	lst.push_back("am");
	lst.push_back("xushiwei!\n");

	NS_STDEXT::String s = NS_STDEXT::implode(alloc, ' ', lst);
	std::cout << s;

	std::vector<std::string> vec;
	vec.push_back("Hello,");
	vec.push_back("world!");
	vec.push_back("I");
	vec.push_back("am");
	vec.push_back("xushiwei!\n");

	s = NS_STDEXT::implode(alloc, " -> ", vec);
	std::cout << s;
}

void testExplode()
{
	NS_STDEXT::AutoAlloc alloc;

	NS_STDEXT::String s(alloc, "Hello, world!  I am xushiwei!");

	NS_STDEXT::BasicArray<NS_STDEXT::String> arr = NS_STDEXT::explode(alloc, ' ', s);
	for (size_t i = 0; i < arr.size(); ++i)
		std::cout << arr[i] << '\n';
	
	std::cout << "+++++++++++++++++++++\n";

	NS_STDEXT::String s2 = NS_STDEXT::implode(alloc, " -> ", arr);
	std::cout << s2 << '\n';

	std::cout << "+++++++++++++++++++++\n";

	arr = NS_STDEXT::explode2<0>(alloc, ' ', s);
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

