#include <stdext/String.h>
#include <stdext/CArray.h>
#include <iostream>
#include <list>
#include <vector>

// -------------------------------------------------------------------------

void testConcat()
{
	NS_STDEXT::AutoAlloc alloc;
	NS_STDEXT::String s;

	std::list<std::string> lst;
	lst.push_back("Hello,");
	lst.push_back(" ");
	lst.push_back("world!\n");

	s = NS_STDEXT::concat(alloc, lst);
	NS_STDEXT::print("%s", s);

	std::vector<std::string> vec;
	vec.push_back("I");
	vec.push_back(" am");
	vec.push_back(" xushiwei!\n");

	s = NS_STDEXT::concat(alloc, vec.begin(), vec.end());
	NS_STDEXT::print("%s", s);

	s = NS_STDEXT::concat(alloc, "Hello", " ", "world!", " I", " am", " xushiwei!\n");
	NS_STDEXT::print("%s", s);
}

#if !defined(X_CC_VC6)

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
	size_t i;

	NS_STDEXT::AutoAlloc alloc;

	NS_STDEXT::String s(alloc, "Hello, world!  I am xushiwei!");

	NS_STDEXT::BasicArray<NS_STDEXT::String> arr = NS_STDEXT::explode(alloc, ' ', s);
	for (i = 0; i < arr.size(); ++i)
		std::cout << arr[i] << '\n';

	std::cout << "+++++++++++++++++++++\n";

	NS_STDEXT::String s2 = NS_STDEXT::implode(alloc, " -> ", arr);
	std::cout << s2 << '\n';

	std::cout << "+++++++++++++++++++++\n";

	arr = NS_STDEXT::explode2<0>(alloc, ' ', s);
	for (i = 0; i < arr.size(); ++i)
		std::cout << arr[i] << '\n';
}

#endif

void testCString()
{
	NS_STDEXT::Pools alloc;
	NS_STDEXT::TlsPools::put(alloc);
	
	NS_STDEXT::CString s("abc");
	NS_STDEXT::CString s1(4, '!');
	NS_STDEXT::CString s2("hello, xsw!", 5);

	NS_STDEXT::print("%s, size: %d\n", s, s.size());
	NS_STDEXT::print("%s, size: %d%s\n", s2, s2.size(), s1);
}

void testCArray()
{
	NS_STDEXT::Pools alloc;
	NS_STDEXT::TlsPools::put(alloc);

	NS_STDEXT::CArray<char> s(4, '!');
	NS_STDEXT::BasicArray<char> s2 = s;
	NS_STDEXT::String s3 = s;
	NS_STDEXT::CArray<char> s4 = s;
	NS_STDEXT::CArray<char> s5;
	s5 = s4;

	NS_STDEXT::CArray<char> s6(5);
	s6[2] = '\0';

	NS_STDEXT::print("%s%s, s6:%s\n", s3, s4, s6.data());
}

int main()
{
	printf("----------------------------------\n");
	testConcat();

#if !defined(X_CC_VC6)
	printf("----------------------------------\n");
	testImplode();
	printf("----------------------------------\n");
	testExplode();
#endif

	printf("----------------------------------\n");
	testCString();
	testCArray();
	printf("----------------------------------\n");
	return 0;
}

// -------------------------------------------------------------------------
