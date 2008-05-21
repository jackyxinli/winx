
#include "stdafx.h"
#include <stdext/Memory.h>
#include <tpl/RegEx.h>
#include <iostream>

int main()
{
	std::AutoFreeAlloc alloc;

	RegExp a(alloc, ch('a'));
	
	RegExp b;
	b.assign(alloc, ch('b'));

	RegExp a_or_b(alloc, a | b);
	
	RegExp a_or_b_or_c(alloc, a_or_b | 'c');

	RegExp a_or_b_or_d(alloc, 'd' | a_or_b);

	SubMatch result;
	SimpleRegExp three_word(alloc, (result = ('d' >> a_or_b_or_c >> 'b')));

	SimpleRegExp repeated = three_word; // *three_word;

	const char* out;
	char data[] = "dcbcdefg";
	bool fail = a_or_b_or_c.match(data, data+sizeof(data), out);
	bool ok = repeated.match(data, data+sizeof(data), out);
	std::cout << result.stl_str() << '\n';
	return 0;
}
