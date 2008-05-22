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

#ifndef TPL_REGEX_COMPOSITION_H
#include "Composition.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// operator*

// Usage: Rule1 * Rule2		--- means: Rule1 Rule2 (but no transaction)

template <class RegExT1, class RegExT2>
class UAnd // Rule1 * Rule2
{
private:
	RegExT1 m_x;
	RegExT2 m_y;

public:
	UAnd() {}
	UAnd(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename SourceT::iterator pos = ar.position();
		if (m_x.match(ar, context)) {
			if (m_y.match(ar, context))
				return true;
		}
		ar.seek(pos);
		return false;
	}
};

template <class T1, class T2> __forceinline
Exp<UAnd<T1, T2> > TPL_CALL operator*(const Exp<T1>& x, const Exp<T2>& y) {
	return Exp<UAnd<T1, T2> >(x, y);
}

// -------------------------------------------------------------------------
// function c_symbol, xml_symbol, etc.

// Usage: c_symbol()		--- means: matching a CSymbol. that is: [a-zA-Z_][0-9a-zA-Z_]*
// Usage: xml_symbol()		--- means: matching a XmlSymbol. that is: [a-zA-Z_][0-9a-zA-Z_\-]*

typedef UAnd<CSymbolFirstChar, Repeat0<CSymbolNextChar> > CSymbol;
typedef UAnd<XmlSymbolFirstChar, Repeat0<XmlSymbolNextChar> > XmlSymbol;

inline Exp<CSymbol> TPL_CALL c_symbol()
{
	return Exp<CSymbol>();
}

inline Exp<XmlSymbol> TPL_CALL xml_symbol()
{
	return Exp<XmlSymbol>();
}

// -------------------------------------------------------------------------
// function c_symbol, integer, etc.

// Usage: integer()			--- means: matching an Integer. that is: [+-]?d+
// Usage: real()			--- means: matching a Real Number or an Integer
// Usage: strict_real()		--- means: matching a Real Number (NOT including an Integer)
// Usage: ...

typedef Ch<'.'> DecimalPointer;
typedef Ch<'+', '-'> Sign;
typedef Ch<'E', 'e'> ExponentSign;

typedef UAnd<Repeat01<Sign>, UInteger> Integer; // [+-]?d+

typedef UAnd<UAnd<Repeat0<Digit>, DecimalPointer>, UInteger> UStrictFraction1_;
typedef UAnd<UInteger, DecimalPointer> UStrictFraction2_;
typedef Or<UStrictFraction1_, UStrictFraction2_> UStrictFraction; // d*\.d+ | d+\.

typedef UAnd<ExponentSign, Integer> Exponent; // [Ee][+-]?d+

typedef UAnd<UStrictFraction, Repeat01<Exponent> > UStrictReal1_;
typedef UAnd<UInteger, Exponent> UStrictReal2_;
typedef Or<UStrictReal1_, UStrictReal2_> UStrictReal;

typedef Or<UStrictFraction, UInteger> UFraction;
typedef Or<UStrictReal, UInteger> UReal;

typedef UAnd<Repeat01<Sign>, UStrictFraction> StrictFraction;
typedef UAnd<Repeat01<Sign>, UStrictReal> StrictReal;

typedef UAnd<Repeat01<Sign>, UFraction> Fraction;
typedef UAnd<Repeat01<Sign>, UReal> Real;

inline Exp<Integer> TPL_CALL integer()
{
	return Exp<Integer>();
}

inline Exp<UStrictFraction> TPL_CALL u_strict_fraction()
{
	return Exp<UStrictFraction>();
}

inline Exp<StrictFraction> TPL_CALL strict_fraction()
{
	return Exp<StrictFraction>();
}

inline Exp<UFraction> TPL_CALL u_fraction()
{
	return Exp<UFraction>();
}

inline Exp<Fraction> TPL_CALL fraction()
{
	return Exp<Fraction>();
}

inline Exp<UStrictReal> TPL_CALL u_strict_real()
{
	return Exp<UStrictReal>();
}

inline Exp<StrictReal> TPL_CALL strict_real()
{
	return Exp<StrictReal>();
}

inline Exp<UReal> TPL_CALL u_real()
{
	return Exp<UReal>();
}

inline Exp<Real> TPL_CALL real()
{
	return Exp<Real>();
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_UCOMPOSITION_H */
