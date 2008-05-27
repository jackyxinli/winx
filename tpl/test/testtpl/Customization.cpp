#include "stdafx.h"

// -------------------------------------------------------------------------
// customize

// What we use:
//  * Customization
//	* Rules: Rule, Rule0, +, /, %, digit(), repeat(), skipws()
//	* Tags: Leaf*, LeafMark
//	* Matching: Rule::match()
//	* Result Processing: Document::all()

class My : public tpl::policy::Default
{
public:
	typedef std::AutoFreeAlloc Allocator;
};

void customize()
{
	using namespace tpl;

	typedef Customization<My> impl;

	// ---- define source ----

	impl::Source source("1928-10-1, 97-1-3, 1937/3/28");

	// ---- define rules ----

	impl::Allocator alloc;

	impl::Leaf delim;
	impl::LeafMark tagTime;

	impl::Rule0 rYear( alloc, repeat<1, 2>(repeat<2>(digit())) );
	impl::Rule0 rMonOrDay( alloc, repeat<1, 2>(digit()) );
	impl::Rule0 rTime( alloc, rYear + ch('-', '/')/&delim + rMonOrDay + ref(delim) + rMonOrDay );
	impl::Rule rDoc( alloc, rTime/tagTime % (',' + skipws()) );

	// ---- do match ----

	impl::Document doc;
	impl::Context context(alloc, doc);

	if (!rDoc.match(source, context)) {
		std::cout << "match failed\n";
		return;
	}

	// ---- print time ----

	for (impl::Document::cons it = doc.all(); it; it = it.tl()) {
		std::cout << it.hd().leaf().stl_str() << "\n";
	}
}

// -------------------------------------------------------------------------
