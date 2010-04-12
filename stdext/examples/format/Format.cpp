#include <stdext/String.h>
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

	NS_STDEXT::StringBuilder s2;
	NS_STDEXT::format(
		s2, "value: %d%s%s %f\n", (unsigned int)123, "! ", NS_STDEXT::String("xushiwei", 2), 134.5);
	std::cout << s2;
	
	std::string dest;

	NS_STDEXT::format(
		dest, "value: %7d%s%s %f\n", 123, "! ", NS_STDEXT::String("xushiwei", 2), 134.5);
	std::cout << dest;
	
	NS_STDEXT::format(
		dest, "value 2: %#x - %s%s\n", 123, "Hi! ", NS_STDEXT::String("xushiwei", 2));
	std::cout << dest;

	NS_STDEXT::format(
		*stdout, "stdout: %o - %s %f\n", 123, "Hello!", 134.5);

	NS_STDEXT::printf(
		"printf - stdout: %o - %s %f\n", 123, "Hello!", 134.5);

	NS_STDEXT::format(
		*stderr, "stderr: %o - %s %f\n", 123, "Hello!", 134.5);

	return 0;
}

// -------------------------------------------------------------------------
