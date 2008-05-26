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
	
	impl::RegExp rMul( alloc, '*' + real()/exec[factor *= _1] );
	impl::RegExp rDiv( alloc, '/' + real()/exec[factor /= _1] );
	impl::RegExp rTerm( alloc, real()/assign(factor) + *(rMul | rDiv) );

	impl::RegExp rAdd( alloc, '+' + rTerm/exec[term += factor] );
	impl::RegExp rSub( alloc, '-' + rTerm/exec[term -= factor] );
	impl::RegExp rExpr( alloc, rTerm/exec[term = factor] + *(rAdd | rSub) );
	
	// ---- do match ----
	
	for (;;)
	{
		std::string strExp;
		std::cout << "input an expression (q to quit): ";
		std::getline(std::cin, strExp);
		
		if (strExp == "q")
			break;

		if (!impl::match(strExp.c_str(), rExpr + eos()))
			std::cout << "invalid expression!\n";
		else
			std::cout << term() << "\n";
	}
}

// -------------------------------------------------------------------------

