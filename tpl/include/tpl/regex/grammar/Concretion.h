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
// Module: tpl/regex/grammar/Concretion.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_GRAMMAR_CONCRETION_H
#define TPL_REGEX_GRAMMAR_CONCRETION_H

#ifndef TPL_REGEX_REF_H
#include "../Concretion.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class GrBind -- Grammar + Skipper => Rule

template <class GrammarT, class SkipperT>
class GrBind
{
private:
	GrammarT m_gr;
	SkipperT m_skipper;

public:
	GrBind(const GrammarT& gr_, const SkipperT& skipper_)
		: m_gr(gr_), m_skipper(skipper_) {}

public:
	enum { character = GrammarT::character | SkipperT::character };

	typedef SelfConvertible convertible_type;
	typedef const Grammar<GrammarT>& grammar_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return m_gr.match(ar, context, (const Skipper<SkipperT>&)m_skipper);
	}

	grammar_type TPL_CALL grammar() const {
		return (grammar_type)m_gr;
	}

	const Skipper<SkipperT>& TPL_CALL skipper() const {
		return (const Skipper<SkipperT>&) m_skipper;
	}
};

template <class GrammarT, class SkipperT> __forceinline
Rule<GrBind<GrammarT, SkipperT> > TPL_CALL operator>>(const Skipper<SkipperT>& y, const Grammar<GrammarT>& x) {
	return Rule<GrBind<GrammarT, SkipperT> >(x, y);
}

// =========================================================================
// Skipper Implementation

// class SkipperTraits

template <class SourceT, class ContextT>
class SkipperTraits
{
public:
	typedef Concretion<CHARACTER_SKIPPER, SourceT, ContextT, false> concretion_type;
};

// class SkipperImpl

template <class SkipperT, class SourceT, class ContextT>
class SkipperImpl : public SkipperT
{
private:
	typedef SkipperTraits<SourceT, ContextT> Tr_;
	typedef typename Tr_::concretion_type ConcretionT;

	ConcretionT m_alter;

public:
	template <class AllocT>
	SkipperImpl(AllocT& alloc, const Rule<SkipperT>& x)
		: SkipperT(x), m_alter(alloc, x) {}

public:
	typedef const ConcretionT& concreation_type;

	concreation_type TPL_CALL concretion() const {
		return m_alter;
	}
};

// -------------------------------------------------------------------------
// class NullSkipper

inline bool TPL_CALL null_skipper_match_(const void* pThis, int& ar, int& context) {
	return true;
}

template <class SourceT, class ContextT>
class NullSkipper : public Null<true>
{
private:
	typedef SkipperTraits<SourceT, ContextT> Tr_;
	typedef typename Tr_::concretion_type ConcretionT;

	typedef bool TPL_CALL _FN_match(const void* pThis, SourceT& ar, ContextT& context);
	typedef _FN_match* FN_match;

public:
	const ConcretionT TPL_CALL concretion() const {
		return ConcretionT(NULL, (FN_match)null_skipper_match_);
	}
};

// -------------------------------------------------------------------------
// class GConcretion

template <int uCharacter, class SourceT, class ContextT, bool bManaged = false>
class GConcretion
{
private:
	typedef SkipperTraits<SourceT, ContextT> Tr_;
	typedef typename Tr_::concretion_type ConcretionT;
	typedef bool TPL_CALL _FN_match(const void* pThis, SourceT& ar, ContextT& context, const ConcretionT& skipper_);
	typedef _FN_match* FN_match;

	const void* m_this;
	FN_match m_fn;

public:
	GConcretion() : m_this(NULL) {}

	template <class AllocT, class GrammarT>
	GConcretion(AllocT& alloc, const Grammar<GrammarT>& x) {
		assign(alloc, x);
	}

public:
	enum { character = uCharacter };

	template <class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const Skipper<SkipperT>& skipper_) const {
		TPL_ASSERT(m_this);
		return m_fn(m_this, ar, context, skipper_.concretion());
	}

private:
	template <class GrammarT>
	class Impl
	{
	private:
		GrammarT m_x;

	public:
		Impl(const Grammar<GrammarT>& x)
			: m_x(static_cast<const GrammarT&>(x)) {
		}

		enum { character = GrammarT::character };

		static bool TPL_CALL match(const void* pThis, SourceT& ar, ContextT& context, const ConcretionT& skipper_) {
			const GrammarT& x = ((const Impl*)pThis)->m_x;
			return x.match(ar, context, (const Skipper<ConcretionT>&)(skipper_));
		}
	};

public:
	template <class AllocT, class GrammarT>
	void TPL_CALL assign(AllocT& alloc, const Grammar<GrammarT>& x)
	{
		typedef Impl<GrammarT> Imp;
		TPL_ASSERT((Imp::character & ~character) == 0);

		if (bManaged)
			m_this = TPL_NEW(alloc, Imp)(x);
		else
			m_this = TPL_UNMANAGED_NEW(alloc, Imp)(x);

		m_fn = Imp::match;
	}

public:
	class Var : public Grammar<GRef<GConcretion> >
	{
	private:
		GConcretion m_x;

	public:
		Var() : Grammar<GRef<GConcretion> >(m_x) {
		}

		template <class AllocT, class GrammarT>
		void TPL_CALL assign(AllocT& alloc, const Grammar<GrammarT>& x) {
			m_x.assign(alloc, x);
		}
	};
};

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_GRAMMAR_CONCRETION_H */
