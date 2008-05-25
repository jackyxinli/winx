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
// Module: tpl/regex/Ref.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_REF_H
#define TPL_REGEX_REF_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_FIND_H
#include "Find.h" // match_while
#endif

#ifndef TPL_REGEX_TERMINAL_H
#include "Terminal.h"
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "Composition.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// Dereference for Rule (unary function)

template <template <class RegExT> class Opera, class RefT>
class Deref
{
private:
	RefT m_ref;

private:
	typedef typename RefT::dereference_type dereference_type;
	typedef typename RefT::rule_type rule_type;
	typedef Opera<rule_type> composition_rule;

public:
	Deref(const RefT& ref_) : m_ref(ref_) {}

public:
	enum { category = composition_rule::category };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		dereference_type der(m_ref());
		rule_type x(der);
		composition_rule op(x);
		return op.match(ar, context);
	}
};

#define TPL_REGEX_REF_UNARY_OP_(op, Op)										\
template <class T1> __forceinline 											\
Rule<Deref<Op, T1> > TPL_CALL operator op(const Reference<T1>& x)			\
	{ return Rule<Deref<Op, T1> >(x); }

// =========================================================================
// Dereference for Rule (binary function)

template <class RefT>
class DelayDeref_
{
private:
	RefT m_ref;

private:
	typedef typename RefT::dereference_type dereference_type;
	typedef typename RefT::rule_type rule_type;

public:
	DelayDeref_(const RefT& ref_) : m_ref(ref_) {}

public:
	enum { category = rule_type::category };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		dereference_type der(m_ref());
		rule_type y(der);
		return y.match(ar, context);
	}
};

template <template <class T1, class T2> class Opera, class RegExT, class RefT>
class DerefR // Dereference the right oprand: Rule <op> Ref
{
private:
	RegExT m_x;
	RefT m_ref;

private:
	typedef DelayDeref_<RefT> rule_type;
	typedef Opera<RegExT, rule_type> composition_rule;

public:
	DerefR(const RegExT& x_, const RefT& ref_)
		: m_x(x_), m_ref(ref_) {}

public:
	enum { category = composition_rule::category };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		rule_type y(m_ref);
		composition_rule op(m_x, y);
		return op.match(ar, context);
	}
};

template <template <class T1, class T2> class Opera, class RefT, class RegExT>
class DerefL // Dereference the left oprand: Ref <op> Rule
{
private:
	RegExT m_y;
	RefT m_ref;

private:
	typedef typename RefT::dereference_type dereference_type;
	typedef typename RefT::rule_type rule_type;
	typedef Opera<rule_type, RegExT> composition_rule;

public:
	DerefL(const RefT& ref_, const RegExT& y_)
		: m_ref(ref_), m_y(y_) {}

public:
	enum { category = composition_rule::category };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		dereference_type der(m_ref());
		rule_type x(der);
		composition_rule op(x, m_y);
		return op.match(ar, context);
	}
};

#define TPL_REGEX_REF_BINARY_OP_(op, Op)									\
template <class T1, class T2>												\
__forceinline Rule<DerefL<Op, T1, T2> > TPL_CALL operator op(const Reference<T1>& x, const Rule<T2>& y) \
	{ return Rule<DerefL<Op, T1, T2> >(x, y); }								\
template <class T1, class T2>												\
__forceinline Rule<DerefR<Op, T1, T2> > TPL_CALL operator op(const Rule<T1>& x, const Reference<T2>& y) \
	{ return Rule<DerefR<Op, T1, T2> >(x, y); }

// =========================================================================
// function ref(a_rule_var)

// Usage: ref(a_rule_var)	--- Work as a rule.
// Note: if two rules A, B circularly refer to each other (or a rule A refer to itself),
//	we need use ref() function to break the circuit.

template <class RegExT>
class Ref
{
private:
	const RegExT& m_x;

public:
	Ref(const RegExT& x) : m_x(x) {}

public:
	enum { category = RegExT::category };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return m_x.match(ar, context);
	}
};

