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

// -------------------------------------------------------------------------
// class BasicContext

template <class Iterator, class AllocT, class TagT = DefaultTag>
class BasicContext
{
private:
	typedef NodeMatchResult<Iterator, TagT> NodeMatchResultT;

	AllocT& m_alloc;
	NodeMatchResultT* m_cur;

public:
	BasicContext(AllocT& alloc, NodeMatchResultT& doc)
		: m_alloc(alloc), m_cur(&doc) {}

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

private:
	typedef BasicMark<TagT, LeafAssign> LeafMarkT;
	typedef BasicMark<TagT, NodeAssign> NodeMarkT;

public:
	class Scope
	{
	public:
		Scope(BasicContext& context, NodeMarkT& mark) {}

		void TPL_CALL setMatched(bool matched) {
		}
	};

	typedef Scope scope_type;

	template <class SourceT>
	void TPL_CALL insertLeaf(const LeafMarkT& mark, SourceT& ar, Iterator pos)
	{
		m_cur->insertLeaf(m_alloc, mark, pos, ar.position());
	}
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_CONTEXT_H */
