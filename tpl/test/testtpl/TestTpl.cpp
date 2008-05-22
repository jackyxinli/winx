
#include "stdafx.h"
#include <tpl/RegEx.h>
#include <stdext/LinkLib.h>
#include <iostream>

// -------------------------------------------------------------------------

void example1()
{
	using namespace tpl;

	std::BlockPool recycle;
	std::ScopeAlloc alloc(recycle);

	// ---- define source ----

	char buf[] = "x1=-.1, x2=+32., x3=-22323.2e+12";
	Source source(buf, buf+sizeof(buf));

	// ---- define rules ----

	LeafMark tagSym;
	LeafMark tagValue;
	LeafMark tagPairValue;
	NodeMark tagPair;

	RegExp rPair(alloc, (c_symbol()/tagSym + '=' + real()/tagValue)/tagPair/tagPairValue);
	RegExp rDoc(alloc, (skipws() + rPair) % ',');

	// ---- do match ----

	Document doc;
	Context context(alloc, doc);

	if (!rDoc.match(source, context)) {
		std::cout << "match failed\n";
		return;
	}

	// ---- print all data ----

	for (Document::cons it = doc.all(); it; it = it.tl())
	{
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
	}

	// ---- select what we are interested in and print ----

	std::cout << "\n";
	Document::leaf_cons it2 = doc.select(alloc, tagPairValue);
	for (; it2; it2 = it2.tl()) {
		Document::leaf_data vPair = *it2;
		std::cout << "Pair: " << vPair.stl_str() << "\n";		
	}
}

void example2()
{
	using namespace tpl;

	std::BlockPool recycle;
	std::ScopeAlloc alloc(recycle);

	// ---- define source ----

	char buf[] = "<tag prop-1=1 prop-2 = -2>text</tag>";
	Source source(buf, buf+sizeof(buf));

	// ---- define rules ----

	LeafMark tagProp = 1;
	LeafMark tagValue = 2;
	LeafMark tagText = 3;
	NodeMark tagProps;

	RegExp rProp(alloc, xml_symbol()/tagProp + skipws() + '=' + skipws() + integer()/tagValue);
	RegExp rProps(alloc, rProp % ws() / tagProps);
	RegExp rTagStart(alloc, '<' + xml_symbol()/"tag" + ws() + rProps + '>');
	RegExp rDoc(alloc, rTagStart + c_symbol()/tagText + "</tag>");

	// ---- do match ----

	Document doc;
	Context context(alloc, doc);

	if (!rDoc.match(source, context)) {
		std::cout << "match failed\n";
		return;
	}

	// ---- print text ----

	Document::leaf_data vText = doc[tagText];
	std::cout << "Text: " << vText.stl_str() << "\n";

	// ---- print properties ----

	Document::node_data vProps = doc[tagProps];
	for (Document::cons it = vProps.all(); it; it = it.tl())
	{
		Document::value_type item = it.hd();
		std::cout << (item.key() == tagProp ? "Prop: " : "Value: ");
		
		Document::leaf_data vData = item.leaf();
		std::cout << vData.stl_str() << "\n";
	}
}

int main()
{
	example2();
	return 0;
}
