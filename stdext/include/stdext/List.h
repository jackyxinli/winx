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
// Module: stdext/List.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: List.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_LIST_H__
#define __STDEXT_LIST_H__

#ifndef __STDEXT_BASIC_H__
#include "Basic.h"
#endif

#ifndef __STDEXT_MEMORY_H__
#include "Memory.h"
#endif

#ifndef __STD_LIST_H__
#include "../std/list.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class List

template <class ValT, class AllocT = ScopeAlloc>
class List : public std::list< ValT, StlAlloc<ValT, AllocT> >
{
private:
	typedef StlAlloc<ValT, AllocT> _Alloc;
	typedef std::list<ValT, _Alloc> _Base;

	List(const List&);
	void operator=(const List&);

public:
    typedef typename _Base::size_type size_type;
    
	explicit List(AllocT& alloc)
		: _Base(alloc)
	{
	}

	List(AllocT& alloc, size_type count, const ValT& val = ValT())
		: _Base(count, val, alloc)
	{
	}

	template <class Iterator>
	List(AllocT& alloc, Iterator aFirst, Iterator aLast)
		: _Base(aFirst, aLast, alloc)
	{
	}
};

// -------------------------------------------------------------------------
// class Slist

template <class ValT, class AllocT = ScopeAlloc>
class Slist : public stdext::slist< ValT, StlAlloc<ValT, AllocT> >
{
private:
	typedef StlAlloc<ValT, AllocT> _Alloc;
	typedef stdext::slist<ValT, _Alloc> _Base;

	Slist(const Slist&);
	void operator=(const Slist&);

public:
    typedef typename _Base::size_type size_type;
    
	explicit Slist(AllocT& alloc)
		: _Base(alloc)
	{
	}

	Slist(AllocT& alloc, size_type count, const ValT& val = ValT())
		: _Base(count, val, alloc)
	{
	}

	template <class Iterator>
	Slist(AllocT& alloc, Iterator aFirst, Iterator aLast)
		: _Base(aFirst, aLast, alloc)
	{
	}
};

// -------------------------------------------------------------------------
// class DclList: Doubly-circularly-linked list

class _DclListNodeBase
{
protected:
	_DclListNodeBase* m_prev;
	_DclListNodeBase* m_next;

private:
	_DclListNodeBase(const _DclListNodeBase&);
	void operator=(const _DclListNodeBase&);

public:
	enum InsertFront { insertAtFront = 0 };
	enum InsertBack { insertAtBack = 1 };

public:
	_DclListNodeBase() {
		m_prev = m_next = this;
	}
	_DclListNodeBase(_DclListNodeBase& head, InsertFront)
		: m_prev(&head), m_next(head.m_next)
	{
		head.m_next->m_prev = this;
		head.m_next = this;
	}
	_DclListNodeBase(_DclListNodeBase& head, InsertBack)
		: m_next(&head), m_prev(head.m_prev)
	{
		head.m_prev->m_next = this;
		head.m_prev = this;
	}

public:
	void winx_call __insertMeFront(_DclListNodeBase& head)
	{
		WINX_ASSERT(empty());
		m_prev = &head;
		m_next = head.m_next;
		head.m_next->m_prev = this;
		head.m_next = this;
	}

	void winx_call __insertMeBack(_DclListNodeBase& head)
	{
		WINX_ASSERT(empty());
		m_next = &head;
		m_prev = head.m_prev;
		head.m_prev->m_next = this;
		head.m_prev = this;
	}

public:
	void winx_call erase()
	{
		m_next->m_prev = m_prev;
		m_prev->m_next = m_next;
		m_prev = m_next = this;
	}

	void winx_call pop_front()
	{
		WINX_ASSERT(!empty());

		_DclListNodeBase* node = m_next;
		node->erase();
	}

	void winx_call pop_back()
	{
		WINX_ASSERT(!empty());

		_DclListNodeBase* node = m_prev;
		node->erase();
	}

	bool winx_call empty() const
	{
		WINX_ASSERT(m_next == this || m_next != m_prev);
		return m_next == this;
	}
};

template <class ValT>
class DclListNode : public _DclListNodeBase
{
public:
	typedef ValT value_type;

public:
	DclListNode() {}
	DclListNode(_DclListNodeBase& head, InsertFront)
		: _DclListNodeBase(head, insertAtFront)
	{
	}
	DclListNode(_DclListNodeBase& head, InsertBack)
		: _DclListNodeBase(head, insertAtBack)
	{
	}

public:
	ValT* winx_call next() { return (ValT*)m_next; }
	const ValT* winx_call next() const { return (const ValT*)m_next; }

	ValT* winx_call prev() { return (ValT*)m_prev; }
	const ValT* winx_call prev() const { return (const ValT*)m_prev; }

	ValT& winx_call data() { return *(ValT*)this; }
	const ValT& winx_call data() const { return *(const ValT*)this; }
};

template <class ValT, class ReferenceT, class NodePtrT>
class _DclIter : public std::iterator<std::bidirectional_iterator_tag, ValT>
{
private:
	typedef _DclIter _Myt;

	NodePtrT m_node;

public:
	_DclIter(NodePtrT node) : m_node(node) {}

	template <class RefT2, class NPtrT2>
	_DclIter(const _DclIter<ValT, RefT2, NPtrT2>& it) : m_node(it.__data()) {}

