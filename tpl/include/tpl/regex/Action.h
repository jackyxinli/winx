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
// Module: tpl/regex/Action.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_ACTION_H
#define TPL_REGEX_ACTION_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class Action

template <class ActionT>
class Action : public ActionT
{
public:
	Action() {}

	template <class T1>
	Action(const T1& x) : ActionT(x) {}

	template <class T1>
	Action(T1& x) : ActionT(x) {}

	template <class T1, class T2>
	Action(const T1& x, const T2& y) : ActionT(x, y) {}

	template <class T1, class T2>
	Action(T1& x, const T2& y) : ActionT(x, y) {}

	template <class T1, class T2>
	Action(const T1& x, T2& y) : ActionT(x, y) {}

	template <class T1, class T2>
	Action(T1& x, T2& y) : ActionT(x, y) {}
};

// -------------------------------------------------------------------------
// class Act

template <class RegExT, class ActionT>
class Act
{
private:
	RegExT m_x;
	ActionT m_action;

public:
	Act() {}
	Act(const RegExT& x, const ActionT& act)
		: m_x(x), m_action(act) {}

public:
	enum { category = RegExT::category };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename SourceT::iterator pos = ar.position();
		if (m_x.match(ar, context)) {
			typename SourceT::iterator pos2 = ar.position();
			m_action(pos, pos2);
			return true;
		}
		return false;
	}
};

// -------------------------------------------------------------------------
// operator/

// Usage: Rule/Action

template <class T1, class T2> __forceinline
Rule<Act<T1, T2> > TPL_CALL operator/(const Rule<T1>& x, const Action<T2>& y) {
	return Rule<Act<T1, T2> >(x, y);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_ACTION_H */
