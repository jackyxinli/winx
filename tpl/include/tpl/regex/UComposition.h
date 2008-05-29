/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: tpl/regex/UComposition.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_UCOMPOSITION_H
#define TPL_REGEX_UCOMPOSITION_H

#if (0)
#define TPL_NO_REDUCE_NAME
#endif

#ifndef TPL_REGEX_OPERATOR_H
#include "Operator.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class UAnd

// Usage: UAnd<Rule1,Rule2>		--- means: Rule1 Rule2 (but no transaction)
// Note: Rule1 and Rule2 cannot contain any Mark operators (See <tpl/regex/Mark.h>)
// If Rule1 or Rule2 contains Mark operators, you should use And operator instead of UAnd.

template <class RegExT1, class RegExT2>
class UAnd // Rule1 * Rule2
{
private:
	RegExT1 m_x;
	RegExT2 m_y;

public:
	UAnd() {
		TPL_ASSERT(RegExT1::character == 0 && RegExT2::character == 0);
	}
	UAnd(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {
		TPL_ASSERT(RegExT1::character == 0 && RegExT2::character == 0);
	}

public:
	enum { character = 0 };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return m_x.match(ar, context) && m_y.match(ar, context);
	}
};

// -------------------------------------------------------------------------
// class URepeat

template <class RegExT, unsigned nMin, unsigned nMax = UINT_MAX>
class URepeat // Rule{nMin, nMax}
{
private:
	RegExT m_x;

public:
	URepeat() {
		TPL_ASSERT(RegExT::character == 0);
	}
	URepeat(const RegExT& x) : m_x(x) {
		TPL_ASSERT(RegExT::character == 0);
	}

public:
	enum { character = 0 };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		unsigned n;
		for (n = 0; n < nMax; ++n) {
			if (!m_x.match(ar, context))
				break;
		}
		return n >= nMin;
	}
};

// -------------------------------------------------------------------------
// class UGuard

// Usage: UGuad<Rule>	--- usually Rule contains some UAnd operators.
// Note: Rule cannot contain any Mark operators (See <tpl/regex/Mark.h>)

template <class RegExT, int uVType>
class UGuard
{
private:
	RegExT m_x;

public:
	UGuard() {
		TPL_ASSERT(RegExT::character == 0);
	}
	UGuard(const RegExT& x) : m_x(x) {
		TPL_ASSERT(RegExT::character == 0);
	}

public:
	enum { character = 0 };
	enum { vtype = uVType };

	typedef AutoConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename SourceT::iterator pos = ar.position();
		if (m_x.match(ar, context))
			return true;
		ar.seek(pos);
		return false;
	}
};

// -------------------------------------------------------------------------
// TPL_REGEX_GUARD, TPL_REGEX_GUARD0

#ifndef TPL_REGEX_GUARD
#if defined(TPL_NO_REDUCE_NAME)
#define TPL_REGEX_GUARD(UnGuard, Guard, uVType)								\
	typedef UGuard<UnGuard, uVType> Guard;
#else
#define TPL_REGEX_GUARD(UnGuard, Guard, uVType)								\
	class Guard : public UGuard<UnGuard, uVType> {};
#endif
#endif

#ifndef TPL_REGEX_GUARD0
#define TPL_REGEX_GUARD0(UnGuard, Guard, uVType)							\
	class Guard : public UnGuard {											\
	public:																	\
		enum { vtype = uVType };											\
		typedef AutoConvertible convertible_type;							\
	};
#endif

// =========================================================================
// function strict_eol(), eol()

// strict_eol = \r\n? | \n
// eol = strict_eol | eof

typedef Ch<'\r'> Return;
typedef Ch<'\n'> NewLine;

typedef Or<UAnd<Return, Repeat01<NewLine> >, NewLine> StrictEol;
typedef Or<StrictEol, Eof> Eol;

TPL_REGEX_GUARD0(StrictEol, StrictEolG, VTYPE_NONE)
TPL_REGEX_GUARD0(Eol, EolG, VTYPE_NONE)

inline Rule<StrictEolG> TPL_CALL strict_eol() {
	return Rule<StrictEolG>();
}

inline Rule<EolG> TPL_CALL eol() {
	return Rule<EolG>();
}

// =========================================================================
// function c_symbol, xml_symbol, etc.

// Usage: c_symbol()		--- means: matching a CSymbol. that is: [a-zA-Z_][0-9a-zA-Z_]*
// Usage: xml_symbol()		--- means: matching a XmlSymbol. that is: [a-zA-Z_][0-9a-zA-Z_\-]*

typedef UAnd<CSymbolFirstChar, Repeat0<CSymbolNextChar> > CSymbol;
typedef UAnd<XmlSymbolFirstChar, Repeat0<XmlSymbolNextChar> > XmlSymbol;

TPL_REGEX_GUARD0(CSymbol, CSymbolG, VTYPE_NONE)
TPL_REGEX_GUARD0(XmlSymbol, XmlSymbolG, VTYPE_NONE)

