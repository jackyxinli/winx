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
// Module: tpl/regex/Composition.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_COMPOSITION_H
#define TPL_REGEX_COMPOSITION_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_BASIC_H
#include "Terminal.h"
#endif

#if !defined(_LIMITS_) && !defined(_LIMITS)
#include <climits>
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// operator>> (boost::spirit, xpressive compatible)
// operator+

// Usage: Rule1 >> Rule2
// Usage: Rule1 + Rule2

template <class RegExT1, class RegExT2>
class And // Rule1 Rule2
{
private:
	RegExT1 m_x;
	RegExT2 m_y;

public:
	And() {}
	And(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename ContextT::trans_type trans(ar, context);
		if (m_x.match(ar, context)) {
			if (m_y.match(ar, context))
				return true;
		}
		trans.rollback(ar);
		return false;
	}
};

template <class T1, class T2> __forceinline
Exp<And<T1, T2> > TPL_CALL operator>>(const Exp<T1>& x, const Exp<T2>& y) {
	return Exp<And<T1, T2> >(x, y);
}

template <class T1, class T2> __forceinline
Exp<And<T1, T2> > TPL_CALL operator+(const Exp<T1>& x, const Exp<T2>& y) {
	return Exp<And<T1, T2> >(x, y);
}

TPL_REGEX_CH_OP_(>>, And)
TPL_REGEX_CH_OP_(+, And)

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
		return m_x.match(ar, context) && m_y.match(ar, context);
	}
};

template <class T1, class T2> __forceinline
Exp<UAnd<T1, T2> > TPL_CALL operator*(const Exp<T1>& x, const Exp<T2>& y) {
	return Exp<UAnd<T1, T2> >(x, y);
}

// -------------------------------------------------------------------------
// operator|

// Usage: Rule1 | Rule2

template <class RegExT1, class RegExT2>
class Or // Rule1 | Rule2
{
private:
	RegExT1 m_x;
	RegExT2 m_y;

public:
	Or() {}
	Or(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		if (m_x.match(ar, context))
			return true;
		return m_y.match(ar, context);
	}
};

template <class T1, class T2> __forceinline
Exp<Or<T1, T2> > TPL_CALL operator|(const Exp<T1>& x, const Exp<T2>& y) {
	return Exp<Or<T1, T2> >(x, y);
}

TPL_REGEX_CH_OP_(|, Or)

// -------------------------------------------------------------------------
// operator* (Unary)

// Usage: *Rule

template <class RegExT>
class Repeat0 // Rule*
{
private:
	RegExT m_x;

public:
	Repeat0() {}
	Repeat0(const RegExT& x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		while (m_x.match(ar, context))
			;
		return true;
	}
};

template <class T1> __forceinline
Exp<Repeat0<T1> > TPL_CALL operator*(const Exp<T1>& x) {
	return Exp<Repeat0<T1> >(x);
}

// -------------------------------------------------------------------------
// operator+ (Unary)

// Usage: +Rule

template <class RegExT>
class Repeat1 // Rule+
{
private:
	RegExT m_x;

public:
	Repeat1() {}
	Repeat1(const RegExT& x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		unsigned n = 0;
		while (m_x.match(ar, context))
			++n;
		return n > 0;
	}
};

template <class T1> __forceinline
Exp<Repeat1<T1> > TPL_CALL operator+(const Exp<T1>& x) {
	return Exp<Repeat1<T1> >(x);
}

// -------------------------------------------------------------------------
// operator!

// Usage: !Rule
// Note: Sorry that we don't have operator?

template <class RegExT>
class Repeat01 // Rule?
{
private:
	RegExT m_x;

public:
	Repeat01() {}
	Repeat01(const RegExT& x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		m_x.match(ar, context);
		return true;
	}
};

template <class T1> __forceinline
Exp<Repeat01<T1> > TPL_CALL operator!(const Exp<T1>& x) {
	return Exp<Repeat01<T1> >(x);
}

// -------------------------------------------------------------------------
// function repeat

// Usage:
//	1. repeat<n>(Rule)		--- means: Rule{n}
//	2. repeat<n, m>(Rule)	--- means: Rule{n, m}
//	3. repeat<0, 1>(Rule)	--- means: Rule{0, 1}, that is: Rule?
//	4. repeat_ge<n>(Rule)	--- means: Rule{n,}

template <class RegExT, unsigned nMin, unsigned nMax = UINT_MAX>
class Repeat // Rule{nMin, nMax}
{
private:
	RegExT m_x;

public:
	Repeat() {}
	Repeat(const RegExT& x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		unsigned n;
		typename SourceT::iterator pos = ar.position();
		typename ContextT::trans_type trans(context);
		for (n = 0; n < nMax; ++n)
		{
			if (!m_x.match(ar, context))
				break;
		}
		if (n >= nMin)
			return true;
		ar.seek(pos);
		trans.rollback();
		return false;
	}
};

