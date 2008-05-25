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

NS_TPL_BEGIN

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
// class Eq

template <class Iterator>
class Eq_
{
private:
	Iterator m_s;
	size_t m_len;

public:
	template <class StringT>
	Eq_(const StringT& s) {
		m_s = s.begin();
		m_len = s.size();
	}

	Eq_(Iterator s, size_t n) {
		m_s = s;
		m_len = n;
	}

public:
	enum { category = 0 };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return match_str(m_s, m_s + m_len, ar, context);
	}
};

template <class CharT>
class Eq : public Eq_<const CharT*>
{
private:
	typedef Eq_<const CharT*> Base;

public:
	Eq(const CharT* s, size_t n)
		: Base(s, n) {
	}

	Eq(const CharT* s)
		: Base(s,std::char_traits<CharT>::length(s)) {
	}
};

typedef Eq<char> EqStr;
typedef Eq<wchar_t> EqWStr;

// -------------------------------------------------------------------------
// function eq/str

// Usage: eq("abcd")			--- Work as a rule. Same as: str("abcd")
// Usage: eq("abcd", 3)			--- Work as a rule. Same as: str("abcd")

template <class CharT>
__forceinline Rule<Eq<CharT> > TPL_CALL eq(const CharT* s) {
	return Rule<Eq<CharT> >(s);
}

template <class CharT>
__forceinline Rule<Eq<CharT> > TPL_CALL eq(const CharT* s, size_t n) {
	return Rule<Eq<CharT> >(s, n);
}

template <class CharT>
__forceinline Rule<Eq<CharT> > TPL_CALL str(const CharT* s) {
	return Rule<Eq<CharT> >(s);
}

template <class CharT>
__forceinline Rule<Eq<CharT> > TPL_CALL str(const CharT* s, size_t n) {
	return Rule<Eq<CharT> >(s, n);
}

#define TPL_REGEX_STR_BINARY_OP1_(op, Op, CharT)									\
template <class T2> __forceinline													\
Rule< Op<Eq<CharT>, T2> > TPL_CALL operator op(const CharT* x, const Rule<T2>& y)	\
	{ return str(x) op y; }															\
template <class T1> __forceinline													\
Rule< Op<T1, Eq<CharT> > > TPL_CALL operator op(const Rule<T1>& x, const CharT* y)	\
	{ return x op str(y); }

#define TPL_REGEX_STR_BINARY_OP_(op, Op)											\
	TPL_REGEX_STR_BINARY_OP1_(op, Op, char)											\
	TPL_REGEX_STR_BINARY_OP1_(op, Op, wchar_t)

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_COMPOSITION_H */
