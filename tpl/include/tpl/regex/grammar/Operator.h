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
// Module: tpl/regex/grammar/Operator.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_GRAMMAR_OPERATOR_H
#define TPL_REGEX_GRAMMAR_OPERATOR_H

#ifndef TPL_REGEX_BASIC_H
#include "../Basic.h"
#endif

#ifndef TPL_REGEX_BASIC_H
#include "../Terminal.h" // TPL_GRAMMAR_CH_BINARY_OP_
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "../Composition.h" // TPL_GRAMMAR_STR_BINARY_OP_
#endif

#ifndef TPL_REGEX_GRAMMAR_REF_H
#include "Ref.h"
#endif

#if !defined(_LIMITS_) && !defined(_LIMITS)
#include <climits>
#endif

NS_TPL_BEGIN

// =========================================================================
// TPL_GRAMMAR_UNARY_OP_, TPL_GRAMMAR_BINARY_OP_

#define TPL_GRAMMAR_UNARY_OP_(op, Op)										\
	TPL_GRAMMAR_REF_UNARY_OP_(op, Op)

#define TPL_GRAMMAR_BINARY_OP_(op, Op)										\
	TPL_GRAMMAR_CH_BINARY_OP_(op, Op)										\
	TPL_GRAMMAR_STR_BINARY_OP_(op, Op)										\
	TPL_GRAMMAR_REF_BINARY_OP_(op, Op)

// =========================================================================
// operator>> (boost::spirit, xpressive compatible)
// operator+

// Usage: Grammar1 >> Grammar2
// Usage: Grammar1 + Grammar2

template <class GrammarT1, class GrammarT2>
class GAnd // Grammar1 Grammar2
{
private:
	GrammarT1 m_x;
	GrammarT2 m_y;

public:
	GAnd() {}
	GAnd(const GrammarT1& x, const GrammarT2& y) : m_x(x), m_y(y) {}

public:
	enum { character = GrammarT1::character | GrammarT2::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Rule<SkipperT>& skipper) const
	{
		typename ContextT::template trans_type<GrammarT1::character> trans(ar, context);
		if (m_x.match(ar, context, skipper) && m_y.match(ar, context, skipper))
			return true;
		trans.rollback(ar);
		return false;
	}
};

template <class T1, class T2> __forceinline
Grammar<GAnd<T1, T2> > TPL_CALL operator>>(const Grammar<T1>& x, const Grammar<T2>& y) {
	return Grammar<GAnd<T1, T2> >(x, y);
}

template <class T1, class T2> __forceinline
Grammar<GAnd<T1, T2> > TPL_CALL operator+(const Grammar<T1>& x, const Grammar<T2>& y) {
	return Grammar<GAnd<T1, T2> >(x, y);
}

TPL_GRAMMAR_BINARY_OP_(>>, GAnd)
TPL_GRAMMAR_BINARY_OP_(+, GAnd)

// =========================================================================
// operator|

// Usage: Grammar1 | Grammar2

template <class GrammarT1, class GrammarT2>
class GOr // Grammar1 | Grammar2
{
private:
	GrammarT1 m_x;
	GrammarT2 m_y;

public:
	GOr() {}
	GOr(const GrammarT1& x, const GrammarT2& y) : m_x(x), m_y(y) {}

public:
	enum { character = GrammarT1::character | GrammarT2::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Rule<SkipperT>& skipper) const
	{
		return m_x.match(ar, context, skipper) || m_y.match(ar, context, skipper);
	}
};

template <class T1, class T2> __forceinline
Grammar<GOr<T1, T2> > TPL_CALL operator|(const Grammar<T1>& x, const Grammar<T2>& y) {
	return Grammar<GOr<T1, T2> >(x, y);
}

TPL_GRAMMAR_BINARY_OP_(|, GOr)

// -------------------------------------------------------------------------
// function gr::eq

