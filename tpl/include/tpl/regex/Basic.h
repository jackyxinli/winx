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
// Module: tpl/regex/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_BASIC_H
#define TPL_REGEX_BASIC_H

#if !defined(_LIMITS_) && !defined(_LIMITS)
#include <climits>
#endif

#if 0
#define TPL_HAS_TEMPLATE_TYPEDEF
#endif

#if !defined(__forceinline) && !defined(_MSC_VER)
#define __forceinline inline
#endif

#ifndef TPL_UNMANAGED_NEW
#define TPL_UNMANAGED_NEW(alloc, Type)	new((alloc).allocate(sizeof(Type))) Type
#endif

#ifndef TPL_ALLOC_ARRAY
#define TPL_ALLOC_ARRAY(alloc, Type, n)	(Type*)(alloc).allocate((n)*sizeof(Type))
#endif

#ifndef TPL_NEW
#if defined(BOOST_NEW)
#define TPL_NEW(alloc, Type)	BOOST_NEW(alloc, Type)
#else
#define TPL_NEW(alloc, Type)	STD_NEW(alloc, Type)
#endif
#endif

#ifndef TPL_CALL
#if defined(winx_call)
#define TPL_CALL winx_call
#else
#define TPL_CALL
#endif
#endif

#ifndef TPL_ASSERT
#if defined(BOOST_MEMORY_ASSERT)
#define TPL_ASSERT(e)	BOOST_MEMORY_ASSERT(e)
#elif defined(_ASSERTE)
#define TPL_ASSERT(e)	_ASSERTE(e)
#else
#define TPL_ASSERT(e)	0
#endif
#endif

// -------------------------------------------------------------------------
// class Exp

template <class RegExT>
class Exp : public RegExT
{
public:
	Exp() {}

	template <class T1>
	Exp(const T1& x) : RegExT(x) {}

	template <class T1, class T2>
	Exp(const T1& x, const T2& y) : RegExT(x, y) {}

	template <class T1, class T2>
	Exp(T1& x, const T2& y) : RegExT(x, y) {}
};

// -------------------------------------------------------------------------
// class BasicMatchCh

template <class PredT>
class BasicMatchCh // Match a Char
{
private:
	PredT m_pred;

public:
	BasicMatchCh() {}
	
	template <class T1>
	BasicMatchCh(const T1& x) : m_pred(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		typename SourceT::int_type c = ar.peek();
		if (m_pred(c))
		{
			ar.get();
			return true;
		}
		return false;
	}
};

// -------------------------------------------------------------------------
// class Ch

// Usage: Ch<'a'>()
// Note: Its function is same as ch('a').

template <int m_c>
class EqCh0
{
public:
	__forceinline bool TPL_CALL operator()(int c) const {
		return m_c == c;
	}
};

template <int m_c>
class Ch : public Exp<BasicMatchCh<EqCh0<m_c> > > {
};

// -------------------------------------------------------------------------
// function ch()

// Usage: ch('a')
// Sorry that we can't use 'a' directly instead of ch('a') in all case.

class EqCh
{
private:
	int m_c;

public:
	EqCh(int c) : m_c(c) {}

	bool TPL_CALL operator()(int c) const {
		return m_c == c;
	}
};

typedef BasicMatchCh<EqCh> MatchCh;

template <class CharT> __forceinline
Exp<MatchCh> TPL_CALL ch(const CharT x) {
	return Exp<MatchCh>(x);
}

#define TPL_REGEX_CH_OP1_(op, Op, CharT)											\
template <class T2> __forceinline													\
Exp< Op<MatchCh, T2> > TPL_CALL operator op(CharT x, Exp<T2>& y)					\
	{ return ch(x) op y; }															\
template <class T1> __forceinline													\
Exp< Op<T1, MatchCh> > TPL_CALL operator op(Exp<T1>& x, CharT y)					\
	{ return x op ch(y); }

#define TPL_REGEX_CH_OP_(op, Op)													\
	TPL_REGEX_CH_OP1_(op, Op, char)

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
	And(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		typename ContextT::trans_type trans = context.beginTrans();
		if (m_x.match(ar, context)) {
			if (m_y.match(ar, context))
				return context.commitTrans(trans);
		}
		return context.rollbackTrans(trans);
	}
};

template <class T1, class T2> __forceinline
Exp<And<T1, T2> > TPL_CALL operator>>(const Exp<T1>& x, Exp<T2>& y) {
	return Exp<And<T1, T2> >(x, y);
}

template <class T1, class T2> __forceinline
Exp<And<T1, T2> > TPL_CALL operator+(const Exp<T1>& x, Exp<T2>& y) {
	return Exp<And<T1, T2> >(x, y);
}

TPL_REGEX_CH_OP_(>>, And)
TPL_REGEX_CH_OP_(+, And)

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
	Or(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		if (m_x.match(ar, context))
			return true;
		return m_y.match(ar, context);
	}
};

template <class T1, class T2> __forceinline
Exp<Or<T1, T2> > TPL_CALL operator|(const Exp<T1>& x, Exp<T2>& y) {
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
	Repeat0(const RegExT& x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
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
	Repeat1(const RegExT& x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
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
	Repeat01(const RegExT& x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
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
	Repeat(const RegExT& x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		unsigned n;
		typename SourceT::iterator pos = ar.position();
		typename ContextT::trans_type trans = context.beginTrans();
		for (n = 0; n < nMax; ++n)
		{
			if (!m_x.match(ar, context))
				break;
		}
		if (n >= nMin)
			return context.commitTrans(trans);
		ar.seek(pos);
		return context.rollbackTrans(trans);
	}
};

template <class RegExT>
class Repeat<RegExT, 0, 1> : public Repeat01<RegExT>
{
public:
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
Exp<And<T1, Repeat0<And<T1, T2> > > > TPL_CALL operator%(const Exp<T1>& x, Exp<T2>& y)
{
	return x >> *(x >> y);
}

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_BASIC_H */
