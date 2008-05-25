#include "stdafx.h"

using namespace tpl;

// -------------------------------------------------------------------------
// calculate

template <template <class Ty> class Fn>
class Op
{
private:
	double& result_;
	const double& operand_;
	Fn<double> op_;

public:
	Op(double& result, const double& operand)
		: result_(result), operand_(operand) {
	}

	void operator()() const {
		double old_ = result_;
		result_ = op_(old_, operand_);
		std::cout << old_ << " op " << operand_ << " = " << result_ << '\n';
	}
};

template <template <class Ty> class Fn>
inline SimpleAction<Op<Fn> > op(double& result, const double& operand_) {
	return SimpleAction<Op<Fn> >(result, operand_);
}

#define mul op<std::multiplies>
#define div op<std::divides>
#define add op<std::plus>
#define sub op<std::minus>

void calculate()
{
	typedef simple_ impl;

	// ---- define rules ----

	impl::Allocator alloc;

	double vTerm = 0;
	double vFactor = 0;
	double vTemp = 0;

	impl::RegExp rFactor( alloc, real() );

	impl::RegExp rMul( alloc, '*' + rFactor/&vTemp/info("push")/mul(vFactor, vTemp) );
	impl::RegExp rDiv( alloc, '/' + rFactor/&vTemp/info("push")/div(vFactor, vTemp) );
	impl::RegExp rTerm( alloc, rFactor/&vFactor/info("push") + *(rMul | rDiv) );

	impl::RegExp rAdd( alloc, '+' + rTerm/add(vTerm, vFactor) );
	impl::RegExp rSub( alloc, '-' + rTerm/sub(vTerm, vFactor) );
	impl::RegExp rExpr( alloc, rTerm/&vTerm/info("push") + *(rAdd | rSub) + eos() );

	// ---- do match ----

	if (!impl::match("1+2/3*4-6", rExpr)) {
		std::cout << "invalid expression!\n";
		return;
	}

	std::cout << vTerm << "\n";
}

// -------------------------------------------------------------------------
