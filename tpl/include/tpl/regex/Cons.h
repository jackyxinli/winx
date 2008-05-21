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

template <class Type>
class ConsList
{
private:
	typedef Cons<Type> Node;

	const Node* m_hd;

public:
	ConsList() : m_hd(NULL) {}

public:
	typedef const Type& reference;
	typedef const Node* cons;

	cons data() const {
		return m_hd;
	}

public:
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
		m_hd = TPL_NEW(alloc, Node)(v, m_hd);
	}

	void TPL_CALL pop_front() {
		m_hd = m_hd->tail;
	}
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_CONS_H */
