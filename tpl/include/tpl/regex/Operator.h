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
// Module: tpl/regex/Operator.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_OPERATOR_H
#define TPL_REGEX_OPERATOR_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_TERMINAL_H
#include "Terminal.h" // TPL_RULE_CH_BINARY_OP_
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "Composition.h" // TPL_RULE_STR_BINARY_OP_
#endif

#ifndef TPL_REGEX_REF_H
#include "Ref.h"
#endif

#if !defined(_LIMITS_) && !defined(_LIMITS)
#include <climits>
#endif

NS_TPL_BEGIN

// =========================================================================
// TPL_RULE_UNARY_OP_, TPL_REGEX_BINARY_OP_

#define TPL_RULE_UNARY_OP_(op, Op)											\
	TPL_RULE_REF_UNARY_OP_(op, Op)

#define TPL_REGEX_BINARY_OP_(op, Op)										\
	TPL_RULE_CH_BINARY_OP_(op, Op)											\
	TPL_RULE_STR_BINARY_OP_(op, Op)											\
	TPL_RULE_REF_BINARY_OP_(op, Op)

// =========================================================================
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

public:
	enum { character = RegExT1::character | RegExT2::character };
	enum { vtype = 0 };

	typedef ExplicitConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename ContextT::template trans_type<RegExT1::character> trans(ar, context);
		if (m_x.match(ar, context) && m_y.match(ar, context))
			return true;
		trans.rollback(ar);
		return false;
	}
};

template <class T1, class T2> __forceinline
Rule<And<T1, T2> > TPL_CALL operator>>(const Rule<T1>& x, const Rule<T2>& y) {
	return Rule<And<T1, T2> >(x, y);
}

template <class T1, class T2> __forceinline
Rule<And<T1, T2> > TPL_CALL operator+(const Rule<T1>& x, const Rule<T2>& y) {
	return Rule<And<T1, T2> >(x, y);
}

TPL_REGEX_BINARY_OP_(>>, And)
TPL_REGEX_BINARY_OP_(+, And)

// =========================================================================
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

public:
	enum { character = RegExT1::character | RegExT2::character };
	enum { vtype = 0 };

	typedef ExplicitConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		return m_x.match(ar, context) || m_y.match(ar, context);
	}
};

template <class T1, class T2> __forceinline
Rule<Or<T1, T2> > TPL_CALL operator|(const Rule<T1>& x, const Rule<T2>& y) {
	return Rule<Or<T1, T2> >(x, y);
}

TPL_REGEX_BINARY_OP_(|, Or)

// -------------------------------------------------------------------------
// function eq

// Usage: Rule/eq("abc", "defg")	--- same as: Rule/(eq("abc") | eq("defg"))
// Usage: Rule/eq("s1", "s2", "s3")	--- same as: Rule/(eq("s1") | eq("s2") | eq("s3"))

template <class CharT>
__forceinline
Rule<Or<Eq<CharT>, Eq<CharT> > >
TPL_CALL eq(const CharT* s1, const CharT* s2) {
	return eq(s1) | eq(s2);
}

template <class CharT>
__forceinline
Rule<Or<Or<Eq<CharT>, Eq<CharT> >, Eq<CharT> > >
TPL_CALL eq(const CharT* s1, const CharT* s2, const CharT* s3) {
	return eq(s1) | eq(s2) | eq(s3);
}

template <class CharT>
__forceinline
Rule<Or<Or<Or<Eq<CharT>, Eq<CharT> >, Eq<CharT> >, Eq<CharT> > >
TPL_CALL eq(const CharT* s1, const CharT* s2, const CharT* s3, const CharT* s4) {
	return eq(s1) | eq(s2) | eq(s3) | eq(s4);
}

// =========================================================================
// operator/

// Usage: Rule1 / Rule2		--- Restriction

template <class RegExT1, class RegExT2>
class Restr // Rule1 / Rule2
{
private:
	RegExT1 m_x;
	RegExT2 m_y;

	typedef typename RegExT1::convertible_type CT1;
	typedef typename RegExT2::convertible_type CT2;

public:
	Restr() {}
	Restr(const RegExT1& x, const RegExT2& y)
		: m_x(x), m_y(y) {}

public:
	enum { character = RegExT1::character | RegExT2::character };
	enum { vtype = RegExT1::vtype };

