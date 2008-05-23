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

#ifndef TPL_REGEX_MATCHRESULT_H
#include "MatchResult.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class Restriction

template <class RestrT>
class Restriction : public RestrT
{
public:
	Restriction() {}

	template <class T1>
	Restriction(const T1& x) : RestrT(x) {}

	template <class T1, class T2>
	Restriction(const T1& x, const T2& y) : RestrT(x, y) {}
};

// =========================================================================
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

template <class Iterator, class SourceT, class ContextT>
inline bool TPL_CALL match_str(Iterator first, Iterator last, SourceT& ar, ContextT& context)
{
	typename SourceT::iterator pos = ar.position();
	for (; first != last; ++first) {
		if (ar.get() != *first) {
			ar.seek(pos);
			return false;
		}
	}
	return true;
}

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

public:
	enum { category = 0 };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return match_str(m_s, m_s + LenStrT::m_len, ar, context);
	}
};

template <const char* m_s>
class Str : public BasicStr<char, m_s> {
};

template <const wchar_t* m_s>
class WStr : public BasicStr<wchar_t, m_s> {
};

// =========================================================================
// class EqLeaf

template <class Iterator>
class EqLeaf
{
private:
	const LeafMatchResult<Iterator>& m_leaf;

public:
	EqLeaf(const LeafMatchResult<Iterator>& leaf)
		: m_leaf(leaf) {}

public:
	enum { category = 0 };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return match_str(m_leaf.first, m_leaf.second, ar, context);
	}
};

#define TPL_REGEX_LEAF_OP_(op, Op)																	\
template <class T1, class Iterator> __forceinline													\
Rule< Op<T1, EqLeaf<Iterator> > > TPL_CALL operator op(const Rule<T1>& x, const LeafMatchResult<Iterator>& y) \
	{ return x op Rule<EqLeaf<Iterator> >(y); }

// =========================================================================
// class Eq

template <class CharT>
class Eq
{
private:
	const CharT* m_s;
	size_t m_len;

public:
	Eq(const CharT* s) {
		m_s = s;
		m_len = std::char_traits<CharT>::length(s);
	}
	Eq(const CharT* s, size_t n) {
		m_s = s;
		m_len = n;
	}

public:
	template <class Iterator2>
	bool TPL_CALL operator()(Iterator2 first, Iterator2 last) const {
		size_t len = std::distance(first, last);
		return len == m_len && std::equal(first, last, m_s);
	}

public:
	enum { category = 0 };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return match_str(m_s, m_s + m_len, ar, context);
	}
};

// -------------------------------------------------------------------------
// class EqStr

typedef Eq<char> EqStr;
typedef Eq<wchar_t> EqWStr;

// -------------------------------------------------------------------------
// function str

// Usage: str("abcd")
// Usage: str("abcd", 3)

template <class CharT>
__forceinline Rule<Eq<CharT> > TPL_CALL str(const CharT* s)
{
	return Rule<Eq<CharT> >(s);
}

template <class CharT>
__forceinline Rule<Eq<CharT> > TPL_CALL str(const CharT* s, size_t n)
{
	return Rule<Eq<CharT> >(s, n);
}

#define TPL_REGEX_STR_OP1_(op, Op, CharT)											\
template <class T2> __forceinline													\
Rule< Op<Eq<CharT>, T2> > TPL_CALL operator op(const CharT* x, const Rule<T2>& y)	\
	{ return str(x) op y; }															\
template <class T1> __forceinline													\
Rule< Op<T1, Eq<CharT> > > TPL_CALL operator op(const Rule<T1>& x, const CharT* y)	\
	{ return x op str(y); }

#define TPL_REGEX_STR_OP_(op, Op)													\
	TPL_REGEX_STR_OP1_(op, Op, char)												\
	TPL_REGEX_STR_OP1_(op, Op, wchar_t)												\
	TPL_REGEX_LEAF_OP_(op, Op)

// -------------------------------------------------------------------------
// function eq

// Usage: Rule/eq("abcd")
// Usage: Rule/eq("abcd", 3)

