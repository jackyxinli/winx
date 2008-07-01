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
// Module: tpl/regex/grammar/Condition.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Condition.h 731 2008-06-25 21:47:21Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_GRAMMAR_CONDITION_H
#define TPL_REGEX_GRAMMAR_CONDITION_H

#ifndef TPL_REGEX_BASIC_H
#include "../Basic.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class GCond

template <class RegExT, class ConditionT>
class GCond
{
public:
	const RegExT m_x;
	const ConditionT m_y;
	
public:
	GCond() : m_x(), m_y() {}
	GCond(const RegExT& x, const ConditionT& y) : m_x(x), m_y(y) {}
	
public:
	enum { character = RegExT::character | ConditionT::character };

	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		typedef typename SourceT::iterator iterator;
		typedef SelectValueType<typename ConditionT::value_type, std::Range<iterator> > SelectT;
		typedef typename SelectT::value_type value_type;
		typedef typename SelectAssig<typename RegExT::assig_tag, value_type>::assig_type assig_type;

		skipper_.match(ar, context);
		typename ContextT::template trans_type<RegExT::character> trans(ar, context);
		const iterator pos = ar.position();
		if (m_x.match(ar, context)) {
			const iterator pos2 = ar.position();
			const value_type val(assig_type::template get<value_type>(pos, pos2, &m_x));
			if (m_y.match_if(val, ar, context, skipper_) == matchOk)
				return true;
		}
		trans.rollback(ar);
		return false;
	}
};

template <class RegExT, class ConditionT>
struct BindTraits<Rule<RegExT>, GCondition<ConditionT> >
{
	typedef Grammar<GCond<RegExT, ConditionT> > bind_type;

	static bind_type TPL_CALL bind(const Rule<RegExT>& rule_, const GCondition<ConditionT>& cond_) {
		return bind_type(rule_, cond_);
	}
};

// =========================================================================
// class GCondBind

template <class CondT, class NextT>
class GCondBind
{
public:
	const CondT m_cond;
	const NextT m_next;

public:
	GCondBind() : m_cond(), m_next() {}
	GCondBind(const CondT& cond, const NextT& next_)
		: m_cond(cond), m_next(next_) {}
	
public:
	enum { character = NextT::character };
	
	typedef typename CondT::value_type value_type;
	
	template <class ValueT, class SourceT, class ContextT, class SkipperT>
	MatchCode TPL_CALL match_if(
		const ValueT& val, SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		if (m_cond(val))
			return m_next.match(ar, context, skipper_) ? matchOk : matchStop;
		else
			return matchFailed;
	}
};

// =========================================================================
// operator,

template <class CondT1, class CondT2>
class GCondOr
{
private:
	const CondT1 m_x;
	const CondT2 m_y;
	
	typedef typename CondT1::value_type T1;
	typedef typename CondT2::value_type T2;

public:
	GCondOr(const CondT1& x, const CondT2& y)
		: m_x(x), m_y(y) {}

public:
	enum { character = CondT1::character | CondT2::character };
	
	typedef typename AndValueType<T1, T2>::value_type value_type;

	template <class ValueT, class SourceT, class ContextT, class SkipperT>
	MatchCode TPL_CALL match_if(
		const ValueT& val, SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		MatchCode result = m_x.match_if(val, ar, context, skipper_);
		if (result != matchFailed)
			return result;
		else
			return m_y.match_if(val, ar, context, skipper_);
	}
};

template <class T1, class T2>
__forceinline
GCondition<GCondOr<T1, T2> >
TPL_CALL operator,(const GCondition<T1>& x, const GCondition<T2>& y) {
	return GCondition<GCondOr<T1, T2> >(x, y);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_GRAMMAR_CONDITION_H */
