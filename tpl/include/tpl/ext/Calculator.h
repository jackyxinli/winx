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

// -------------------------------------------------------------------------

NS_TPL_BEGIN

inline void throw_arity_error() {
	throw std::invalid_argument("function arity is not consistent!");
}

// class UnaryFn

template <class Op, class StackT>
class UnaryFn
{
private:
	typedef typename StackT::value_type value_type;

private:
	StackT& m_stk;
	Op m_op;

public:
	UnaryFn(StackT& stk) : m_stk(stk) {}

	template <class T1>
	UnaryFn(StackT& stk, const T1& arg) : m_stk(stk), m_op(arg) {}

public:
	void TPL_CALL operator()() const {
		TPL_ASSERT(m_stk.size() >= 1);
		if (m_stk.size() < 1)
			throw_arity_error();
		value_type x = m_stk.top();
		m_stk.pop();
		m_stk.push(m_op(x));
	}
};

// class BinaryFn

template <class Op, class StackT>
class BinaryFn
{
private:
	typedef typename StackT::value_type value_type;

private:
	StackT& m_stk;
	Op m_op;

public:
	BinaryFn(StackT& stk) : m_stk(stk) {}

	template <class T1>
	BinaryFn(StackT& stk, const T1& arg) : m_stk(stk), m_op(arg) {}

public:
	void TPL_CALL operator()() const {
		TPL_ASSERT(m_stk.size() >= 2);
		value_type y = m_stk.top();
		if (m_stk.size() < 2)
			throw_arity_error();
		m_stk.pop();
		value_type x = m_stk.top();
		m_stk.pop();
		m_stk.push(m_op(x, y));
	}
};

// class TernaryFn: 3-ary

template <class Op, class StackT>
class TernaryFn
{
private:
	typedef typename StackT::value_type value_type;

private:
	StackT& m_stk;
	Op m_op;

public:
	TernaryFn(StackT& stk) : m_stk(stk) {}

	template <class T1>
	TernaryFn(StackT& stk, const T1& arg) : m_stk(stk), m_op(arg) {}

public:
	void TPL_CALL operator()() const {
		TPL_ASSERT(m_stk.size() >= 3);
		if (m_stk.size() < 3)
			throw_arity_error();
		value_type z = m_stk.top();
		m_stk.pop();
		value_type y = m_stk.top();
		m_stk.pop();
		value_type x = m_stk.top();
		m_stk.pop();
		m_stk.push(m_op(x, y, z));
	}
};

// -------------------------------------------------------------------------
// function calc/1-ary

template <int nArity, class Op, class StackT>
struct CalcActionTraits_ {
};

template <class Op, class StackT>
struct CalcActionTraits_<1, Op, StackT> {
	typedef UnaryFn<Op, StackT> action_type;
};

template <class Op, class StackT>
struct CalcActionTraits_<2, Op, StackT> {
	typedef BinaryFn<Op, StackT> action_type;
};

template <class Op, class StackT>
struct CalcActionTraits_<3, Op, StackT> {
	typedef TernaryFn<Op, StackT> action_type;
};

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
	enum { is_unary = boost::detail::is_convertible_impl<Op, std::unary_function<Ty, Ty> >::value };
	enum { is_binary = boost::detail::is_convertible_impl<Op, std::binary_function<Ty, Ty, Ty> >::value };
	enum { n_arity = ArityTratis_<is_unary, is_binary>::value };

public:
	typedef typename CalcActionTraits_<n_arity, Op, StackT>::action_type action_type;
};

template <template <class Type> class Op_, class StackT> __forceinline
SimpleAction<typename CalcTraits_<Op_, StackT>::action_type> TPL_CALL calc(StackT& stk) {
	return SimpleAction<typename CalcTraits_<Op_, StackT>::action_type>(stk);	
}

// -------------------------------------------------------------------------
// function calc/2-ary

template <class StackT>
class CalcTraits_1_
{
private:
	typedef typename StackT::value_type Ty;
	typedef typename StackT::value_type (*fn_)(typename StackT::value_type);
	typedef fn_ Op;

public:
	typedef typename CalcActionTraits_<1, Op, StackT>::action_type action_type;
};

template <class StackT>
class CalcTraits_2_
{
private:
	typedef typename StackT::value_type Ty;
	typedef typename StackT::value_type (*fn_)(typename StackT::value_type, typename StackT::value_type);
	typedef fn_ Op;

public:
	typedef typename CalcActionTraits_<2, Op, StackT>::action_type action_type;
};

template <class StackT>
class CalcTraits_3_
{
private:
	typedef typename StackT::value_type Ty;
	typedef typename StackT::value_type (*fn_)(typename StackT::value_type, typename StackT::value_type, typename StackT::value_type);
	typedef fn_ Op;

public:
	typedef typename CalcActionTraits_<3, Op, StackT>::action_type action_type;
};

template <class StackT> __forceinline
SimpleAction<typename CalcTraits_1_<StackT>::action_type>
TPL_CALL calc(StackT& stk, typename StackT::value_type (*fn)(typename StackT::value_type)) {
	return SimpleAction<typename CalcTraits_1_<StackT>::action_type>(stk, fn);
}

template <class StackT> __forceinline
SimpleAction<typename CalcTraits_2_<StackT>::action_type>
TPL_CALL calc(StackT& stk, typename StackT::value_type (*fn)(typename StackT::value_type, typename StackT::value_type)) {
	return SimpleAction<typename CalcTraits_2_<StackT>::action_type>(stk, fn);
}

template <class StackT> __forceinline
SimpleAction<typename CalcTraits_3_<StackT>::action_type>
TPL_CALL calc(StackT& stk, typename StackT::value_type (*fn)(typename StackT::value_type, typename StackT::value_type, typename StackT::value_type)) {
	return SimpleAction<typename CalcTraits_2_<StackT>::action_type>(stk, fn);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_EXT_CALCULATOR_H */
