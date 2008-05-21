
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

	Mark mRoot;
	Mark mResult(mRoot);
	SimpleRegEx three_word(alloc, (mResult = ('d' >> a_or_b_or_c >> 'b')));

	SimpleRegEx repeated(alloc, *three_word);
	SimpleRegEx repeated2(alloc, repeat<2>(three_word));
	SimpleRegEx repeated2Or3(alloc, repeat<2, 3>(three_word));

	// ---- parse source ----

	char buf[] = "dcbdabcdefg";
	Source source(buf, buf+sizeof(buf));

	Document doc(alloc, mRoot);
	Context context(alloc, doc);

	bool fail = a_or_b_or_c.match(source, context);
	bool ok = repeated.match(source, context);
	
	Context::document_type::cons result = doc[mResult].data();
	while (result) {
		std::cout << result->value.stl_str() << '\n';
		result = result->tl();
	}
	return 0;
}