template <class RegExT>
class Repeat<RegExT, 0, 1> : public Repeat01<RegExT>
{
public:
	Repeat<RegExT, 0, 1>() {}
	Repeat<RegExT, 0, 1>(const RegExT& x) : Repeat01<RegExT>(x) {}
};

template <unsigned nMin, class T1> __forceinline
Exp<Repeat<T1, nMin> > TPL_CALL repeat_ge(const Exp<T1>& x) {
	return Exp<Repeat<T1, nMin> >(x);
}

template <unsigned nMin, class T1> __forceinline
Exp<Repeat<T1, nMin, nMin> > TPL_CALL repeat(const Exp<T1>& x) {
	return Exp<Repeat<T1, nMin, nMin> >(x);
}

template <unsigned nMin, unsigned nMax, class T1> __forceinline
Exp<Repeat<T1, nMin, nMax> > TPL_CALL repeat(const Exp<T1>& x) {
	return Exp<Repeat<T1, nMin, nMax> >(x);
}

// -------------------------------------------------------------------------
// operator%

// Usage: Rule1 % Rule2		--- means: Rule1 (Rule2 Rule1)*
// Example: Arg % ','		--- means: matching Function ArgList

template <class T1, class T2> __forceinline
Exp<And<T1, Repeat0<And<T2, T1> > > > TPL_CALL operator%(const Exp<T1>& x, const Exp<T2>& y)
{
	return x + *(y + x);
}

template <class T1> __forceinline
Exp<And<T1, Repeat0<And<ChEq, T1> > > > TPL_CALL operator%(const Exp<T1>& x, int c)
{
	Exp<ChEq> y = c;
	return x + *(y + x);
}

template <class T2> __forceinline
Exp<And<ChEq, Repeat0<And<T2, ChEq> > > > TPL_CALL operator%(int c, const Exp<T2>& y)
{
	Exp<ChEq> x = c;
	return x + *(y + x);
}

// -------------------------------------------------------------------------
// function csymbol, integer, etc.

// Usage: ws()				--- means: matching Whitespaces. that is: w+
// Usage: skipws()			--- means: skip Whitespaces. that is: w*
// Usage: csymbol()			--- means: matching a CSymbol. that is: [a-zA-Z_][0-9a-zA-Z_]*
// Usage: u_integer()		--- means: matching an Unsigned Integer. that is: d+
// Usage: integer()			--- means: matching an Integer. that is: [+-]?d+
// Usage: normal_real()		--- means: matching a normal Real Number (no exponent).
// Usage: real()			--- means: matching a Real Number (including exponent).

typedef Ch<'.'> DecimalPointer;
typedef Ch<'+', '-'> Sign;
typedef Ch<'E', 'e'> ExponentSign;

typedef Repeat0<Space> SkipWhiteSpaces; // w*
typedef Repeat1<Space> WhiteSpaces; // w+

typedef UAnd<CSymbolFirstChar, Repeat0<CSymbolNextChar> > CSymbol;
typedef UAnd<XmlSymbolFirstChar, Repeat0<XmlSymbolNextChar> > XmlSymbol;

typedef Repeat1<Digit> UInteger; // u := d+
typedef UAnd<Repeat01<Sign>, UInteger> Integer; // i := [+-]?d+

typedef UAnd<DecimalPointer, UInteger> DecimalPointerStarted_; // \.d+
typedef UAnd<DecimalPointer, Repeat0<Digit> > DecimalPointerStarted0_; // \.d*
typedef UAnd<UInteger, Repeat01<DecimalPointerStarted0_> > IntegerStarted_; // d+(\.d*)?

typedef Or<IntegerStarted_, DecimalPointerStarted_> UNormalReal_; // d+(\.d*)? | \.d+
typedef UAnd<Repeat01<Sign>, UNormalReal_> NormalReal;

typedef UAnd<ExponentSign, Integer> Exponent; // [Ee]<Integer>
typedef UAnd<NormalReal, Repeat01<Exponent> > Real; // <NormalReal><Exponent>?

inline Exp<WhiteSpaces> TPL_CALL ws()
{
	return Exp<WhiteSpaces>();
}

inline Exp<SkipWhiteSpaces> TPL_CALL skipws()
{
	return Exp<SkipWhiteSpaces>();
}

inline Exp<CSymbol> TPL_CALL c_symbol()
{
	return Exp<CSymbol>();
}

inline Exp<XmlSymbol> TPL_CALL xml_symbol()
{
	return Exp<XmlSymbol>();
}

inline Exp<UInteger> TPL_CALL u_integer()
{
	return Exp<UInteger>();
}

inline Exp<Integer> TPL_CALL integer()
{
	return Exp<Integer>();
}

inline Exp<NormalReal> TPL_CALL normal_real()
{
	return Exp<NormalReal>();
}

inline Exp<Real> TPL_CALL real()
{
	return Exp<Real>();
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_COMPOSITION_H */
