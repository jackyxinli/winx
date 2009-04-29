//
//  boost/memory/container/dcl_list.hpp (*)
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_CONTAINER_DCL_LIST_HPP
#define BOOST_MEMORY_CONTAINER_DCL_LIST_HPP

#ifndef BOOST_MEMORY_BASIC_HPP
#include "../basic.hpp"
#endif

#ifndef _ITERATOR_
#include <iterator>
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class dcl_list_node

class dcl_list_node_base_
{
protected:
	dcl_list_node_base_* m_prev;
	dcl_list_node_base_* m_next;

private:
	dcl_list_node_base_(const dcl_list_node_base_&);
	void operator=(const dcl_list_node_base_&);

public:
	enum InsertFront { insertAtFront = 0 };
	enum InsertBack { insertAtBack = 1 };

public:
	dcl_list_node_base_() {
		m_prev = m_next = this;
	}
	
	dcl_list_node_base_(dcl_list_node_base_& head, InsertFront)
		: m_prev(&head), m_next(head.m_next)
	{
		head.m_next->m_prev = this;
		head.m_next = this;
	}

	dcl_list_node_base_(dcl_list_node_base_& head, InsertBack)
		: m_prev(head.m_prev), m_next(&head)
	{
		head.m_prev->m_next = this;
		head.m_prev = this;
	}

public:
	void BOOST_MEMORY_CALL insert_me_front_(dcl_list_node_base_& head)
	{
		m_prev = &head;
		m_next = head.m_next;
		head.m_next->m_prev = this;
		head.m_next = this;
	}

	void BOOST_MEMORY_CALL insert_me_back_(dcl_list_node_base_& head)
	{
		m_next = &head;
		m_prev = head.m_prev;
		head.m_prev->m_next = this;
		head.m_prev = this;
	}

public:
	bool BOOST_MEMORY_CALL empty() const
	{
		return m_next == this;
	}

	void BOOST_MEMORY_CALL clear()
	{
		m_next = m_prev = this;
	}

	void BOOST_MEMORY_CALL erase()
	{
		m_next->m_prev = m_prev;
		m_prev->m_next = m_next;
		m_prev = m_next = this;
	}

	void BOOST_MEMORY_CALL pop_front()
	{
		BOOST_MEMORY_ASSERT(!empty());
		static_cast<dcl_list_node_base_*>(m_next)->erase();
	}

	void BOOST_MEMORY_CALL pop_back()
	{
		BOOST_MEMORY_ASSERT(!empty());
		static_cast<dcl_list_node_base_*>(m_prev)->erase();
	}
};

template <class NodeT>
class dcl_list_node : public dcl_list_node_base_
{
public:
	typedef NodeT value_type;

public:
	dcl_list_node() {}
	dcl_list_node(dcl_list_node_base_& head, InsertFront)
		: dcl_list_node_base_(head, insertAtFront)
	{
	}
	dcl_list_node(dcl_list_node_base_& head, InsertBack)
		: dcl_list_node_base_(head, insertAtBack)
	{
	}

public:
	NodeT* BOOST_MEMORY_CALL next() { return (NodeT*)m_next; }
	const NodeT* BOOST_MEMORY_CALL next() const { return (const NodeT*)m_next; }

	NodeT* BOOST_MEMORY_CALL prev() { return (NodeT*)m_prev; }
	const NodeT* BOOST_MEMORY_CALL prev() const { return (const NodeT*)m_prev; }

	NodeT& BOOST_MEMORY_CALL data() { return *(NodeT*)this; }
	const NodeT& BOOST_MEMORY_CALL data() const { return *(const NodeT*)this; }
};

// -------------------------------------------------------------------------
// class dcl_list_iterator_

template <class NodeT, class ReferenceT, class NodePtrT>
class dcl_list_iterator_ : public std::iterator<std::bidirectional_iterator_tag, NodeT>
{
private:
	typedef dcl_list_iterator_ Myt_;

	NodePtrT m_node;

public:
	dcl_list_iterator_(NodePtrT node) : m_node(node) {}

	template <class RefT2, class NPtrT2>
	dcl_list_iterator_(const dcl_list_iterator_<NodeT, RefT2, NPtrT2>& it) : m_node(it.inner_data__()) {}

	NodePtrT BOOST_MEMORY_CALL inner_data__() const { return m_node; }
	ReferenceT BOOST_MEMORY_CALL operator*() const { return m_node->data(); }

	bool BOOST_MEMORY_CALL operator==(const Myt_& it) const { return m_node == it.inner_data__(); }
	bool BOOST_MEMORY_CALL operator!=(const Myt_& it) const { return m_node != it.inner_data__(); }

	Myt_& BOOST_MEMORY_CALL operator++() { m_node = m_node->next(); return *this; }
	Myt_& BOOST_MEMORY_CALL operator--() { m_node = m_node->prev(); return *this; }

	Myt_ BOOST_MEMORY_CALL operator++(int)
	{ 
		Myt_ it = *this;
		operator++();
		return it;
	}

	Myt_ BOOST_MEMORY_CALL operator--(int)
	{ 
		Myt_ it = *this;
		operator--();
		return it;
	}
};

// -------------------------------------------------------------------------
// class dcl_list: Doubly-circularly-linked list

template <class NodeT>
class dcl_list : public dcl_list_node_base_
{
public:
	typedef NodeT value_type;
	typedef NodeT node_type;
	typedef NodeT& reference;
	typedef const NodeT& const_reference;

public:
	void BOOST_MEMORY_CALL push_front(dcl_list_node<NodeT>* node)
	{
		node->insert_me_front_(*this);
	}

	void BOOST_MEMORY_CALL push_back(dcl_list_node<NodeT>* node)
	{
		node->insert_me_back_(*this);
	}

public:
	NodeT& BOOST_MEMORY_CALL front()
	{
		BOOST_MEMORY_ASSERT(!empty());
		return *(NodeT*)m_next;
	}
	const NodeT& BOOST_MEMORY_CALL front() const
	{
		BOOST_MEMORY_ASSERT(!empty());
		return *(const NodeT*)m_next;
	}

	NodeT& BOOST_MEMORY_CALL back()
	{
		BOOST_MEMORY_ASSERT(!empty());
		return *(NodeT*)m_prev;
	}
	const NodeT& BOOST_MEMORY_CALL back() const
	{
		BOOST_MEMORY_ASSERT(!empty());
		return *(const NodeT*)m_prev;
	}

public:
	NodeT* BOOST_MEMORY_CALL first() { return (NodeT*)m_next; }
	const NodeT* BOOST_MEMORY_CALL first() const { return (const NodeT*)m_next; }

	bool BOOST_MEMORY_CALL done(const NodeT* node) const
	{
		return static_cast<const dcl_list_node_base_*>(node) == this;
	}

private:
	typedef dcl_list_node<NodeT> BaseNodeT;

public:
	typedef dcl_list_iterator_<BaseNodeT, BaseNodeT&, BaseNodeT*> iterator;
	typedef dcl_list_iterator_<BaseNodeT, const BaseNodeT&, const BaseNodeT*> const_iterator;

	iterator BOOST_MEMORY_CALL begin() { return iterator(first()); }
	const_iterator BOOST_MEMORY_CALL begin() const { return const_iterator(first()); }

	iterator BOOST_MEMORY_CALL end() { return iterator((NodeT*)this); }
	const_iterator BOOST_MEMORY_CALL end() const { return const_iterator((const NodeT*)this); }
};

// -------------------------------------------------------------------------
//	$Log: $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_CONTAINER_DCL_LIST_HPP */