// Usage: Grammar/gr::eq("abc", "defg")		--- same as: Grammar/(gr::eq("abc") | gr::eq("defg"))
// Usage: Grammar/gr::eq("s1", "s2", "s3")	--- same as: Grammar/(gr::eq("s1") | gr::eq("s2") | gr::eq("s3"))

namespace gr {

template <class CharT>
__forceinline
Grammar<GOr<GEq<CharT>, GEq<CharT> > >
TPL_CALL eq(const CharT* s1, const CharT* s2) {
	return eq(s1) | eq(s2);
}

template <class CharT>
__forceinline
Grammar<GOr<GOr<GEq<CharT>, GEq<CharT> >, GEq<CharT> > >
TPL_CALL eq(const CharT* s1, const CharT* s2, const CharT* s3) {
	return eq(s1) | eq(s2) | eq(s3);
}

template <class CharT>
__forceinline
Grammar<GOr<GOr<GOr<GEq<CharT>, GEq<CharT> >, GEq<CharT> >, GEq<CharT> > >
TPL_CALL eq(const CharT* s1, const CharT* s2, const CharT* s3, const CharT* s4) {
	return eq(s1) | eq(s2) | eq(s3) | eq(s4);
}

} // namespace gr

// =========================================================================
// operator/

// Usage: Grammar1 / Grammar2		--- Restriction

template <class GrammarT1, class GrammarT2>
class GRestr // Grammar1 / Grammar2
{
private:
	GrammarT1 m_x;
	GrammarT2 m_y;

public:
	GRestr() {}
	GRestr(const GrammarT1& x, const GrammarT2& y)
		: m_x(x), m_y(y) {}

public:
	enum { character = GrammarT1::character | GrammarT2::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Rule<SkipperT>& skipper) const
	{
		typename SourceT::iterator pos = ar.position();
		typename ContextT::template trans_type<character> trans(ar, context);
		if (m_x.match(ar, context, skipper)) {
			typename SourceT::iterator pos2 = ar.position();
			typename SourceT::restriction_type restr(ar, pos, pos2);
			if (m_y.match(ar, context, skipper))
				return true;
		}
		trans.rollback(ar);
		return false;
	}
};

template <class T1, class T2> __forceinline
Grammar<GRestr<T1, T2> > TPL_CALL operator/(const Grammar<T1>& x, const Grammar<T2>& y) {
	return Grammar<GRestr<T1, T2> >(x, y);
}

TPL_GRAMMAR_BINARY_OP_(/, GRestr)

// =========================================================================
// operator* (Unary)

// Usage: *Grammar

template <class GrammarT>
class GRepeat0 // Grammar*
{
private:
	GrammarT m_x;

public:
	GRepeat0() {}
	GRepeat0(const GrammarT& x) : m_x(x) {}

public:
	enum { character = GrammarT::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Rule<SkipperT>& skipper) const
	{
		while (m_x.match(ar, context, skipper))
			;
		return true;
	}
};

template <class T1> __forceinline
Grammar<GRepeat0<T1> > TPL_CALL operator*(const Grammar<T1>& x) {
	return Grammar<GRepeat0<T1> >(x);
}

TPL_GRAMMAR_UNARY_OP_(*, GRepeat0)

// =========================================================================
// operator+ (Unary)

// Usage: +Grammar

template <class GrammarT>
class GRepeat1 // Grammar+
{
private:
	GrammarT m_x;

public:
	GRepeat1() {}
	GRepeat1(const GrammarT& x) : m_x(x) {}

public:
	enum { character = GrammarT::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Rule<SkipperT>& skipper) const
	{
		unsigned n = 0;
		while (m_x.match(ar, context, skipper))
			++n;
		return n > 0;
	}
};

template <class T1> __forceinline
Grammar<GRepeat1<T1> > TPL_CALL operator+(const Grammar<T1>& x) {
	return Grammar<GRepeat1<T1> >(x);
}