	NodePtrT winx_call __data() const { return m_node; }
	ReferenceT winx_call operator*() const { return m_node->data(); }
	
	bool winx_call operator==(const _Myt& it) const { return m_node == it.__data(); }
	bool winx_call operator!=(const _Myt& it) const { return m_node != it.__data(); }

	_Myt& winx_call operator++() { m_node = m_node->next(); return *this; }
	_Myt& winx_call operator--() { m_node = m_node->prev(); return *this; }

	_Myt winx_call operator++(int)
	{ 
		_Myt it = *this;
		operator++();
		return it;
	}

	_Myt winx_call operator--(int)
	{ 
		_Myt it = *this;
		operator--();
		return it;
	}
};

template <class ValT>
class DclList : public _DclListNodeBase
{
public:
	typedef ValT value_type;
	typedef ValT& reference;
	typedef const ValT& const_reference;

public:
	void winx_call push_front(DclListNode<ValT>* node)
	{
		node->__insertMeFront(*this);
	}
	
	void winx_call push_back(DclListNode<ValT>* node)
	{
		node->__insertMeBack(*this);
	}

public:
	ValT& winx_call front()
	{
		WINX_ASSERT(!empty());
		return *(ValT*)m_next;
	}
	const ValT& winx_call front() const
	{
		WINX_ASSERT(!empty());
		return *(const ValT*)m_next;
	}

	ValT& winx_call back()
	{
		WINX_ASSERT(!empty());
		return *(ValT*)m_prev;
	}
	const ValT& winx_call back() const
	{
		WINX_ASSERT(!empty());
		return *(const ValT*)m_prev;
	}

public:
	ValT* winx_call first() { return (ValT*)m_next; }
	const ValT* winx_call first() const { return (const ValT*)m_next; }

	bool winx_call done(const ValT* node) const
	{
		return static_cast<const _DclListNodeBase*>(node) == this;
	}

private:
	typedef DclListNode<ValT> NodeT;
	
public:
	typedef _DclIter<ValT, ValT&, NodeT*> iterator;
	typedef _DclIter<ValT, const ValT&, const NodeT*> const_iterator;

	iterator winx_call begin() { return iterator(first()); }
	const_iterator winx_call begin() const { return const_iterator(first()); }

	iterator winx_call end() { return iterator((ValT*)this); }
	const_iterator winx_call end() const { return const_iterator((const ValT*)this); }
};

// -------------------------------------------------------------------------
// class TestList

template <class LogT>
class TestList : public TestCase
{
	WINX_TEST_SUITE(TestList);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	class Obj
	{
	private:
		int m_val;
	public:
		Obj(int arg = 0) {
			m_val = arg;
		}
	};

	void testBasic(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		std::List<Obj> coll(alloc);
		coll.push_back(1);
		coll.push_back(2);
		coll.push_back(4);
	}
};

// -------------------------------------------------------------------------
// class TestSlist

template <class LogT>
class TestSlist : public TestCase
{
	WINX_TEST_SUITE(TestSlist);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		std::Slist<int> coll(alloc);
		coll.push_front(1);
		coll.push_front(2);
		coll.push_front(4);
		for (std::Slist<int>::const_iterator it = coll.begin(); it != coll.end(); ++it)
		{
			log.print(*it).newline();
		}
	}
};

// -------------------------------------------------------------------------
// class TestDclList

template <class LogT>
class TestDclList : public TestCase
{
	WINX_TEST_SUITE(TestDclList);
		WINX_TEST(testBasic);
		WINX_TEST(testInsertFront);
	WINX_TEST_SUITE_END();

public:
	class Obj : public DclListNode<Obj>
	{
	public:
		int m_val;
	public:
		Obj(int arg)
		{
			m_val = arg;
		}
		Obj(int arg, DclList<Obj>& lst, bool fInsertBack)
			: DclListNode<Obj>(lst, Obj::insertAtBack)
		{
			m_val = arg;
		}
		Obj(int arg, DclList<Obj>& lst)
			: DclListNode<Obj>(lst, Obj::insertAtFront)
		{
			m_val = arg;
		}
	};

	template <class ListType>
	void print(LogT& log, ListType& coll)
	{
		for (typename ListType::const_iterator it = coll.begin(); it != coll.end(); ++it)
		{
			const Obj& val = *it;
			log.print(val.m_val).newline();
		}
	}

	template <class ListType>
	void print2(LogT& log, const ListType& coll)
	{
		for (const Obj* o = coll.first(); !coll.done(o); o = o->next())
		{
			log.print(o->m_val).newline();
		}
	}

	void testBasic(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		
		std::DclList<Obj> coll;

		coll.push_back(STD_NEW(alloc, Obj)(1));
		STD_NEW(alloc, Obj)(2, coll, true);
		STD_NEW(alloc, Obj)(2, coll, true);
		coll.push_back(STD_NEW(alloc, Obj)(4));

		print(log, coll);
	}

	void testInsertFront(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		
		std::DclList<Obj> coll;

		STD_NEW(alloc, Obj)(1, coll);
		coll.push_front(STD_NEW(alloc, Obj)(2));
		coll.push_front(STD_NEW(alloc, Obj)(2));
		STD_NEW(alloc, Obj)(4, coll);
		
		print2(log, coll);
	}
};

// -------------------------------------------------------------------------
// $Log: List.h,v $

__NS_STD_END

#endif /* __STDEXT_LIST_H__ */
