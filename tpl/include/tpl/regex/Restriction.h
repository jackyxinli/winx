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
// Module: tpl/regex/Restriction.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_RESTRICTION_H
#define TPL_REGEX_RESTRICTION_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class Pred

template <class PredT>
class Pred : public PredT
{
public:
	Pred() {}

	template <class T1>
	Pred(const T1& x) : PredT(x) {}

	template <class T1, class T2>
	Pred(const T1& x, const T2& y) : PredT(x, y) {}
};

// -------------------------------------------------------------------------
// class LenStr

template <class CharT, const CharT* m_s>
class LenStr
{
public:
	static const size_t m_len;
};

template <class CharT, const CharT* m_s>
const size_t LenStr<CharT, m_s>::m_len = std::char_traits<CharT>::length(m_s);

// -------------------------------------------------------------------------
// class Str

template <class CharT, const CharT* m_s>
class BasicStr
{
private:
	typedef LenStr<CharT, m_s> LenStrT;

public:
	template <class Iterator>
	bool TPL_CALL operator()(Iterator first, Iterator last) const {
		size_t len = std::distance(first, last);
		return len == LenStrT::m_len && std::equal(first, last, m_s);
	}
};

template <const char* m_s>
class Str : public BasicStr<char, m_s> {
};

template <const wchar_t* m_s>
class WStr : public BasicStr<wchar_t, m_s> {
};

// -------------------------------------------------------------------------
// class StrEq

template <class CharT>
class BasicStrEq
{
private:
	const CharT* m_s;
	size_t m_len;

public:
	BasicStrEq(const CharT* s) {
		m_s = s;
		m_len = std::char_traits<CharT>::length(s);
	}
	BasicStrEq(const CharT* s, size_t n) {
		m_s = s;
		m_len = n;
	}
	BasicStrEq(const CharT* first, const CharT* last) {
		m_s = first;
		m_len = last - first;
	}

public:
	template <class Iterator>
	bool TPL_CALL operator()(Iterator first, Iterator last) const {
		size_t len = std::distance(first, last);
		return len == m_len && std::equal(first, last, m_s);
	}
};

typedef BasicStrEq<char> StrEq;
typedef BasicStrEq<wchar_t> WStrEq;

// -------------------------------------------------------------------------
// function str

// Usage: Rule/str("abcd")
// Usage: Rule/str("abcd", 3)
// Usage: Rule/str(first, last)

template <class CharT>
__forceinline Pred<BasicStrEq<CharT> > TPL_CALL str(const CharT* s)
{
	return Pred<BasicStrEq<CharT> >(s);
}

template <class CharT>
__forceinline Pred<BasicStrEq<CharT> > TPL_CALL str(const CharT* s, size_t n)
{
	return Pred<BasicStrEq<CharT> >(s, n);
}

template <class CharT>
__forceinline Pred<BasicStrEq<CharT> > TPL_CALL str(const CharT* first, const CharT* last)
{
	return Pred<BasicStrEq<CharT> >(first, last);
}

// -------------------------------------------------------------------------
// class Restriction

template <class RegExT, class PredT>
class Restriction
{
private:
	RegExT m_x;
	PredT m_pred;

public:
	Restriction() {}
	Restriction(const RegExT& x, const PredT& pred)
		: m_pred(pred), m_x(x) {}

public:
	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename SourceT::iterator pos = ar.position();
		typename ContextT::trans_type trans(ar, context);
		if (m_x.match(ar, context)) {
			typename SourceT::iterator pos2 = ar.position();
			if (m_pred(pos, pos2))
				return true;
		}
		trans.rollback(ar);
		return false;
	}
};

// -------------------------------------------------------------------------
// operator/

// Usage: Rule/Restriction	--- eg. Rule/str("abcd")
// Usage: Rule/"abcd"
// Usage: Rule/L"abcd"

template <class T1, class T2>
Exp<Restriction<T1, T2> > TPL_CALL operator/(const Exp<T1>& x, const Pred<T2>& y) {
	return Exp<Restriction<T1, T2> >(x, y);
}

template <class T1>
Exp<Restriction<T1, StrEq> > TPL_CALL operator/(const Exp<T1>& x, const char* y) {
	return x / str(y);
}

template <class T1>
Exp<Restriction<T1, WStrEq> > TPL_CALL operator/(const Exp<T1>& x, const wchar_t* y) {
	return x / str(y);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_RESTRICTION_H */
