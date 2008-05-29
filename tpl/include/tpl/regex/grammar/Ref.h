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
// Module: tpl/regex/grammar/GRef.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_GRAMMAR_REF_H
#define TPL_REGEX_GRAMMAR_REF_H

#error "don't include me!!!"

#ifndef TPL_REGEX_BASIC_H
#include "../Basic.h"
#endif

#ifndef TPL_REGEX_TERMINAL_H
#include "../Terminal.h"
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "../Composition.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class GReference - Reference of a Grammar

template <class GRefT>
class GReference : public GRefT
{
public:
	template <class T1>
	GReference(const T1& x) : GRefT(x) {}

private:
	// concept:

	typedef typename GRefT::grammar_type grammar_type;
	typedef typename GRefT::dereference_type dereference_type;

	dereference_type TPL_CALL operator()() const;
};

// =========================================================================
// Dereference for Grammar (unary function)

template <template <class GrammarT> class Opera, class GRefT>
class GDeref
{
private:
	GRefT m_ref;

private:
	typedef typename GRefT::dereference_type dereference_type;
	typedef typename GRefT::grammar_type grammar_type;
	typedef Opera<grammar_type> composition_grammar;

public:
	GDeref(const GRefT& ref_) : m_ref(ref_) {}

public:
	enum { character = composition_grammar::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Skipper<SkipperT>& skipper_) const
	{
		dereference_type der(m_ref());
		grammar_type x(der);
		composition_grammar op(x);
		return op.match(ar, context, skipper_);
	}
};

#define TPL_GRAMMAR_REF_UNARY_OP_(op, Op)									\
template <class T1> __forceinline 											\
Grammar<GDeref<Op, T1> > TPL_CALL operator op(const GReference<T1>& x)		\
	{ return Grammar<GDeref<Op, T1> >(x); }

// =========================================================================
// Dereference for Grammar (binary function)

template <class GRefT>
class GDelayDeref_
{
private:
	GRefT m_ref;

private:
	typedef typename GRefT::dereference_type dereference_type;
	typedef typename GRefT::grammar_type grammar_type;

public:
	GDelayDeref_(const GRefT& ref_) : m_ref(ref_) {}

public:
	enum { character = grammar_type::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Skipper<SkipperT>& skipper_) const
	{
		dereference_type der(m_ref());
		grammar_type y(der);
		return y.match(ar, context, skipper_);
	}
};

template <template <class T1, class T2> class Opera, class GrammarT, class GRefT>
class GDerefR // Dereference the right oprand: Grammar <op> GRef
{
private:
	GrammarT m_x;
	GRefT m_ref;

private:
	typedef GDelayDeref_<GRefT> grammar_type;
	typedef Opera<GrammarT, grammar_type> composition_grammar;

public:
	GDerefR(const GrammarT& x_, const GRefT& ref_)
		: m_x(x_), m_ref(ref_) {}

public:
	enum { character = composition_grammar::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Skipper<SkipperT>& skipper_) const
	{
		grammar_type y(m_ref);
		composition_grammar op(m_x, y);
		return op.match(ar, context, skipper_);
	}
};

template <template <class T1, class T2> class Opera, class GRefT, class GrammarT>
class GDerefL // Dereference the left oprand: GRef <op> Grammar
{
private:
	GrammarT m_y;
	GRefT m_ref;

private:
	typedef typename GRefT::dereference_type dereference_type;
	typedef typename GRefT::grammar_type grammar_type;
	typedef Opera<grammar_type, GrammarT> composition_grammar;

public:
	GDerefL(const GRefT& ref_, const GrammarT& y_)
		: m_ref(ref_), m_y(y_) {}

public:
	enum { character = composition_grammar::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Skipper<SkipperT>& skipper_) const
	{
		dereference_type der(m_ref());
		grammar_type x(der);
		composition_grammar op(x, m_y);
		return op.match(ar, context, skipper_);
	}
};

#define TPL_GRAMMAR_REF_BINARY_OP_(op, Op)									\
template <class T1, class T2>												\
__forceinline Grammar<GDerefL<Op, T1, T2> > TPL_CALL operator op(const GReference<T1>& x, const Grammar<T2>& y) \
	{ return Grammar<GDerefL<Op, T1, T2> >(x, y); }							\
template <class T1, class T2>												\
__forceinline Grammar<GDerefR<Op, T1, T2> > TPL_CALL operator op(const Grammar<T1>& x, const GReference<T2>& y) \
	{ return Grammar<GDerefR<Op, T1, T2> >(x, y); }

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_GRAMMAR_REF_H */
