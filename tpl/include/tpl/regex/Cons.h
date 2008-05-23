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
// Module: tpl/regex/Cons.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_CONS_H
#define TPL_REGEX_CONS_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class Cons

template <class Type>
class Cons
{
public:
	typedef const Type& reference;
	typedef const Cons<Type>* cons;

	Type const value;
	cons const tail;

public:
	Cons(Type const& v, cons const t)
		: value(v), tail(t) {}

	reference hd() const {
		return value;
	}

	cons TPL_CALL tl() const {
		return tail;
	}
};

// -------------------------------------------------------------------------
// class ConsList

template <class Type, bool bManaged = true>
class ConsList
{
public:
	typedef const Type& reference;
	typedef const Cons<Type>* cons;

private:
	cons m_hd;

private:
	typedef Cons<Type> Node;

	template <class AllocT>
	static Node* TPL_CALL _newNode(AllocT& alloc, reference val, cons tail)
	{
		if (bManaged)
			return TPL_NEW(alloc, Node)(val, tail);
		else
			return TPL_UNMANAGED_NEW(alloc, Node)(val, tail);
	}

	template <class AllocT, class CondT>
	const Node*& TPL_CALL _selectRetTail(AllocT& alloc, CondT cond, cons* ptail) const
	{
		for (cons n = m_hd; n; n = n->tail)
		{
			reference val = n->value;
			if (cond(val))
			{
				Node* node = _newNode(alloc, val, NULL);
				*ptail = node;
				ptail = (cons*)&node->tail;
			}
		}
		return *ptail;
	}

public:
	template <class AllocT, class CondT>
	cons TPL_CALL select(AllocT& alloc, CondT cond) const
	{
		cons lstRet = NULL;
		_selectRetTail(alloc, cond, &lstRet);
		return lstRet;
	}

	template <class CondT>
	cons TPL_CALL find(CondT cond) const
	{
		for (cons n = m_hd; n; n = n->tail)
		{
			reference val = n->value;
			if (cond(val))
				return n;
		}
		return NULL;
	}

	template <class AllocT>
	void TPL_CALL reverse(AllocT& alloc)
	{
		cons lst = NULL;
		for (cons n = m_hd; n; n = n->tail)
		{
			lst = _newNode(alloc, n->value, lst);
		}
		m_hd = lst;
	}

public:
	ConsList() : m_hd(NULL) {}
	ConsList(cons hd_) : m_hd(hd_) {}

	operator cons() const {
		return m_hd;
	}

	cons TPL_CALL operator=(cons hd_) {
		return m_hd = hd_;
	}

	cons TPL_CALL data() const {
		return m_hd;
	}

	bool TPL_CALL empty() const {
		return m_hd == NULL;
	}

	bool TPL_CALL hasValue() const {
		return m_hd != NULL;
	}

	reference TPL_CALL front() const {
		return m_hd->value;
	}

	template <class AllocT>
	void TPL_CALL push_front(AllocT& alloc, reference v) {
		m_hd = _newNode(alloc, v, m_hd);
	}

	void TPL_CALL pop_front() {
		m_hd = m_hd->tail;
	}
};

// -------------------------------------------------------------------------
// function length

template <class Type>
inline size_t TPL_CALL length(const Cons<Type>* hd) {
	size_t len = 0;
	for (; hd; hd = hd->tail)
		++len;
	return len;
}

template <class Type, bool bManaged>
__forceinline size_t TPL_CALL length(const ConsList<Type, bManaged>& hd) {
	return length(hd.data());
}

// -------------------------------------------------------------------------
// function count_if

template <class Type, class CondT>
inline size_t TPL_CALL count_if(const Cons<Type>* hd, CondT cond) {
	size_t len = 0;
	for (; hd; hd = hd->tail) {
		if (cond(hd->value))	
			++len;
	}
	return len;
}

template <class Type, bool bManaged, class CondT>
__forceinline size_t TPL_CALL count_if(const ConsList<Type, bManaged>& hd, CondT cond) {
	return count_if(hd.data(), cond);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CONS_H */
