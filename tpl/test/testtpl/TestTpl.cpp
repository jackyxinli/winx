
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

	LeafMark tagSym = "symbol";
	LeafMark tagValue = "value";
	LeafMark tagPairValue;
	NodeMark tagPair = "pair";

	RegExp rPair(alloc, (csymbol()/tagSym + '=' + real()/tagValue)/tagPair/tagPairValue);
	RegExp rDoc(alloc, (skipws() + rPair) % ',');

	// ---- do match ----

	Document doc;
	Context context(alloc, doc);

	if (!rDoc.match(source, context)) {
		std::cout << "match failed\n";
		return;
	}

	// ---- print all data ----

	Document::cons it = doc.all();
	while (it) {
		Document::value_type item = it.hd();
		if (item.key() == tagPair) {
			Document::node_data vPair = item.node();
			Document::leaf_data vSym = vPair[tagSym];
			Document::leaf_data vValue = vPair[tagValue];
			std::cout << vSym.stl_str() << ": " << vValue.stl_str() << "\n";
		}
		else {
			Document::leaf_data vPair = item.leaf();
			std::cout << "Pair: " << vPair.stl_str() << "\n";
		}
		it = it.tl();
	}

	// ---- select what we are interested in and print ----

	std::cout << "\n";
	Document::leaf_cons it2 = doc.select(alloc, tagPairValue);
	while (it2) {
		Document::leaf_data vPair = *it2;
		std::cout << "Pair: " << vPair.stl_str() << "\n";		
		it2 = it2.tl();
	}
}

int main()
{
	example();
	return 0;
}
