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

#ifndef TPL_REGEX_RULETRAITS_H
#include "RuleTraits.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class Act0

template <class RegExT, class ActionT>
class Act0
{
public:
	const RegExT m_x;
	const ActionT m_action;

public:
	Act0() : m_x(), m_action() {}
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
// operator+

// Usage: SimpleAction1 + SimpleAction2

template <class T1, class T2>
class AndSAct
{
public:
	const T1 m_x;
	const T2 m_y;

public:
	AndSAct() : m_x(), m_y() {}
	AndSAct(const T1& x, const T2& y) : m_x(x), m_y(y) {}

public:
	void TPL_CALL operator()() const {
		m_x();
		m_y();
	}
};

template <class T1, class T2>
inline SimpleAction<AndSAct<T1, T2> > const
TPL_CALL operator+(const SimpleAction<T1>& x, const SimpleAction<T2>& y) {
	return SimpleAction<AndSAct<T1, T2> >(x, y);
}

// =========================================================================
// Action

// TPL_ASSIG

template <class AssigTag>
struct AssigTraits {
};

#define TPL_ASSIG_(AssigTag, Assig)											\
	template <>																\
	struct AssigTraits<AssigTag> {											\
		typedef Assig assig_type;											\
	};

#define TPL_ASSIG(AssigTag, Assig)											\
namespace tpl {																\
	TPL_ASSIG_(AssigTag, Assig)												\
}

#define TPL_ASSIG_DEFVALTYPE(ValueType)										\
	template <class Iterator>												\
	struct Default {														\
		typedef ValueType value_type;										\
	};

#define TPL_ASSIG_PREPARE(AssigTag, ValueT)									\
	typedef typename SourceT::iterator iterator;							\
	typedef SelectValueType<ValueT, NS_STDEXT::Range<iterator> > SelectT_;	\
	typedef typename SelectT_::value_type value_type;						\
	typedef typename SelectAssig<AssigTag, value_type>::assig_type assig_type;
	
#define TPL_ASSIG_PREPARE1(AssigTag)										\
	typedef typename SourceT::iterator iterator;							\
	typedef typename AssigTraits<AssigTag>::assig_type assig_type;			\
	typedef typename assig_type::template Default<iterator> DefaultT_;		\
	typedef typename DefaultT_::value_type value_type;

#define TPL_ASSIG_GET(pos, pos2, pParam)									\
	assig_type::template get<value_type>(pos, pos2, pParam)

template <class AssigTag, class ValueT>
struct SelectAssig {
	typedef typename AssigTraits<AssigTag>::assig_type assig_type;
};

// class Act

template <class RegExT, class ActionT>
class Act
{
public:
	const RegExT m_x;
	const ActionT m_action;

public:
	Act() : m_x(), m_action() {}
	Act(const RegExT& x, const ActionT& act)
		: m_x(x), m_action(act) {}

public:
	enum { character = RegExT::character };
	
	typedef typename RegExT::convertible_type convertible_type;
	typedef typename RegExT::assig_tag assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		TPL_ASSIG_PREPARE(assig_tag, typename ActionT::value_type)
		
		const iterator pos = ar.position();
		if (m_x.match(ar, context)) {
			const iterator pos2 = ar.position();
			const value_type val(TPL_ASSIG_GET(pos, pos2, &m_x));
			m_action(val);
			return true;
		}
		return false;
	}
};

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
inline Action<Info<CharT> > const TPL_CALL info(const CharT* prompt_) {
	return Action<Info<CharT> >(prompt_);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_ACTION_H */

