#include "stdafx.h"
#include <tpl/boost/Lambda.h>

using namespace tpl;
using namespace boost::lambda;

void calculate()
{
	typedef SimpleImplementation impl;

	// ---- define rules ----

	impl::Allocator alloc;

	TPL_LOCAL(double, term);
	TPL_LOCAL(double, factor);
	
	impl::RegExp rFactor( alloc, real() );

	impl::RegExp rMul( alloc, '*' + rFactor/exec[factor *= _1] );
	impl::RegExp rDiv( alloc, '/' + rFactor/exec[factor /= _1] );
	impl::RegExp rTerm( alloc, rFactor/exec[factor = _1] + *(rMul | rDiv) );

	impl::RegExp rAdd( alloc, '+' + rTerm/exec[term += factor] );
	impl::RegExp rSub( alloc, '-' + rTerm/exec[term -= factor] );
	impl::RegExp rExpr( alloc, rTerm/assign(term) + *(rAdd | rSub) + eos() );
	
	// ---- do match ----
	
	for (;;)
	{
		std::string strExp;
		std::cout << "input an expression (q to ): ";
		std::cin >> strExp;
		
		if (strExp == "q")
			break;

		if (!impl::match(strExp.c_str(), rExpr))
			std::cout << "invalid expression!\n";
		else
			std::cout << term() << "\n";
	}
}

// -------------------------------------------------------------------------

