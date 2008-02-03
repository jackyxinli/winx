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
// Module: docx/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Basic.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __DOCX_BASIC_H__
#define __DOCX_BASIC_H__

#ifndef __STDEXT_H__
#include "../../../stdext/include/stdext.h"
#endif

// -------------------------------------------------------------------------
// __DOCX

namespace docx {};

#ifndef __DOCX
#define __DOCX				docx::
#define __DOCX_BEGIN		namespace docx {
#define __DOCX_END			}
#endif

#ifndef DOCX_VER
#define DOCX_VER			0x0100	// Version 1.0
#endif

// -------------------------------------------------------------------------
// class String

__DOCX_BEGIN

class String
{
public:
	typedef size_t size_type;

public:
    size_type const length;
    const char* const pszBuf;
	
public:
	template <class AllocT>
	static const char* makeBuf(AllocT& alloc, const char* pszVal, UINT cch)
	{
		char* psz = (char*)alloc.allocate(cch);
		memcpy(psz, pszVal, cch);
		return psz;
	}

public:
	String(const char* pszVal, size_type cch) :
		length(cch), pszBuf(pszVal)
	{
	}

	template <class AllocT>
	String(AllocT& alloc, const char* pszVal, size_type cch)
		: length(cch), pszBuf(makeBuf(alloc, pszVal, length))
	{
	}

	template <class AllocT>
	String(AllocT& alloc, const char* pszVal)
		: length(strlen(pszVal)), pszBuf(makeBuf(alloc, pszVal, length))
	{
	}

	std::string winx_call str() const
	{
		return std::string(pszBuf, length);
	}

	size_type winx_call size() const
	{
		return length;
	}

	template <class LogT>
	void winx_call trace(LogT& log) const
	{
		log.printString(pszBuf, pszBuf+length);
	}
};

__DOCX_END

// -------------------------------------------------------------------------
// TypeTraits

STD_NO_DESTRUCTOR(__DOCX String);

// -------------------------------------------------------------------------
// class Object, List

__DOCX_BEGIN

enum ObjectType
{
	otUndefined,
	otList,
	otString,
	otDouble,
	otInteger
};

class List
{
public:
	typedef size_t size_type;

	class Element;
	class Object
	{
	protected:
		ObjectType ot;
		union {
			const Element* lstVal;
			String* strVal;
			double* dblVal;
			int iVal;
		};

		const List& _toList() const {
			WINX_ASSERT(ot == otList);
			return (const List&)lstVal;
		}

	public:
		Object(const List& lst) {
			ot = otList;
			lstVal = lst.hd();
		}

		Object(int i) {
			ot = otInteger;
			iVal = i;
		}

		template <class AllocT>
		Object(AllocT& alloc, const String& s) {
			ot = otString;
			strVal = STD_NEW(alloc, String)(s);
		}

		template <class AllocT>
		Object(AllocT& alloc, const char* s) {
			ot = otString;
			strVal = STD_NEW(alloc, String)(alloc, s);
		}

		template <class AllocT>
		Object(AllocT& alloc, const double& dbl) {
			ot = otString;
			dblVal = STD_NEW(alloc, double)(dbl);
		}

		template <class LogT>
		void winx_call trace(LogT& log) const
		{
			switch (ot)
			{
			case otString:
				strVal->trace(log);
				break;
			case otList:
				_toList().trace(log);
				break;
			case otInteger:
				log.print(iVal);
				break;
			case otDouble:
				log.print(*dblVal);
				break;
			default:
				log.print("(undefined)");
			}
		}
	};

public:
	class Element
	{
	public:
		const String name;
		const Object value;

	public:
		Element(String n, Object v) : name(n), value(v)
		{
		}
	};

private:
	class Node : public Element
	{
	public:
		const Node* tail;
	public:
		Node(String name, Object value, const Node* t = NULL)
			: Element(name, value), tail(t)
		{
		}
	};
	const Node* m_list;

	List(const Node* lst) : m_list(lst)
	{
	}

#define _DOCX_FOREACH_NODE(n) \
	for (const Node* n = m_list; n; n = n->tail)

public:
	List() : m_list(NULL)
	{
	}

public:
	List winx_call tl() const
	{
		return m_list->tail;
	}

	const Element* winx_call hd() const
	{
		return m_list;
	}

	bool winx_call empty() const
	{
		return m_list != NULL;
	}

	size_type winx_call size() const
	{
		size_type count = 0;
		_DOCX_FOREACH_NODE(n) { ++count; }
		return count;
	}

	template <class AllocT>
	void winx_call reverse(AllocT& alloc)
	{
		Node* lst = NULL;
		_DOCX_FOREACH_NODE(n)
		{
			lst = STD_NEW(alloc, Node)(n->name, n->value, lst);
		}
		m_list = lst;
	}
	
	template <class AllocT>
	void winx_call push_front(AllocT& alloc, String name, Object value)
	{
		m_list = STD_NEW(alloc, Node)(name, value, m_list);
	}

	template <class AllocT>
	void winx_call push_front(AllocT& alloc, const List& lst)
	{
		const Node* lstRet;
		lst._dupRetTail(alloc, &lstRet) = m_list;
		m_list = lstRet;
	}

public:
	template <class LogT>
	void winx_call trace(LogT& log) const
	{
		log.print("\n[");
		_DOCX_FOREACH_NODE(n)
		{
			log.print("\t{").printObj(n->name).print(": ")
				.printObj(n->value).print("}\n");
		}
		log.print("]");
	}

private:
	template <class AllocT>
	const Node*& winx_call _dupRetTail(AllocT& alloc, const Node** ptail) const
	{
		_DOCX_FOREACH_NODE(n)
		{
			Node* node = STD_NEW(alloc, Node)(n->name, n->value);
			*ptail = node;
			ptail = &node->tail;
		}
		return *ptail;
	}
};

typedef List::Object Object;
typedef List::Element Element;

__DOCX_END

// -------------------------------------------------------------------------
// TypeTraits

STD_NO_DESTRUCTOR(__DOCX List);
STD_NO_DESTRUCTOR(__DOCX Object);
STD_NO_DESTRUCTOR(__DOCX Element);

// -------------------------------------------------------------------------
// class TestDocxBasic

__DOCX_BEGIN

template <class LogT>
class TestDocxBasic : public TestCase
{
	WINX_TEST_SUITE(TestDocxBasic);
		WINX_TEST(testList);
	WINX_TEST_SUITE_END();

public:
	void testList(LogT& log)
	{
		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		
		docx::List lst;
		lst.push_front(alloc, String(alloc, "Class"), Object(alloc, "TestDocxBasic"));
		lst.push_front(alloc, String(alloc, "Method"), Object(alloc, "testList"));
		lst.push_front(alloc, String(alloc, "Allocator"), Object(alloc, "ScopeAlloc"));
		log.newline().printObj(lst);
		AssertExp(lst.size() == 3);

		docx::List list2 = lst;
		lst.push_front(alloc, String(alloc, "List"), 1);
		list2.push_front(alloc, String(alloc, "List"), 2);
		log.newline().printObj(lst);
		log.newline().printObj(list2);
		AssertExp(lst.size() == 4);
		AssertExp(list2.size() == 4);

		lst.push_front(alloc, list2);
		log.newline().printObj(lst);
		AssertExp(lst.size() == 8);

		lst.push_front(alloc, String(alloc, "SubList"), list2);
		log.newline().printObj(lst);
		AssertExp(lst.size() == 9);
	}
};

__DOCX_END

// -------------------------------------------------------------------------
// $Log: Basic.h,v $

#endif /* __DOCX_BASIC_H__ */
