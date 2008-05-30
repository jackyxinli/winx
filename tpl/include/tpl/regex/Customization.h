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
#include "../../../../stdext/include/stdext/Memory.h"
#endif

#ifndef STDEXT_ARCHIVE_MEM_H
#include "../../../../stdext/include/stdext/archive/Mem.h"
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
// class Customization

template <bool bHas, class Iterator, class Allocator, class Tag>
struct CustomizationContextTraits_ {
	typedef BasicContext<Iterator, Allocator, Tag> Context;
};

template <class Iterator, class Allocator, class Tag>
struct CustomizationContextTraits_<false, Iterator, Allocator, Tag> {
	typedef Context0<Iterator> Context;
};

template <bool bHas>
struct CustomizationCharacterTraits_ {
	enum { characterMarked	= CHARACTER_MARKED };
};

template <>
struct CustomizationCharacterTraits_<false> {
	enum { characterMarked	= 0 };
};

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
	struct ContextTraits_ : public CustomizationContextTraits_<bHas, Iterator, Allocator, Tag> {
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
	typedef CustomizationCharacterTraits_<bHasDocument> CharacterTraits_;

public:
	// Rule

	template <int uCharacter>
	class RuleT : public tpl::Rule<Concretion<uCharacter, Source, Context, false> >
	{
	private:
		typedef Concretion<uCharacter, Source, Context, false> Impl;
		typedef tpl::Rule<Impl> Base;

	public:
		RuleT() {}

		template <class AllocT, class RegExT>
		RuleT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}
	
		template <class SourceT, class ContextT> __forceinline
		bool TPL_CALL match(SourceT& ar, ContextT& context) const {
			return Impl::match(ar, context);
		}
	};

	typedef RuleT<0> Rule;
	typedef RuleT<CharacterTraits_::characterMarked> MarkedRule;

public:
	// ManagedRule

	template <int uCharacter>
	class ManagedRuleT : public tpl::Rule<Concretion<uCharacter, Source, Context, true> >
	{
	private:
		typedef Concretion<uCharacter, Source, Context, true> Impl;
		typedef tpl::Rule<Impl> Base;

	public:
		ManagedRuleT() {}

		template <class AllocT, class RegExT>
		ManagedRuleT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}

		template <class SourceT, class ContextT> __forceinline
		bool TPL_CALL match(SourceT& ar, ContextT& context) const {
			return Impl::match(ar, context);
		}
	};

	typedef ManagedRuleT<0> ManagedRule;
	typedef ManagedRuleT<CharacterTraits_::characterMarked> ManagedMarkedRule;

public:
	// Rule helper functions:

	template <class RegExT>
	static inline bool TPL_CALL match(Iterator pos, Iterator pos2, const tpl::Rule<RegExT>& rule_)
	{
		Source source(pos, pos2);
		Context context;
		return rule_.match(source, context);
	}

	template <class RegExT>
	static inline bool TPL_CALL match(Iterator pos, const tpl::Rule<RegExT>& rule_)
	{
		Source source(pos);
		Context context;
		return rule_.match(source, context);
	}

private:
	typedef SkipperTraits<Source, Context> SkipperTr_;
	typedef typename SkipperTr_::concretion_type ConcretionSkipper_;

public:
	// Skipper

	template <class SkipperT>
	static Skipper<SkipperImpl<SkipperT, Source, Context> > TPL_CALL skipper(const tpl::Rule<SkipperT>& skipper_) {
		return Skipper<SkipperImpl<SkipperT, Source, Context> >(skipper_);
	}

	template <class AllocT>
	static Skipper<ConcretionSkipper_> TPL_CALL skipper(const tpl::Rule<ConcretionSkipper_>& skipper_) {
		return (const Skipper<ConcretionSkipper_>&)skipper_;
	}

	template <class AllocT>
	static Skipper<SkipperImpl<SkipWhiteSpaces, Source, Context> > TPL_CALL skipws() {
		return skipper(tpl::skipws());
	}

public:
	// Grammar

	template <int uCharacter>
	class GrammarT : public tpl::Grammar<GConcretion<uCharacter, Source, Context, false> >
	{
	private:
		typedef GConcretion<uCharacter, Source, Context, false> Impl;
		typedef tpl::Grammar<Impl> Base;

	public:
		GrammarT() {}

		template <class AllocT, class RegExT>
		GrammarT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}

		template <class SourceT, class ContextT, class SkipperT> __forceinline
		bool TPL_CALL match(SourceT& ar, ContextT& context, const Skipper<SkipperT>& skipper_) const {
			return Impl::match(ar, context, skipper_);
		}
	};

	typedef GrammarT<0> Grammar;
	typedef GrammarT<CharacterTraits_::characterMarked> MarkedGrammar;

public:
	// Grammar helper functions:

	template <class RegExT, class SkipperT>
	static inline bool TPL_CALL match(
		Iterator pos, Iterator pos2, const tpl::Grammar<RegExT>& grammar_, 
		const tpl::Rule<SkipperT>& skipper_)
	{
		// equal to:
		//	 return match(pos, pos2, skipper(skipper_) >> grammar_);

		Source source(pos, pos2);
		Context context;
		return grammar_.match(source, context, skipper(skipper_));
	}

	template <class RegExT, class SkipperT>
	static inline bool TPL_CALL match(
		Iterator pos, const tpl::Grammar<RegExT>& grammar_,
		const tpl::Rule<SkipperT>& skipper_)
	{
		// equal to:
		//	 return match(pos, skipper(skipper_) >> grammar_);

		Source source(pos);
		Context context;
		return grammar_.match(source, context, skipper(skipper_));
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
