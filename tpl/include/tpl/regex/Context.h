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

#ifndef TPL_REGEX_MARK_H
#include "Mark.h"
#endif

#ifndef TPL_REGEX_MATCHRESULT_H
#include "MatchResult.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class Context0

template <class Iterator>
class Context0
{
public:
	template <int category>
	class trans_type
	{
	private:
		Iterator vPos;

	public:
		template <class SourceT>
		trans_type(const SourceT& ar, const Context0&) {
			vPos = ar.position();
		}

		template <class SourceT>
		void TPL_CALL rollback(SourceT& ar) {
			ar.seek(vPos);
		}
	};
};

// -------------------------------------------------------------------------
// class ContextTransaction

template <int uCharacter, class Iterator, class Scope>
class ContextTransaction {
};

template <class Iterator, class Scope>
class ContextTransaction<0, Iterator, Scope>
{
private:
	Iterator vPos;

public:
	template <class SourceT, class ContextT>
	ContextTransaction(const SourceT& ar, const ContextT&) {
		vPos = ar.position();
	}

	template <class SourceT>
	void TPL_CALL rollback(SourceT& ar) {
		ar.seek(vPos);
	}
};

template <class Iterator, class Scope>
class ContextTransaction<CHARACTER_MARKED, Iterator, Scope>
{
private:
	Scope vScope;
	Iterator vPos;

public:
	template <class SourceT, class ContextT>
	ContextTransaction(const SourceT& ar, const ContextT& context) : vScope(context) {
		vPos = ar.position();
	}

	template <class SourceT>
	void TPL_CALL rollback(SourceT& ar) {
		ar.seek(vPos);
		vScope.rollback();
	}
};

// -------------------------------------------------------------------------
// class BasicContext

template <class Iterator, class AllocT, class TagT = DefaultTag>
class BasicContext
{
private:
	typedef Node<Iterator, TagT> NodeT;
	typedef ConsList<NodeT*, false> StackT;

	AllocT& m_alloc;
	StackT m_stk;

public:
	BasicContext(AllocT& alloc, NodeT& doc)
		: m_alloc(alloc)
	{
		m_stk.push_front(alloc, &doc);
	}

private:
	typedef BasicMark<TagT, LeafAssign> LeafMarkT;
	typedef BasicMark<TagT, NodeAssign> NodeMarkT;

public:
	typedef AllocT allocator_type;

	allocator_type& TPL_CALL getAllocator() const {
		return m_alloc;
	}

	void TPL_CALL insertLeaf(const LeafMarkT& mark, Iterator pos, Iterator pos2) {
		m_stk.front()->insertLeaf(m_alloc, mark, pos, pos2);
	}

public:
	class ScopeTransaction
	{
	private:
		NodeT* vParent;
		NodeT vOld;

	public:
		ScopeTransaction(const BasicContext& context) {
			StackT stk = context.m_stk;
			vParent = stk.front();
			vOld = *vParent;
		}

		void TPL_CALL rollback() {
			*vParent = vOld;
		}
	};

	typedef ScopeTransaction scope_trans_type;

public:
	template <int uCharacter>
	class trans_type : public ContextTransaction<uCharacter, Iterator, ScopeTransaction>
	{
	private:
		typedef ContextTransaction<uCharacter, Iterator, ScopeTransaction> Base;
		
	public:
		template <class SourceT>
		trans_type(const SourceT& ar, const BasicContext& context)
			: Base(ar, context) {
		}
	};

public:
	class Scope
	{
	private:
		StackT& m_stk;
	
	public:
		Scope(BasicContext& context, const NodeMarkT& mark)
			: m_stk(context.m_stk)
		{
			NodeT* v = m_stk.front()->insertNode(context.m_alloc, mark);
			m_stk.push_front(context.m_alloc, v);
		}
		~Scope() {
			m_stk.pop_front();
		}
	};
	friend class Scope;

	typedef Scope scope_type;
};

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CONTEXT_H */
