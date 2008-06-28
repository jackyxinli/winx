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
		return m_y.match_if(m_x, ar, context);
	}

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const {
		return m_y.match_if(m_x, ar, context, skipper_);
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
	Rule<CondSel<ValueT, ConditionT> > TPL_CALL operator[](const Condition<ConditionT>& cond_) const {
		return Rule<CondSel<ValueT, ConditionT> >(m_val, cond_);
	}

	template <class ConditionT>
	Grammar<CondSel<ValueT, ConditionT> > TPL_CALL operator[](const GCondition<ConditionT>& cond_) const {
		return Grammar<CondSel<ValueT, ConditionT> >(m_val, cond_);
	}
};

template <class ValueT>
inline Switch<ValueT> TPL_CALL switch_(const ValueT& val) {
	return Switch<ValueT>(val);
}

template <class ValueT>
inline Switch<ValueT> TPL_CALL switch_(const Var<ValueT>& var_) {
	return Switch<ValueT>(var_.val);
}

// =========================================================================
// class Case

template <class CondT>
class Case
{
public:
	const CondT m_cond;
	
public:
	Case() : m_cond() {}
	
	template <class T1>
	Case(const T1& x) : m_cond(x) {}

public:
	template <class NextT>
	Condition<CondBind<CondT, NextT> > TPL_CALL operator[](const Rule<NextT>& next_) const {
		return Condition<CondBind<CondT, NextT> >(m_cond, next_);
	}

	template <class NextT>
	GCondition<GCondBind<CondT, NextT> > TPL_CALL operator[](const Grammar<NextT>& next_) const {
		return GCondition<GCondBind<CondT, NextT> >(m_cond, next_);
	}
};

// -------------------------------------------------------------------------
// ConditionValueTypeTraits

template <class ValueT>
struct ConditionValueTypeTraits {
	typedef typename ValueT::value_type value_type;
	typedef ValueT pred_type;
};

template <class ValueT>
struct ConditionValueTypeTraits<ValueT const&> {
	typedef typename ConditionValueTypeTraits<ValueT>::value_type value_type;
	typedef typename ConditionValueTypeTraits<ValueT>::pred_type pred_type;
};

// -------------------------------------------------------------------------
// class Pred_

#define TPL_DEFAULT_VT	void

#define TPL_CONDITION_PRED(Cmp, op)											\
struct Cmp {																\
	template <class T1, class T2>											\
	static bool TPL_CALL test(const T1& x, const T2& y) {					\
		return x op y;														\
	}																		\
};

TPL_CONDITION_PRED(EQ, ==)
TPL_CONDITION_PRED(NE, !=)
TPL_CONDITION_PRED(GT, >)
TPL_CONDITION_PRED(LT, <)
TPL_CONDITION_PRED(GE, >=)
TPL_CONDITION_PRED(LE, <=)

template <class ValueT, class PredT = EQ>
class Pred_
{
private:
	const ValueT m_val2;
	
public:
	template <class T1>
	explicit Pred_(const T1& val) : m_val2(val) {}
	
	typedef typename ConditionValueTypeTraits<ValueT>::value_type value_type;
	
	template <class ValueT2>
	bool TPL_CALL operator()(const ValueT2& val) const {
		return PredT::test(val, m_val2);
	}
};

class True_
{
public:
	typedef TPL_DEFAULT_VT value_type;
	
	template <class ValueT2>
	bool TPL_CALL operator()(const ValueT2& val) const {
		return true;
	}
};

// -------------------------------------------------------------------------

#define TPL_CONDITION_VT(ValueT, CondValT)									\
template <>																	\
struct ConditionValueTypeTraits<ValueT> {									\
	typedef CondValT value_type;											\
	typedef Pred_<ValueT> pred_type;										\
};																			\
template <>																	\
struct ConditionValueTypeTraits<ValueT const&> {							\
	typedef CondValT value_type;											\
	typedef Pred_<ValueT const&> pred_type;									\
};

