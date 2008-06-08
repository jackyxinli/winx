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
// Module: tpl/ext/Calculator.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_EXT_CALCULATOR_H
#define TPL_EXT_CALCULATOR_H

#ifndef TPL_REGEXP_H
#include "../RegExp.h"
#endif

#if !defined(_FUNCTIONAL_) && !defined(_FUNCTIONAL)
#include <functional>
#endif

#pragma warning(disable:4819)

#ifndef BOOST_TT_IS_CONVERTIBLE_HPP_INCLUDED
#include <boost/type_traits/is_convertible.hpp>
#endif

NS_TPL_BEGIN

// =========================================================================
// class Caller

template <int nArity>
struct Caller {};

template <>
struct Caller<1> {
	template <class Op, class ValT>
	__forceinline static ValT TPL_CALL call(const Op& op_, const ValT args[]) {
		return op_(args[0]);
	}
};

template <>
struct Caller<2> {
	template <class Op, class ValT>
	__forceinline static ValT TPL_CALL call(const Op& op_, const ValT args[]) {
		return op_(args[0], args[1]);
	}
};

template <>
struct Caller<3> {
	template <class Op, class ValT>
	__forceinline static ValT TPL_CALL call(const Op& op_, const ValT args[]) {
		return op_(args[0], args[1], args[2]);
	}
};

template <>
struct Caller<4> {
	template <class Op, class ValT>
	__forceinline static ValT TPL_CALL call(const Op& op_, const ValT args[]) {
		return op_(args[0], args[1], args[2], args[3]);
	}
};

template <>
struct Caller<5> {
	template <class Op, class ValT>
	__forceinline static ValT TPL_CALL call(const Op& op_, const ValT args[]) {
		return op_(args[0], args[1], args[2], args[3], args[4]);
	}
};

template <>
struct Caller<6> {
	template <class Op, class ValT>
	__forceinline static ValT TPL_CALL call(const Op& op_, const ValT args[]) {
		return op_(args[0], args[1], args[2], args[3], args[4], args[5]);
	}
};

// =========================================================================
// class Function

template <class Op, class StackT, int nArity>
class Function
{
private:
	typedef typename StackT::value_type value_type;

private:
	StackT& m_stk;
	const Op m_op;

public:
	Function(StackT& stk) : m_stk(stk), m_op() {}

	template <class T1>
	Function(StackT& stk, const T1& arg) : m_stk(stk), m_op(arg) {}

public:
	void TPL_CALL operator()() const {
		TPL_ASSERT(m_stk.size() >= nArity);
		value_type args[nArity];
		for (int i = nArity; i--;) {		
			args[i] = m_stk.back();
			m_stk.pop_back();
		}
		m_stk.push_back(Caller<nArity>::call(m_op, args));
	}
};

// class Unary-Function

template <class Op, class StackT>
class Function<Op, StackT, 1>
{
private:
	typedef typename StackT::value_type value_type;

private:
	StackT& m_stk;
	const Op m_op;

public:
	Function(StackT& stk) : m_stk(stk), m_op() {}

	template <class T1>
	Function(StackT& stk, const T1& arg) : m_stk(stk), m_op(arg) {}

public:
	void TPL_CALL operator()() const {
		TPL_ASSERT(m_stk.size() >= 1);
		value_type x = m_stk.back();
		m_stk.pop_back();
		m_stk.push_back(m_op(x));
	}
};

// -------------------------------------------------------------------------
// function calc/1-ary

template <int isUnary, int isBinary, int isTernary = 0>
struct ArityTratis_ {};

template <>
struct ArityTratis_<1, 0, 0> { enum { value = 1 }; };

template <>
struct ArityTratis_<0, 1, 0> { enum { value = 2 }; };

template <>
struct ArityTratis_<0, 0, 1> { enum { value = 3 }; };

template <template <class Type> class Op_, class StackT>
class CalcTraits_
{
private:
	typedef typename StackT::value_type Ty;
	typedef Op_<Ty> Op;
	enum { is_unary = boost::is_convertible<Op, std::unary_function<Ty, Ty> >::value };
	enum { is_binary = boost::is_convertible<Op, std::binary_function<Ty, Ty, Ty> >::value };
	enum { n_arity = ArityTratis_<is_unary, is_binary>::value };

public:
	typedef Function<Op, StackT, n_arity> action_type;
};

template <template <class Type> class Op_, class StackT> __forceinline
SimpleAction<typename CalcTraits_<Op_, StackT>::action_type> TPL_CALL calc(StackT& stk) {
	return SimpleAction<typename CalcTraits_<Op_, StackT>::action_type>(stk);	
}

// =========================================================================
// function check_arity

// Usage: check_arity<nArity>(nArgv)

inline void throw_arity_error() {
	throw std::invalid_argument("function arity is not consistent!");
}

template <int nArity>
class CheckArity
{
private:
	const int& m_nArgv;

public:
	CheckArity(const int& nArgv)
		: m_nArgv(nArgv) {}

public:
	void TPL_CALL operator()() const {
		if (m_nArgv != nArity)
			throw_arity_error();
	}
};

template <int nArity> __forceinline
SimpleAction<CheckArity<nArity> > TPL_CALL check_arity(const int& nArgv) {
	return SimpleAction<CheckArity<nArity> >(nArgv);	
}

