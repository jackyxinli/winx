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
// Module: tpl/regex/Context.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_CONTEXT_H
#define TPL_REGEX_CONTEXT_H

#ifndef TPL_REGEX_CONS_H
#include "Cons.h"
#endif

#if !defined(_STRING_) && !defined(_STRING)
#include <string>
#endif

// -------------------------------------------------------------------------
// class Mark

class Mark
{
public:
	typedef unsigned size_type;

private:
	const Mark* m_parent;
	size_type m_id;
	size_type m_childs;

	Mark(const Mark&);
	void operator=(const Mark&);

public:
	Mark() : m_parent(NULL), m_childs(0) {};
	Mark(Mark* p) : m_parent(NULL), m_childs(0) {
		parent(p);
	}

	bool TPL_CALL good() const {
		return m_parent != NULL;
	}

	size_type TPL_CALL id() const {
		return m_id;
	}

	size_type TPL_CALL size() const {
		return m_childs;
	}

	const Mark* TPL_CALL parent() const {
		return m_parent;
	}

	void TPL_CALL parent(Mark* p)
	{ 
		TPL_ASSERT(m_parent == NULL);
		m_id = p->m_childs++;
		m_parent = p;
	}

	template <class RegExT>
	class Assign
	{
	private:
		const Mark& m_mark;
		RegExT m_x;

	public:
		Assign(const Mark& val, const RegExT& x) : m_mark(val), m_x(x) {}

	public:
		template <class SourceT, class ContextT>
		bool TPL_CALL match(SourceT& ar, ContextT& context)
		{
			typename ContextT::scope_type scope = context.enterScope(m_mark);
			typename SourceT::iterator pos = ar.position();
			bool matched = m_x.match(ar, context);
			if (matched)
				context.add_matched(m_mark, ar, pos);
			context.leaveScope(scope);
			return matched;
		}
	};

	template <class T1>
	Exp<Assign<Exp<T1> > > TPL_CALL operator=(const Exp<T1>& x) {
		return Exp<Assign<Exp<T1> > >(*this, x);
	}
};

// -------------------------------------------------------------------------
// class BasicSubMatch

template <class Iterator>
class BasicSubMatch
{
public:
	Iterator const first;
	Iterator const second;

public:
	typedef ptrdiff_t difference_type;

	BasicSubMatch(Iterator first_, Iterator second_)
		: first(first_), second(second_) {}

	difference_type TPL_CALL length() const {
		return std::distance(first, second);
	}

	std::string TPL_CALL stl_str() const {
		return std::string(first, second);
	}
};

// -------------------------------------------------------------------------
// class BasicMatchResult

template <class Iterator>
class BasicMatchResult
{
public:
	typedef Mark::size_type size_type;
	
	typedef BasicSubMatch<Iterator> value_type;
	typedef typename ConsList<value_type>::cons cons;
	typedef const value_type& reference;
	
	typedef BasicMatchResult child_type;
	typedef const BasicMatchResult& child_reference;
	typedef const BasicMatchResult* child_pointer;

private:
	ConsList<value_type> m_vals;
	const Mark* m_self;
	child_pointer* m_childs;
	const size_type m_size;

	BasicMatchResult(const BasicMatchResult&);
	void operator=(const BasicMatchResult&);

public:
	template <class AllocT>
	child_type* TPL_CALL _getChild(AllocT& alloc, const Mark& sub) // private!
	{
		TPL_ASSERT(m_self != NULL);
		TPL_ASSERT(sub.parent() == m_self);
		TPL_ASSERT(sub.id() < m_size);

		const child_type*& child = m_childs[sub.id()];
		if (child == NULL)
			child = TPL_UNMANAGED_NEW(alloc, BasicMatchResult)(alloc, sub);

		TPL_ASSERT(child->m_self == &sub);
		return (child_type*)child;
	}

	ConsList<value_type>& _getValueContainer() { // private
		return m_vals;
	}

public:
	typedef size_type size_type;

	template <class AllocT>
	BasicMatchResult(AllocT& alloc, const Mark& self_)
		: m_self(&self_), m_size(self_.size())
	{
		m_childs = TPL_ALLOC_ARRAY(alloc, const BasicMatchResult*, m_size);
		for (size_type i = 0; i < m_size; ++i)
			m_childs[i] = NULL;
	}

	const Mark* TPL_CALL self() const {
		return m_self;
	}

public:
	bool TPL_CALL hasValue() const {
		return m_vals.hasValue();
	}

	reference TPL_CALL operator*() const {
		TPL_ASSERT(hasValue());
		return m_vals.front();
	}

	cons TPL_CALL data() const {
		return m_vals.data();
	}

public:
	size_type TPL_CALL getChildCount() const {
		return m_size;
	}

	bool TPL_CALL isChild(const Mark& sub) const {
		return m_self == sub.parent() && sub.id() < m_size;
	}

	child_pointer TPL_CALL getChild(const Mark& sub) const
	{
		TPL_ASSERT(m_self != NULL);
		TPL_ASSERT(sub.parent() == m_self);
		TPL_ASSERT(sub.id() < m_size);
		
		child_pointer child = m_childs[sub.id()];
		TPL_ASSERT(child->m_self == &sub);

		return child;
	}

	child_reference TPL_CALL operator[](const Mark& sub) const
	{
		return *getChild(sub);
	}
};

// -------------------------------------------------------------------------
// class BasicContext

template <class Iterator, class AllocT>
class BasicContext
{
private:
	AllocT& m_alloc;
	BasicMatchResult<Iterator>& m_dom;
	BasicMatchResult<Iterator>* m_cur;

public:
	typedef BasicMatchResult<Iterator> document_type;
	typedef const document_type& document_reference;

public:
	BasicContext(AllocT& alloc, document_type& doc)
		: m_alloc(alloc), m_dom(doc), m_cur(&m_dom) {}

	document_reference TPL_CALL document() const {
		return m_dom;
	}

	template <class SourceT>
	void TPL_CALL add_matched(const Mark& mark, SourceT& ar, Iterator pos)
	{
		typedef typename SourceT::size_type SizeT;
		typedef typename SourceT::char_type CharT;

		BasicMatchResult<Iterator>* child = m_cur->_getChild(m_alloc, mark);
		BasicSubMatch<Iterator> submatch(pos, ar.position());
		child->_getValueContainer().push_front(m_alloc, submatch);
	}

public:
	typedef int trans_type;

	trans_type TPL_CALL beginTrans() {
		return trans_type();
	}

	bool TPL_CALL commitTrans(const trans_type& trans) {
		return true;
	}

	bool TPL_CALL rollbackTrans(const trans_type& trans) {
		return false;
	}

public:
	typedef int scope_type;

	scope_type TPL_CALL enterScope(const Mark& mark) {
		return scope_type();
	}

	void TPL_CALL leaveScope(const scope_type& scope) {
	}
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_CONTEXT_H */
