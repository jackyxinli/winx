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
// Module: tpl/regex/result/Customization.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Customization.h 766 2008-06-28 15:41:08Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_RESULT_CUSTOMIZATION_H
#define TPL_REGEX_RESULT_CUSTOMIZATION_H

#ifndef TPL_REGEX_RESULT_CONTEXT_H
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
		typedef char TagChar;
		typedef std::Range<const char*> Leaf;
		typedef std::PointerReadArchive Source;
		typedef tpl::DefaultAllocator Allocator;
	};
}

// -------------------------------------------------------------------------
// class Customization

template <bool bHas, class Iterator, class LeafT, class Allocator, class TagCharT>
struct CustomizationContextTraits_ {
	typedef Context<Iterator, LeafT, Allocator, TagCharT> Context;
};

template <class Iterator, class LeafT, class Allocator, class TagCharT>
struct CustomizationContextTraits_<false, Iterator, LeafT, Allocator, TagCharT> {
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

	typedef typename Policy::TagChar TagChar;
	typedef typename Policy::Source Source;
	typedef typename Policy::Allocator Allocator;
	typedef typename Policy::Leaf Leaf;

public:
	// Iterator

	typedef typename Source::iterator Iterator;

private:
	template <bool bHas>
	struct ContextTraits_ : public 
		CustomizationContextTraits_<bHas, Iterator, Leaf, Allocator, TagChar> {
	};

public:
	// Context

	typedef typename ContextTraits_<bHasDocument>::Context Context;

public:
	// Leaf, Node, Document

	typedef tpl::Node<Leaf, TagChar> Node;
	typedef tpl::Document<Leaf, Allocator, TagChar> Document;

private:
	typedef CustomizationCharacterTraits_<bHasDocument> CharacterTraits_;

public:
	// Rule

	template <int uCharacter>
	class RuleT : public tpl::Rule<Concretion<uCharacter, Source, Context, false> >
	{
	private:
		typedef tpl::Rule<Concretion<uCharacter, Source, Context, false> > Base;

	public:
		RuleT() {}

		template <class AllocT, class RegExT>
		RuleT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}
	};

	typedef RuleT<0> Rule;
	typedef RuleT<CharacterTraits_::characterMarked> MarkedRule;

public:
	// ManagedRule

	template <int uCharacter>
	class ManagedRuleT : public tpl::Rule<Concretion<uCharacter, Source, Context, true> >
	{
	private:
		typedef tpl::Rule<Concretion<uCharacter, Source, Context, true> > Base;

	public:
		ManagedRuleT() {}

		template <class AllocT, class RegExT>
		ManagedRuleT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}
	};

	typedef ManagedRuleT<0> ManagedRule;
	typedef ManagedRuleT<CharacterTraits_::characterMarked> ManagedMarkedRule;

public:
	// Rule helper functions:

	template <class RegExT>
	static bool TPL_CALL match(Iterator pos, Iterator pos2, const tpl::Rule<RegExT>& rule_)
	{
		Source source(pos, pos2);
		Context context;
		return rule_.match(source, context);
	}

	template <class ContainerT, class RegExT>
	static bool TPL_CALL match(const ContainerT& src, const tpl::Rule<RegExT>& rule_)
	{
		Source source(src);
		Context context;
		return rule_.match(source, context);
	}

public:
	// Grammar

	template <int uCharacter>
	class GrammarT : public tpl::Grammar<GConcretion<uCharacter, Source, Context, false> >
	{
	private:
		typedef tpl::Grammar<GConcretion<uCharacter, Source, Context, false> > Base;

	public:
		GrammarT() {}

		template <class AllocT, class RegExT>
		GrammarT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}
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
		Source source(pos, pos2);
		Context context;
		return grammar_.match(source, context, skipper_);
	}

	template <class ContainerT, class RegExT, class SkipperT>
	static inline bool TPL_CALL match(
		const ContainerT& src, const tpl::Grammar<RegExT>& grammar_,
		const tpl::Rule<SkipperT>& skipper_)
	{
		Source source(src);
		Context context;
		return grammar_.match(source, context, skipper_);
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

#endif /* TPL_REGEX_RESULT_CUSTOMIZATION_H */

