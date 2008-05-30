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

#ifndef TPL_REGEX_MATCHRESULT_H
#include "MatchResult.h"
#endif

#ifndef TPL_REGEX_REF_H
#include "Ref.h"
#endif

#if !defined(_STRING_) && !defined(_STRING)
#include <string>
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class AssigStr

// Usage: Rule/assign(a_string_var)
// Usage: Rule/assign(a_leaf_node_var)

template <class StringT>
class AssigStr
{
private:
	StringT& m_result;

public:
	AssigStr(StringT& result) : m_result(result) {
	}

	enum { required_vtypes = 0 };

	template <class Iterator>
	void TPL_CALL operator()(Iterator pos, Iterator pos2) const {
		m_result.assign(pos, pos2);
	}
};

// -------------------------------------------------------------------------
// class AssigCh

// Usage: Rule/assign(a_char_var)

template <class CharT>
class AssigCh
{
private:
	CharT& m_result;

public:
	AssigCh(CharT& result) : m_result(result) {
	}

	enum { required_vtypes = VTYPE_CHAR };

	template <class Iterator>
	void TPL_CALL operator()(Iterator pos, Iterator pos2) const {
		TPL_ASSERT(std::distance(pos, pos2) == 1);
		m_result = *pos;
	}
};

// -------------------------------------------------------------------------
// class AssigUInt

// Usage: Rule/assign(a_uint_var)
// Usage: Rule/&a_uint_var

template <class Iterator, class UIntT>
inline Iterator TPL_CALL _parseUInt(UIntT& result, Iterator first, Iterator last)
{
	UIntT val = 0;
	for (; first != last; ++first) {
		TPL_ASSERT(*first >= '0' && *first <= '9');
		val = val * 10 + (*first - '0');
	}
	result = val;
	return first;
}

template <class UIntT>
class AssigUInt
{
private:
	UIntT& m_result;

public:
	AssigUInt(UIntT& result) : m_result(result) {
	}

	enum { required_vtypes = VTYPE_U_INTEGER };

	template <class Iterator>
	void TPL_CALL operator()(Iterator pos, Iterator pos2) const {
		_parseUInt(m_result, pos, pos2);
	}
};

// -------------------------------------------------------------------------
// class AssigInt

// Usage: Rule/assign(a_int_var)
// Usage: Rule/&a_int_var

template <class Iterator, class IntT>
inline Iterator TPL_CALL _parseInt(IntT& result, Iterator first, Iterator last)
{
	if (*first == '-') {
		_parseUInt(result, first+1, last);
		result = -result;
		return last;
	}
	else if (*first == '+') {
		++first;
	}
	return _parseUInt(result, first, last);
}

template <class IntT>
class AssigInt
{
private:
	IntT& m_result;

public:
	AssigInt(IntT& result) : m_result(result) {
	}

	enum { required_vtypes = VTYPE_U_INTEGER | VTYPE_INTEGER };

	template <class Iterator>
	void TPL_CALL operator()(Iterator pos, Iterator pos2) const {
		_parseInt(m_result, pos, pos2);
	}
};

// -------------------------------------------------------------------------
// class AssigReal

// Usage: Rule/assign(a_real_var)
// Usage: Rule/&a_real_var

template <class RealT>
inline const char* TPL_CALL _parseReal(RealT& result, const char* first, const char* last)
{
	result = (RealT)strtod(first, (char**)&first);
	TPL_ASSERT(first == last);
	return last;
}

template <class RealT>
inline const wchar_t* TPL_CALL _parseReal(RealT& result, const wchar_t* first, const wchar_t* last)
{
	result = (RealT)wcstod(first, (wchar_t**)&first);
	TPL_ASSERT(first == last);
	return last;
}

template <class RealT>
class AssigReal
{
private:
	RealT& m_result;

public:
	AssigReal(RealT& result) : m_result(result) {
	}

	enum { required_vtypes = VTYPE_U_INTEGER | VTYPE_INTEGER | VTYPE_U_REAL | VTYPE_REAL };

	template <class Iterator>
	void TPL_CALL operator()(Iterator pos, Iterator pos2) const {
		_parseReal(m_result, pos, pos2);
	}
};

// -------------------------------------------------------------------------
// class AssignmentTypeTraits

template <class Type>
struct AssignmentTypeTraits {
};

template <class CharT>
struct AssignmentTypeTraits<std::basic_string<CharT> > {
	typedef AssigStr<std::basic_string<CharT> > assignment_type;
};

template <class Iterator>
struct AssignmentTypeTraits<Leaf<Iterator> > {
	typedef AssigStr<Leaf<Iterator> > assignment_type;
};

template <>
struct AssignmentTypeTraits<char> {
	typedef AssigCh<char> assignment_type;
};

template <>
struct AssignmentTypeTraits<wchar_t> {
	typedef AssigCh<wchar_t> assignment_type;
};

template <>
struct AssignmentTypeTraits<int> {
	typedef AssigInt<int> assignment_type;
};

template <>
struct AssignmentTypeTraits<unsigned> {
	typedef AssigUInt<unsigned> assignment_type;
};

template <>
struct AssignmentTypeTraits<float> {
	typedef AssigReal<float> assignment_type;
};

template <>
struct AssignmentTypeTraits<double> {
	typedef AssigReal<double> assignment_type;
};

template <class ValueType>
__forceinline 
Action<typename AssignmentTypeTraits<ValueType>::assignment_type>
TPL_CALL assign(ValueType& result) {
	return Action<typename AssignmentTypeTraits<ValueType>::assignment_type>(result);
}

template <class ValueType>
__forceinline 
Action<typename AssignmentTypeTraits<ValueType>::assignment_type>
TPL_CALL assign(Var<ValueType>& result) {
	return Action<typename AssignmentTypeTraits<ValueType>::assignment_type>(result.val);
}

// -------------------------------------------------------------------------
// class Cast

template <class ValueType, class RegExT>
class Cast : public RegExT
{
private:
	typedef AssignmentTypeTraits<ValueType> Tr_;
	typedef typename Tr_::assignment_type Assig_;

public:
	enum { vtype = Assig_::required_vtypes };
};

template <class ValueType, class RegExT> __forceinline
const Rule<Cast<ValueType, RegExT> >& TPL_CALL cast(const Rule<RegExT>& rule_) {
	return *(const Rule<Cast<ValueType, RegExT> >*)&rule_;
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_ASSIGNMENT_H */
