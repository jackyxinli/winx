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
// Module: tpl/regex/Mark.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_MARK_H
#define TPL_REGEX_MARK_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

// -------------------------------------------------------------------------
// class DefaultTag

typedef const char* DefaultTag;

// -------------------------------------------------------------------------
// class Mark

template <class TagT>
class Mark
{
public:
	TagT const tag;

private:
	Mark(const Mark&);
	void operator=(const Mark&);

public:
	Mark(const TagT tag_ = TagT())
		: tag(tag_) {}
};

// -------------------------------------------------------------------------
// class BasicMark

template <class TagT, template <class RegExT, class MarkT> class Assign>
class BasicMark : public Mark<TagT>
{
private:
	typedef Mark<TagT> Base;

	BasicMark(const BasicMark&);
	void operator=(const BasicMark&);

public:
	BasicMark(const TagT tag_ = TagT())
		: Base(tag_) {}

	template <class T1>
	Exp<Assign<T1, BasicMark> > TPL_CALL operator=(const Exp<T1>& x) {
		return Exp<Assign<T1, BasicMark> >(*this, x);
	}
};

// -------------------------------------------------------------------------
// class LeafAssign

template <class RegExT, class MarkT>
class LeafAssign
{
private:
	const MarkT& m_mark;
	RegExT m_x;

public:
	LeafAssign(const MarkT& val, const RegExT& x)
		: m_mark(val), m_x(x) {}

public:
	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		typename SourceT::iterator pos = ar.position();
		bool matched = m_x.match(ar, context);
		if (matched)
			context.insertLeaf(m_mark, ar, pos);
		return matched;
	}
};

// -------------------------------------------------------------------------
// class LeafMark

typedef BasicMark<DefaultTag, LeafAssign> LeafMark;

// -------------------------------------------------------------------------
// class NodeAssign

template <class RegExT, class MarkT>
class NodeAssign
{
private:
	const MarkT& m_mark;
	RegExT m_x;

public:
	NodeAssign(const MarkT& val, const RegExT& x)
		: m_mark(val), m_x(x) {}

public:
	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		typename ContextT::trans_type trans(context);
		typename ContextT::scope_type scope(context, m_mark);
		if (m_x.match(ar, context))
			return true;
		trans.rollback();
		return false;
	}
};

// -------------------------------------------------------------------------
// class NodeMark

typedef BasicMark<DefaultTag, NodeAssign> NodeMark; 

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_MARK_H */
