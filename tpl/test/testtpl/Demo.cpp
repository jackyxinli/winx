#include "stdafx.h"

// -------------------------------------------------------------------------
// demo

// What we use:
//	* Rules: RegExp, +, /, %, c_symbol(), real(), skipws()
//	* Tags: LeafMark, NodeMark, double*
//	* Matching: RegExp::match()
//	* Result Processing: Document::all(), select(), []

void demo()
{
	using namespace tpl;

	std::BlockPool recycle;
	std::ScopeAlloc alloc(recycle);

	// ---- define source ----

	char buf[] = "x1=-.1, x2=+32., x3=-22323.2e+12";
	impl::Source source(buf, buf+sizeof(buf));

	// ---- define rules ----

	double dblValue;
	impl::LeafMark tagSym;
	impl::LeafMark tagValue;
	impl::LeafMark tagPairValue;
	impl::NodeMark tagPair;

	impl::RegExp rPair( alloc, (c_symbol()/tagSym + '=' + real()/tagValue/&dblValue)/tagPair/tagPairValue );
	impl::RegExp rDoc( alloc, (skipws() + rPair) % ',' );

	// ---- do match ----

	impl::Document doc;
	impl::Context context(alloc, doc);

	if (!rDoc.match(source, context)) {
		std::cout << "match failed\n";
		return;
	}

	// ---- print dblValue ----

	std::cout << "DoubleValue: " << dblValue << "\n"; // guess what it is!

	// ---- print all data ----

	for (impl::Document::cons it = doc.all(); it; it = it.tl())
	{
		impl::Document::value_type item = it.hd();
		if (item.key() == tagPair) {
			impl::Document::node_data vPair = item.node();
			impl::Document::leaf_data vSym = vPair[tagSym];
			impl::Document::leaf_data vValue = vPair[tagValue];
			std::cout << vSym.stl_str() << ": " << vValue.stl_str() << "\n";
		}
		else {
			impl::Document::leaf_data vPair = item.leaf();
			std::cout << "Pair: " << vPair.stl_str() << "\n";
		}
	}

	// ---- select what we are interested in and print ----

	std::cout << "\n";
	impl::Document::leaf_cons it2 = doc.select(alloc, tagPairValue);
	for (; it2; it2 = it2.tl()) {
		impl::Document::leaf_data vPair = *it2;
		std::cout << "Pair: " << vPair.stl_str() << "\n";		
	}
}

// -------------------------------------------------------------------------
