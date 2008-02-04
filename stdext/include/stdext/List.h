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

#ifndef _LIST_
#include <list>
#endif

#ifndef __STDEXT_MEMORY_H__
#include "Memory.h"
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

public:
	explicit List(AllocT& alloc)
		: _Base(alloc)
	{
	}

	List(AllocT& alloc, size_type count, const ValT& val = ValT())
		: _Base(count, val, alloc)
	{
	}

	template <class Iterator>
	List(AllocT& alloc, Iterator first, Iterator last)
		: _Base(first, last, alloc)
	{
	}
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
		WINX_DECLARE_COUNT(Obj);
	private:
		int m_val;
	public:
		Obj(int arg = 0) {
			m_val = arg;
			printf("construct Obj: %d\n", m_val);
		}
		Obj(const Obj& o) {
			m_val = o.m_val;
			printf("copy constructor: %d\n", m_val);
		}
		~Obj() {
			printf("destruct Obj: %d\n", m_val);
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
// $Log: List.h,v $

__NS_STD_END

#endif /* __STDEXT_LIST_H__ */
