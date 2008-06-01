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
// Module: tpl/Lambda.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Lambda.h 554 2008-05-25 07:50:50Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BOOST_LAMBDA_H
#define TPL_BOOST_LAMBDA_H

#pragma warning(disable:4819)

#ifndef BOOST_LAMBDA_LAMBDA_HPP
#include <boost/lambda/lambda.hpp>
#endif

#ifndef TPL_REGEX_ASSIGNMENT_H
#include "../regex/Assignment.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class Lambda

template <class ValueT, class OperaT, int arity = boost::lambda::get_arity<OperaT>::value >
class Calc
{
};

template <class ValueT, class OperaT>
class Calc<ValueT, OperaT, 0x01>
{
private:
	OperaT m_opera;

public:
	typedef ValueT value_type;
	typedef Action<Calc> action_type;

public:
	Calc(const OperaT& opera) : m_opera(opera) {
	}

	void TPL_CALL operator()(const value_type& val) const {
		m_opera(val);
	}
};

template <class ValueT, class OperaT>
class Calc<ValueT, OperaT, 0x00>
{
private:
	OperaT m_opera;

public:
	typedef SimpleAction<Calc> action_type;

public:
	Calc(const OperaT& opera) : m_opera(opera) {
	}

	void TPL_CALL operator()() const {
		m_opera();
	}
};

template <class ValueT>
class Lambda
{
public:
	template <class OperaT>
	typename Calc<ValueT, OperaT>::action_type TPL_CALL operator[](const OperaT& op) const {
		return typename Calc<ValueT, OperaT>::action_type(op);
	}
};

// -------------------------------------------------------------------------
// class tpl::Local<ValueT>::Ref

template <class ValueT>
class Local
{
public:
	typedef boost::lambda::lambda_functor<boost::lambda::identity<ValueT&> > Ref;
};

#define TPL_LOCAL(ValueT, variable)												\
	ValueT _tpl_##variable = ValueT();											\
	tpl::Local<ValueT>::Ref variable = boost::lambda::var(_tpl_##variable)

template <class ValueT>
__forceinline Action<Assign<ValueT> > TPL_CALL assign(
	const boost::lambda::lambda_functor<boost::lambda::identity<ValueT&> >& var_)
{
	ValueT& result = var_();
	return Action<Assign<ValueT> >(result);
}

template <class ValueT>
__forceinline Action<Assign<ValueT> > TPL_CALL assign(
	boost::lambda::lambda_functor<boost::lambda::identity<ValueT&> >& var_)
{
	ValueT& result = var_();
	return Action<Assign<ValueT> >(result);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEXP_H */
