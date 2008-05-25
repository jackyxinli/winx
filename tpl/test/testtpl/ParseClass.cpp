#include "stdafx.h"

// -------------------------------------------------------------------------
// parse_class

// What we use:
//	* Rules: RegExp, +, /, %, c_symbol(), ws(), skipws()
//	* Restrictions: eq()
//	* Tags: Leaf*, LeafMark
//	* Matching: RegExp::match()
//	* Result Processing: Document::select(), []

void parse_class()
{
	using namespace tpl;

	impl::Allocator alloc;

	// ---- define source ----

	impl::Source source("struct Foo : public Base1, private Base2 {};");

	// ---- define rules ----

	impl::Leaf className;
	impl::LeafMark tagAccess;
	impl::LeafMark tagBaseClass;

	impl::RegExp rDecl( alloc, c_symbol()/eq("class", "struct") + ws() + c_symbol()/&className );
	impl::RegExp rBase( alloc, c_symbol()/eq("public", "private", "protected")/tagAccess + ws() + c_symbol()/tagBaseClass );
	impl::RegExp rBases( alloc, rBase % (skipws() + ',' + skipws()) );
	impl::RegExp rBody( alloc, '{' + skipws() + '}' + skipws() + ';' );
	impl::RegExp rDoc( alloc, rDecl + skipws() + ':' + skipws() + rBases + skipws() + rBody );

	// ---- do match ----

	impl::Document doc;
	impl::Context context(alloc, doc);

	if (!rDoc.match(source, context)) {
		std::cout << "match failed\n";
		return;
	}

	// ---- print class name ----

	std::cout << "\nClassName: " << className.stl_str() << "\n";

	// ---- print base class names ----

	impl::Document::leaf_cons it = doc.select(alloc, tagBaseClass);
	for (; it; it = it.tl()) {
		std::cout << (*it).stl_str() << "\n";
	}
}
