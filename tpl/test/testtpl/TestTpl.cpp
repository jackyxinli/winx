
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

	char buf[] = "x1=1, x2=32, x3=22323";
	Source source(buf, buf+sizeof(buf));

	// ---- define rules ----

	LeafMark tSym = "symbol";
	LeafMark tValue = "value";
	NodeMark tPair = "pair";

	RegEx rDoc(alloc, tPair/(skipws() + tSym/csymbol() + '=' + tValue/integer()) % ',');

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
		std::cout << vSym.stl_str() << "," << vValue.stl_str() << "\n";
		itPair = itPair.tl();
	}
}

int main()
{
	using namespace tpl;

	example();
	return 0;

	// ---- define rules ----

	Allocator alloc;

	RegEx a(alloc, ch('a'));
	
	RegEx b;
	b.assign(alloc, ch('b'));

	RegEx a_or_b(alloc, a | b);
	
	RegEx a_or_b_or_c(alloc, a_or_b | 'c');

	RegEx a_or_b_or_d(alloc, 'd' | a_or_b);

	LeafMark mVal("value");
	LeafMark mSym("symbol");
	NodeMark mNode("node");

	SimpleRegEx three_word(alloc, mVal = 'd' + a_or_b_or_c + 'b');
	SimpleRegEx repeated(alloc, mNode = repeat<2>(three_word));

	SimpleRegEx rDoc(alloc, *repeated + mSym/csymbol());

	// ---- parse source ----

	char buf[] = "dcbdabdbbdcbcdef9g";
	Source source(buf, buf+sizeof(buf));

	Document doc;
	Context context(alloc, doc);

	bool fail = a_or_b_or_c.match(source, context);
	bool ok = rDoc.match(source, context);

	Document::leaf_cons sym = doc.select(alloc, mSym);
	std::cout << mSym.tag << ": " << sym.hd().data().stl_str() << "\n";

	Document::node_cons result = doc.select(alloc, mNode);
	std::cout << mNode.tag << ":\n";
	while (result) {
		Document::node_data node = result.hd().data();
		Document::leaf_cons leaf = node.select(alloc, mVal);
		std::cout << "  " << mVal.tag << ":\n";
		while (leaf) {
			Document::leaf_data val = leaf.hd().data();
			std::cout << "    " << val.stl_str() << '\n';
			leaf = leaf.tl();
		}
		result = result.tl();
	}
	return 0;
}
