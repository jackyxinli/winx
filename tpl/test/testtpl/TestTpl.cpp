
#include "stdafx.h"
#include <tpl/RegEx.h>
#include <stdext/LinkLib.h>
#include <iostream>

int main()
{
	BlockPool recycle;
	Allocator alloc(recycle);

	// ---- define rules ----

	RegEx a(alloc, ch('a'));
	
	RegEx b;
	b.assign(alloc, ch('b'));

	RegEx a_or_b(alloc, a | b);
	
	RegEx a_or_b_or_c(alloc, a_or_b | 'c');

	RegEx a_or_b_or_d(alloc, 'd' | a_or_b);

	LeafMark mResult;

	SimpleRegEx three_word(alloc, mResult = 'd' + a_or_b_or_c + 'b');

	SimpleRegEx repeated(alloc, *three_word);
	SimpleRegEx repeated3(alloc, repeat<3>(three_word));
	SimpleRegEx repeated2Or3(alloc, repeat<2, 3>(three_word));

	// ---- parse source ----

	char buf[] = "dcbdabcdefg";
	Source source(buf, buf+sizeof(buf));

	Document doc;
	Context context(alloc, doc);

	bool fail = a_or_b_or_c.match(source, context);
	bool ok = repeated3.match(source, context);

	Document::leaf_cons result = doc.select(alloc, mResult);
	while (result) {
		std::cout << result.hd().data().stl_str() << '\n';
		result = result.tl();
	}
	return 0;
}
