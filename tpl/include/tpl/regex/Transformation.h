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
// Module: tpl/regex/Transformation.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_TRANSFORMATION_H
#define TPL_REGEX_TRANSFORMATION_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class ICase, UCase

class ICase
{
public:
	int TPL_CALL operator()(int ch) const {
		return tolower(ch);
	}
};

class UCase
{
public:
	int TPL_CALL operator()(int ch) const {
		return toupper(ch);
	}
};

__forceinline Transformation<ICase> TPL_CALL icase() {
	return Transformation<ICase>();
}

__forceinline Transformation<UCase> TPL_CALL ucase() {
	return Transformation<UCase>();
}

// =========================================================================
// class TransfSource

template <class IteratorT, class TransformT>
class TransfIterator : public IteratorT
{
private:
	typedef typename IteratorT::value_type CharT;

	TransformT m_transf;

public:
	TransfIterator(const IteratorT& it, const TransformT& transf)
		: IteratorT(it), m_transf(transf) {
	}

	CharT TPL_CALL operator*() const {
		return m_transf(*(*this));
	}
};

template <class CharT, class TransformT>
class TransfIterator<const CharT*, TransformT> :
	public std::iterator<std::random_access_iterator_tag, CharT>
{
private:
	typedef const CharT* IteratorT;
	typedef std::iterator<std::random_access_iterator_tag, CharT> Base;

private:
	IteratorT m_it;
	TransformT m_transf;

public:
	typedef typename Base::difference_type difference_type;
	
public:
	TransfIterator(const CharT* it, const TransformT& transf)
		: m_it(it), m_transf(transf) {
	}

	operator IteratorT() const {
		return m_it;
	}

	CharT TPL_CALL operator*() const {
		return m_transf(*m_it);
	}

	const TransfIterator& TPL_CALL operator++() {
		++m_it;
		return *this;
	}

	const TransfIterator& TPL_CALL operator--() {
		--m_it;
		return *this;
	}

	TransfIterator TPL_CALL operator++(int) const {
		TransfIterator it = *this;
		++m_it;
		return it;
	}

	TransfIterator TPL_CALL operator--(int) const {
		TransfIterator it = *this;
		--m_it;
		return it;
	}

	TransfIterator& TPL_CALL operator+=(difference_type off) {
		m_it += off;
		return *this;
	}

	TransfIterator TPL_CALL operator+(difference_type off) const {
		return TransfIterator(m_it + off, m_transf);
	}

	TransfIterator& TPL_CALL operator-=(difference_type off) {
		m_it -= off;
		return *this;
	}

	TransfIterator TPL_CALL operator-(difference_type off) const {
		return TransfIterator(m_it - off, m_transf);
	}

	difference_type TPL_CALL operator-(const TransfIterator& o) const {
		return m_it - o.m_it;
	}

	bool TPL_CALL operator==(const TransfIterator& it) const {
		return m_it == it;
	}

	bool TPL_CALL operator!=(const TransfIterator& it) const {
		return m_it != it;
	}
};

template <class SourceT, class TransformT>
class TransfSource
{
private:
	SourceT& m_ar;
	TransformT m_transf;

	typedef typename SourceT::iterator OldIt_;

public:
	typedef TransfIterator<OldIt_, TransformT> iterator;
	typedef typename SourceT::char_type char_type;
	typedef typename SourceT::uchar_type uchar_type;
	typedef typename SourceT::int_type int_type;
	typedef typename SourceT::restriction_type restriction_type;

public:
	TransfSource(SourceT& ar)
		: m_ar(ar) {
	}

	TransfSource(SourceT& ar, const TransformT& transf)
		: m_ar(ar), m_transf(transf) {
	}

public:
	operator SourceT&() const {
		return m_ar;
	}

	iterator TPL_CALL position() const {
		return iterator(m_ar.position(), m_transf);
	}

	void TPL_CALL seek(OldIt_ it) const {
		return m_ar.seek(it);
	}

	int_type TPL_CALL get() const {
		return m_transf(m_ar.get());
	}

	void TPL_CALL unget() const {
		m_ar.unget();
	}

	void TPL_CALL unget(int_type c) const {
		m_ar.unget(c);
	}

	int_type TPL_CALL peek() const {
		return m_transf(m_ar.peek());
	}
};

// -------------------------------------------------------------------------
// class SourceICase, SourceUCase

template <class SourceT>
class SourceICase : public TransfSource<SourceT, ICase>
{
private:
	typedef TransfSource<SourceT, ICase> Base;

public:
	SourceICase(SourceT& ar) : Base(ar) {}
};

template <class SourceT>
class SourceUCase : public TransfSource<SourceT, UCase>
{
private:
	typedef TransfSource<SourceT, UCase> Base;

public:
	SourceUCase(SourceT& ar) : Base(ar) {}
};

// -------------------------------------------------------------------------
// class Transf

// Usage: Transformation->*Rule		--- eg. icase()->*Rule
// Usage: Transformation>>Rule		--- same as: Transformation->*Rule

template <class RegExT, class TransformT>
class Transf
{
private:
	RegExT m_x;
	TransformT m_transf;

public:
	Transf() {}
	Transf(const RegExT& x, const TransformT& transf)
		: m_x(x), m_transf(transf) {}

public:
	enum { character = RegExT::character };

	typedef typename RegExT::convertible_type convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		TransfSource<SourceT, TransformT> arTransf(ar, m_transf);
		return m_x.match(arTransf, context);
	}
};

template <class T1, class T2> __forceinline
Rule<Transf<T1, T2> > TPL_CALL operator->*(Transformation<T2>& y, const Rule<T1>& x) {
	return Rule<Transf<T1, T2> >(x, y);
}

template <class T1, class T2> __forceinline
Rule<Transf<T1, T2> > TPL_CALL operator>>(Transformation<T2>& y, const Rule<T1>& x) {
	return Rule<Transf<T1, T2> >(x, y);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_TRANSFORMATION_H */
