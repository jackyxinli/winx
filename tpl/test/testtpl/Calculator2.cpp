#include "stdafx.h"
#include <tpl/ext/Calculator.h>
#include <cmath>

using namespace tpl;

void calculate2()
{
	typedef SimpleImplementation impl;

	// ---- define rules ----

	impl::Allocator alloc;

	std::stack<double> stk;

	impl::Rule rFactor;

	impl::Rule rMul( alloc, '*' + ref(rFactor)/calc<std::multiplies>(stk) );
	impl::Rule rDiv( alloc, '/' + ref(rFactor)/calc<std::divides>(stk) );
	impl::Rule rTerm( alloc, ref(rFactor) + *(rMul | rDiv) );

	impl::Rule rAdd( alloc, '+' + rTerm/calc<std::plus>(stk) );
	impl::Rule rSub( alloc, '-' + rTerm/calc<std::minus>(stk) );
	impl::Rule rExpr( alloc, rTerm + *(rAdd | rSub) );
	
	impl::Rule rFun( alloc, "sin"/calc(stk, sin) | "cos"/calc(stk, cos) | "pow"/calc(stk, pow) );

	rFactor.assign( alloc, 
		real()/assign(stk) |
		'-' + ref(rFactor)/calc<std::negate>(stk) |
		'(' + rExpr + ')' |
		(c_symbol() + '('+ rExpr % ',' + ')')/rFun |
		'+' + ref(rFactor) );

	// ---- do match ----
	
	for (;;)
	{
		std::string strExp;
		std::cout << "input an expression (q to quit): ";
		std::getline(std::cin, strExp);
		
		if (strExp == "q")
			break;

		try {
			while (!stk.empty())
				stk.pop();
			if (!impl::match(strExp.c_str(), rExpr + eos()))
				std::cout << ">>> ERROR: invalid expression!\n";
			else
				std::cout << stk.top() << "\n";
		}
		catch (std::logic_error e) {
			std::cout << ">>> ERROR: " << e.what() << "\n";
		}
	}
}

// -------------------------------------------------------------------------