inline Rule<CSymbolG> TPL_CALL c_symbol()
{
	return Rule<CSymbolG>();
}

inline Rule<XmlSymbolG> TPL_CALL xml_symbol()
{
	return Rule<XmlSymbolG>();
}

// -------------------------------------------------------------------------
// function u_integer, integer, etc.

// Usage: u_integer()		--- means: matching an Unsigned Integer. that is: d+
// Usage: integer()			--- means: matching an Integer. that is: [+-]?d+

typedef Repeat1<Digit> UInteger;

typedef Ch<'+', '-'> Sign;
typedef UAnd<Repeat01<Sign>, UInteger> Integer; // [+-]?d+

TPL_REGEX_GUARD0(UInteger, UIntegerG, VTYPE_U_INTEGER)
TPL_REGEX_GUARD(Integer, IntegerG, VTYPE_INTEGER)

inline Rule<UIntegerG> TPL_CALL u_integer()
{
	return Rule<UIntegerG>();
}

inline Rule<IntegerG> TPL_CALL integer()
{
	return Rule<IntegerG>();
}

// -------------------------------------------------------------------------
// function strict_fraction, fraction, etc.

typedef Ch<'.'> DecimalPointer;

typedef UAnd<DecimalPointer, UInteger> DecimalPointerStarted_; // \.d+
typedef UAnd<DecimalPointer, Repeat0<Digit> > DecimalPointerStarted0_; // \.d*

typedef UAnd<UInteger, DecimalPointerStarted0_> DigitStartedUStrictFraction_; // d+\.d*
typedef Or<DecimalPointerStarted_, DigitStartedUStrictFraction_> UStrictFraction; // \.d+ | d+\.d*

typedef UAnd<UInteger, Repeat01<DecimalPointerStarted0_> > DigitStartedUFraction_; // d+(\.d*)?
typedef Or<DecimalPointerStarted_, DigitStartedUFraction_> UFraction; // \.d+ | d+(\.d*)?

typedef UAnd<Repeat01<Sign>, UStrictFraction> StrictFraction;
typedef UAnd<Repeat01<Sign>, UFraction> Fraction;

TPL_REGEX_GUARD(UStrictFraction, UStrictFractionG, VTYPE_U_REAL)
TPL_REGEX_GUARD(StrictFraction, StrictFractionG, VTYPE_REAL)
TPL_REGEX_GUARD(UFraction, UFractionG, VTYPE_U_REAL)
TPL_REGEX_GUARD(Fraction, FractionG, VTYPE_REAL)

inline Rule<UStrictFractionG> TPL_CALL u_strict_fraction()
{
	return Rule<UStrictFractionG>();
}

inline Rule<StrictFractionG> TPL_CALL strict_fraction()
{
	return Rule<StrictFractionG>();
}

inline Rule<UFractionG> TPL_CALL u_fraction()
{
	return Rule<UFractionG>();
}

inline Rule<FractionG> TPL_CALL fraction()
{
	return Rule<FractionG>();
}

// -------------------------------------------------------------------------
// function real, strict_real, etc.

// Usage: real()			--- means: matching a Real Number or an Integer
// Usage: strict_real()		--- means: matching a Real Number (NOT including an Integer)

typedef Ch<'E', 'e'> ExponentSign;

typedef UAnd<ExponentSign, Integer> Exponent; // [Ee][+-]?d+

typedef UAnd<DecimalPointerStarted_, Repeat01<Exponent> > DPS_UStrictReal_;

typedef UAnd<DecimalPointerStarted0_, Repeat01<Exponent> > DPS0_UStrictReal_;
typedef Or<DPS0_UStrictReal_, Exponent> DS_UStrictRealSuffix_;
typedef UAnd<UInteger, DS_UStrictRealSuffix_> DigitStartUStrictReal_;

typedef Or<DPS_UStrictReal_, DigitStartUStrictReal_> UStrictReal;
typedef UAnd<Repeat01<Sign>, UStrictReal> StrictReal;

typedef UAnd<UFraction, Repeat01<Exponent> > UReal;
typedef UAnd<Repeat01<Sign>, UReal> Real;

TPL_REGEX_GUARD(UStrictReal, UStrictRealG, VTYPE_U_REAL)
TPL_REGEX_GUARD(StrictReal, StrictRealG, VTYPE_REAL)
TPL_REGEX_GUARD(UReal, URealG, VTYPE_U_REAL)
TPL_REGEX_GUARD(Real, RealG, VTYPE_REAL)

inline Rule<UStrictRealG> TPL_CALL u_strict_real()
{
	return Rule<UStrictRealG>();
}

inline Rule<StrictRealG> TPL_CALL strict_real()
{
	return Rule<StrictRealG>();
}

inline Rule<URealG> TPL_CALL u_real()
{
	return Rule<URealG>();
}

inline Rule<RealG> TPL_CALL real()
{
	return Rule<RealG>();
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_UCOMPOSITION_H */
