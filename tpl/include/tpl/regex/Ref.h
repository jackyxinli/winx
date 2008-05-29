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
	enum { character = RegExT::character };
	enum { vtype = RegExT::vtype };

	typedef typename RegExT::convertible_type convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return m_x.match(ar, context);
	}
};

template <class T1>
__forceinline Rule<Ref<T1> > TPL_CALL ref(const Rule<T1>& x) {
	return Rule<Ref<T1> >(x);
}

// =========================================================================
// function ref(a_grammar_var)

// Usage: ref(a_grammar_var)	--- Work as a grammar.
// Note: if two rules A, B circularly refer to each other (or a grammar A refer to itself),
//	we need use ref() function to break the circuit.

template <class GrammarT>
class GRef
{
private:
	const GrammarT& m_x;

public:
	GRef(const GrammarT& x) : m_x(x) {}

public:
	enum { character = GrammarT::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Skipper<SkipperT>& skipper_) const {
		return m_x.match(ar, context, skipper_);
	}
};

template <class T1>
__forceinline Grammar<GRef<T1> > TPL_CALL ref(const Grammar<T1>& x) {
	return Grammar<GRef<T1> >(x);
}

// =========================================================================
// Dereference for Rule

template <class RefT>
class Deref
{
private:
	RefT m_ref;

private:
	typedef typename RefT::dereference_type dereference_type;
	typedef typename RefT::rule_type rule_type;

public:
	Deref(const RefT& ref_) : m_ref(ref_) {}

public:
	enum { character = rule_type::character };
	enum { vtype = rule_type::vtype };

	typedef typename rule_type::convertible_type convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		dereference_type der(m_ref());
		rule_type x(der);
		return x.match(ar, context);
	}
};

#define TPL_RULE_REF_UNARY_OP_(op, Op)										\
template <class T1> __forceinline 											\
Rule<Op<Deref<T1> > > TPL_CALL operator op(const Reference<T1>& x)			\
	{ return Rule<Op<Deref<T1> > >(x); }

#define TPL_RULE_REF_BINARY_OP_(op, Op)										\
template <class T1, class T2>												\
__forceinline Rule<Op<Deref<T1>, T2> > TPL_CALL operator op(const Reference<T1>& x, const Rule<T2>& y) \
	{ return Rule<Op<Deref<T1>, T2> >(x, y); }								\
template <class T1, class T2>												\
__forceinline Rule<Op<T1, Deref<T2> > > TPL_CALL operator op(const Rule<T1>& x, const Reference<T2>& y) \
	{ return Rule<Op<T1, Deref<T2> > >(x, y); }

// =========================================================================
// function ref(a_ch_var)

template <class CharT>
struct CharTraits {
};

template <>
struct CharTraits<char> {
	typedef char char_type;
	typedef unsigned char uchar_type;
};

template <>
struct CharTraits<wchar_t> {
	typedef wchar_t char_type;
	typedef wchar_t uchar_type;
};

template <>
struct CharTraits<int> {
	typedef int char_type;
	typedef unsigned int uchar_type;
};

template <class CharT>
class RefCh
{
private:
	typedef CharTraits<CharT> Tr_;
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

// =========================================================================
// function ref(var)

template <class Type>
struct ReferenceTratis {
};

template <class CharT>
struct ReferenceTratis<std::basic_string<CharT> > {
	typedef RefStr<CharT> reference_type;
};

template <class Iterator>
struct ReferenceTratis<Leaf<Iterator> > {
	typedef RefLeaf<Iterator> reference_type;
};

template <>
struct ReferenceTratis<char> {
	typedef RefCh<char> reference_type;
};

template <>
struct ReferenceTratis<wchar_t> {
	typedef RefCh<wchar_t> reference_type;
};

template <class Type> __forceinline
Reference<typename ReferenceTratis<Type>::reference_type> TPL_CALL ref(const Type& var_) {
	return Reference<typename ReferenceTratis<Type>::reference_type>(var_);
}

// =========================================================================
// function find_ref(var) = find(ref(var))

template <class Type, bool bEat = false>
class FindRef
{
private:
	typedef typename ReferenceTratis<Type>::reference_type reference_type;
	typedef typename reference_type::dereference_type dereference_type;
	typedef typename FindTraits<dereference_type, bEat>::find_type find_type;

	reference_type m_ref;

public:
	FindRef(const Type& var_) : m_ref(var_) {}

public:
	enum { character = find_type::character };
	enum { vtype = find_type::vtype };

	typedef typename find_type::convertible_type convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		dereference_type der(m_ref());
		find_type x(der);
		return x.match(ar, context);
	}
};

template <bool bEat, class Type> __forceinline
Rule<FindRef<Type, bEat> > TPL_CALL find_ref(const Type& var_) {
	return Rule<FindRef<Type, bEat> >(var_);
}

template <class Type> __forceinline
Rule<FindRef<Type, false> > TPL_CALL find_ref(const Type& var_) {
	return Rule<FindRef<Type, false> >(var_);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_REF_H */
