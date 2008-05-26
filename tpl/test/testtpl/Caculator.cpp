#include "stdafx.h"

using namespace tpl;

// -------------------------------------------------------------------------
// calculate

template <class Type, class OperaT>
class Calc
{
private:
	typedef AssignmentTypeTraits<Type> Tr_;

	OperaT m_opera;

public:
	typedef Type value_type;
	typedef typename Tr_::assignment_type assignment_type;

public:
	Calc(const OperaT& opera)
		: m_opera(opera) {
	}

	template <class Iterator>
	void TPL_CALL operator()(Iterator pos, Iterator pos2) const {
		value_type val = value_type();
		assignment_type assig(val);
		assig(pos, pos2);
		m_opera(val);
	}
};

template <class Type>
class Lambda
{
public:
	template <class OperaT>
	Action<Calc<Type, OperaT> > TPL_CALL operator[](const OperaT& op) const {
		return Action<Calc<Type, OperaT> >(op);
	}
};

#pragma warning(disable:4819)
#include <boost/lambda/lambda.hpp>

using namespace boost::lambda;

void calculate()
{
	typedef simple_ impl;

	// ---- define rules ----

	impl::Allocator alloc;

	Lambda<double> exec;

	double vTerm = 0;
	double vFactor = 0;

	impl::RegExp rFactor( alloc, real()/info("push") );

	impl::RegExp rMul( alloc, '*' + rFactor/exec[var(vFactor) *= _1] );
	impl::RegExp rDiv( alloc, '/' + rFactor/exec[var(vFactor) /= _1] );
	impl::RegExp rTerm( alloc, rFactor/&vFactor + *(rMul | rDiv) );

	impl::RegExp rAdd( alloc, '+' + rTerm/exec[var(vTerm) += var(vFactor)] );
	impl::RegExp rSub( alloc, '-' + rTerm/exec[var(vTerm) -= var(vFactor)] );
	impl::RegExp rExpr( alloc, rTerm/&vTerm + *(rAdd | rSub) + eos() );

	// ---- do match ----

	if (!impl::match("1+2/3*4-6", rExpr)) {
		std::cout << "invalid expression!\n";
		return;
	}

	std::cout << vTerm << "\n";
}

// -------------------------------------------------------------------------
