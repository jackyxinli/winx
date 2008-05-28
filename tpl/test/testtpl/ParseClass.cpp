#include "stdafx.h"

// -------------------------------------------------------------------------
// parse_class

// What we use:
//	* Rules: Rule, +, /, %, c_symbol(), ws(), skipws()
//	* Restrictions: eq_s()
//	* Tags: Leaf*, LeafMark
//	* Matching: Rule::match()
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

	impl::Rule rDecl( alloc, c_symbol()/eq_s("class", "struct") + ws() + c_symbol()/assign(className) );
	impl::Rule rBody( alloc, '{' + skipws() + '}' + skipws() + ';' );
	impl::MarkedRule rBase( alloc, c_symbol()/eq_s("public", "private", "protected")/tagAccess + ws() + c_symbol()/tagBaseClass );
	impl::MarkedRule rBases( alloc, rBase % (skipws() + ',' + skipws()) );
	impl::MarkedRule rDoc( alloc, rDecl + skipws() + ':' + skipws() + rBases + skipws() + rBody );

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
