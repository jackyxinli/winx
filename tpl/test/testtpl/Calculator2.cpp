#include "stdafx.h"
#include <tpl/ext/Calculator.h>

using namespace tpl;

void calculate2()
{
	typedef SimpleImplementation impl;

	// ---- define rules ----

	impl::Allocator alloc;

	std::stack<double> stk;

	impl::RegExp rFactor;

	impl::RegExp rMul( alloc, '*' + ref(rFactor)/calc<std::multiplies>(stk) );
	impl::RegExp rDiv( alloc, '/' + ref(rFactor)/calc<std::divides>(stk) );
	impl::RegExp rTerm( alloc, ref(rFactor) + *(rMul | rDiv) );

	impl::RegExp rAdd( alloc, '+' + rTerm/calc<std::plus>(stk) );
	impl::RegExp rSub( alloc, '-' + rTerm/calc<std::minus>(stk) );
	impl::RegExp rExpr( alloc, rTerm + *(rAdd | rSub) );
	
	rFactor.assign( alloc, 
		u_real()/assign(stk) |
		'(' + rExpr + ')' |
		'-' + ref(rFactor)/calc<std::negate>(stk) |
		'+' + ref(rFactor) );

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
			std::cout << stk.top() << "\n";
	}
}

// -------------------------------------------------------------------------

