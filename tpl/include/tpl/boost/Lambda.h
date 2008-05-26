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

template <class Type, class OperaT, int arity = boost::lambda::get_arity<OperaT>::value >
class Calc
{
};

template <class Type, class OperaT>
class Calc<Type, OperaT, 0x01>
{
private:
	typedef AssignmentTypeTraits<Type> Tr_;
	typedef typename Tr_::assignment_type assignment_type;
	typedef Type value_type;

	OperaT m_opera;

public:
	typedef Action<Calc> action_type;

public:
	Calc(const OperaT& opera)
		: m_opera(opera) {
	}

	template <class Iterator>
	void TPL_CALL operator()(Iterator pos, Iterator pos2) const {
		value_type val = value_type();
		assignment_type assig(val);
		assig(pos, pos2);
		m_opera(val);
	}
};

template <class Type, class OperaT>
class Calc<Type, OperaT, 0x00>
{
private:
	OperaT m_opera;

public:
	typedef SimpleAction<Calc> action_type;

public:
	Calc(const OperaT& opera)
		: m_opera(opera) {
	}

	void TPL_CALL operator()() const {
		m_opera();
	}
};

template <class Type>
class Lambda
{
public:
	template <class OperaT>
	typename Calc<Type, OperaT>::action_type TPL_CALL operator[](const OperaT& op) const {
		return typename Calc<Type, OperaT>::action_type(op);
	}
};

namespace {
	const Lambda<double> exec = Lambda<double>();
} // unnamed

// -------------------------------------------------------------------------
// class tpl::Local<double>::Ref

template <class T>
class Local
{
public:
	typedef boost::lambda::lambda_functor<boost::lambda::identity<T&> > Ref;
};

#define TPL_LOCAL(T, variable)												\
	T _tpl_##variable = T();												\
	tpl::Local<T>::Ref variable = boost::lambda::var(_tpl_##variable)

template <class T>
__forceinline Action<typename AssignmentTypeTraits<T>::assignment_type> TPL_CALL assign(
	const boost::lambda::lambda_functor<boost::lambda::identity<T&> >& var_)
{
	T& result = var_();
	return Action<typename AssignmentTypeTraits<T>::assignment_type>(result);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEXP_H */

