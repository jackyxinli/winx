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

#ifndef TPL_REGEX_REF_H
#include "Ref.h"
#endif

#ifndef STDEXT_KMP_FINDER_H
#include "../../../../stdext/include/stdext/kmp/Finder.h"
#endif

NS_TPL_BEGIN

// =========================================================================
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

// =========================================================================
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

template <class Iterator, bool bEat = false>
class FindLeaf : public FindStr_<Iterator, bEat>
{
public:
	FindLeaf(const Leaf<Iterator>& s_)
		: FindStr_<Iterator, bEat>(s_.begin(), s_.end()) {};
};

// =========================================================================
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

template <class Iterator, bool bEat>
struct FindTraits<Leaf<Iterator>, bEat> {
	typedef FindLeaf<Iterator, bEat> find_type;
};

template <class RegExT, bool bEat>
struct FindTraits<Rule<RegExT>, bEat> {
	typedef FindIf<RegExT, bEat> find_type;
	TPL_REQUIRE(VTYPE_CHAR & RegExT::vtype, ChRuleRequire_);
};

template <class Type, bool bEat>
struct FindTraits<const Type&, bEat> {
	typedef typename FindTraits<Type, bEat>::find_type find_type;
};

// -------------------------------------------------------------------------
// function class OpRef

template <class RefT, class OpType>
class OpRef
{
private:
	typedef typename RefT::value_type value_type;
	typedef RefT reference_type;
	typedef typename reference_type::dereference_type dereference_type;
	typedef OpType op_type;

	reference_type m_ref;

public:
	OpRef(const value_type& var_) : m_ref(var_) {}
	OpRef(const RefT& ref_) : m_ref(ref_) {}

public:
	enum { character = op_type::character };
	enum { vtype = op_type::vtype };

	typedef typename op_type::convertible_type convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		dereference_type der(m_ref());
		op_type x(der);
		return x.match(ar, context);
	}
};

template <class RefT, bool bEat>
struct FindTraits<Rule<Deref<RefT> >, bEat> {
	typedef typename RefT::dereference_type dereference_type;
	typedef typename FindTraits<dereference_type, bEat>::find_type op_type;
	typedef OpRef<RefT, op_type> find_type;
};

// -------------------------------------------------------------------------
// function find_ref(var) = find(ref(var))

template <class Type, bool bEat = false>
class FindRefTraits
{
private:
	typedef typename ReferenceTratis<Type>::reference_type reference_type;
	typedef typename reference_type::dereference_type dereference_type;
	typedef typename FindTraits<dereference_type, bEat>::find_type op_type;

public:
	typedef OpRef<reference_type, op_type> find_type;
};

template <bool bEat, class Type> __forceinline
Rule<typename FindTraits<Type, bEat>::find_type> TPL_CALL find(const Type& val) {
	return Rule<typename FindTraits<Type, bEat>::find_type>(val);
}

template <class Type> __forceinline
Rule<typename FindTraits<Type, false>::find_type> TPL_CALL find(const Type& val) {
	return Rule<typename FindTraits<Type, false>::find_type>(val);
}

template <bool bEat, class Type> __forceinline
Rule<typename FindRefTraits<Type, bEat>::find_type> TPL_CALL find_ref(const Type& var_) {
	return Rule<typename FindRefTraits<Type, bEat>::find_type>(var_);
}

template <class Type> __forceinline
Rule<typename FindRefTraits<Type, false>::find_type> TPL_CALL find_ref(const Type& var_) {
	return Rule<typename FindRefTraits<Type, false>::find_type>(var_);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_FIND_H */
