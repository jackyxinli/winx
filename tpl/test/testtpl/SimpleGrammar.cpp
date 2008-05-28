#include "stdafx.h"

using namespace tpl;

// -------------------------------------------------------------------------
// simple_grammar

// A simple grammar example.

// What we use:
//	* Rules: /assign(), %, real(), gr(','), skipws()
//	* Matching: tpl::simple::match()

void simple_grammar()
{
	simple::Allocator alloc;

	std::vector<double> values; // you can change vector to other stl containers.

	if ( simple::match(alloc, " -.1 , -0.1 , +32. , -22323.2e+12 ", real()/assign(values) % gr(','), skipws()) )
	{
		for (std::vector<double>::iterator it = values.begin(); it != values.end(); ++it)
		{
			std::cout << *it << "\n";
		}
	}
}

// -------------------------------------------------------------------------
