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
// Module: tpl/regex/Find.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_FIND_H
#define TPL_REGEX_FIND_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_TERMINAL_H
#include "Terminal.h"
#endif

#ifndef STDEXT_KMP_FINDER_H
#include "../../../../stdext/include/stdext/kmp/Finder.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// function find_if

template <bool bEat, class SourceT, class PredT>
inline bool TPL_CALL do_find_if(SourceT& ar, PredT pred)
{
	typename SourceT::iterator pos = ar.position();
	for (;;)
	{
		typename SourceT::int_type c = ar.get();
		if (pred(c)) {
			if (!bEat)
				ar.unget();
			return true;
		}
		else if (c == SourceT::endch) {
			ar.seek(pos);
			return false;
		}
	}
}

template <class PredT, bool bEat = false>
class FindIf
{
private:
	PredT m_pred;

public:
	FindIf() {}

	template <class T1>
	FindIf(const T1& pred) : m_pred(pred) {}

public:
	enum { character = 0 };
	enum { vtype = 0 };

	typedef ExplicitConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return do_find_if<bEat>(ar, m_pred);
	}
};

template <bool bEat, class PredT>
__forceinline Rule<FindIf<PredT, bEat> > TPL_CALL find_if(PredT pred) {
	return Rule<FindIf<PredT, bEat> >(pred);
}

template <class PredT>
__forceinline Rule<FindIf<PredT, false> > TPL_CALL find_if(PredT pred) {
	return Rule<FindIf<PredT, false> >(pred);
}

// -------------------------------------------------------------------------
// class FindCh

template <bool bEat>
class FindCh : public FindIf<C1, bEat>
{
public:
	FindCh(int x) : FindIf<C1, bEat>(x) {}
};

// -------------------------------------------------------------------------
// class FindChSet

template <bool bEat, int m_c1, int m_c2 = m_c1, int m_c3 = m_c2>
class FindChSet : public FindIf<C_<m_c1, m_c2, m_c3>, bEat> {
};

template <int m_c1, int m_c2>
__forceinline Rule<FindChSet<false, m_c1, m_c2> > TPL_CALL find_set() {
	return Rule<FindChSet<false, m_c1, m_c2> >();
}

template <int m_c1, int m_c2, int m_c3>
__forceinline Rule<FindChSet<false, m_c1, m_c2, m_c3> > TPL_CALL find_set() {
	return Rule<FindChSet<false, m_c1, m_c2, m_c3> >();
}

// -------------------------------------------------------------------------
// class FindStr

template <class Iterator, bool bEat = false>
class FindStr_
{
private:
	typedef std::iterator_traits<Iterator> IteratorTr_;
	typedef typename IteratorTr_::value_type CharT_;
	typedef NS_KMP Finder<CharT_> Finder_;

	Iterator m_begin;
	Iterator m_end;

public:
	FindStr_(Iterator patternBegin, Iterator patternEnd)
		: m_begin(patternBegin), m_end(patternEnd) {
	}

public:
	enum { character = 0 };
	enum { vtype = 0 };

	typedef ExplicitConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		Finder_ finder(m_begin, m_end);
		const typename SourceT::iterator pos = ar.position();
		if (finder.next(ar) != S_OK) {
			ar.seek(pos);
			return false;
		}
		else {
			if (!bEat) {
				typename SourceT::iterator seekTo = pos;
				const typename SourceT::iterator pos2 = ar.position();
				std::advance(seekTo, std::distance(pos, pos2) - finder.size());
				ar.seek(seekTo);
			}
			return true;
		}
	}
};

template <class CharT, bool bEat = false>
class FindStr : public FindStr_<const CharT*, bEat>
{
public:
	FindStr(const CharT* s_)
		: FindStr_<const CharT*, bEat>(s_, s_ + std::char_traits<CharT>::length(s_)) {};
};

// -------------------------------------------------------------------------
// function find(val)

// Usage: find('c')				--- means: find character 'c'. ('c' remains in the inputstream)
// Usage: find<true>('c')		--- means: find character 'c' and eat it.

template <class Type, bool bEat = false>
struct FindTraits {
};

template <bool bEat>
struct FindTraits<char, bEat> {
	typedef FindCh<bEat> find_type;
};

template <bool bEat>
struct FindTraits<wchar_t, bEat> {
	typedef FindCh<bEat> find_type;
};

template <bool bEat>
struct FindTraits<int, bEat> {
	typedef FindCh<bEat> find_type;
};

template <class CharT, bool bEat>
struct FindTraits<const CharT*, bEat> {
	typedef FindStr<CharT, bEat> find_type;
};

template <class CharT, bool bEat>
struct FindTraits<CharT*, bEat> {
	typedef FindStr<CharT, bEat> find_type;
};

template <class CharT, size_t sz, bool bEat>
struct FindTraits<CharT[sz], bEat> {
	typedef FindStr<CharT, bEat> find_type;
};

template <class CharT, size_t sz, bool bEat>
struct FindTraits<const CharT[sz], bEat> {
	typedef FindStr<CharT, bEat> find_type;
};

template <bool bEat, class Type> __forceinline
Rule<typename FindTraits<Type, bEat>::find_type> TPL_CALL find(const Type& val) {
	return Rule<typename FindTraits<Type, bEat>::find_type>(val);
}

template <class Type> __forceinline
Rule<typename FindTraits<Type, false>::find_type> TPL_CALL find(const Type& val) {
	return Rule<typename FindTraits<Type, false>::find_type>(val);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_FIND_H */
