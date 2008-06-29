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

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "../../../../stdext/include/stdext/archive/Basic.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// function eps, nothing

// Usage: eps()	--- means: always be successful.
// Usage: nothing()	--- means: always be failed.

template <bool bOk>
class Null
{
public:
	enum { character = 0 };

	typedef SelfConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return bOk;
	}

	TPL_SIMPLEST_GRAMMAR_();
};

class Eps : public Null<true> {};
class Nothing : public Null<false> {};

__forceinline Rule<Eps> TPL_CALL eps() {
	return Rule<Eps>();
}

__forceinline Rule<Nothing> TPL_CALL nothing() {
	return Rule<Nothing>();
}

// -------------------------------------------------------------------------
// function done

// Usage: done() --- means: seek to end-of-stream and return ok

class Done
{
public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		ar.seek_end();
		return true;
	}
};

__forceinline Rule<Done> TPL_CALL done() {
	return Rule<Done>();
}

// -------------------------------------------------------------------------
// function eos/eof

// Usage: eos() --- means: matching end-of-stream
// Usage: eof() --- same as: eos()

class Eos
{
public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return ar.peek() == SourceT::endch;
	}
};

typedef Eos Eof;

__forceinline Rule<Eos> TPL_CALL eos() {
	return Rule<Eos>();
}

__forceinline Rule<Eos> TPL_CALL eof() {
	return Rule<Eos>();
}

// -------------------------------------------------------------------------
// function not_eos/not_eof

// Usage: not_eos() --- means: true if current position is not end-of-stream
// Usage: not_eof() --- same as: not_eos()

class NotEos
{
public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return ar.peek() != SourceT::endch;
	}
};

typedef NotEos NotEof;

__forceinline Rule<NotEos> TPL_CALL not_eos() {
	return Rule<NotEos>();
}

__forceinline Rule<NotEos> TPL_CALL not_eof() {
	return Rule<NotEos>();
}

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
	typedef typename std::ArchiveIteratorTraits<Iterator>::value_type char_type;
	typedef typename std::ArchiveCharTraits<char_type>::uchar_type uchar_type;
	typename SourceT::iterator pos = ar.position();
	for (; first != last; ++first) {
		if (ar.get() != (uchar_type)*first) {
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
		return len == LenStrT::m_len && std::equal(m_s, m_s + len, first);
	}

public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;

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
	template <class Iterator2>
	bool TPL_CALL operator()(Iterator2 first, Iterator2 last) const {
		size_t len = std::distance(first, last);
		return len == m_len && std::equal(m_s, m_s + len, first);
	}

public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;
	typedef TagAssigNone assig_tag;

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

__forceinline Grammar<Gr<EqStr> > TPL_CALL gr(const char* s) {
	return Grammar<Gr<EqStr> >(s);
}

__forceinline Grammar<Gr<EqWStr> > TPL_CALL gr(const wchar_t* s) {
	return Grammar<Gr<EqWStr> >(s);
}

#define TPL_RULE_STR_BINARY_OP1_(op, Op, CharT)										\
template <class T2> __forceinline													\
Rule< Op<Eq<CharT>, T2> > TPL_CALL operator op(const CharT* x, const Rule<T2>& y)	\
	{ return str(x) op y; }															\
template <class T1> __forceinline													\
Rule< Op<T1, Eq<CharT> > > TPL_CALL operator op(const Rule<T1>& x, const CharT* y)	\
	{ return x op str(y); }

#define TPL_GRAMMAR_STR_BINARY_OP1_(op, Op, CharT)									\
template <class T2> __forceinline													\
Grammar< Op<Gr<Eq<CharT> >, T2> > TPL_CALL operator op(const CharT* x, const Grammar<T2>& y) \
	{ return str(x) op y; }															\
template <class T1> __forceinline													\
Grammar< Op<T1, Gr<Eq<CharT> > > > TPL_CALL operator op(const Grammar<T1>& x, const CharT* y) \
	{ return x op str(y); }

#define TPL_RULE_STR_BINARY_OP_(op, Op)												\
	TPL_RULE_STR_BINARY_OP1_(op, Op, char)											\
	TPL_RULE_STR_BINARY_OP1_(op, Op, wchar_t)

#define TPL_GRAMMAR_STR_BINARY_OP_(op, Op)											\
	TPL_GRAMMAR_STR_BINARY_OP1_(op, Op, char)										\
	TPL_GRAMMAR_STR_BINARY_OP1_(op, Op, wchar_t)

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_COMPOSITION_H */
