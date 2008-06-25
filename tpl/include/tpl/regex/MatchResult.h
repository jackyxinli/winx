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
// Module: tpl/regex/MatchResult.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_MATCHRESULT_H
#define TPL_REGEX_MATCHRESULT_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_MARK_H
#include "Mark.h"
#endif

#ifndef TPL_REGEX_CONS_H
#include "Cons.h"
#endif

#ifndef TPL_REGEX_STLHEADERS_H
#include "STLHeaders.h"
#endif

#if !defined(_STRING_) && !defined(_STRING)
#include <string>
#endif

#ifndef STDEXT_STRING_H
#include "../../../../stdext/include/stdext/String.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// Leaf

template <class Iterator>
class Leaf
{
private:
	Iterator first;
	Iterator second;

private:
	typedef std::iterator_traits<Iterator> Tr_;

public:
	typedef Iterator iterator;
	typedef Iterator const_iterator;

	typedef typename Tr_::iterator_category iterator_category;

	typedef typename Tr_::difference_type difference_type;
	typedef size_t size_type;

	typedef typename Tr_::value_type value_type;
	typedef typename Tr_::pointer pointer;
	typedef typename Tr_::reference reference;

private:
	typedef std::basic_string<value_type> StlString_;

public:
	Leaf()
		: first(), second() {}

	Leaf(Iterator first_, Iterator second_)
		: first(first_), second(second_) {}

	template <class StringT>
	Leaf(const StringT& str_)
		: first(str_.begin()), second(str_.end()) {
	}

public:
	void TPL_CALL assign(Iterator first_, Iterator second_) {
		first = first_;
		second = second_;
	}

	template <class StringT>
	void TPL_CALL assign(const StringT& str_) {
		first = str_.begin();
		second = str_.end();
	}
	
	size_type TPL_CALL length() const {
		return std::distance(first, second);
	}

	size_type TPL_CALL size() const {
		return std::distance(first, second);
	}

	iterator TPL_CALL begin() const {
		return first;
	}

	iterator TPL_CALL end() const {
		return second;
	}

	StlString_ TPL_CALL stl_str() const {
		return StlString_(first, second);
	}
	
public:
	int TPL_CALL compare(const Leaf& b) const
		{return std::compare(first, second, b.begin(), b.end()); }

	int TPL_CALL compare(const std::TempString<value_type> b) const
		{return std::compare(first, second, b.begin(), b.end()); }

	int TPL_CALL compare(const value_type* b) const
		{return std::compare(first, second, b); }
};

template <class Iterator, class T2> __forceinline
	bool TPL_CALL operator==(const Leaf<Iterator>& a, const T2& b)
    {return (a.compare(b) == 0); }

template <class Iterator> __forceinline
    bool TPL_CALL operator==(const typename Leaf<Iterator>::value_type* a, const Leaf<Iterator>& b)
    {return (b.compare(a) == 0); }

template <class Iterator, class CharT, class Tr, class AllocT> __forceinline
    bool TPL_CALL operator==(const std::basic_string<CharT, Tr, AllocT>& a, const Leaf<Iterator>& b)
    {return (b.compare(a) == 0); }

template <class Iterator, class CharT, class AllocT> __forceinline
    bool TPL_CALL operator==(const std::vector<CharT, AllocT>& a, const Leaf<Iterator>& b)
    {return (b.compare(a) == 0); }

// -------------------------------------------------------------------------

template <class Iterator, class T2> __forceinline
    bool TPL_CALL operator!=(const Leaf<Iterator>& a, const T2& b)
    {return (a.compare(b) != 0); }

template <class Iterator> __forceinline
    bool TPL_CALL operator!=(const typename Leaf<Iterator>::value_type* a, const Leaf<Iterator>& b)
    {return (b.compare(a) != 0); }

template <class Iterator, class CharT, class Tr, class AllocT> __forceinline
    bool TPL_CALL operator!=(const std::basic_string<CharT, Tr, AllocT>& a, const Leaf<Iterator>& b)
    {return (b.compare(a) != 0); }

template <class Iterator, class CharT, class AllocT> __forceinline
    bool TPL_CALL operator!=(const std::vector<CharT, AllocT>& a, const Leaf<Iterator>& b)
    {return (b.compare(a) != 0); }

// -------------------------------------------------------------------------

template <class Iterator, class T2> __forceinline
    bool TPL_CALL operator<(const Leaf<Iterator>& a, const T2& b)
    {return (a.compare(b) < 0); }

template <class Iterator> __forceinline
    bool TPL_CALL operator<(const typename Leaf<Iterator>::value_type* a, const Leaf<Iterator>& b)
    {return (b.compare(a) > 0); }

template <class Iterator, class CharT, class Tr, class AllocT> __forceinline
    bool TPL_CALL operator<(const std::basic_string<CharT, Tr, AllocT>& a, const Leaf<Iterator>& b)
    {return (b.compare(a) > 0); }

