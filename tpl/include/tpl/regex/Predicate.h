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
// PredVTTraits

template <class ValueT>
struct PredVTTraits {
	typedef typename ValueT::value_type value_type;
	typedef ValueT pred_type;
};

template <class ValueT>
struct PredVTTraits<ValueT const&> {
	typedef typename PredVTTraits<ValueT>::value_type value_type;
	typedef typename PredVTTraits<ValueT>::pred_type pred_type;
};

// -------------------------------------------------------------------------
// TPL_PRED_TEST

struct ICaseEQ
{
	template <class T1, class T2>
	static bool TPL_CALL test(const T1& x, const T2& y) {
		return std::icompare(x, y) == 0;
	}
};

#define TPL_PRED_TEST(Cmp, op)												\
struct Cmp {																\
	template <class T1, class T2>											\
	static bool TPL_CALL test(const T1& x, const T2& y) {					\
		return x op y;														\
	}																		\
};

TPL_PRED_TEST(EQ, ==)
TPL_PRED_TEST(NE, !=)
TPL_PRED_TEST(GT, >)
TPL_PRED_TEST(LT, <)
TPL_PRED_TEST(GE, >=)
TPL_PRED_TEST(LE, <=)

template <class ValueT, class PredT = EQ>
class Pred_
{
private:
	const ValueT m_val2;
	
public:
	template <class T1>
	explicit Pred_(const T1& val) : m_val2(val) {}
	
	typedef typename PredVTTraits<ValueT>::value_type value_type;
	
	template <class ValueT2>
	bool TPL_CALL operator()(const ValueT2& val) const {
		return PredT::test(val, m_val2);
	}
};

// -------------------------------------------------------------------------
// TPL_PRED

template <class RefT, class PredT>
struct PredOpTraits {
	typedef SmartRefTraits<RefT> Tr_;
	typedef Pred_<typename Tr_::const_type, PredT> pred_type;
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
TPL_PRED(eqICase, ICaseEQ)

// -------------------------------------------------------------------------

#define TPL_PRED_VT(ValueT, CondValT)										\
template <>																	\
struct PredVTTraits<ValueT> {												\
	typedef CondValT value_type;											\
	typedef Pred_<ValueT> pred_type;										\
};																			\
template <>																	\
struct PredVTTraits<ValueT const&> {										\
	typedef CondValT value_type;											\
	typedef Pred_<ValueT const&> pred_type;									\
};

TPL_PRED_VT(bool, bool)
TPL_PRED_VT(char, char)
TPL_PRED_VT(wchar_t, wchar_t)
TPL_PRED_VT(short, int)
TPL_PRED_VT(int, int)
TPL_PRED_VT(long, long)
TPL_PRED_VT(unsigned short, unsigned int)
TPL_PRED_VT(unsigned int, unsigned int)
TPL_PRED_VT(unsigned long, unsigned long)

TPL_PRED_VT(char*, DefaultType)
TPL_PRED_VT(wchar_t*, DefaultType)
TPL_PRED_VT(char const*, DefaultType)
TPL_PRED_VT(wchar_t const*, DefaultType)

// =========================================================================
// class PredRule

template <class RegExT, class PredT>
class PredRule : public RegExT
{
private:
	const PredT m_pred;
	
public:
	PredRule() : RegExT(), m_pred() {}

	template <class T1>
	PredRule(const RegExT& rule_, const T1& ref_)
		: RegExT(rule_), m_pred(ref_) {}
	
	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		TPL_ASSIG_PREPARE(typename RegExT::assig_tag, typename PredT::value_type)
		
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

template <class RefT>
struct PredRefTraits {
	typedef SmartRefTraits<RefT> Tr_;
	typedef PredVTTraits<typename Tr_::const_type> Tr2_;
	typedef typename Tr2_::pred_type pred_type;
};

template <class RegExT, class T1Ref>
struct EvalOpTraits<Rule<RegExT>, T1Ref>
{
	typedef Rule<PredRule<RegExT, typename PredRefTraits<T1Ref>::pred_type> > result_type;

	static result_type TPL_CALL call(const Rule<RegExT>& rule_, T1Ref ref_) {
		return result_type(rule_, ref_);
	}
};

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_PREDICATE_H */

