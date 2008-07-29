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
// Module: tpl/regex/Predicate.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Predicate.h 636 2008-06-11 07:19:52Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_PREDICATE_H
#define TPL_REGEX_PREDICATE_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_DETAIL_TYPETRAITS_H
#include "detail/TypeTraits.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// TPL_PRED_TEST

template <class ValueT>
class EQICase
{
private:
	const ValueT m_val;
	
public:
	template <class T1>
	explicit EQICase(const T1& val) : m_val(val) {
	}

	template <class ValueT2>
	bool TPL_CALL operator()(const ValueT2& val) const {
		return std::icompare(val, m_val) == 0;
	}
};

#define TPL_PRED_IMPL_(PredT, op)							\
template <class ValueT>										\
class PredT													\
{															\
private:													\
	const ValueT m_val;										\
															\
public:														\
	template <class T1>										\
	explicit PredT(const T1& val) : m_val(val) {			\
	}														\
															\
	template <class ValueT2>								\
	bool TPL_CALL operator()(const ValueT2& val) const {	\
		return m_val op val;								\
	}														\
};

TPL_PRED_IMPL_(EQ, ==)
TPL_PRED_IMPL_(NE, !=)
TPL_PRED_IMPL_(GT, >)
TPL_PRED_IMPL_(LT, <)
TPL_PRED_IMPL_(GE, >=)
TPL_PRED_IMPL_(LE, <=)

// -------------------------------------------------------------------------
// TPL_PRED

template <class RefT, template <class ValueT> class PredT>
struct PredOpTraits {
	typedef SmartRefTraits<RefT> Tr_;
	typedef PredT<typename Tr_::const_type> Pred_;
	typedef Predicate<Pred_> pred_type;
};

#define TPL_PRED(op, PredT)													\
	template <class ValueT>													\
	inline typename PredOpTraits<const ValueT&, PredT>::pred_type			\
	TPL_CALL op(const ValueT& val) {										\
		return typename PredOpTraits<const ValueT&, PredT>::pred_type(val);	\
	}																		\
																			\
	template <class ValueT>													\
	inline typename PredOpTraits<ValueT&, PredT>::pred_type					\
	TPL_CALL op(ValueT& val) {												\
		return typename PredOpTraits<ValueT&, PredT>::pred_type(val);		\
	}

TPL_PRED(eq, EQ)
TPL_PRED(ne, NE)
TPL_PRED(lt, LT)
TPL_PRED(gt, GT)
TPL_PRED(le, LE)
TPL_PRED(ge, GE)

TPL_PRED(notEq, NE)
TPL_PRED(eqICase, EQICase)

// =========================================================================
// class PredRule

template <class RegExT, class PredT>
class PredRule : public RegExT
{
private:
	const PredT m_pred;
	
public:
	PredRule() : RegExT(), m_pred() {}

	PredRule(const RegExT& rule_, const PredT& ref_)
		: RegExT(rule_), m_pred(ref_) {}
	
	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		TPL_ASSIG_PREPARE1(typename RegExT::assig_tag)
		
		typename ContextT::template trans_type<RegExT::character> trans(ar, context);
		const iterator pos = ar.position();
		if (RegExT::match(ar, context)) {
			const iterator pos2 = ar.position();
			const value_type val(TPL_ASSIG_GET(pos, pos2, (const RegExT*)this));
			if (m_pred(val))
				return true;
		}
		trans.rollback(ar);
		return false;
	}
};

template <class T1, class T2>
inline Rule<PredRule<T1, T2> > const
TPL_CALL operator/(const Rule<T1>& x, const Predicate<T2>& y) {
	return Rule<PredRule<T1, T2> >(x, y);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_PREDICATE_H */

