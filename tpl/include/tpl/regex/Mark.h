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
// Module: tpl/regex/Mark.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// Description: AOP - Aspect Oriented Programming
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_MARK_H
#define TPL_REGEX_MARK_H

#ifndef TPL_REGEX_REGEXP_H
#include "RegExp.h"
#endif

#if !defined(_STRING_) && !defined(_STRING)
#include <string>
#endif

// -------------------------------------------------------------------------
// class SubMatchT

template <class Iterator>
class SubMatchT
{
public:
	Iterator first;
	Iterator second;
	bool matched;

public:
	typedef ptrdiff_t difference_type;

	SubMatchT() : matched(false) {}
	SubMatchT(Iterator first_, Iterator second_, bool matched_ = false)
		: first(first_), second(second_), matched(matched_) {}

	difference_type TPL_CALL length() const {
		return matched ? std::distance(first, second) : 0;
	}

	std::string TPL_CALL stl_str() const {
		return matched ? std::string(first, second) : std::string();
	}

public:
	template <class RegExT>
	class Assign
	{
	public:
		typedef SubMatchT value_type;

	private:
		value_type& m_val;
		RegExT m_x;

	public:
		Assign(value_type& val, const RegExT& x) : m_val(val), m_x(x) {}

	public:
		bool TPL_CALL match(Iterator curr, Iterator last, Iterator& out)
		{
			m_val.matched = m_x.match(curr, last, out);
			if (m_val.matched)
			{
				m_val.first = curr;
				m_val.second = out;
			}
			return m_val.matched;
		}
	};

	template <class T1>
	Exp<Assign<Exp<T1> > > TPL_CALL operator=(const Exp<T1>& x) {
		return Exp<Assign<Exp<T1> > >(*this, x);
	}
};

typedef SubMatchT<const char*> SubMatch;

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_MARK_H */