template <class Iterator, class CharT, class AllocT> __forceinline
    bool TPL_CALL operator<(const std::vector<CharT, AllocT>& a, const Leaf<Iterator>& b)
    {return (b.compare(a) > 0); }

// -------------------------------------------------------------------------

template <class Iterator, class T2> __forceinline
    bool TPL_CALL operator>(const Leaf<Iterator>& a, const T2& b)
    {return (b < a); }

template <class Iterator> __forceinline
    bool TPL_CALL operator>(const typename Leaf<Iterator>::value_type* a, const Leaf<Iterator>& b)
    {return (b < a); }

template <class Iterator, class CharT, class Tr, class AllocT> __forceinline
    bool TPL_CALL operator>(const std::basic_string<CharT, Tr, AllocT>& a, const Leaf<Iterator>& b)
    {return (b < a); }

template <class Iterator, class CharT, class AllocT> __forceinline
    bool TPL_CALL operator>(const std::vector<CharT, AllocT>& a, const Leaf<Iterator>& b)
    {return (b < a); }

// -------------------------------------------------------------------------

template <class Iterator, class T2> __forceinline
	bool TPL_CALL operator<=(const Leaf<Iterator>& a, const T2& b)
	{return (!(b < a)); }

template <class Iterator> __forceinline
	bool TPL_CALL operator<=(const typename Leaf<Iterator>::value_type* a, const Leaf<Iterator>& b)
	{return (!(b < a)); }

template <class Iterator, class CharT, class Tr, class AllocT> __forceinline
	bool TPL_CALL operator<=(const std::basic_string<CharT, Tr, AllocT>& a, const Leaf<Iterator>& b)
	{return (!(b < a)); }

template <class Iterator, class CharT, class AllocT> __forceinline
	bool TPL_CALL operator<=(const std::vector<CharT, AllocT>& a, const Leaf<Iterator>& b)
	{return (!(b < a)); }

// -------------------------------------------------------------------------

template <class Iterator, class T2> __forceinline
	bool TPL_CALL operator>=(const Leaf<Iterator>& a, const T2& b)
	{return (!(a < b)); }

template <class Iterator> __forceinline
	bool TPL_CALL operator>=(const typename Leaf<Iterator>::value_type* a, const Leaf<Iterator>& b)
	{return (!(a < b)); }

template <class Iterator, class CharT, class Tr, class AllocT> __forceinline
	bool TPL_CALL operator>=(const std::basic_string<CharT, Tr, AllocT>& a, const Leaf<Iterator>& b)
	{return (!(a < b)); }

template <class Iterator, class CharT, class AllocT> __forceinline
	bool TPL_CALL operator>=(const std::vector<CharT, AllocT>& a, const Leaf<Iterator>& b)
	{return (!(a < b)); }

// -------------------------------------------------------------------------

#if defined(TPL_HAS_OSTREAM)

template <class CharT, class Tr, class Iterator>
inline std::basic_ostream<CharT, Tr>& 
TPL_CALL operator<<(std::basic_ostream<CharT, Tr>& os, const Leaf<Iterator>& v) {
	std::copy(v.begin(), v.end(), std::ostream_iterator<CharT>(os));
	return os;
}

#endif

// -------------------------------------------------------------------------
// Node

template <class Iterator, class TagT = DefaultTag>
class Node
{
private:
	typedef BasicMark<TagT, LeafAssign> LeafMarkT;
	typedef BasicMark<TagT, NodeAssign> NodeMarkT;

	typedef Leaf<Iterator> LeafT;
	typedef Node NodeT;

	typedef std::pair<const void*, const void*> ValueT;
	typedef ConsList<ValueT, false> ContainerT;

	ContainerT m_data;

public:
	template <class AllocT>
	void TPL_CALL insertLeaf(AllocT& alloc, const LeafMarkT& mark, Iterator first, Iterator last) {
		const LeafT* v = TPL_UNMANAGED_NEW(alloc, LeafT)(first, last);
		m_data.push_front(alloc, ValueT(&mark, v));
	}

	template <class AllocT>
	NodeT* TPL_CALL insertNode(AllocT& alloc, const NodeMarkT& mark) {
		NodeT* v = TPL_UNMANAGED_NEW(alloc, NodeT);
		m_data.push_front(alloc, ValueT(&mark, v));
		return v;
	}

public:
	template <class DataT>
	class Value
	{
	private:
		const ValueT& m_val;

	public:
		typedef const Mark<TagT>& key_type;
		typedef const DataT& data_type;
		typedef const Leaf<Iterator>& leaf_data;
		typedef const Node& node_data;

		Value(const ValueT& val) : m_val(val) {}
	
		key_type TPL_CALL key() const {
			return *(const Mark<TagT>*)m_val.first;
		}

