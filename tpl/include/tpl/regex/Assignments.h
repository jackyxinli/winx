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
// Module: tpl/regex/Assignments.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_ASSIGNMENTS_H
#define TPL_REGEX_ASSIGNMENTS_H

#ifndef TPL_REGEX_ASSIGNMENT_H
#include "Assignment.h"
#endif

#ifndef TPL_REGEX_STLHEADERS_H
#include "STLHeaders.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class Push_

template <class StackT>
class Push_
{
private:
	StackT& m_result;

public:
	Push_(StackT& result) : m_result(result) {
	}

	typedef typename StackT::value_type value_type;

	void TPL_CALL operator()(const value_type& val) const {
		m_result.push(val);
	}
};

// -------------------------------------------------------------------------

#if defined(TPL_HAS_STACK)

template <class ValueT, class Container>
class Assign<std::stack<ValueT, Container> > : public Push_<std::stack<ValueT, Container> >
{
public:
	Assign(std::stack<ValueT, Container>& result)
		: Push_<std::stack<ValueT, Container> >(result) {}
};

#endif // defined(TPL_HAS_STACK)

// -------------------------------------------------------------------------

#if defined(TPL_HAS_QUEUE)

template <class ValueT, class Container>
class Assign<std::queue<ValueT, Container> > : public Push_<std::queue<ValueT, Container> >
{
public:
	Assign(std::queue<ValueT, Container>& result)
		: Push_<std::queue<ValueT, Container> >(result) {}
};

#endif // defined(TPL_HAS_QUEUE)

// =========================================================================
// class PushBack_

template <class ContainerT>
class PushBack_
{
private:
	ContainerT& m_result;

public:
	PushBack_(ContainerT& result) : m_result(result) {
	}

	typedef typename ContainerT::value_type value_type;

	void TPL_CALL operator()(const value_type& val) const {
		m_result.push_back(val);
	}
};

// -------------------------------------------------------------------------

#if defined(TPL_HAS_VECTOR)

template <class ValueT, class Ax>
class Assign<std::vector<ValueT, Ax> > : public PushBack_<std::vector<ValueT, Ax> >
{
public:
	Assign(std::vector<ValueT, Ax>& result)
		: PushBack_<std::vector<ValueT, Ax> >(result) {}
};

#endif // defined(TPL_HAS_VECTOR)

// -------------------------------------------------------------------------

#if defined(TPL_HAS_DEQUE)

template <class ValueT, class Ax>
class Assign<std::deque<ValueT, Ax> > : public PushBack_<std::deque<ValueT, Ax> >
{
public:
	Assign(std::deque<ValueT, Ax>& result)
		: PushBack_<std::deque<ValueT, Ax> >(result) {}
};

#endif

// -------------------------------------------------------------------------

#if defined(TPL_HAS_LIST)

template <class ValueT, class Ax>
class Assign<std::list<ValueT, Ax> > : public PushBack_<std::list<ValueT, Ax> >
{
public:
	Assign(std::list<ValueT, Ax>& result)
		: PushBack_<std::list<ValueT, Ax> >(result) {}
};

#endif

// =========================================================================
// class Insert_

template <class ContainerT>
class Insert_
{
private:
	ContainerT& m_result;

public:
	Insert_(ContainerT& result) : m_result(result) {
	}

	typedef typename ContainerT::value_type value_type;

	void TPL_CALL operator()(const value_type& val) const {
		m_result.insert(val);
	}
};

// -------------------------------------------------------------------------

#if defined(TPL_HAS_SET)

template <class ValueT, class Pr, class Ax>
class Assign<std::set<ValueT, Pr, Ax> > : public Insert_<std::set<ValueT, Pr, Ax> >
{
public:
	Assign(std::set<ValueT, Pr, Ax>& result)
		: Insert_<std::set<ValueT, Pr, Ax> >(result) {}
};

template <class ValueT, class Pr, class Ax>
class Assign<std::multiset<ValueT, Pr, Ax> > : public Insert_<std::multiset<ValueT, Pr, Ax> >
{
public:
	Assign(std::multiset<ValueT, Pr, Ax>& result)
		: Insert_<std::multiset<ValueT, Pr, Ax> >(result) {}
};

#endif

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_ASSIGNMENTS_H */