// =========================================================================
// function calc/2-ary, calc/3-ary

template <int nArity, class StackT>
struct CalcTraits_N_ {};

template <class StackT>
struct CalcTraits_N_<1, StackT>
{
	typedef typename StackT::value_type Ty;
	typedef Ty (*Op)(Ty);
	typedef Function<Op, StackT, 1> action_type;
};

template <class StackT>
struct CalcTraits_N_<2, StackT>
{
	typedef typename StackT::value_type Ty;
	typedef Ty (*Op)(Ty, Ty);
	typedef Function<Op, StackT, 2> action_type;
};

template <class StackT>
struct CalcTraits_N_<3, StackT>
{
	typedef typename StackT::value_type Ty;
	typedef Ty (*Op)(Ty, Ty, Ty);
	typedef Function<Op, StackT, 3> action_type;
};

template <class StackT>
struct CalcTraits_N_<4, StackT>
{
	typedef typename StackT::value_type Ty;
	typedef Ty (*Op)(Ty, Ty, Ty, Ty);
	typedef Function<Op, StackT, 4> action_type;
};

template <class StackT>
struct CalcTraits_N_<5, StackT>
{
	typedef typename StackT::value_type Ty;
	typedef Ty (*Op)(Ty, Ty, Ty, Ty, Ty);
	typedef Function<Op, StackT, 5> action_type;
};

template <class StackT>
struct CalcTraits_N_<6, StackT>
{
	typedef typename StackT::value_type Ty;
	typedef Ty (*Op)(Ty, Ty, Ty, Ty, Ty, Ty);
	typedef Function<Op, StackT, 6> action_type;
};

template <int nArity, class StackT>
struct CalcTraitsEx_
{
	typedef CalcTraits_N_<nArity, StackT> Tr_;
	typedef CheckArity<nArity> Action1;
	typedef typename Tr_::action_type Action2;
	typedef typename Tr_::Op Op;
	typedef AndSAct<Action1, Action2> action_type;
};

#define TPL_CALC_IMPL2_(nArity)															\
template <class StackT> __forceinline													\
SimpleAction<typename CalcTraits_N_<nArity, StackT>::action_type> TPL_CALL				\
calc(StackT& stk, typename CalcTraits_N_<nArity, StackT>::Op fn) {						\
	return SimpleAction<typename CalcTraits_N_<nArity, StackT>::action_type>(stk, fn);	\
}

#define TPL_CALC_IMPL3_(nArity)															\
template <class StackT> __forceinline													\
SimpleAction<typename CalcTraitsEx_<nArity, StackT>::action_type> TPL_CALL				\
calc(StackT& stk, typename CalcTraitsEx_<nArity, StackT>::Op fn, const int& nArgv) {	\
	return SimpleAction<typename CalcTraitsEx_<nArity, StackT>::Action1>(nArgv) +		\
			SimpleAction<typename CalcTraitsEx_<nArity, StackT>::Action2>(stk, fn);		\
}

#define TPL_CALC_IMPL_(nArity)															\
	TPL_CALC_IMPL2_(nArity)																\
	TPL_CALC_IMPL3_(nArity)

TPL_CALC_IMPL_(1)
TPL_CALC_IMPL_(2)
TPL_CALC_IMPL_(3)
TPL_CALC_IMPL_(4)
TPL_CALC_IMPL_(5)
TPL_CALC_IMPL_(6)

// =========================================================================
// class VargFunction

template <class Op, class StackT>
class VargFunction
{
private:
	typedef typename StackT::value_type value_type;

private:
	StackT& m_stk;
	const Op m_op;
	const int& m_nArgv;

public:
	VargFunction(StackT& stk, const Op& op, const int& nArgv)
		: m_stk(stk), m_op(op), m_nArgv(nArgv) {}

public:
	void TPL_CALL operator()() const {
		const int count = m_nArgv;
		value_type* args = (value_type*)alloca(sizeof(value_type) * count);	
		for (int i = count; i--;) {		
			args[i] = m_stk.back();
			m_stk.pop_back();
		}
		m_stk.push_back(m_op(static_cast<const value_type*>(args), count));
	}
};

// -------------------------------------------------------------------------
// function calc/3-ary

template <class StackT, class IntT>
class CalcTraits_Varg_
{
public:
	typedef typename StackT::value_type Ty;
	typedef Ty (*Op)(const Ty args[], IntT count);
	typedef VargFunction<Op, StackT> action_type;
};

template <class StackT> __forceinline
SimpleAction<typename CalcTraits_Varg_<StackT, int>::action_type> TPL_CALL
calc(StackT& stk, typename CalcTraits_Varg_<StackT, int>::Op fn, const int& nArity) {
	return SimpleAction<typename CalcTraits_Varg_<StackT, int>::action_type>(stk, fn, nArity);
}

template <class StackT> __forceinline
SimpleAction<typename CalcTraits_Varg_<StackT, size_t>::action_type> TPL_CALL
calc(StackT& stk, typename CalcTraits_Varg_<StackT, size_t>::Op fn, const int& nArity) {
	return SimpleAction<typename CalcTraits_Varg_<StackT, size_t>::action_type>(stk, fn, nArity);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_EXT_CALCULATOR_H */