TPL_CONDITION_VT(char, char)
TPL_CONDITION_VT(wchar_t, wchar_t)
TPL_CONDITION_VT(short, int)
TPL_CONDITION_VT(int, int)
TPL_CONDITION_VT(long, long)
TPL_CONDITION_VT(unsigned short, unsigned int)
TPL_CONDITION_VT(unsigned int, unsigned int)
TPL_CONDITION_VT(unsigned long, unsigned long)

TPL_CONDITION_VT(char*, TPL_DEFAULT_VT)
TPL_CONDITION_VT(wchar_t*, TPL_DEFAULT_VT)
TPL_CONDITION_VT(char const*, TPL_DEFAULT_VT)
TPL_CONDITION_VT(wchar_t const*, TPL_DEFAULT_VT)

template <class CharT, size_t n>
struct ConditionValueTypeTraits<const CharT[n]> {
	typedef TPL_DEFAULT_VT value_type;
	typedef Pred_<const CharT*> pred_type;
};

template <class CharT, size_t n>
struct ConditionValueTypeTraits<CharT[n]> {
	typedef TPL_DEFAULT_VT value_type;
	typedef Pred_<const CharT*> pred_type;
};

// -------------------------------------------------------------------------
// function case_/default_

template <class PredT, class ValueT>
inline Case<Pred_<ValueT, PredT> > TPL_CALL case_(const ValueT& val) {
	return Case<Pred_<ValueT, PredT> >(val);
}

template <class PredT, class ValueT>
inline Case<Pred_<const ValueT&, PredT> > TPL_CALL case_(ValueT& val) {
	return Case<Pred_<const ValueT&, PredT> >(val);
}

template <class PredT, class ValueT>
inline Case<Pred_<const ValueT&, PredT> > TPL_CALL case_(const Var<ValueT>& var_) {
	return Case<Pred_<const ValueT&, PredT> >(var_.val);
}

template <class PredT, class ValueT>
inline Case<Pred_<const ValueT&, PredT> > TPL_CALL case_(Var<ValueT>& var_) {
	return Case<Pred_<const ValueT&, PredT> >(var_.val);
}

template <class ValueT>
inline Case<typename ConditionValueTypeTraits<ValueT>::pred_type> TPL_CALL case_(const ValueT& val) {
	return Case<typename ConditionValueTypeTraits<ValueT>::pred_type>(val);
}

template <class ValueT>
inline Case<typename ConditionValueTypeTraits<const ValueT&>::pred_type> TPL_CALL case_(ValueT& val) {
	return Case<typename ConditionValueTypeTraits<const ValueT&>::pred_type>(val);
}

template <class ValueT>
inline Case<Pred_<const ValueT&> > TPL_CALL case_(const Var<ValueT>& var_) {
	return Case<Pred_<const ValueT&> >(var_.val);
}

template <class ValueT>
inline Case<Pred_<const ValueT&> > TPL_CALL case_(Var<ValueT>& var_) {
	return Case<Pred_<const ValueT&> >(var_.val);
}

inline Case<True_> TPL_CALL default_() {
	return Case<True_>();
}

// =========================================================================
// class CondAct

template <class CondT, class ActT>
class CondAct
{
public:
	const CondT m_cond;
	const ActT m_act;

public:
	CondAct() : m_cond(), m_act() {}
	CondAct(const CondT& cond, const ActT& act)
		: m_cond(cond), m_act(act) {}

public:
	enum { character = 0 };

	typedef typename CondT::value_type value_type;

	template <class ValueT, class SourceT, class ContextT>
	bool TPL_CALL match_if(const ValueT& val, SourceT& ar, ContextT& context) const
	{
		if (!m_cond(val))
			return false;
		m_act();
		return true;
	}
};

// Usage: Case/Action ==> Case[eps()/Action]

template <class T1, class T2>
__forceinline
Condition<CondAct<T1, T2> >
TPL_CALL operator/(const Case<T1>& x, const SimpleAction<T2>& y) {
	return Condition<CondAct<T1, T2> >(x.m_cond, y);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CASE_H */
