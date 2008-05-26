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

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class DefaultTag

typedef size_t DefaultTag;

// -------------------------------------------------------------------------
// TPL_DBG_TAG

#ifndef TPL_DBG_STR
#define TPL_DBG_STR(tag)	((impl::Tag)(tag))
#endif

#ifndef TPL_DBG_TAG
#define TPL_DBG_TAG(mark)	(const char*)((size_t)(mark).tag & ~TPL_REGEX_NODE_MARK)
#endif

// -------------------------------------------------------------------------
// class Mark

#ifndef TPL_REGEX_NODE_MARK	
#define TPL_REGEX_NODE_MARK	(1 << (sizeof(void*)*8 - 1))
#endif

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

	__forceinline bool TPL_CALL operator==(const Mark& b) const {
		return this == &b;
	}

	__forceinline bool TPL_CALL operator!=(const Mark& b) const {
		return this != &b;
	}

	__forceinline TagT TPL_CALL isNode() const {
		return tag & TPL_REGEX_NODE_MARK;
	}

	__forceinline TagT TPL_CALL isLeaf() const {
		return !(tag & TPL_REGEX_NODE_MARK);
	}
};

// -------------------------------------------------------------------------
// class BasicMark

struct FakeRegEx_ {
	enum { category = 0 };
};

template <class TagT, template <class RegExT, class MarkT> class Assign>
struct TagTraits_ {
	__forceinline static TagT TPL_CALL calcTag(TagT tag_) {
		return tag_;
	}
};

template <template <class RegExT, class MarkT> class Assign>
struct TagTraits_<size_t, Assign> {
	__forceinline static size_t TPL_CALL calcTag(size_t tag_) {
		return (tag_ | Assign<FakeRegEx_, int>::tag);
	}
};

template <class TagT, template <class RegExT, class MarkT> class Assign>
class BasicMark : public Mark<TagT>
{
private:
	typedef TagTraits_<TagT, Assign> Tr_;
	typedef Mark<TagT> Base;

	BasicMark(const BasicMark&);
	void operator=(const BasicMark&);

public:
	BasicMark(const TagT tag_ = TagT())
		: Base(Tr_::calcTag(tag_)) {
	}

	template <class T1>
	Rule<Assign<T1, BasicMark> > TPL_CALL operator=(const Rule<T1>& x) const {
		return Rule<Assign<T1, BasicMark> >(*this, x);
	}

	template <class T1>
	friend Rule<Assign<T1, BasicMark> > TPL_CALL operator/(const Rule<T1>& x, const BasicMark& y) {
		return Rule<Assign<T1, BasicMark> >(y, x);
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
	enum { tag = 0 };
	enum { category = RegExT::category | CATEGORY_MARKED };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename SourceT::iterator pos = ar.position();
		bool matched = m_x.match(ar, context);
		if (matched)
			context.insertLeaf(m_mark, pos, ar.position());
		return matched;
	}
};

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
	enum { tag = TPL_REGEX_NODE_MARK };
	enum { category = RegExT::category | CATEGORY_MARKED };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename ContextT::scope_trans_type trans(context);
		typename ContextT::scope_type scope(context, m_mark);
		if (m_x.match(ar, context))
			return true;
		trans.rollback();
		return false;
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_MARK_H */
