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
// class ChT

template <class CharT>
class ChT
{
private:
	CharT m_x;

public:
	ChT(CharT x) : m_x(x) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		if (ar.peek() == m_x)
		{
			ar.get();
			return true;
		}
		return false;
	}
};

typedef ChT<char> Ch;

template <class CharT> __forceinline
Exp< ChT<CharT> > TPL_CALL ch(const CharT x)
{
	typedef ChT<CharT> RegExT;
	return Exp<RegExT>(RegExT(x));
}

#define TPL_REGEX_CH_OP1_(op, Op, CharT)											\
template <class T2> __forceinline													\
Exp< Op< Exp< ChT<CharT> >, Exp<T2> > > TPL_CALL operator op(CharT x, Exp<T2>& y)	\
	{ return ch(x) op y; }															\
template <class T1> __forceinline													\
Exp< Op< Exp<T1>, Exp< ChT<CharT> > > > TPL_CALL operator op(Exp<T1>& x, CharT y)	\
	{ return x op ch(y); }

#define TPL_REGEX_CH_OP_(op, Op)													\
	TPL_REGEX_CH_OP1_(op, Op, char)

// -------------------------------------------------------------------------
// class And

template <class RegExT1, class RegExT2>
class And
{
private:
	RegExT1 m_x;
	RegExT2 m_y;

public:
	And(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		if (!m_x.match(ar, context))
			return false;
		return m_y.match(ar, context);
	}
};

template <class T1, class T2> __forceinline
Exp<And<Exp<T1>, Exp<T2> > > TPL_CALL operator>>(const Exp<T1>& x, Exp<T2>& y) {
	return Exp<And<Exp<T1>, Exp<T2> > >(x, y);
}

TPL_REGEX_CH_OP_(>>, And)

// -------------------------------------------------------------------------
// class Or

template <class RegExT1, class RegExT2>
class Or
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
Exp<Or<Exp<T1>, Exp<T2> > > TPL_CALL operator|(const Exp<T1>& x, Exp<T2>& y) {
	return Exp<Or<Exp<T1>, Exp<T2> > >(x, y);
}

TPL_REGEX_CH_OP_(|, Or)

// -------------------------------------------------------------------------
// class Repeat0

template <class RegExT>
class Repeat0
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
Exp<Repeat0<Exp<T1> > > TPL_CALL operator*(const Exp<T1>& x) {
	return Exp<Repeat0<Exp<T1> > >(x);
}

// -------------------------------------------------------------------------
// class Repeat1

template <class RegExT>
class Repeat1
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
Exp<Repeat1<Exp<T1> > > TPL_CALL operator+(const Exp<T1>& x) {
	return Exp<Repeat1<Exp<T1> > >(x);
}

// -------------------------------------------------------------------------
// class Repeat

template <class RegExT, unsigned nMin, unsigned nMax = UINT_MAX>
class Repeat
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
		for (n = 0; n < nMax; ++n)
		{
			if (!m_x.match(ar, context))
				break;
		}
		if (n >= nMin)
			return true;
		ar.seek(pos);
		return false;
	}
};

template <unsigned nMin, class T1> __forceinline
Exp<Repeat<Exp<T1>, nMin> > TPL_CALL repeat_ge(const Exp<T1>& x) {
	return Exp<Repeat<Exp<T1>, nMin> >(x);
}

template <unsigned nMin, class T1> __forceinline
Exp<Repeat<Exp<T1>, nMin, nMin> > TPL_CALL repeat(const Exp<T1>& x) {
	return Exp<Repeat<Exp<T1>, nMin, nMin> >(x);
}

template <unsigned nMin, unsigned nMax, class T1> __forceinline
Exp<Repeat<Exp<T1>, nMin, nMax> > TPL_CALL repeat(const Exp<T1>& x) {
	return Exp<Repeat<Exp<T1>, nMin, nMax> >(x);
}

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_BASIC_H */
