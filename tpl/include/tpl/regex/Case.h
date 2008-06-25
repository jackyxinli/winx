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

#ifndef TPL_REGEX_CONDITION_H
#include "Condition.h"
#endif

#ifndef TPL_REGEX_GRAMMAR_CONDITION_H
#include "grammar/Condition.h"
#endif

NS_TPL_BEGIN

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

// =========================================================================
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

// =========================================================================
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

// =========================================================================

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

// =========================================================================
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
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CASE_H */