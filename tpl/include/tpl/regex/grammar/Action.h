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
// Module: tpl/regex/grammar/Action.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_GRAMMAR_ACTION_H
#define TPL_REGEX_GRAMMAR_ACTION_H

#ifndef TPL_REGEX_BASIC_H
#include "../Basic.h"
#endif

#ifndef TPL_REGEX_ACTION_H
#include "../Action.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class GAct0

template <class GrammarT, class ActionT>
class GAct0
{
public:
	const GrammarT m_x;
	const ActionT m_action;

public:
	GAct0() : m_x(), m_action() {}
	GAct0(const GrammarT& x, const ActionT& act)
		: m_x(x), m_action(act) {}

public:
	enum { character = GrammarT::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		if (m_x.match(ar, context, skipper_)) {
			m_action();
			return true;
		}
		return false;
	}
};

// -------------------------------------------------------------------------
// operator/

// Usage: Grammar/SimpleAction

template <class T1, class T2> __forceinline
Grammar<GAct0<T1, T2> > TPL_CALL operator/(const Grammar<T1>& x, const SimpleAction<T2>& y) {
	return Grammar<GAct0<T1, T2> >(x, y);
}

// =========================================================================
// class GAct

template <class GrammarT, class ActionT>
class GAct
{
public:
	const GrammarT m_x;
	const ActionT m_action;

public:
	GAct() : m_x(), m_action() {}
	GAct(const GrammarT& x, const ActionT& act)
		: m_x(x), m_action(act) {}

public:
	enum { character = GrammarT::character };

	typedef typename GrammarT::assig_tag assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		typedef typename SourceT::iterator iterator;
		typedef SelectValueType<typename ActionT::value_type, Leaf<iterator> > SelectT;
		typedef typename SelectT::value_type value_type;
		typedef typename SelectAssig<assig_tag, value_type>::assig_type assig_type;

		const iterator pos = ar.position();
		if (m_x.match(ar, context, skipper_)) {
			const iterator pos2 = ar.position();
			const value_type val(assig_type::template get<value_type>(pos, pos2, &m_x));
			m_action(val);
			return true;
		}
		return false;
	}
};

// Usage: Grammar/Action	--- eg. Grammar/assign(a_variable)

template <class T1, class T2> __forceinline
Grammar<GAct<T1, T2> > TPL_CALL operator/(const Grammar<T1>& x, const Action<T2>& y) {
	return Grammar<GAct<T1, T2> >(x, y);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_GRAMMAR_ACTION_H */
