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
// Module: tpl/regex/Case.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Case.h 636 2008-06-11 07:19:52Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_CASE_H
#define TPL_REGEX_CASE_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_ACTION_H
#include "Action.h"
#endif

#ifndef TPL_REGEX_PREDICATE_H
#include "Predicate.h"
#endif

#ifndef TPL_REGEX_CONDITION_H
#include "Condition.h"
#endif

#ifndef TPL_REGEX_GRAMMAR_CONDITION_H
#include "grammar/Condition.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class CondSel

template <class ValueT, class ConditionT>
class CondSel
{
public:
	const ValueT& m_x;
	const ConditionT m_y;
	
public:
	CondSel() : m_x(), m_y() {}
	CondSel(const ValueT& x, const ConditionT& y) : m_x(x), m_y(y) {}
	
public:
	enum { character = ConditionT::character };

	typedef TagAssigNone assig_tag;
	typedef ExplicitConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return m_y.match_if(m_x, ar, context) == matchOk;
	}

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const {
		return m_y.match_if(m_x, ar, context, skipper_) == matchOk;
	}
};

// -------------------------------------------------------------------------
// class Switch

template <class ValueT>
class Switch
{
private:
	const ValueT& m_val;
	
public:
	Switch(const ValueT& val) : m_val(val) {}
	
	template <class ConditionT>
	Rule<CondSel<ValueT, ConditionT> > const TPL_CALL operator[](const Condition<ConditionT>& cond_) const {
		return Rule<CondSel<ValueT, ConditionT> >(m_val, cond_);
	}

	template <class ConditionT>
	Grammar<CondSel<ValueT, ConditionT> > const TPL_CALL operator[](const GCondition<ConditionT>& cond_) const {
		return Grammar<CondSel<ValueT, ConditionT> >(m_val, cond_);
	}
};

template <class ValueT>
inline Switch<ValueT> const TPL_CALL switch_(const ValueT& val) {
	return Switch<ValueT>(val);
}

template <class ValueT>
inline Switch<ValueT> const TPL_CALL switch_(const Var<ValueT>& var_) {
	return Switch<ValueT>(var_.val);
}

// =========================================================================
// class Case

template <class CondT>
class Case : public CondT
{
public:
	Case() : CondT() {}
	
	template <class T1>
	Case(const T1& x) : CondT(x) {}

public:
	template <class NextT>
	Condition<CondBind<CondT, NextT> > const TPL_CALL operator[](const Rule<NextT>& next_) const {
		return Condition<CondBind<CondT, NextT> >(*(const CondT*)this, next_);
	}

	template <class NextT>
	GCondition<GCondBind<CondT, NextT> > const TPL_CALL operator[](const Grammar<NextT>& next_) const {
		return GCondition<GCondBind<CondT, NextT> >(*(const CondT*)this, next_);
	}
};

// -------------------------------------------------------------------------
// function default_

class True_
{
public:
	typedef DefaultType value_type;
	
	template <class ValueT2>
	bool TPL_CALL operator()(const ValueT2& val) const {
		return true;
	}
};

inline Case<True_> const TPL_CALL default_() {
	return Case<True_>();
}

// -------------------------------------------------------------------------
// function case_

template <class PredT>
inline Case<PredT> const TPL_CALL case_() {
	return Case<PredT>();
}

template <class ValueT>
inline Case<typename PredRefTraits<const ValueT&>::pred_type> const
TPL_CALL case_(const ValueT& val) {
	return Case<typename PredRefTraits<const ValueT&>::pred_type>(val);
}

template <class ValueT>
inline Case<typename PredRefTraits<ValueT&>::pred_type> const
TPL_CALL case_(ValueT& val) {
	return Case<typename PredRefTraits<ValueT&>::pred_type>(val);
}

// -------------------------------------------------------------------------
// function case_if

#if defined(TPL_REGEX_BACKWARD)

template <class PredT, class ValueT>
inline Case<Pred_<typename SmartRefTraits<const ValueT&>::const_type, PredT> > const
TPL_CALL case_if(const ValueT& val) {
	return Case<Pred_<typename SmartRefTraits<const ValueT&>::const_type, PredT> >(val);
}

template <class PredT, class ValueT>
inline Case<Pred_<typename SmartRefTraits<ValueT&>::const_type, PredT> > const
TPL_CALL case_if(ValueT& val) {
	return Case<Pred_<typename SmartRefTraits<ValueT&>::const_type, PredT> >(val);
}

#endif

// =========================================================================
// class CaseAct

template <class CondT, class ActT>
class CaseAct
{
public:
	const CondT m_cond;
	const ActT m_act;

public:
	CaseAct() : m_cond(), m_act() {}
	CaseAct(const CondT& cond, const ActT& act)
		: m_cond(cond), m_act(act) {}

public:
	enum { character = 0 };

	typedef typename CondT::value_type value_type;

	template <class ValueT, class SourceT, class ContextT>
	MatchCode TPL_CALL match_if(const ValueT& val, SourceT& ar, ContextT& context) const
	{
		if (!m_cond(val))
			return matchFailed;
		m_act();
		return matchOk;
	}
};

// Usage: Case/Action ==> Case[eps()/Action]
// Implementation: move to <tpl/regex/grammar/Action.h>

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CASE_H */