template <class T1>
__forceinline Rule<Ref<T1> > TPL_CALL ref(const Rule<T1>& x) {
	return Rule<Ref<T1> >(x);
}

// -------------------------------------------------------------------------
// function ref(a_ch_var)

template <class CharT>
struct CharTraits_ {
};

template <>
struct CharTraits_<char> {
	typedef char char_type;
	typedef unsigned char uchar_type;
};

template <>
struct CharTraits_<wchar_t> {
	typedef wchar_t char_type;
	typedef wchar_t uchar_type;
};

template <class CharT>
class RefCh
{
private:
	typedef CharTraits_<CharT> Tr_;
	typedef typename Tr_::uchar_type UCharT;

	const CharT& m_c;

public:
	RefCh(const CharT& c) : m_c(c) {}

public:
	typedef int dereference_type;
	typedef Ch1_ rule_type;

	dereference_type TPL_CALL operator()() const {
		return (UCharT)m_c;
	}
};

__forceinline Reference<RefCh<char> > TPL_CALL ref(const char& x) {
	return Reference<RefCh<char> >(x);
}

__forceinline Reference<RefCh<wchar_t> > TPL_CALL ref(const wchar_t& x) {
	return Reference<RefCh<wchar_t> >(x);
}

// =========================================================================
// function ref(a_str_var), ref(leaf)

template <class StringT>
class RefStr_
{
private:
	const StringT& m_s;

	typedef typename StringT::const_iterator Iterator_;

public:
	RefStr_(const StringT& s) : m_s(s) {}

public:
	typedef StringT dereference_type;
	typedef Eq_<Iterator_> rule_type;

	dereference_type TPL_CALL operator()() const {
		return m_s;
	}
};

template <class CharT>
class RefStr : public RefStr_<std::basic_string<CharT> >
{
public:
	RefStr(const std::basic_string<CharT>& s)
		: RefStr_<std::basic_string<CharT> >(s) {}
};

template <class Iterator>
class RefLeaf : public RefStr_<Leaf<Iterator> >
{
public:
	RefLeaf(const Leaf<Iterator>& leaf)
		: RefStr_<Leaf<Iterator> >(leaf) {}
};

template <class CharT> __forceinline
Reference<RefStr<CharT> > TPL_CALL ref(const std::basic_string<CharT>& s) {
	return Reference<RefStr<CharT> >(s);
}

template <class Iterator> __forceinline
Reference<RefLeaf<Iterator> > TPL_CALL ref(const Leaf<Iterator>& leaf) {
	return Reference<RefLeaf<Iterator> >(leaf);
}

// =========================================================================
// function find_ref(a_ch_var)

// Usage: find_ref(a_ch_var)	--- Work as a rule.
// See: function find_ch

template <class CharT, bool bEat = false>
class FindRefCh
{
private:
	const CharT& m_c;

public:
	FindRefCh(const CharT& c) : m_c(c) {}

public:
	enum { category = 0 };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typedef typename SourceT::int_type int_type;
		typedef typename SourceT::uchar_type uchar_type;
		TPL_ASSERT(sizeof(CharT) == sizeof(uchar_type));

		NotCh_ notCh((uchar_type)m_c);
		return match_while<bEat>(ar, notCh);
	}
};

template <bool bEat>
__forceinline Rule<FindRefCh<char, bEat> > TPL_CALL find_ref(const char& x) {
	return Rule<FindRefCh<char, bEat> >(x);
}

template <bool bEat>
__forceinline Rule<FindRefCh<wchar_t, bEat> > TPL_CALL find_ref(const wchar_t& x) {
	return Rule<FindRefCh<wchar_t, bEat> >(x);
}

__forceinline Rule<FindRefCh<char> > TPL_CALL find_ref(const char& x) {
	return Rule<FindRefCh<char> >(x);
}

__forceinline Rule<FindRefCh<wchar_t> > TPL_CALL find_ref(const wchar_t& x) {
	return Rule<FindRefCh<wchar_t> >(x);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_REF_H */
