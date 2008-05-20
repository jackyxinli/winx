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
// Module: tpl/regex/RegExp.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// Description: AOP - Aspect Oriented Programming
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_REGEXP_H
#define TPL_REGEX_REGEXP_H

#ifndef TPL_CALL
#define TPL_CALL
#endif

#if 0
#define TPL_HAS_TEMPLATE_TYPEDEF
#endif

#ifndef TPL_UNMANAGED_NEW
#define TPL_UNMANAGED_NEW(alloc, Type)	new((alloc).allocate(sizeof(Type))) Type
#endif

#ifndef TPL_NEW
	#if defined(BOOST_NEW)
	#define TPL_NEW(alloc, Type)	BOOST_NEW(alloc, Type)
	#else
	#define TPL_NEW(alloc, Type)	STD_NEW(alloc, Type)
	#endif
#endif

// -------------------------------------------------------------------------
// class Exp

template <class RegExT>
class Exp : public RegExT
{
public:
	Exp() {}

	template <class T1>
	Exp(const T1& x) : RegExT(x) {}

	template <class T1, class T2>
	Exp(const T1& x, const T2& y) : RegExT(x, y) {}

	template <class T1, class T2>
	Exp(T1& x, const T2& y) : RegExT(x, y) {}
};

// -------------------------------------------------------------------------
// class IRegExp

template <class Iterator>
class IRegExp
{
public:
	virtual bool TPL_CALL match(Iterator curr, Iterator last, Iterator& out) = 0;
};

template <class RegExT, class Iterator>
class ImplRegEx : public IRegExp<Iterator>
{
private:
	RegExT m_x;

public:
	ImplRegEx(const RegExT& x) : m_x(x) {}

	bool TPL_CALL match(Iterator curr, Iterator last, Iterator& out) {
		return m_x.match(curr, last, out);
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

template <class Iterator, template <class T> class FactoryT = Factory>
class Propxy
{
private:
	IRegExp<Iterator>* m_x;

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
		typedef ImplRegEx<RegExT, Iterator> Impl;
		m_x = FactoryT<Impl>::create(alloc, x);
	}

	bool TPL_CALL match(Iterator curr, Iterator last, Iterator& out)
	{
		if (!m_x)
			return false;
		return m_x->match(curr, last, out);
	}
};

// -------------------------------------------------------------------------
// class RegExp

#if defined(TPL_HAS_TEMPLATE_TYPEDEF)

template <class Iterator, template <class T> class FactoryT = Factory>
typedef Exp< Propxy<Iterator, FactoryT> > RegExpT;

#else

template <class Iterator, template <class T> class FactoryT = Factory>
class RegExpT : public Exp< Propxy<Iterator, FactoryT> >
{
private:
	typedef Exp< Propxy<Iterator, FactoryT> > Base;

public:
	RegExpT() {}

	template <class AllocT, class RegExT>
	RegExpT(AllocT& alloc, const RegExT& x)
		: Base(alloc, x) {}
};

#endif

typedef RegExpT<const char*> RegExp; // RegExp with destructor
typedef RegExpT<const char*, UnmanagedFactory> SimpleRegExp; // RegExp without destructor

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_REGEXP_H */
