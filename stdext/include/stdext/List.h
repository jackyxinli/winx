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
#ifndef STDEXT_LIST_H
#define STDEXT_LIST_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "Memory.h"
#endif

#ifndef STD_LIST_H
#include "../std/list.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class List

template <class ValT, class AllocT = ScopedAlloc>
class List : public std::list< ValT, StlAlloc<ValT, AllocT> >
{
private:
	typedef StlAlloc<ValT, AllocT> StlAllocT;
	typedef std::list<ValT, StlAllocT> Base;

	List(const List&);
	void operator=(const List&);

public:
    typedef typename Base::size_type size_type;
    
	explicit List(AllocT& alloc)
		: Base(alloc)
	{
	}

	List(AllocT& alloc, size_type count, const ValT& val = ValT())
		: Base(count, val, alloc)
	{
	}

	template <class Iterator>
	List(AllocT& alloc, Iterator aFirst, Iterator aLast)
		: Base(aFirst, aLast, alloc)
	{
	}

	void winx_call copy(const Base& from) {
		Base::operator=(from);
	}
};

// -------------------------------------------------------------------------
// class Slist

#ifndef _WINX_NO_SLIST

template <class ValT, class AllocT = ScopedAlloc>
class Slist : public stdext::slist< ValT, StlAlloc<ValT, AllocT> >
{
private:
	typedef StlAlloc<ValT, AllocT> StlAllocT;
	typedef stdext::slist<ValT, StlAllocT> Base;

	Slist(const Slist&);
	void operator=(const Slist&);

public:
    typedef typename Base::size_type size_type;
    
	explicit Slist(AllocT& alloc)
		: Base(alloc)
	{
	}

	Slist(AllocT& alloc, size_type count, const ValT& val = ValT())
		: Base(count, val, alloc)
	{
	}

	template <class Iterator>
	Slist(AllocT& alloc, Iterator aFirst, Iterator aLast)
		: Base(aFirst, aLast, alloc)
	{
	}

	void winx_call copy(const Base& from) {
		Base::operator=(from);
	}
};

#endif // _WINX_NO_SLIST

// -------------------------------------------------------------------------
// class DclList: Doubly-circularly-linked list

class DclListNodeBase_
{
protected:
	DclListNodeBase_* m_prev;
	DclListNodeBase_* m_next;

private:
	DclListNodeBase_(const DclListNodeBase_&);
	void operator=(const DclListNodeBase_&);

public:
	enum InsertFront { insertAtFront = 0 };
	enum InsertBack { insertAtBack = 1 };

public:
	DclListNodeBase_() {
		m_prev = m_next = this;
	}
	DclListNodeBase_(DclListNodeBase_& head, InsertFront)
		: m_prev(&head), m_next(head.m_next)
	{
		head.m_next->m_prev = this;
		head.m_next = this;
	}
	DclListNodeBase_(DclListNodeBase_& head, InsertBack)
		: m_prev(head.m_prev), m_next(&head)
	{
		head.m_prev->m_next = this;
		head.m_prev = this;
	}

public:
	void winx_call _insertMeFront(DclListNodeBase_& head)
	{
		WINX_ASSERT(empty());
		m_prev = &head;
		m_next = head.m_next;
		head.m_next->m_prev = this;
		head.m_next = this;
	}

	void winx_call _insertMeBack(DclListNodeBase_& head)
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

		DclListNodeBase_* node = m_next;
		node->erase();
	}

	void winx_call pop_back()
	{
		WINX_ASSERT(!empty());

		DclListNodeBase_* node = m_prev;
		node->erase();
	}

	bool winx_call empty() const
	{
		WINX_ASSERT(m_next == this || m_next != m_prev);
		return m_next == this;
	}
};

template <class ValT>
class DclListNode : public DclListNodeBase_
{
public:
	typedef ValT value_type;

public:
	DclListNode() {}
	DclListNode(DclListNodeBase_& head, InsertFront)
		: DclListNodeBase_(head, insertAtFront)
	{
	}
	DclListNode(DclListNodeBase_& head, InsertBack)
		: DclListNodeBase_(head, insertAtBack)
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
class DclIter_ : public std::iterator<std::bidirectional_iterator_tag, ValT>
{
private:
	typedef DclIter_ Myt_;

	NodePtrT m_node;

public:
	DclIter_(NodePtrT node) : m_node(node) {}

	template <class RefT2, class NPtrT2>
	DclIter_(const DclIter_<ValT, RefT2, NPtrT2>& it) : m_node(it.__data()) {}

	NodePtrT winx_call __data() const { return m_node; }
	ReferenceT winx_call operator*() const { return m_node->data(); }
	
	bool winx_call operator==(const Myt_& it) const { return m_node == it.__data(); }
	bool winx_call operator!=(const Myt_& it) const { return m_node != it.__data(); }

	Myt_& winx_call operator++() { m_node = m_node->next(); return *this; }
	Myt_& winx_call operator--() { m_node = m_node->prev(); return *this; }

	Myt_ winx_call operator++(int)
	{ 
		Myt_ it = *this;
		operator++();
		return it;
	}

	Myt_ winx_call operator--(int)
	{ 
		Myt_ it = *this;
		operator--();
		return it;
	}
};

template <class ValT>
class DclList : public DclListNodeBase_
{
public:
	typedef ValT value_type;
	typedef ValT& reference;
	typedef const ValT& const_reference;

public:
	void winx_call push_front(DclListNode<ValT>* node)
	{
		node->_insertMeFront(*this);
	}
	
	void winx_call push_back(DclListNode<ValT>* node)
	{
		node->_insertMeBack(*this);
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
		return static_cast<const DclListNodeBase_*>(node) == this;
	}

private:
	typedef DclListNode<ValT> NodeT;
	
public:
	typedef DclIter_<ValT, ValT&, NodeT*> iterator;
	typedef DclIter_<ValT, const ValT&, const NodeT*> const_iterator;

	iterator winx_call begin() { return iterator(first()); }
	const_iterator winx_call begin() const { return const_iterator(first()); }

	iterator winx_call end() { return iterator((ValT*)this); }
	const_iterator winx_call end() const { return const_iterator((const ValT*)this); }
};

// -------------------------------------------------------------------------
// class TestList

#if defined(STD_UNITTEST)

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
		std::ScopedAlloc alloc(recycle);
		std::List<Obj> coll(alloc);
		coll.push_back(1);
		coll.push_back(2);
		coll.push_back(4);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// class TestSlist

#if defined(STD_UNITTEST)

#ifndef _WINX_NO_SLIST

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
		std::ScopedAlloc alloc(recycle);
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

#endif // _WINX_NO_SLIST

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// class TestDclList

#if defined(STD_UNITTEST)

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
		std::ScopedAlloc alloc(recycle);
		
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
		std::ScopedAlloc alloc(recycle);
		
		std::DclList<Obj> coll;

		STD_NEW(alloc, Obj)(1, coll);
		coll.push_front(STD_NEW(alloc, Obj)(2));
		coll.push_front(STD_NEW(alloc, Obj)(2));
		STD_NEW(alloc, Obj)(4, coll);
		
		print2(log, coll);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: List.h,v $

NS_STDEXT_END

#endif /* STDEXT_LIST_H */
