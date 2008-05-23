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
// Module: tpl/regex/Ref.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_REF_H
#define TPL_REGEX_REF_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_FIND_H
#include "regex/Find.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class Ref

template <class RegExT>
class Ref
{
private:
	const RegExT& m_x;

public:
	Ref(const RegExT& x) : m_x(x) {}

public:
	enum { category = RegExT::category };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return m_x.match(ar, context);
	}
};

template <class T1>
__forceinline Rule<Ref<T1> > TPL_CALL ref(const Rule<T1>& x) {
	return Rule<Ref<T1> >(x);
}

// -------------------------------------------------------------------------
// class FindRefCh

template <class CharT, bool bEat = false>
class FindRefCh
{
private:
	const CharT& m_c;

public:
	FindRefCh(const CharT& c) : m_c(c) {}

public:
	enum { category = 0 };

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typedef typename SourceT::int_type int_type;
		typedef typename SourceT::uchar_type uchar_type;
		WINX_ASSERT(sizeof(CharT == sizeof(uchar_type)));

		NotCh_ notCh((uchar_type)m_c);
		return match_while<bEat>(ar, notCh);
	}
};

template <bool bEat>
__forceinline Rule<FindRefCh<char, bEat> > TPL_CALL find_ref(const char& x) {
	return Rule<FindRefCh<char, bEat> >(x);
}

template <bool bEat>
__forceinline Rule<FindRefCh<wchar_t, bEat> > TPL_CALL find_ref(const wchar_t& x) {
	return Rule<FindRefCh<wchar_t, bEat> >(x);
}

__forceinline Rule<FindRefCh<char> > TPL_CALL find_ref(const char& x) {
	return Rule<FindRefCh<char> >(x);
}

__forceinline Rule<FindRefCh<wchar_t> > TPL_CALL find_ref(const wchar_t& x) {
	return Rule<FindRefCh<wchar_t> >(x);
}

// -------------------------------------------------------------------------
// class RefCh

template <class CharT>
class RCh_
{
private:
	const CharT& m_c;

public:
	RCh_(const CharT& c) : m_c(c) {}

	__forceinline bool TPL_CALL operator()(CharT c) const {
		return c == m_c;
	}
};

template <class CharT>
class RefCh : public EqCh<RCh_<CharT> >
{
public:
	RefCh(const CharT& c)
		: EqCh<RCh_<CharT> >(c) {}
};

__forceinline Rule<RefCh<char> > TPL_CALL ref(const char& x) {
	return Rule<RefCh<char> >(x);
}

__forceinline Rule<RefCh<wchar_t> > TPL_CALL ref(const wchar_t& x) {
	return Rule<RefCh<wchar_t> >(x);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_REF_H */
