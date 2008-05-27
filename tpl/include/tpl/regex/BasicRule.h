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
// Module: tpl/regex/BasicRule.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_BASICRULE_H
#define TPL_REGEX_BASICRULE_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_REF_H
#include "Ref.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class BasicRule

template <int uCharacter, class SourceT, class ContextT, bool bManaged = false>
class BasicRule
{
private:
	typedef bool TPL_CALL _FN_match(const void* pThis, SourceT& ar, ContextT& context);
	typedef _FN_match* FN_match;

	const void* m_this;
	FN_match m_fn;

public:
	BasicRule() : m_this(NULL) {}
	
	template <class AllocT, class RegExT>
	BasicRule(AllocT& alloc, const RegExT& x)
	{
		assign(alloc, x);
	}

public:
	enum { character = uCharacter };

	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		TPL_ASSERT(m_this);
		return m_fn(m_this, ar, context);
	}

private:
	template <class RegExT>
	class Impl
	{
	private:
		RegExT m_x;

	public:
		Impl(const Rule<RegExT>& x)
			: m_x(static_cast<const RegExT&>(x)) {
		}

		enum { character = RegExT::character };

		static bool TPL_CALL match(const void* pThis, SourceT& ar, ContextT& context) {
			const RegExT& x = ((const Impl*)pThis)->m_x;
			return x.match(ar, context);
		}
	};

public:
	template <class AllocT, class RegExT>
	void TPL_CALL assign(AllocT& alloc, const Rule<RegExT>& x)
	{
		typedef Impl<RegExT> Imp;
		TPL_ASSERT((Imp::character & ~character) == 0);

		if (bManaged)
			m_this = TPL_NEW(alloc, Imp)(x);
		else
			m_this = TPL_UNMANAGED_NEW(alloc, Imp)(x);
	
		m_fn = Imp::match;
	}

public:
	class Var : public Rule<Ref<BasicRule> >
	{
	private:
		BasicRule m_x;

	public:
		Var() : Rule<Ref<BasicRule> >(m_x) {
		}

		template <class AllocT, class RegExT>
		void TPL_CALL assign(AllocT& alloc, const Rule<RegExT>& x) {
			m_x.assign(alloc, x);
		}
	};
};

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_BASICRULE_H */
