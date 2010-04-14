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

	// ----------------------------------------

	NS_STDEXT::StringBuilder s2;
	NS_STDEXT::format(s2, "1: %.5d%c %5s! %f\n", -123, '\"'-1, NS_STDEXT::String("xushiwei", 2), 134.5);
	std::cout << s2;

	printf("2: %.5d%c %5s! %f\n", -123, '\"'-1, "xu", 134.5);

	// ----------------------------------------

	std::string s3;
	NS_STDEXT::format(s3, "1: %05d%s%-5s! %f\n", -123, "! ", NS_STDEXT::String("xushiwei", 2), 134.5);
	std::cout << s3;

	printf("2: %05d%s%-5s! %f\n", -123, "! ", "xu", 134.5);

	// ----------------------------------------

	std::vector<char> s4;
	NS_STDEXT::format(s4, "1: %-#6X! %x! - %s%s\n", 123, -1, "Hi! ", NS_STDEXT::String("xushiwei", 2));
	s4.push_back('\0');
	std::cout << &s4[0];

	printf("2: %-#6X! %x! - %s%s\n", 123, -1, "Hi! ", "xu");

	// ----------------------------------------

	NS_STDEXT::format(*stdout, "1: %#6o! - %p %g\n", 123, &s4, 134.5);
	NS_STDEXT::print("2: %#6o! - %p %g\n", 123, &s4, 134.5);
	printf("3: %#6o! - %p %g\n", 123, &s4, 134.5);

	// ----------------------------------------

	NS_STDEXT::format(*stderr, "stderr: %o - %s %f\n", 123, "Hello!", 134.5);

	// ----------------------------------------

	return 0;
}

// -------------------------------------------------------------------------
