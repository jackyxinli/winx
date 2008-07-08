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
// Module: tpl/regex/Condition.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Condition.h 731 2008-06-25 21:47:21Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_CONDITION_H
#define TPL_REGEX_CONDITION_H

#ifndef TPL_REGEX_ACTION_H
#include "Action.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// AndValueType

template <class T1, class T2>
struct AndValueType {
};

template <class T1>
struct AndValueType<T1, T1> {
	typedef T1 value_type;
};

template <class T1>
struct AndValueType<T1, DefaultType> {
	typedef T1 value_type;
};

template <class T2>
struct AndValueType<DefaultType, T2> {
	typedef T2 value_type;
};

template <>
struct AndValueType<DefaultType, DefaultType> {
	typedef DefaultType value_type;
};

// =========================================================================
// class Cond

template <class RegExT, class ConditionT>
class Cond
{
public:
	const RegExT m_x;
	const ConditionT m_y;
	
public:
	Cond() : m_x(), m_y() {}
	Cond(const RegExT& x, const ConditionT& y) : m_x(x), m_y(y) {}
	
public:
	enum { character = RegExT::character | ConditionT::character };

	typedef TagAssigNone assig_tag;
	typedef ExplicitConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		TPL_ASSIG_PREPARE(typename RegExT::assig_tag, typename ConditionT::value_type)
		
		typename ContextT::template trans_type<RegExT::character> trans(ar, context);
		const iterator pos = ar.position();
		if (m_x.match(ar, context)) {
			const iterator pos2 = ar.position();
			const value_type val(TPL_ASSIG_GET(pos, pos2, &m_x));
			if (m_y.match_if(val, ar, context) == matchOk)
				return true;
		}
		trans.rollback(ar);
		return false;
	}
};

template <class RegExT, class ConditionT>
struct IndexOpTraits<Rule<RegExT>, Condition<ConditionT> >
{
	typedef Rule<Cond<RegExT, ConditionT> > result_type;

	static result_type TPL_CALL call(const Rule<RegExT>& rule_, const Condition<ConditionT>& cond_) {
		return result_type(rule_, cond_);
	}
};

// =========================================================================
// class CondBind

template <class CondT, class NextT>
class CondBind
{
public:
	const CondT m_cond;
	const NextT m_next;

public:
	CondBind() : m_cond(), m_next() {}
	CondBind(const CondT& cond, const NextT& next_)
		: m_cond(cond), m_next(next_) {}

public:
	enum { character = NextT::character };

	typedef typename CondT::value_type value_type;

	template <class ValueT, class SourceT, class ContextT>
	MatchCode TPL_CALL match_if(const ValueT& val, SourceT& ar, ContextT& context) const
	{
		if (m_cond(val))
			return m_next.match(ar, context) ? matchOk : matchStop;
		else
			return matchFailed;
	}
};

// =========================================================================
// class CondTrue

template <class ValT>
class CondTrue
{
public:
	enum { character = 0 };

	typedef ValT value_type;

	template <class ValueT, class SourceT, class ContextT>
	MatchCode TPL_CALL match_if(const ValueT& val, SourceT& ar, ContextT& context) const {
		return matchOk;
	}
};

// =========================================================================
// operator,

template <class CondT1, class CondT2>
class CondOr
{
private:
	const CondT1 m_x;
	const CondT2 m_y;

	typedef typename CondT1::value_type T1;
	typedef typename CondT2::value_type T2;

public:
	CondOr(const CondT1& x, const CondT2& y)
		: m_x(x), m_y(y) {}

public:
	enum { character = CondT1::character | CondT2::character };
	
	typedef typename AndValueType<T1, T2>::value_type value_type;
	
	template <class ValueT, class SourceT, class ContextT>
	MatchCode TPL_CALL match_if(const ValueT& val, SourceT& ar, ContextT& context) const
	{
		MatchCode result = m_x.match_if(val, ar, context);
		if (result != matchFailed)
			return result;
		else
			return m_y.match_if(val, ar, context);
	}
};

template <class T1, class T2>
inline Condition<CondOr<T1, T2> > const
TPL_CALL operator,(const Condition<T1>& x, const Condition<T2>& y) {
	return Condition<CondOr<T1, T2> >(x, y);
}

template <class T1>
inline Condition<CondOr<T1, CondTrue<typename T1::value_type> > > const
TPL_CALL operator,(const Condition<T1>& x, const bool fTrue) {
	TPL_ASSERT(fTrue == true);
	typedef CondTrue<typename T1::value_type> T2;
	return Condition<CondOr<T1, T2> >(x, T2());
}

// =========================================================================
// class CondAct

template <class CondT, class ActT>
class CondAct
{
private:
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
	MatchCode TPL_CALL match_if(const ValueT& val, SourceT& ar, ContextT& context) const
	{
		MatchCode result = m_cond.match_if(val, ar, context);
		if (result == matchOk)
			m_act();
		return result;
	}
};

// Usage: Condition/Action ==> Condition[eps()/Action]
// Implementation: move to <tpl/regex/grammar/Action.h>

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CONDITION_H */
