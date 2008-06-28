#define TPL_USE_AUTO_ALLOC
#include <iostream> 	// std::cout
#include <tpl/RegExp.h>
#include <tpl/boost/Lambda.h>

using namespace tpl;
using namespace boost::lambda;

int main()
{
	typedef SimpleImplementation impl;

	// ---- define rules ----

	impl::Allocator alloc;

	Lambda<double> exec;

	TPL_LAMBDA_LOCAL(double, term);
	TPL_LAMBDA_LOCAL(double, factor);
	
	impl::Rule rMul( alloc, '*' + real()/exec[factor *= _1] );
	impl::Rule rDiv( alloc, '/' + real()/exec[factor /= _1] );
	impl::Rule rTerm( alloc, real()/assign(factor) + *(rMul | rDiv) );

	impl::Rule rAdd( alloc, '+' + rTerm/exec[term += factor] );
	impl::Rule rSub( alloc, '-' + rTerm/exec[term -= factor] );
	impl::Rule rExpr( alloc, rTerm/exec[term = factor] + *(rAdd | rSub) );

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

