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

#if !defined(_STRING_) && !defined(_STRING)
#include <string>
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// LeafMatchResult

template <class Iterator>
class LeafMatchResult
{
public:
	Iterator const first;
	Iterator const second;

public:
	typedef ptrdiff_t difference_type;

	LeafMatchResult(Iterator first_, Iterator second_)
		: first(first_), second(second_) {}

	difference_type TPL_CALL length() const {
		return std::distance(first, second);
	}

	std::string TPL_CALL stl_str() const {
		return std::string(first, second);
	}
};

// -------------------------------------------------------------------------
// NodeMatchResult

template <class Iterator, class TagT = DefaultTag>
class NodeMatchResult
{
private:
	typedef BasicMark<TagT, LeafAssign> LeafMarkT;
	typedef BasicMark<TagT, NodeAssign> NodeMarkT;

	typedef LeafMatchResult<Iterator> LeafMatchResultT;
	typedef NodeMatchResult NodeMatchResultT;

	typedef std::pair<const void*, const void*> ValueT;
	typedef ConsList<ValueT, false> ContainerT;

	ContainerT m_data;

public:
	template <class AllocT>
	void TPL_CALL insertLeaf(AllocT& alloc, const LeafMarkT& mark, Iterator first, Iterator last) {
		const LeafMatchResultT* v = TPL_UNMANAGED_NEW(alloc, LeafMatchResultT)(first, last);
		m_data.push_front(alloc, ValueT(&mark, v));
	}

	template <class AllocT>
	NodeMatchResultT* TPL_CALL insertNode(AllocT& alloc, const NodeMarkT& mark) {
		NodeMatchResultT* v = TPL_UNMANAGED_NEW(alloc, NodeMatchResultT);
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

		Value(const ValueT& val) : m_val(val) {}
	
		key_type TPL_CALL key() const {
			return *(const Mark<TagT>*)m_val.first;
		}

		data_type TPL_CALL data() const {
			return *(const DataT*)m_val.second;
		}

		const LeafMatchResult<Iterator>& TPL_CALL leaf() const {
			return *(const LeafMatchResult<Iterator>*)m_val.second;
		}

		const NodeMatchResult& TPL_CALL node() const {
			return *(const NodeMatchResult*)m_val.second;
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

public:
	struct Null {};
	typedef Value<Null> value_type;
	typedef Position<Null> cons;

	typedef Value<LeafMatchResultT> leaf_value;
	typedef Position<LeafMatchResultT> leaf_cons;
	typedef const LeafMatchResultT& leaf_data;

	typedef Value<NodeMatchResultT> node_value;
	typedef Position<NodeMatchResultT> node_cons;
	typedef const NodeMatchResultT& node_data;

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
		return m_data.find(cond);
	}

	node_cons TPL_CALL find(const NodeMarkT& mark) const {
		DoSel cond(mark);
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
};

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_MATCHRESULT_H */
