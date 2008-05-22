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

#ifndef TPL_REGEX_CONS_H
#include "Cons.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class BasicContext

template <class Iterator, class AllocT, class TagT = DefaultTag>
class BasicContext
{
private:
	typedef NodeMatchResult<Iterator, TagT> NodeMatchResultT;
	typedef ConsList<NodeMatchResultT*, false> StackT;

	AllocT& m_alloc;
	StackT m_stk;

public:
	BasicContext(AllocT& alloc, NodeMatchResultT& doc)
		: m_alloc(alloc)
	{
		m_stk.push_front(alloc, &doc);
	}

private:
	typedef BasicMark<TagT, LeafAssign> LeafMarkT;
	typedef BasicMark<TagT, NodeAssign> NodeMarkT;

public:
	class Transaction
	{
	private:
		NodeMatchResultT* vParent;
		NodeMatchResultT vOld;

	public:
		Transaction(BasicContext& context) {
			StackT stk = context.m_stk;
			vParent = stk.front();
			vOld = *vParent;
		}

		void TPL_CALL rollback() {
			*vParent = vOld;
		}
	};

	typedef Transaction trans_type;

public:
	class Scope
	{
	private:
		StackT& m_stk;
	public:
		Scope(BasicContext& context, const NodeMark& mark)
			: m_stk(context.m_stk)
		{
			NodeMatchResultT* v = m_stk.front()->insertNode(context.m_alloc, mark);
			m_stk.push_front(context.m_alloc, v);
		}
		~Scope() {
			m_stk.pop_front();
		}
	};
	friend class Scope;

	typedef Scope scope_type;

public:
	template <class SourceT>
	void TPL_CALL insertLeaf(const LeafMarkT& mark, SourceT& ar, Iterator pos)
	{
		m_stk.front()->insertLeaf(m_alloc, mark, pos, ar.position());
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CONTEXT_H */
