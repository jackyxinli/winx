#include <iostream> 	// std::cout
#include <stdext/text/Format.h>

// -------------------------------------------------------------------------
// format

int main()
{
	std::AutoFreeAlloc alloc;
	std::String s;
	std::WString ws;
	
	s = std::str(alloc, 123);
	std::cout << s << ' '; // 123
	
	ws = std::wstr(alloc, 123, 16);
	std::wcout << ws << L' '; // 7B;
	
	s = std::str(alloc, 12.3);
	std::cout << s << ' '; // 12.3
	
	ws = std::wstr(alloc, 12.3);
	std::wcout << ws << L'\n'; // 12.3;
	
	return 0;
}

// -------------------------------------------------------------------------

