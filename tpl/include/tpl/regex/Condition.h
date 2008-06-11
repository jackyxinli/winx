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
// $Id: Condition.h 636 2008-06-11 07:19:52Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_CONDITION_H
#define TPL_REGEX_CONDITION_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// function case_()

template <class CondT, class NextT>
class CaseBind
{
public:
	const CondT m_cond;
	const NextT m_next;

public:
	CaseBind() : m_cond(), m_next() {}
	CaseBind(const CondT& cond, const NextT& next_)
		: m_cond(cond), m_next(next_) {}
	
public:
	enum { character = NextT::character };
	
	typedef typename NextT::assig_tag assig_tag;
	typedef typename NextT::convertible_type convertible_type;
	
	template <class Iterator, class SourceT, class ContextT>
	bool TPL_CALL match_if(
		Iterator pos, Iterator pos2,
		SourceT& ar, ContextT& context) const
	{
		return m_cond(pos, pos2) && m_next.match(ar, context);
	}
};

template <class CondT, class NextT>
class GCaseBind
{
public:
	const CondT m_cond;
	const NextT m_next;

public:
	GCaseBind() : m_cond(), m_next() {}
	GCaseBind(const CondT& cond, const NextT& next_)
		: m_cond(cond), m_next(next_) {}
	
public:
	enum { character = NextT::character };
	
	typedef typename NextT::assig_tag assig_tag;
	
	template <class Iterator, class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match_if(
		Iterator pos, Iterator pos2,
		SourceT& ar, ContextT& context, const Skipper<SkipperT>& skipper_) const
	{
		return m_cond(pos, pos2) && m_next.match(ar, context, skipper_);
	}
};

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
	Condition<CaseBind<CondT, NextT> > TPL_CALL operator[](const Rule<NextT>& next_) const {
		return Condition<CaseBind<CondT, NextT> >(m_cond, next_);
	}

	template <class NextT>
	GCondition<GCaseBind<CondT, NextT> > TPL_CALL operator[](const Grammar<NextT>& next_) const {
		return GCondition<GCaseBind<CondT, NextT> >(m_cond, next_);
	}
};

template <class CondT>
inline Case<CondT> TPL_CALL case_(const CondT& cond) {
	return Case<CondT>(cond);
}

inline Case<EqStr> TPL_CALL case_(const char* s) {
	return Case<EqStr>(s);
}

inline Case<EqWStr> TPL_CALL case_(const wchar_t* s) {
	return Case<EqWStr>(s);
}

// =========================================================================
// operator,

template <class CondT1, class CondT2>
class CondOr
{
private:
	const CondT1 m_x;
	const CondT2 m_y;

public:
	CondOr(const CondT1& x, const CondT2& y)
		: m_x(x), m_y(y) {}

public:
	enum { character = CondT1::character | CondT2::character };
	
	typedef TagAssigNone assig_tag;
	typedef ExplicitConvertible convertible_type;
	
	template <class Iterator, class SourceT, class ContextT>
	bool TPL_CALL match_if(
		Iterator pos, Iterator pos2,
		SourceT& ar, ContextT& context) const
	{
		return m_x.match_if(pos, pos2, ar, context) ||
			m_y.match_if(pos, pos2, ar, context);
	}
};

template <class CondT1, class CondT2>
class GCondOr
{
private:
	const CondT1 m_x;
	const CondT2 m_y;

public:
	GCondOr(const CondT1& x, const CondT2& y)
		: m_x(x), m_y(y) {}

public:
	enum { character = CondT1::character | CondT2::character };
	
	typedef TagAssigNone assig_tag;
	
	template <class Iterator, class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match_if(
		Iterator pos, Iterator pos2,
		SourceT& ar, ContextT& context, const Skipper<SkipperT>& skipper_) const
	{
		return m_x.match_if(pos, pos2, ar, context, skipper_) ||
			m_y.match_if(pos, pos2, ar, context, skipper_);
	}
};

template <class T1, class T2>
__forceinline
Condition<CondOr<T1, T2> >
TPL_CALL operator,(const Condition<T1>& x, const Condition<T2>& y) {
	return Condition<CondOr<T1, T2> >(x, y);
}

template <class T1, class T2>
__forceinline
GCondition<GCondOr<T1, T2> >
TPL_CALL operator,(const GCondition<T1>& x, const GCondition<T2>& y) {
	return GCondition<GCondOr<T1, T2> >(x, y);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CONDITION_H */
