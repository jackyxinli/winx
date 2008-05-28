#include "stdafx.h"

// -------------------------------------------------------------------------
// circuit

// Note:
//	if two rules A, B circularly refer to each other (or a rule A refer to itself),
//	we need use Rule::Var to break the circuit.

void circuit()
{
	using namespace tpl;

	typedef simple impl;

	// ---- define rules ----

	impl::Allocator alloc;

	std::vector<int> values;

	impl::Rule::Var rDoc;
	rDoc.assign( alloc, integer()/assign(values) + !(',' + rDoc) );

	// This is only an example. A better way to do the same thing is:
	//	rDoc.assign( alloc, integer()/assign(values) % ',' );

	// ---- do match ----

	if (impl::match("10,97,37", rDoc))
	{
		for (std::vector<int>::const_iterator it = values.begin(); it != values.end(); ++it) {
			std::cout << *it << "\n";
		}
	}
}

// -------------------------------------------------------------------------