	typedef typename OrConvertable<CT1, CT2>::convertible_type convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename SourceT::iterator pos = ar.position();
		typename ContextT::template trans_type<character> trans(ar, context);
		if (m_x.match(ar, context)) {
			typename SourceT::iterator pos2 = ar.position();
			typename SourceT::restriction_type restr(ar, pos, pos2);
			if (m_y.match(ar, context))
				return true;
		}
		trans.rollback(ar);
		return false;
	}
};

template <class T1, class T2> __forceinline
Rule<Restr<T1, T2> > TPL_CALL operator/(const Rule<T1>& x, const Rule<T2>& y) {
	return Rule<Restr<T1, T2> >(x, y);
}

TPL_REGEX_BINARY_OP_(/, Restr)

// =========================================================================
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

public:
	enum { character = RegExT::character };
	enum { vtype = 0 };

	typedef ExplicitConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		while (m_x.match(ar, context))
			;
		return true;
	}
};

template <class T1> __forceinline
Rule<Repeat0<T1> > TPL_CALL operator*(const Rule<T1>& x) {
	return Rule<Repeat0<T1> >(x);
}

TPL_RULE_UNARY_OP_(*, Repeat0)

// =========================================================================
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

public:
	enum { character = RegExT::character };
	enum { vtype = 0 };

	typedef ExplicitConvertible convertible_type;

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
Rule<Repeat1<T1> > TPL_CALL operator+(const Rule<T1>& x) {
	return Rule<Repeat1<T1> >(x);
}

TPL_RULE_UNARY_OP_(+, Repeat1)

// =========================================================================
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

public:
	enum { character = RegExT::character };
	enum { vtype = 0 };

	typedef ExplicitConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		m_x.match(ar, context);
		return true;
	}
};

template <class T1> __forceinline
Rule<Repeat01<T1> > TPL_CALL operator!(const Rule<T1>& x) {
	return Rule<Repeat01<T1> >(x);
}

TPL_RULE_UNARY_OP_(!, Repeat01)

// =========================================================================
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

public:
	enum { character = RegExT::character };
	enum { vtype = 0 };

	typedef ExplicitConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		unsigned n;
		typename ContextT::template trans_type<character> trans(ar, context);
		for (n = 0; n < nMax; ++n)
		{
			if (!m_x.match(ar, context))
				break;
		}
		if (n >= nMin)
			return true;
		trans.rollback(ar);
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
Rule<Repeat<T1, nMin> > TPL_CALL repeat_ge(const Rule<T1>& x) {
	return Rule<Repeat<T1, nMin> >(x);
}

template <unsigned nMin, class T1> __forceinline
Rule<Repeat<T1, nMin, nMin> > TPL_CALL repeat(const Rule<T1>& x) {
	return Rule<Repeat<T1, nMin, nMin> >(x);
}

template <unsigned nMin, unsigned nMax, class T1> __forceinline
Rule<Repeat<T1, nMin, nMax> > TPL_CALL repeat(const Rule<T1>& x) {
	return Rule<Repeat<T1, nMin, nMax> >(x);
}

// =========================================================================
// operator%

// Usage: Rule1 % Rule2		--- means: Rule1 + *(Rule2 + Rule1)

template <class T1, class T2>
class Lst : public And<T1, Repeat0<And<T2, T1> > > // Rule1 % Rule2 <=> Rule1 (Rule2 Rule1)*
{
private:
	typedef And<T1, Repeat0<And<T2, T1> > > Base;

public:
	Lst() {}
	Lst(const Rule<T1>& x, const Rule<T2>& y) : Base(x + *(y + x)) {}
};

template <class T1, class T2> __forceinline
Rule<Lst<T1, T2> > TPL_CALL operator%(const Rule<T1>& x, const Rule<T2>& y) {
	return Rule<Lst<T1, T2> >(x, y);
}

TPL_REGEX_BINARY_OP_(%, Lst)

// =========================================================================
// function ws, skipws

// Usage: ws()				--- means: matching Whitespaces. that is: w+
// Usage: skipws()			--- means: skip Whitespaces. that is: w*

typedef Repeat0<Space> SkipWhiteSpaces; // w*
typedef Repeat1<Space> WhiteSpaces; // w+

inline Rule<WhiteSpaces> TPL_CALL ws()
{
	return Rule<WhiteSpaces>();
}

inline Rule<SkipWhiteSpaces> TPL_CALL skipws()
{
	return Rule<SkipWhiteSpaces>();
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_OPERATOR_H */
