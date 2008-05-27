#include "stdafx.h"

// -------------------------------------------------------------------------
// circuit

// Note:
//	if two rules A, B circularly refer to each other (or a rule A refer to itself),
//	we need use ref() function to break the circuit.

void circuit()
{
	using namespace tpl;

	typedef simple impl;

	// ---- define source ----

	impl::Source source("10,97,37");

	// ---- define rules ----

	impl::Allocator alloc;

	std::vector<int> values;
	impl::Rule rDoc;
	rDoc.assign( alloc, integer()/&values + !(',' + ref(rDoc)) );

	// This is only an example. A better way to do the same thing is:
	//	rDoc.assign( alloc, integer()/&values % ',' );

	// ---- do match ----

	impl::Context context;
	if (!rDoc.match(source, context)) {
		std::cout << "match failed\n";
		return;
	}

	// ---- print all values ----

	for (std::vector<int>::const_iterator it = values.begin(); it != values.end(); ++it) {
		std::cout << *it << "\n";
	}
}

// -------------------------------------------------------------------------
