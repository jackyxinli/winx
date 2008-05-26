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

// -------------------------------------------------------------------------
// class AssigStackT

template <class StackT>
class AssigStackT
{
private:
	typedef typename StackT::value_type value_type;
	typedef AssignmentTypeTraits<value_type> Tr_;
	typedef typename Tr_::assignment_type assignment_type;

	StackT& m_result;

public:
	AssigStackT(StackT& result) : m_result(result) {
	}

	template <class Iterator>
	void TPL_CALL operator()(Iterator pos, Iterator pos2) const {
		value_type val = value_type();
		assignment_type assig(val);
		assig(pos, pos2);
		m_result.push(val);
	}
};

// -------------------------------------------------------------------------
// class AssigStk

#if defined(TPL_HAS_STACK)

template <class Type, class Container>
class AssigStk : public AssigStackT<std::stack<Type, Container> >
{
public:
	AssigStk(std::stack<Type, Container>& result)
		: AssigStackT<std::stack<Type, Container> >(result) {}
};

template <class Type, class Container> __forceinline
Action<AssigStk<Type, Container> > TPL_CALL assign(std::stack<Type, Container>& result) {
	return Action<AssigStk<Type, Container> >(result);
}

template <class T1, class Type, class Container>
__forceinline
Rule<Act<T1, AssigStk<Type, Container> > >
TPL_CALL operator/(const Rule<T1>& x, std::stack<Type, Container>* result) {
	return x / assign(*result);
}

#endif // defined(TPL_HAS_STACK)

// -------------------------------------------------------------------------
// class AssigContainerT

template <class ContainerT>
class AssigContainerT
{
private:
	typedef typename ContainerT::value_type value_type;
	typedef AssignmentTypeTraits<value_type> Tr_;
	typedef typename Tr_::assignment_type assignment_type;

	ContainerT& m_result;

public:
	AssigContainerT(ContainerT& result) : m_result(result) {
	}

	template <class Iterator>
	void TPL_CALL operator()(Iterator pos, Iterator pos2) const {
		value_type val = value_type();
		assignment_type assig(val);
		assig(pos, pos2);
		m_result.push_back(val);
	}
};

// -------------------------------------------------------------------------

#if defined(TPL_HAS_VECTOR)

template <class Type, class Ax>
class AssigVec : public AssigContainerT<std::vector<Type, Ax> >
{
public:
	AssigVec(std::vector<Type, Ax>& result)
		: AssigContainerT<std::vector<Type, Ax> >(result) {}
};

template <class Type, class Ax> __forceinline
Action<AssigVec<Type, Ax> > TPL_CALL assign(std::vector<Type, Ax>& result) {
	return Action<AssigVec<Type, Ax> >(result);
}

template <class T1, class Type, class Ax>
__forceinline
Rule<Act<T1, AssigVec<Type, Ax> > >
TPL_CALL operator/(const Rule<T1>& x, std::vector<Type, Ax>* result) {
	return x / assign(*result);
}

#endif // defined(TPL_HAS_VECTOR)

// -------------------------------------------------------------------------

#if defined(TPL_HAS_DEQUE)

template <class Type, class Ax>
class AssigDeq : public AssigContainerT<std::deque<Type, Ax> >
{
public:
	AssigDeq(std::deque<Type, Ax>& result)
		: AssigContainerT<std::deque<Type, Ax> >(result) {}
};

template <class Type, class Ax> __forceinline
Action<AssigDeq<Type, Ax> > TPL_CALL assign(std::deque<Type, Ax>& result) {
	return Action<AssigDeq<Type, Ax> >(result);
}

template <class T1, class Type, class Ax>
__forceinline
Rule<Act<T1, AssigDeq<Type, Ax> > >
TPL_CALL operator/(const Rule<T1>& x, std::deque<Type, Ax>* result) {
	return x / assign(*result);
}

#endif

// -------------------------------------------------------------------------

#if defined(TPL_HAS_LIST)

template <class Type, class Ax>
class AssigLst : public AssigContainerT<std::list<Type, Ax> >
{
public:
	AssigLst(std::list<Type, Ax>& result)
		: AssigContainerT<std::list<Type, Ax> >(result) {}
};

template <class Type, class Ax> __forceinline
Action<AssigLst<Type, Ax> > TPL_CALL assign(std::list<Type, Ax>& result) {
	return Action<AssigLst<Type, Ax> >(result);
}

template <class T1, class Type, class Ax>
__forceinline
Rule<Act<T1, AssigLst<Type, Ax> > >
TPL_CALL operator/(const Rule<T1>& x, std::list<Type, Ax>* result) {
	return x / assign(*result);
}

#endif

// -------------------------------------------------------------------------
// class AssigSetT

template <class ContainerT>
class AssigSetT
{
private:
	typedef typename ContainerT::value_type value_type;
	typedef AssignmentTypeTraits<value_type> Tr_;
	typedef typename Tr_::assignment_type assignment_type;

	ContainerT& m_result;

public:
	AssigSetT(ContainerT& result) : m_result(result) {
	}

	template <class Iterator>
	void TPL_CALL operator()(Iterator pos, Iterator pos2) const {
		value_type val = value_type();
		assignment_type assig(val);
		assig(pos, pos2);
		m_result.insert(val);
	}
};

// -------------------------------------------------------------------------

#if defined(TPL_HAS_SET)

template <class Type, class Pr, class Ax>
class AssigSet : public AssigSetT<std::set<Type, Pr, Ax> >
{
public:
	AssigSet(std::set<Type, Pr, Ax>& result)
		: AssigSetT<std::set<Type, Pr, Ax> >(result) {}
};

template <class Type, class Pr, class Ax> __forceinline
Action<AssigSet<Type, Pr, Ax> > TPL_CALL assign(std::set<Type, Pr, Ax>& result) {
	return Action<AssigSet<Type, Pr, Ax> >(result);
}

template <class T1, class Type, class Pr, class Ax>
__forceinline
Rule<Act<T1, AssigSet<Type, Pr, Ax> > >
TPL_CALL operator/(const Rule<T1>& x, std::set<Type, Pr, Ax>* result) {
	return x / assign(*result);
}

template <class Type, class Pr, class Ax>
class AssigMSet : public AssigSetT<std::multiset<Type, Pr, Ax> >
{
public:
	AssigMSet(std::multiset<Type, Pr, Ax>& result)
		: AssigSetT<std::multiset<Type, Pr, Ax> >(result) {}
};

template <class Type, class Pr, class Ax> __forceinline
Action<AssigMSet<Type, Pr, Ax> > TPL_CALL assign(std::multiset<Type, Pr, Ax>& result) {
	return Action<AssigMSet<Type, Pr, Ax> >(result);
}

template <class T1, class Type, class Pr, class Ax>
__forceinline
Rule<Act<T1, AssigMSet<Type, Pr, Ax> > >
TPL_CALL operator/(const Rule<T1>& x, std::multiset<Type, Pr, Ax>* result) {
	return x / assign(*result);
}

#endif

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_ASSIGNMENTS_H */
