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

	impl::Grammar::Var rFactor;

	impl::Grammar rMul( alloc, '*' + rFactor/calc<std::multiplies>(stk) );
	impl::Grammar rDiv( alloc, '/' + rFactor/calc<std::divides>(stk) );
	impl::Grammar rTerm( alloc, ref(rFactor) + *(rMul | rDiv) );

	impl::Grammar rAdd( alloc, '+' + rTerm/calc<std::plus>(stk) );
	impl::Grammar rSub( alloc, '-' + rTerm/calc<std::minus>(stk) );
	impl::Grammar rExpr( alloc, rTerm + *(rAdd | rSub) );
	
	impl::Rule rFun( alloc, "sin"/calc(stk, sin) | "cos"/calc(stk, cos) | "pow"/calc(stk, pow) );

	rFactor.assign( alloc, 
		real()/assign(stk) |
		'-' + rFactor/calc<std::negate>(stk) |
		'(' + rExpr + ')' |
		(gr(c_symbol()) + '(' + rExpr % ',' + ')')/(gr(rFun) + '(') |
		'+' + rFactor );

	// ---- do match ----
	
	for (;;)
	{
		std::string strExp;
		std::cout << "input an expression (q to quit): ";
		std::getline(std::cin, strExp);
		
		if (strExp == "q")
			break;

		try {
			while ( !stk.empty() )
				stk.pop();
			if ( !impl::match(alloc, strExp.c_str(), rExpr + eos(), skipws()) )
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
