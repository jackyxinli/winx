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

#ifndef TPL_REGEX_REF_H
#include "Ref.h"
#endif

#ifndef TPL_REGEX_BASIC_H
#include "Terminal.h"
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "Composition.h"
#endif

#if !defined(_STRING_) && !defined(_STRING)
#include <string>
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

	typedef typename RegExT::convertible_type convertible_type;
	typedef typename RegExT::assig_tag assig_tag;

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

// -------------------------------------------------------------------------
// function push_back

template <class ContainerT, class ValueT>
class PushBack
{
private:
	ContainerT& m_cont;
	const ValueT& m_ref;

public:
	PushBack(ContainerT& cont, const ValueT& val)
		: m_cont(cont), m_ref(val) {
	}

	void TPL_CALL operator()() const {
		m_cont.push_back(m_ref);
	}
};

template <class ContainerT, class ValueT> __forceinline
SimpleAction<PushBack<ContainerT, ValueT> > push_back(ContainerT& cont, const ValueT& val) {
	return SimpleAction<PushBack<ContainerT, ValueT> >(cont, val);
}

template <class ContainerT, class ValueT> __forceinline
SimpleAction<PushBack<ContainerT, ValueT> > push_back(ContainerT& cont, const Var<ValueT>& var_) {
	return SimpleAction<PushBack<ContainerT, ValueT> >(cont, var_.val);
}

// =========================================================================
// Action

// TPL_ASSIG

template <class AssigTag>
struct AssigTraits {
};

#define TPL_ASSIG(AssigTag, Assig)											\
namespace tpl {																\
	template <>																\
	struct AssigTraits<AssigTag> {											\
		typedef Assig assig_type;											\
	};																		\
}

template <class AssigTag, class ValueT>
struct SelectAssig {
	typedef typename AssigTraits<AssigTag>::assig_type assig_type;
};

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
	
	typedef typename RegExT::convertible_type convertible_type;
	typedef typename RegExT::assig_tag assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typedef typename SourceT::iterator iterator;
		typedef typename ActionT::value_type value_type;
		typedef typename SelectAssig<assig_tag, value_type>::assig_type assig_type;

		iterator pos = ar.position();
		if (m_x.match(ar, context)) {
			iterator pos2 = ar.position();
			value_type val = value_type();
			assig_type::assign(val, pos, pos2, this);
			m_action(val);
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
// function info

// Usage: Rule/info("...")

inline void TPL_CALL _trace_info(const char* prompt_, const std::string& val) {
	TPL_TRACE("%s: %s\n", prompt_, val.c_str());
}

inline void TPL_CALL _trace_info(const wchar_t* prompt_, const std::wstring& val) {
	TPL_TRACEW(L"%s: %s\n", prompt_, val.c_str());
}

template <class CharT>
class Info
{
private:
	const CharT* m_prompt;

public:
	Info(const CharT* prompt_) : m_prompt(prompt_) {
	}

public:
	typedef std::basic_string<CharT> value_type;

	void TPL_CALL operator()(const value_type& val) const {
		_trace_info(m_prompt, val);
	}
};

template <class CharT>
inline Action<Info<CharT> > TPL_CALL info(const CharT* prompt_) {
	return Action<Info<CharT> >(prompt_);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_ACTION_H */
