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
// Module: tpl/regex/Document.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_DOCUMENT_H
#define TPL_REGEX_DOCUMENT_H

#ifndef TPL_REGEX_BASIC_H
#include "../Basic.h"
#endif

#ifndef TPL_REGEX_DETAIL_CONS_H
#include "../detail/Cons.h"
#endif

#ifndef TPL_REGEX_RESULT_MARK_H
#include "Mark.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// Node

template <class LeafT, class TagCharT = char>
class Node
{
private:
	typedef Mark<LeafT, TagCharT> LeafMarkT;
	typedef Mark<TagNodeType, TagCharT> NodeMarkT;

	typedef std::pair<const void*, const void*> ValueT;
	typedef ConsList<ValueT, false> ContainerT;

	ContainerT m_data;

public:
	template <class AllocT>
	void TPL_CALL insertLeaf(AllocT& alloc, const LeafMarkT& mark, const LeafT& val) {
		const LeafT* v = TPL_UNMANAGED_NEW(alloc, LeafT)(val);
		m_data.push_front(alloc, ValueT(&mark, v));
	}

	template <class AllocT>
	Node* TPL_CALL insertNode(AllocT& alloc, const NodeMarkT& mark) {
		Node* v = TPL_UNMANAGED_NEW(alloc, Node);
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
		typedef Mark<DataT, TagCharT> mark_type;
		typedef const mark_type& key_type;
		typedef const DataT& data_type;
		typedef const LeafT& leaf_data;
		typedef const Node& node_data;

		Value(const ValueT& val) : m_val(val) {}
	
		key_type TPL_CALL key() const {
			return *(const mark_type*)m_val.first;
		}

		data_type TPL_CALL data() const {
			return *(const DataT*)m_val.second;
		}

		leaf_data TPL_CALL leaf() const {
			return *(const LeafT*)m_val.second;
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

	typedef Value<Node> node_value;
	typedef Position<Node> node_cons;
	typedef const Node& node_data;

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
			const bool bPrev = (it2 != 0);
			if (bPrev)
				_travel(it2, op);
			op(it, bPrev);
		}
	}

public:
	template <class OperaT>
	void TPL_CALL travel(OperaT& op) const {
		_travel(all(), op);
	}
};

// -------------------------------------------------------------------------
// Document

template <class LeafT, class AllocT, class TagCharT = char>
class Document : public Node<LeafT, TagCharT>
{
private:
	AllocT& m_alloc;
	
public:
	explicit Document(AllocT& alloc)
		: m_alloc(alloc) {}

public:
	typedef AllocT alloc_type;

	alloc_type& TPL_CALL get_alloc() const {
		return m_alloc;
	}
};

// =========================================================================
// function dump

template <class LogT, int delta = 2>
class NodeDump_
{
private:
	LogT& log_;
	int indent_;

public:
	NodeDump_(LogT& log, int indent)
		: log_(log), indent_(indent) {}

public:
	template <class ConsT>
	void TPL_CALL operator()(ConsT it, bool bPrev)
	{
		typename ConsT::value_type val = it.hd();
		typename ConsT::key_type key_ = val.key(); 
		log_.put(indent_, ' ')
			.print('<')
			.print(key_.tag())
			.print('>');
		if (key_.isNode())
		{
			log_.newline();
			indent_ += delta;
			val.node().travel(*this);
			indent_ -= delta;
			log_.put(indent_, ' ');
		}
		else
		{
			log_.printString(val.leaf().begin(), val.leaf().end());
		}
		log_.print('<')
			.print('/')
			.print(key_.tag())
			.print('>')
			.newline();
	}
};

template <class LogT, class LeafT, class TagCharT>
inline void TPL_CALL dump(LogT& log_, const Node<LeafT, TagCharT>& node_, int indent_ = 0)
{
	NodeDump_<LogT> op(log_, indent_);
	node_.travel(op);
}

// =========================================================================
// function json_print

template <class CharT>
struct JsonEscape
{
private:
	enum { ESCAPE_MAX = 64 };

	static CharT m_escape[ESCAPE_MAX];
	
	static void TPL_CALL _insert(CharT c, CharT alt) {
		TPL_ASSERT(c >= 0 && c < ESCAPE_MAX);
		m_escape[(size_t)c] = alt;
	}
	
public:
	JsonEscape() {
		_insert('\t', 't');
		_insert('\r', 'r');
		_insert('\n', 'n');
		_insert('\"', '\"');
	}
	
	struct Escape {
		CharT TPL_CALL operator()(unsigned c) const {
			return c < ESCAPE_MAX ? m_escape[c] : 0;
		}
	};
};

template <class CharT>
CharT JsonEscape<CharT>::m_escape[ESCAPE_MAX];

// -------------------------------------------------------------------------
// json_print_string

template <class LogT, class Iterator>
inline void TPL_CALL json_print_string(LogT& log_, Iterator first_, Iterator last_)
{
	typedef typename std::iterator_traits<Iterator>::value_type CharT;
	
	typedef JsonEscape<CharT> Json;
	static const Json json = Json();

	typedef typename Json::Escape Escape;	
	const Escape escape = Escape();
	
	Iterator it;
	for (;;)
	{
		it = std::find_if(first_, last_, escape);
		if (it != first_)
			log_.printString(first_, it);
		if (it == last_)
			break;
		log_.print('\\')
			.print(escape(*it));
		first_ = ++it;
	}
}

template <class LogT, class StringT>
inline void TPL_CALL json_print_string(LogT& log_, const StringT& str_) {
	json_print_string(log_, str_.begin(), str_.end());
}

// -------------------------------------------------------------------------

template <class LogT, int delta = 2>
class NodeJsonPrint_
{
private:
	LogT& log_;
	int indent_;

public:
	NodeJsonPrint_(LogT& log, int indent)
		: log_(log), indent_(indent) {}

public:
	template <class ConsT>
	void TPL_CALL operator()(ConsT it, bool bPrev)
	{
		typename ConsT::value_type val = it.hd();
		typename ConsT::key_type key_ = val.key();
		if (bPrev)
			log_.print(',');
		log_.newline()
			.put(indent_, ' ')
			.print('\"')
			.print(key_.tag())
			.print("\": ");
		if (key_.isNode())
		{
			print(val.node());
		}
		else
		{
			log_.print('\"');
			json_print_string(log_, val.leaf());
			log_.print('\"');
		}
	}
	
	template <class LeafT, class TagCharT>
	void TPL_CALL print(const Node<LeafT, TagCharT>& node_)
	{
		log_.print('{');
		indent_ += delta;
		node_.travel(*this);
		indent_ -= delta;
		log_.newline()
			.put(indent_, ' ')
			.put('}');
	}
};

template <class LogT, class LeafT, class TagCharT>
inline void TPL_CALL json_print(LogT& log_, const Node<LeafT, TagCharT>& node_, int indent_ = 0)
{
	NodeJsonPrint_<LogT> op(log_, indent_);
	op.print(node_);
	log_.newline();
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_DOCUMENT_H */