TPL_GRAMMAR_UNARY_OP_(+, GRepeat1)

// =========================================================================
// operator!

// Usage: !Grammar
// Note: Sorry that we don't have operator?

template <class GrammarT>
class GRepeat01 // Grammar?
{
private:
	GrammarT m_x;

public:
	GRepeat01() {}
	GRepeat01(const GrammarT& x) : m_x(x) {}

public:
	enum { character = GrammarT::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Rule<SkipperT>& skipper) const
	{
		m_x.match(ar, context, skipper);
		return true;
	}
};

template <class T1> __forceinline
Grammar<GRepeat01<T1> > TPL_CALL operator!(const Grammar<T1>& x) {
	return Grammar<GRepeat01<T1> >(x);
}

TPL_GRAMMAR_UNARY_OP_(!, GRepeat01)

// =========================================================================
// function repeat

// Usage:
//	1. repeat<n>(Grammar)		--- means: Grammar{n}
//	2. repeat<n, m>(Grammar)	--- means: Grammar{n, m}
//	3. repeat<0, 1>(Grammar)	--- means: Grammar{0, 1}, that is: Grammar?
//	4. repeat_ge<n>(Grammar)	--- means: Grammar{n,}

template <class GrammarT, unsigned nMin, unsigned nMax = UINT_MAX>
class GRepeat // Grammar{nMin, nMax}
{
private:
	GrammarT m_x;

public:
	GRepeat() {}
	GRepeat(const GrammarT& x) : m_x(x) {}

public:
	enum { character = GrammarT::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Rule<SkipperT>& skipper) const
	{
		unsigned n;
		typename ContextT::template trans_type<character> trans(ar, context);
		for (n = 0; n < nMax; ++n)
		{
			if (!m_x.match(ar, context, skipper))
				break;
		}
		if (n >= nMin)
			return true;
		trans.rollback(ar);
		return false;
	}
};

template <class GrammarT>
class GRepeat<GrammarT, 0, 1> : public GRepeat01<GrammarT>
{
public:
	GRepeat<GrammarT, 0, 1>() {}
	GRepeat<GrammarT, 0, 1>(const GrammarT& x) : GRepeat01<GrammarT>(x) {}
};

template <unsigned nMin, class T1> __forceinline
Grammar<GRepeat<T1, nMin> > TPL_CALL repeat_ge(const Grammar<T1>& x) {
	return Grammar<GRepeat<T1, nMin> >(x);
}

template <unsigned nMin, class T1> __forceinline
Grammar<GRepeat<T1, nMin, nMin> > TPL_CALL repeat(const Grammar<T1>& x) {
	return Grammar<GRepeat<T1, nMin, nMin> >(x);
}

template <unsigned nMin, unsigned nMax, class T1> __forceinline
Grammar<GRepeat<T1, nMin, nMax> > TPL_CALL repeat(const Grammar<T1>& x) {
	return Grammar<GRepeat<T1, nMin, nMax> >(x);
}

// =========================================================================
// operator%

// Usage: Grammar1 % Grammar2		--- means: Grammar1 + *(Grammar2 + Grammar1)

template <class T1, class T2>
class GLst : public GAnd<T1, GRepeat0<GAnd<T2, T1> > > // Grammar1 % Grammar2 <=> Grammar1 (Grammar2 Grammar1)*
{
private:
	typedef GAnd<T1, GRepeat0<GAnd<T2, T1> > > Base;

public:
	GLst() {}
	GLst(const Grammar<T1>& x, const Grammar<T2>& y) : Base(x + *(y + x)) {}
};

template <class T1, class T2> __forceinline
Grammar<GLst<T1, T2> > TPL_CALL operator%(const Grammar<T1>& x, const Grammar<T2>& y) {
	return Grammar<GLst<T1, T2> >(x, y);
}

TPL_GRAMMAR_BINARY_OP_(%, GLst)

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_GRAMMAR_OPERATOR_H */
