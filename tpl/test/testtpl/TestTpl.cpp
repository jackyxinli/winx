
#include "stdafx.h"
#include <tpl/RegEx.h>
#include <stdext/LinkLib.h>
#include <iostream>

void example()
{
	using namespace tpl;

	std::BlockPool recycle;
	std::ScopeAlloc alloc(recycle);

	// ---- define source ----

	char buf[] = "x1=-.1, x2=+32., x3=-22323.2e+12";
	Source source(buf, buf+sizeof(buf));

	// ---- define rules ----

	LeafMark tSym = "symbol";
	LeafMark tValue = "value";
	NodeMark tPair = "pair";

	RegExp rReal(alloc, real());
	RegExp rDoc(alloc, tPair/(skipws() + tSym/csymbol() + '=' + tValue/rReal) % ',');

	// ---- do match ----

	Document doc;
	Context context(alloc, doc);

	if (!rDoc.match(source, context)) {
		std::cout << "match failed\n";
		return;
	}

	Document::cons itPair = doc.all();
	while (itPair) {
		Document::node_data vPair = itPair.hd().node();
		Document::leaf_data vSym = vPair[tSym];
		Document::leaf_data vValue = vPair[tValue];
		std::cout << vSym.stl_str() << ": " << vValue.stl_str() << "\n";
		itPair = itPair.tl();
	}
}

int main()
{
	example();
	return 0;
}
