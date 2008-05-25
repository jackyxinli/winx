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

template <class Policy = policy::Default, bool bHasDocument = true>
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

private:
	template <bool bHas>
	struct ContextTraits_ {
		typedef BasicContext<Iterator, Allocator, Tag> Context;
	};

	template <>
	struct ContextTraits_<false> {
		typedef tpl::FakeContext<Iterator> Context;
	};

public:
	// Context

	typedef typename ContextTraits_<bHasDocument>::Context Context;

public:
	// Leaf, Node, Document

	typedef tpl::Leaf<Iterator> Leaf;
	typedef tpl::Node<Iterator, Tag> Node;
	typedef Node Document;

public:
	// Mark, LeafMark, NodeMark

	typedef tpl::Mark<Tag> Mark;
	typedef BasicMark<Tag, NodeAssign> NodeMark;
	typedef BasicMark<Tag, LeafAssign> LeafMark;

private:
	template <bool bHas>
	struct CategoryTraits_ {
		enum { categoryTerminal = CATEGORY_TERMINAL };
		enum { categoryDefault	= CATEGORY_DEFAULT };
		enum { categoryMarked	= CATEGORY_MARKED };
	};

	template <>
	struct CategoryTraits_<false> {
		enum { categoryTerminal = 0 };
		enum { categoryDefault	= 0 };
		enum { categoryMarked	= 0 };
	};

	typedef CategoryTraits_<bHasDocument> CateTraits_;

public:
	// RegExp

	template <int nCategory>
	class RegExpT : public Rule<BasicRegExp<nCategory, Source, Context, false> >
	{
	private:
		typedef BasicRegExp<nCategory, Source, Context, false> Impl;
		typedef Rule<Impl> Base;

	public:
		RegExpT() {}

		template <class AllocT, class RegExT>
		RegExpT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}
	
		template <class SourceT, class ContextT>
		__forceinline bool TPL_CALL match(SourceT& ar, ContextT& context) const {
			return Impl::match(ar, context);
		}
	};

	typedef RegExpT<0> RegExp0;
	typedef RegExpT<CateTraits_::categoryDefault> RegExp;
	typedef RegExpT<CateTraits_::categoryMarked> MarkedRegExp;

public:
	// ManagedRegExp

	template <int nCategory>
	class ManagedRegExpT : public Rule<BasicRegExp<nCategory, Source, Context, true> >
	{
	private:
		typedef BasicRegExp<nCategory, Source, Context, true> Impl;
		typedef Rule<Impl> Base;

	public:
		ManagedRegExpT() {}

		template <class AllocT, class RegExT>
		ManagedRegExpT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}

		template <class SourceT, class ContextT>
		__forceinline bool TPL_CALL match(SourceT& ar, ContextT& context) const {
			return Impl::match(ar, context);
		}
	};

	typedef ManagedRegExpT<0> ManagedRegExp0;
	typedef ManagedRegExpT<CateTraits_::categoryDefault> ManagedRegExp;
	typedef ManagedRegExpT<CateTraits_::categoryMarked> ManagedMarkedRegExp;

public:
	// helper functions:

	template <class RegExT>
	static inline bool TPL_CALL match(Iterator pos, Iterator pos2, const Rule<RegExT>& rule)
	{
		Source source(pos, pos2);
		Context context;
		return static_cast<const RegExT&>(rule).match(source, context);
	}

	template <class RegExT>
	static inline bool TPL_CALL match(Iterator pos, const Rule<RegExT>& rule)
	{
		Source source(pos);
		Context context;
		return static_cast<const RegExT&>(rule).match(source, context);
	}
};

// -------------------------------------------------------------------------
// SimpleImplementation

typedef Customization<policy::Default, false> SimpleImplementation;

typedef SimpleImplementation simple;

// -------------------------------------------------------------------------
// DefaultImplementation

typedef Customization<> DefaultImplementation;

typedef DefaultImplementation impl;

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CUSTOMIZATION_H */
