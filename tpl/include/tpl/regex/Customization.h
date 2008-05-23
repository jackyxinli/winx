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
// Module: tpl/regex/Customization.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_CUSTOMIZATION_H
#define TPL_REGEX_CUSTOMIZATION_H

#ifndef STDEXT_MEMORY_H
#include "../../../stdext/include/stdext/Memory.h"
#endif

#ifndef STDEXT_ARCHIVE_MEM_H
#include "../../../stdext/include/stdext/archive/Mem.h"
#endif

#ifndef TPL_REGEX_BASICREGEXP_H
#include "BasicRegExp.h"
#endif

#ifndef TPL_REGEX_MATCHRESULT_H
#include "MatchResult.h"
#endif

#ifndef TPL_REGEX_CONTEXT_H
#include "Context.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class policy::Default

namespace policy
{
	class Default
	{
	public:
		typedef DefaultTag Tag;
		typedef std::PointerReadArchive Source;
		typedef std::ScopeAlloc Allocator;
	};
}

// -------------------------------------------------------------------------
// class Context

template <class Policy = policy::Default>
class Customization
{
public:
	// Tag, Source, Allocator

	typedef typename Policy::Tag Tag;
	typedef typename Policy::Source Source;
	typedef typename Policy::Allocator Allocator;

public:
	// Iterator

	typedef typename Source::iterator Iterator;

public:
	// Context

	typedef BasicContext<Iterator, Allocator, Tag> Context;

public:
	// Leaf, Node, Document

	typedef LeafMatchResult<Iterator> Leaf;
	typedef NodeMatchResult<Iterator, Tag> Node;
	typedef Node Document;

public:
	// Mark, LeafMark, NodeMark

	typedef tpl::Mark<Tag> Mark;
	typedef BasicMark<Tag, NodeAssign> NodeMark;
	typedef BasicMark<Tag, LeafAssign> LeafMark;

public:
	// RegExp

	template <int nCategory>
	class RegExpT : public Rule<BasicRegExp<nCategory, Source, Context, false> >
	{
	private:
		typedef Rule<BasicRegExp<nCategory, Source, Context, false> > Base;

	public:
		RegExpT() {}

		template <class AllocT, class RegExT>
		RegExpT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}
	};

	typedef RegExpT<0> RegExp0;
	typedef RegExpT<CATEGORY_DEFAULT> RegExp;
	typedef RegExpT<CATEGORY_MARKED> MarkedRegExp;

public:
	// ManagedRegExp

	template <int nCategory = CATEGORY_DEFAULT>
	class ManagedRegExpT : public Rule<BasicRegExp<nCategory, Source, Context, true> >
	{
	private:
		typedef Rule<BasicRegExp<nCategory, Source, Context, true> > Base;

	public:
		ManagedRegExpT() {}

		template <class AllocT, class RegExT>
		ManagedRegExpT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}
	};

	typedef ManagedRegExpT<0> ManagedRegExp0;
	typedef ManagedRegExpT<CATEGORY_DEFAULT> ManagedRegExp;
	typedef ManagedRegExpT<CATEGORY_MARKED> ManagedMarkedRegExp;
};

// -------------------------------------------------------------------------

namespace impl
{
	typedef Customization<> DefaultImplementation;

	// Source, Allocator

	typedef DefaultImplementation::Source Source;
	typedef DefaultImplementation::Allocator Allocator;

	// Context

	typedef DefaultImplementation::Context Context;

	// Leaf, Node, Document

	typedef DefaultImplementation::Leaf Leaf;
	typedef DefaultImplementation::Node Node;
	typedef DefaultImplementation::Document Document;

	// LeafMark, NodeMark

	typedef DefaultImplementation::NodeMark NodeMark;
	typedef DefaultImplementation::LeafMark LeafMark;

	// RegExp

	typedef DefaultImplementation::RegExp RegExp;
	typedef DefaultImplementation::RegExp0 RegExp0;
	typedef DefaultImplementation::MarkedRegExp MarkedRegExp;

	// ManagedRegExp

	typedef DefaultImplementation::ManagedRegExp ManagedRegExp;
	typedef DefaultImplementation::ManagedRegExp0 ManagedRegExp0;
	typedef DefaultImplementation::ManagedMarkedRegExp ManagedMarkedRegExp;
};

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CUSTOMIZATION_H */
