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
// Description: AOP - Aspect Oriented Programming
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_BASIC_H
#define TPL_REGEX_BASIC_H

#ifndef TPL_REGEX_REGEXP_H
#include "RegExp.h"
#endif

// -------------------------------------------------------------------------
// class ChT

template <class CharT>
class ChT
{
private:
	CharT m_x;

public:
	ChT(CharT x) : m_x(x) {}

	template <class Iterator>
	bool TPL_CALL match(Iterator curr, Iterator, Iterator& out)
	{
		if (*curr == m_x)
		{
			out = ++curr;
			return true;
		}
		return false;
	}
};

typedef ChT<char> Ch;

template <class CharT>
Exp< ChT<CharT> > TPL_CALL ch(const CharT x)
{
	typedef ChT<CharT> RegExT;
	return Exp<RegExT>(RegExT(x));
}

#define TPL_REGEX_CH_OP1_(op, Op, CharT)												\
	template <class T2> __forceinline													\
	Exp< Op< Exp< ChT<CharT> >, Exp<T2> > > TPL_CALL operator op(CharT x, Exp<T2>& y)	\
{ return ch(x) op y; }																	\
	template <class T1> __forceinline													\
	Exp< Op< Exp<T1>, Exp< ChT<CharT> > > > TPL_CALL operator op(Exp<T1>& x, CharT y)	\
{ return x op ch(y); }

#define TPL_REGEX_CH_OP_(op, Op)														\
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

	template <class Iterator>
	bool TPL_CALL match(Iterator curr, Iterator last, Iterator& out)
	{
		if (!m_x.match(curr, last, out))
			return false;
		return m_y.match(out, last, out);
	}
};

template <class T1, class T2> inline
Exp< And< Exp<T1>, Exp<T2> > > TPL_CALL operator>>(const Exp<T1>& x, Exp<T2>& y) {
	typedef And< Exp<T1>, Exp<T2> > RegExT;
	return Exp<RegExT>(RegExT(x, y));
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

	template <class Iterator>
	bool TPL_CALL match(Iterator curr, Iterator last, Iterator& out)
	{
		if (m_x.match(curr, last, out))
			return true;
		return m_y.match(curr, last, out);
	}
};

template <class T1, class T2>
Exp< Or< Exp<T1>, Exp<T2> > > TPL_CALL operator|(const Exp<T1>& x, Exp<T2>& y) {
	typedef Or< Exp<T1>, Exp<T2> > RegExT;
	return Exp<RegExT>(RegExT(x, y));
}

TPL_REGEX_CH_OP_(|, Or)

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_BASIC_H */