		data_type TPL_CALL data() const {
			return *(const DataT*)m_val.second;
		}

		leaf_data TPL_CALL leaf() const {
			return *(const Leaf<Iterator>*)m_val.second;
		}

		node_data TPL_CALL node() const {
			return *(const Node*)m_val.second;
		}
	};

	template <class DataT>
	class Position
	{
	private:
		ContainerT::cons m_pos;

	public:
		typedef Value<DataT> value_type;
		typedef typename value_type::data_type data_type;
		typedef typename value_type::key_type key_type;
		typedef Node container_type;

		Position(ContainerT::cons pos) : m_pos(pos) {}

		Position TPL_CALL tl() const {
			return m_pos->tl();
		}

		value_type TPL_CALL hd() const {
			return m_pos->value;
		}

		data_type TPL_CALL operator*() const {
			return hd().data();
		}

		const Position* TPL_CALL operator->() const {
			return m_pos;
		}

		bool TPL_CALL operator!() const {
			return m_pos == NULL;
		}

		operator const void*() const {
			return m_pos;
		}

		operator bool() const {
			return m_pos != NULL;
		}
	};

	template <class ConsT>
	static size_t TPL_CALL length(ConsT hd_) {
		size_t len = 0;
		for (; hd_; hd_ = hd_.tl())
			++len;
		return len;
	}

public:
	struct Null {};
	typedef Value<Null> value_type;
	typedef Position<Null> cons;

	typedef Value<LeafT> leaf_value;
	typedef Position<LeafT> leaf_cons;
	typedef const LeafT& leaf_data;

	typedef Value<NodeT> node_value;
	typedef Position<NodeT> node_cons;
	typedef const NodeT& node_data;

private:
	struct DoSel
	{
		const void* m_mark;

		template <class MarkT>
		DoSel(const MarkT& mark) : m_mark(&mark) {}

		bool TPL_CALL operator()(const ValueT& val) {
			return val.first == m_mark;
		}
	};

public:
	template <class AllocT>
	leaf_cons TPL_CALL select(AllocT& alloc, const LeafMarkT& mark) const {
		DoSel cond(mark);
		return m_data.select(alloc, cond);
	}

	template <class AllocT>
	node_cons TPL_CALL select(AllocT& alloc, const NodeMarkT& mark) const {
		DoSel cond(mark);
		return m_data.select(alloc, cond);
	}

	cons TPL_CALL all() const {
		return m_data.data();
	}

public:
	leaf_cons TPL_CALL find(const LeafMarkT& mark) const {
		DoSel cond(mark);
		TPL_ASSERT(count_if(m_data, cond) <= 1);
		return m_data.find(cond);
	}

	node_cons TPL_CALL find(const NodeMarkT& mark) const {
		DoSel cond(mark);
		TPL_ASSERT(count_if(m_data, cond) <= 1);
		return m_data.find(cond);
	}

	leaf_data TPL_CALL operator[](const LeafMarkT& mark) const {
		leaf_cons it = find(mark);
		TPL_ASSERT(it);
		return it.hd().data();
	}

	node_data TPL_CALL operator[](const NodeMarkT& mark) const {
		node_cons it = find(mark);
		TPL_ASSERT(it);
		return it.hd().data();
	}

private:
	template <class OperaT>
	static void TPL_CALL _travel(cons it, OperaT& op) {
		if (it) {
			cons it2 = it.tl();
			_travel(it2, op);
			op(it);
		}
	}

public:
	template <class OperaT>
	void TPL_CALL travel(OperaT& op) const {
		_travel(all(), op);
	}
};

// -------------------------------------------------------------------------
// function print

template <class LogT, int delta = 2>
class NodePrint_
{
private:
	LogT& log_;
	int indent_;

public:
	NodePrint_(LogT& log, int indent)
		: log_(log), indent_(indent) {}

public:
	template <class ConsT>
	void TPL_CALL operator()(ConsT it)
	{
		typename ConsT::value_type val = it.hd();
		typename ConsT::key_type key_ = val.key(); 
		log_.put(indent_, ' ');
		log_.trace("<%s>", TPL_DBG_TAG(key_));
		if (key_.isNode())
		{
			indent_ += delta;
			log_.newline();
			val.node().travel(*this);
			indent_ -= delta;
			log_.put(indent_, ' ');
		}
		else
		{
			log_.printString(val.leaf().begin(), val.leaf().end());
		}
		log_.trace("</%s>\n", TPL_DBG_TAG(key_));
	}
};

template <class LogT, class Iterator, class TagT>
inline void TPL_CALL print(LogT& log_, const Node<Iterator, TagT>& node_, int indent_ = 0)
{
	NodePrint_<LogT> op(log_, indent_);
	node_.travel(op);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_MATCHRESULT_H */

