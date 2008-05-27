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

#ifndef TPL_REGEX_BASIC_H
#include "Terminal.h"
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "Composition.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class Act0

template <class RegExT, class ActionT>
class Act0
{
private:
	RegExT m_x;
	ActionT m_action;

public:
	Act0() {}
	Act0(const RegExT& x, const ActionT& act)
		: m_x(x), m_action(act) {}

public:
	enum { character = RegExT::character };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		if (m_x.match(ar, context)) {
			m_action();
			return true;
		}
		return false;
	}
};

// -------------------------------------------------------------------------
// operator/

// Usage: Rule/SimpleAction

template <class T1, class T2> __forceinline
Rule<Act0<T1, T2> > TPL_CALL operator/(const Rule<T1>& x, const SimpleAction<T2>& y) {
	return Rule<Act0<T1, T2> >(x, y);
}

template <class T2> __forceinline
Rule<Act0<Ch1_, T2> > TPL_CALL operator/(char x, const SimpleAction<T2>& y) {
	return ch((unsigned char)x) / y;
}

template <class T2> __forceinline
Rule<Act0<Ch1_, T2> > TPL_CALL operator/(wchar_t x, const SimpleAction<T2>& y) {
	return ch(x) / y;
}

template <class T2> __forceinline
Rule<Act0<EqStr, T2> > TPL_CALL operator/(const char* x, const SimpleAction<T2>& y) {
	return str(x) / y;
}

template <class T2> __forceinline
Rule<Act0<EqWStr, T2> > TPL_CALL operator/(const wchar_t* x, const SimpleAction<T2>& y) {
	return str(x) / y;
}

// =========================================================================
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
	enum { character = RegExT::character };

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

// Usage: Rule/Action	--- eg. Rule/assign(a_variable)

template <class T1, class T2> __forceinline
Rule<Act<T1, T2> > TPL_CALL operator/(const Rule<T1>& x, const Action<T2>& y) {
	return Rule<Act<T1, T2> >(x, y);
}

template <class T2> __forceinline
Rule<Act<Ch1_, T2> > TPL_CALL operator/(char x, const Action<T2>& y) {
	return ch((unsigned char)x) / y;
}

template <class T2> __forceinline
Rule<Act<Ch1_, T2> > TPL_CALL operator/(wchar_t x, const Action<T2>& y) {
	return ch(x) / y;
}

template <class T2> __forceinline
Rule<Act<EqStr, T2> > TPL_CALL operator/(const char* x, const Action<T2>& y) {
	return str(x) / y;
}

template <class T2> __forceinline
Rule<Act<EqWStr, T2> > TPL_CALL operator/(const wchar_t* x, const Action<T2>& y) {
	return str(x) / y;
}

// =========================================================================
// class Info

// Usage: Rule/info("...")

class Info
{
private:
	const char* m_prompt;

public:
	Info(const char* prompt_) : m_prompt(prompt_) {
	}

public:
	template <class Iterator>
	void operator()(Iterator pos, Iterator pos2) const {
		std::string str_(pos, pos2);
		printf("%s: %s\n", m_prompt, str_.c_str());
	}
};

inline Action<Info> TPL_CALL info(const char* prompt_) {
	return Action<Info>(prompt_);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_ACTION_H */
