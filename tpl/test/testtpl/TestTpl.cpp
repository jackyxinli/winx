
#include "stdafx.h"
#include <stdext/Memory.h>
#include <tpl/RegEx.h>

int main()
{
	std::AutoFreeAlloc alloc;

	RegExp a(alloc, ch('a'));
	
	RegExp b;
	b.assign(alloc, ch('b'));

	RegExp a_or_b(alloc, a | b);
	
	RegExp a_or_b_or_c(alloc, a_or_b | 'c');

	RegExp a_or_b_or_d(alloc, 'd' | a_or_b);

	SimpleRegExp three_word(alloc, 'd' >> a_or_b_or_c >> 'b');

	const char* out;
	char data[] = "dcbcdefg";
	bool fail = a_or_b_or_c.match(data, data+sizeof(data), out);
	bool ok = three_word.match(data, data+sizeof(data), out);
	return 0;
}
