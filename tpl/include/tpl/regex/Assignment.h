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
// Module: tpl/regex/Assignment.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_ASSIGNMENT_H
#define TPL_REGEX_ASSIGNMENT_H

#ifndef TPL_REGEX_ACTION_H
#include "Action.h"
#endif

#ifndef TPL_REGEX_REF_H
#include "Ref.h"
#endif

#ifndef TPL_REGEX_MATCHRESULT_H
#include "MatchResult.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class AssigStr

// Usage: Rule/assign(a_string_var)
// Usage: Rule/assign(a_leaf_node_var)

class AssigStr
{
public:
	template <class StringT, class Iterator>
	static void TPL_CALL assign(StringT& result, Iterator pos, Iterator pos2, const void* pExtra) {
		result.assign(pos, pos2);
	}
};

// -------------------------------------------------------------------------
// class AssigCh

// Usage: Rule/assign(a_char_var)

class AssigCh
{
public:
	template <class CharT, class Iterator>
	static void TPL_CALL assign(CharT& result, Iterator pos, Iterator pos2, const void* pExtra) {
		TPL_ASSERT(std::distance(pos, pos2) == 1);
		result = *pos;
	}
};

// -------------------------------------------------------------------------
// class AssigUInt

// Usage: Rule/assign(a_uint_var)

class AssigUInt
{
public:
	template <class UIntT, class Iterator>
	static void TPL_CALL assign(UIntT& result, Iterator first, Iterator last, const void* pExtra)
	{
		UIntT val = 0;
		for (; first != last; ++first) {
			TPL_ASSERT(*first >= '0' && *first <= '9');
			val = val * 10 + (*first - '0');
		}
		result = val;
		TPL_ASSERT(first == last);
	}
};

// -------------------------------------------------------------------------
// class AssigInt

// Usage: Rule/assign(a_int_var)

class AssigInt
{
public:
	template <class IntT, class Iterator>
	static void TPL_CALL assign(IntT& result, Iterator first, Iterator last, const void* pExtra)
	{
		if (*first == '-') {
			AssigUInt::assign(result, first+1, last, pExtra);
			result = -result;
			return;
		}
		else if (*first == '+') {
			++first;
		}
		return AssigUInt::assign(result, first, last, pExtra);
	}
};

// -------------------------------------------------------------------------
// class AssigReal

// Usage: Rule/assign(a_real_var)

template <class RealT>
inline void TPL_CALL _parseReal(RealT& result, const char* first, const char* last)
{
	result = (RealT)strtod(first, (char**)&first);
	TPL_ASSERT(first == last);
}

template <class RealT>
inline void TPL_CALL _parseReal(RealT& result, const wchar_t* first, const wchar_t* last)
{
	result = (RealT)wcstod(first, (wchar_t**)&first);
	TPL_ASSERT(first == last);
}

class AssigReal
{
public:
	template <class RealT, class Iterator>
	static void TPL_CALL assign(RealT& result, Iterator pos, Iterator pos2, const void* pExtra) {
		_parseReal(result, pos, pos2);
	}
};

// -------------------------------------------------------------------------
// SelectAssig

template <class AssigTag, class CharT, class Tr, class Ax>
struct SelectAssig<AssigTag, std::basic_string<CharT, Tr, Ax> > {
	typedef AssigStr assig_type;
};

template <class AssigTag, class Iterator>
struct SelectAssig<AssigTag, Leaf<Iterator> > {
	typedef AssigStr assig_type;
};

NS_TPL_END

// =========================================================================

TPL_ASSIG(TagAssigNone, AssigStr)
TPL_ASSIG(TagAssigChar, AssigCh)
TPL_ASSIG(TagAssigUInteger, AssigUInt)
TPL_ASSIG(TagAssigInteger, AssigInt)
TPL_ASSIG(TagAssigUReal, AssigReal)
TPL_ASSIG(TagAssigReal, AssigReal)
TPL_ASSIG(TagAssigUFraction, AssigReal)
TPL_ASSIG(TagAssigFraction, AssigReal)

// =========================================================================
// operator/

// Usage: Rule/assign	--- eg. Rule/assign(a_variable)

NS_TPL_BEGIN

template <class ValueT>
class Assign
{
private:
	ValueT& m_val;

public:
	Assign(ValueT& val) : m_val(val) {}

	typedef ValueT value_type;

	void TPL_CALL operator()(const value_type& val) const {
		m_val = val;
	}
};

template <class ValueT> __forceinline
Action<Assign<ValueT> > TPL_CALL assign(ValueT& result) {
	return Action<Assign<ValueT> >(result);
}

template <class ValueT> __forceinline
Action<Assign<ValueT> > TPL_CALL assign(Var<ValueT>& result) {
	return Action<Assign<ValueT> >(result.val);
}

NS_TPL_END

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_ASSIGNMENT_H */
