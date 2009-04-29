#include <iostream> 	// std::cout
#include <stdext/text/Format.h>

// -------------------------------------------------------------------------
// format

int main()
{
	NS_STDEXT::AutoAlloc alloc;
	NS_STDEXT::String s;
	NS_STDEXT::WString ws;
	
	s = NS_STDEXT::str(alloc, 123);
	std::cout << s << ' '; // 123
	
	ws = NS_STDEXT::wstr(alloc, 123, 16);
	std::wcout << ws << L' '; // 7B;
	
	s = NS_STDEXT::str(alloc, 12.3);
	std::cout << s << ' '; // 12.3
	
	ws = NS_STDEXT::wstr(alloc, 12.3);
	std::wcout << ws << L'\n'; // 12.3;
	
	return 0;
}

// -------------------------------------------------------------------------