template <class CharT>
__forceinline Restriction<Eq<CharT> > TPL_CALL eq(const CharT* s)
{
	return Restriction<Eq<CharT> >(s);
}

template <class CharT>
__forceinline Restriction<Eq<CharT> > TPL_CALL eq(const CharT* s, size_t n)
{
	return Restriction<Eq<CharT> >(s, n);
}

// =========================================================================
// operator|

// Usage: Rule/(Restr1 | Restr2)
//		--- eg. Rule/(eq("abc") | eq("defg"))
//		--- eg. Rule/(eq("abc") | "defg")

template <class RestrT1, class RestrT2>
class OrRestr // Restr1 | Restr2
{
private:
	RestrT1 m_x;
	RestrT2 m_y;

public:
	OrRestr() {}
	OrRestr(const RestrT1& x, const RestrT2& y) : m_x(x), m_y(y) {}

public:
	template <class Iterator>
	bool TPL_CALL operator()(Iterator first, Iterator last) const {
		return m_x(first, last) || m_y(first, last);
	}
};

template <class T1, class T2> __forceinline
Restriction<OrRestr<T1, T2> > TPL_CALL operator|(const Restriction<T1>& x, const Restriction<T2>& y) {
	return Restriction<OrRestr<T1, T2> >(x, y);
}

template <class T1> __forceinline
Restriction<OrRestr<T1, EqStr> > TPL_CALL operator|(const Restriction<T1>& x, const char* y) {
	return x | eq(y);
}

template <class T1> __forceinline
Restriction<OrRestr<T1, EqWStr> > TPL_CALL operator|(const Restriction<T1>& x, const wchar_t* y) {
	return x | eq(y);
}

// -------------------------------------------------------------------------
// function eq

// Usage: Rule/eq("abc", "defg")	--- same as: Rule/(eq("abc") | eq("defg"))
// Usage: Rule/eq("s1", "s2", "s3")	--- same as: Rule/(eq("s1") | eq("s2") | eq("s3"))

template <class CharT>
__forceinline
Restriction<OrRestr<Eq<CharT>, Eq<CharT> > >
TPL_CALL eq(const CharT* s1, const CharT* s2)
{
	return eq(s1) | eq(s2);
}

template <class CharT>
__forceinline
Restriction<OrRestr<OrRestr<Eq<CharT>, Eq<CharT> >, Eq<CharT> > >
TPL_CALL eq(const CharT* s1, const CharT* s2, const CharT* s3)
{
	return eq(s1) | eq(s2) | eq(s3);
}

template <class CharT>
__forceinline
Restriction<OrRestr<OrRestr<OrRestr<Eq<CharT>, Eq<CharT> >, Eq<CharT> >, Eq<CharT> > >
TPL_CALL eq(const CharT* s1, const CharT* s2, const CharT* s3, const CharT* s4)
{
	return eq(s1) | eq(s2) | eq(s3) | eq(s4);
}

// =========================================================================
// class Restr

template <class RegExT, class RestrT>
class Restr
{
private:
	RegExT m_x;
	RestrT m_pred;

public:
	Restr() {}
	Restr(const RegExT& x, const RestrT& pred)
		: m_pred(pred), m_x(x) {}

public:
	enum { category = RegExT::category };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename SourceT::iterator pos = ar.position();
		typename ContextT::trans_type<category> trans(ar, context);
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

// Usage: Rule/Restriction	--- eg. Rule/eq("abcd")
// Usage: Rule/"abcd"
// Usage: Rule/L"abcd"

template <class T1, class T2>
Rule<Restr<T1, T2> > TPL_CALL operator/(const Rule<T1>& x, const Restriction<T2>& y) {
	return Rule<Restr<T1, T2> >(x, y);
}

template <class T1>
Rule<Restr<T1, EqStr> > TPL_CALL operator/(const Rule<T1>& x, const char* y) {
	return x / eq(y);
}

template <class T1>
Rule<Restr<T1, EqWStr> > TPL_CALL operator/(const Rule<T1>& x, const wchar_t* y) {
	return x / eq(y);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_RESTRICTION_H */
