#include <stdext/text/Format.h>
#include <iostream> 	// std::cout

// -------------------------------------------------------------------------
// format

int main()
{
	NS_STDEXT::AutoAlloc alloc;
	NS_STDEXT::String s;
	
	s = NS_STDEXT::str(alloc, 123);
	std::cout << s << '\n'; // 123
	
	s = NS_STDEXT::str(alloc, 123, 16);
	std::cout << s << '\n'; // 7b
	
	s = NS_STDEXT::str(alloc, 12.3);
	std::cout << s << '\n'; // 12.3
	
	s = NS_STDEXT::str(alloc, 12.33, 3);
	std::cout << s << '\n'; // 12.3
	
	std::string dest;

	NS_STDEXT::format(
		dest, "value: %d%s%s %f\n", 123, "! ", NS_STDEXT::String("xushiwei", 2), 134.5);
	std::cout << dest;
	
	NS_STDEXT::format(
		dest, "value 2: %X - %s%s\n", 123, "Hi! ", NS_STDEXT::String("xushiwei", 2));
	std::cout << dest;

	return 0;
}

// -------------------------------------------------------------------------
