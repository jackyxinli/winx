//
//  boost/lockfree/stack.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/lockfree/todo.htm for documentation.
//
#ifndef BOOST_LOCKFREE_STACK_HPP
#define BOOST_LOCKFREE_STACK_HPP

#ifndef BOOST_DETAIL_DEBUG_HPP
#include <boost/detail/debug.hpp>
#endif

#ifndef BOOST_DETAIL_WINAPI_WINBASE_H
#include <boost/detail/winapi/winbase.h>
#endif

// -------------------------------------------------------------------------

#ifndef NS_BOOST_LOCKFREE_BEGIN
#define NS_BOOST_LOCKFREE_BEGIN		namespace boost { namespace lockfree {
#define NS_BOOST_LOCKFREE_END		} }
#define NS_BOOST_LOCKFREE			boost::lockfree
#endif

#ifndef BOOST_LOCKFREE_CALL
#define BOOST_LOCKFREE_CALL
#endif

NS_BOOST_LOCKFREE_BEGIN

// -------------------------------------------------------------------------
// class stack

class stack
{
private:
	stack(const stack&);
	void operator=(const stack&);

public:
	class node
	{
	private:
		node* m_prev;
		friend class stack;
	
	public:
		node* prev() const { return m_prev; }
	};

private:
	node* m_top;

public:
	stack() : m_top(NULL)
	{
	}

	void BOOST_LOCKFREE_CALL push(node* val)
	{
		node* the_top = m_top;
		for (;;)
		{
			val->m_prev = the_top;
			node* the_top2 = (node*)
				InterlockedCompareExchangePointer((PVOID*)&m_top, val, the_top);
			if (the_top2 == the_top)
				return;
			the_top = the_top2;
		}
	}

	node* BOOST_LOCKFREE_CALL clear()
	{
		return (node*)InterlockedExchangePointer((PVOID*)&m_top, NULL);
	}

	node* BOOST_LOCKFREE_CALL pop()
	{
		node* the_top = m_top;
		for (;;)
		{
			if (the_top == NULL)
				return NULL;
			node* the_top2 = (node*)
				InterlockedCompareExchangePointer((PVOID*)&m_top, the_top->m_prev, the_top);
			if (the_top2 == the_top)
				return the_top;
			the_top = the_top2;
		}
	}
};

// -------------------------------------------------------------------------
// class typed_stack

template <class Type>
class typed_stack
{
private:
	stack m_impl;

public:
	void BOOST_LOCKFREE_CALL push(Type* val)
	{
		m_impl.push(val);
	}

	Type* BOOST_LOCKFREE_CALL clear()
	{
		return static_cast<Type*>(m_impl.clear());
	}

	Type* BOOST_LOCKFREE_CALL pop()
	{
		return static_cast<Type*>(m_impl.pop());
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_BOOST_LOCKFREE_END

#endif /* BOOST_LOCKFREE_STACK_HPP */
