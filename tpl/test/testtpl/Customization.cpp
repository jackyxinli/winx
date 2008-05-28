#include "stdafx.h"

// -------------------------------------------------------------------------
// customize

// What we use:
//  * Customization
//	* Rules: Rule, Rule0, +, /, %, digit(), repeat(), skipws()
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

	typedef Customization<My, false> impl;

	// ---- define rules ----

	impl::Allocator alloc;

	char delim;
	std::vector<std::string> times;

	impl::Rule rYear( alloc, repeat<1, 2>(repeat<2>(digit())) );
	impl::Rule rMonOrDay( alloc, repeat<1, 2>(digit()) );
	impl::Rule rTime( alloc, rYear + ch('-', '/')/assign(delim) + rMonOrDay + ref(delim) + rMonOrDay );
	impl::Rule rDoc( alloc, rTime/assign(times) % (',' + skipws()) );

	// ---- do match ----

	if (impl::match("1928-10-1, 97-1-3, 1937/3/28", rDoc))
	{
		for (std::vector<std::string>::iterator it = times.begin(); it != times.end(); ++it) {
			std::cout << *it << "\n";
		}
	}
}

// -------------------------------------------------------------------------
