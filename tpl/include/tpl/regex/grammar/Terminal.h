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
// Module: tpl/regex/grammar/Terminal.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_GRAMMAR_TERMINAL_H
#define TPL_REGEX_GRAMMAR_TERMINAL_H

#ifndef TPL_REGEX_TERMINAL_H
#include "../Terminal.h"
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "../Composition.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// function gr::ch

template <class PredT>
class GEqCh : public Gr<EqCh<PredT> >
{
private:
	typedef Gr<EqCh<PredT> > Base;

public:
	GEqCh() {}

	template <class T1>
	GEqCh(const T1& x) : Base(x) {}

	template <class T1, class T2>
	GEqCh(const T1& x, const T2& y) : Base(x, y) {}

	template <class T1, class T2, class T3>
	GEqCh(const T1& x, const T2& y, const T3& z) : Base(x, y, z) {}
};

namespace gr {

	typedef GEqCh<C1> GCh1_;
	typedef GEqCh<C2> GCh2_;
	typedef GEqCh<C3> GCh3_;

	__forceinline Grammar<GCh1_> TPL_CALL ch(int x) {
		return Grammar<GCh1_>(x);
	}

	__forceinline Grammar<GCh2_> TPL_CALL ch(int c1, int c2) {
		return Grammar<GCh2_>(c1, c2);
	}

	__forceinline Grammar<GCh3_> TPL_CALL ch(int c1, int c2, int c3) {
		return Grammar<GCh3_>(c1, c2, c3);
	}

} // namespace gr

// =========================================================================
// function gr::eq

template <class Iterator>
class GEq_ : public Gr<Eq_<Iterator> >
{
private:
	typedef Gr<Eq_<Iterator> > Base;

public:
	template <class StringT>
	GEq_(const StringT& s) : Base(s) {}

	GEq_(Iterator s, size_t n) : Base(s, n) {}
};

template <class CharT>
class GEq : public Gr<Eq_<const CharT*> >
{
private:
	typedef public Gr<Eq_<const CharT*> > Base;

public:
	GEq(const CharT* s, size_t n)
		: Base(s, n) {
	}

	GEq(const CharT* s)
		: Base(s,std::char_traits<CharT>::length(s)) {
	}
};

namespace gr {

	template <class CharT>
	__forceinline Grammar<GEq<CharT> > TPL_CALL eq(const CharT* s) {
		return Grammar<GEq<CharT> >(s);
	}

	template <class CharT>
	__forceinline Grammar<GEq<CharT> > TPL_CALL eq(const CharT* s, size_t n) {
		return Grammar<GEq<CharT> >(s, n);
	}

	template <class CharT>
	__forceinline Grammar<GEq<CharT> > TPL_CALL str(const CharT* s) {
		return Grammar<GEq<CharT> >(s);
	}

	template <class CharT>
	__forceinline Grammar<GEq<CharT> > TPL_CALL str(const CharT* s, size_t n) {
		return Grammar<GEq<CharT> >(s, n);
	}

} // namespace gr

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_GRAMMAR_TERMINAL_H */
