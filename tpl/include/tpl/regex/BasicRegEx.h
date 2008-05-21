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
// Module: tpl/regex/BasicRegEx.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_BASICREGEX_H
#define TPL_REGEX_BASICREGEX_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

// -------------------------------------------------------------------------
// class IRegExp

template <class SourceT, class ContextT>
class IRegExp
{
public:
	virtual bool TPL_CALL match(SourceT& ar, ContextT& context) = 0;
};

template <class RegExT, class SourceT, class ContextT>
class ImplRegEx : public IRegExp<SourceT, ContextT>
{
private:
	RegExT m_x;

public:
	ImplRegEx(const RegExT& x) : m_x(x) {}

	bool TPL_CALL match(SourceT& ar, ContextT& context) {
		return m_x.match(ar, context);
	}
};

// -------------------------------------------------------------------------
// class Propxy

template <class ImplT>
class Factory
{
public:
	template <class AllocT, class T1>
	static ImplT* TPL_CALL create(AllocT& alloc, const T1& x) {
		return TPL_NEW(alloc, ImplT)(x);
	}
};

template <class ImplT>
class UnmanagedFactory
{
public:
	template <class AllocT, class T1>
	static ImplT* TPL_CALL create(AllocT& alloc, const T1& x) {
		return TPL_UNMANAGED_NEW(alloc, ImplT)(x);
	}
};

template <class SourceT, class ContextT, template <class T> class FactoryT = Factory>
class Propxy
{
private:
	IRegExp<SourceT, ContextT>* m_x;

public:
	Propxy() : m_x(NULL) {}

	template <class AllocT, class RegExT>
	Propxy(AllocT& alloc, const RegExT& x)
	{
		assign(alloc, x);
	}

	template <class AllocT, class RegExT>
	void TPL_CALL assign(AllocT& alloc, const RegExT& x)
	{
		typedef ImplRegEx<RegExT, SourceT, ContextT> Impl;
		m_x = FactoryT<Impl>::create(alloc, x);
	}

	bool TPL_CALL match(SourceT& ar, ContextT& context)
	{
		if (!m_x)
			return false;
		return m_x->match(ar, context);
	}
};

// -------------------------------------------------------------------------
// class BasicRegEx

#if defined(TPL_HAS_TEMPLATE_TYPEDEF)

template <class SourceT, class ContextT, template <class T> class FactoryT = Factory>
typedef Exp< Propxy<SourceT, ContextT, FactoryT> > BasicRegEx;

#else

template <class SourceT, class ContextT, template <class T> class FactoryT = Factory>
class BasicRegEx : public Exp< Propxy<SourceT, ContextT, FactoryT> >
{
private:
	typedef Exp< Propxy<SourceT, ContextT, FactoryT> > Base;

public:
	BasicRegEx() {}

	template <class AllocT, class RegExT>
	BasicRegEx(AllocT& alloc, const RegExT& x)
		: Base(alloc, x) {}
};

#endif

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_BASICREGEX_H */
