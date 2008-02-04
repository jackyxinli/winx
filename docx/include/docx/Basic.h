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
	static const char* makeBuf(AllocT& alloc, const char* pszVal, size_type cch)
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

	bool winx_call operator==(const String& s) const
	{
		return length == s.length && 
			(memcmp(pszBuf, s.pszBuf, length) == 0);
	}

	bool winx_call operator!=(const String& s) const
	{
		return length != s.length || 
			(memcmp(pszBuf, s.pszBuf, length) != 0);
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

		ObjectType winx_call type() const {
			return ot;
		}

		const List& winx_call toList() const
		{
			WINX_ASSERT(ot == otList);
			if (ot != otList)
				throw std::bad_cast();
			return (const List&)lstVal;
		}

		bool winx_call contains(const Object& o) const
		{
			if (ot != o.ot)
				return false;
			switch (ot)
			{
			case otString:
				return *strVal == *o.strVal;
			case otList:
				return toList().contains(o.toList());
			case otInteger:
				return iVal == o.iVal;
			case otDouble:
				return *dblVal == *o.dblVal;
			default:
				return false;
			}
		}

		bool winx_call operator==(const Object& o) const
		{
			if (ot != o.ot)
				return false;
			switch (ot)
			{
			case otString:
				return *strVal == *o.strVal;
			case otList:
				return toList() == o.toList();
			case otInteger:
				return iVal == o.iVal;
			case otDouble:
				return *dblVal == *o.dblVal;
			default:
				return false;
			}
		}

		bool winx_call operator!=(const Object& o) const
		{
			return !operator==(o);
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
				toList().trace(log);
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
		Element(const String& n, const Object& v)
			: name(n), value(v)
		{
		}

		bool winx_call operator==(const Element& e) const
		{
			return (name == e.name) && (value == e.value);
		}

		bool winx_call operator!=(const Element& e) const
		{
			return (name != e.name) || (value != e.value);
		}

		bool winx_call contains(const Element& e) const
		{
			return name == e.name && value.contains(e.value);
		}
		
		template <class LogT>
		void winx_call trace(LogT& log) const
		{
			log.print('{').printObj(name).print(": ")
				.printObj(value).print('}');
		}
	};

private:
	class Node : public Element
	{
	public:
		const Node* tail;
	public:
		Node(const Element& e, const Node* t = NULL)
			: Element(e), tail(t)
		{
		}
		Node(const String& n, const Object& v, const Node* t = NULL)
			: Element(n, v), tail(t)
		{
		}
	};
	const Node* m_list;

	List(const Node* lst) : m_list(lst)
	{
	}

#define _DOCX_NEXT_NODE(n) \
	n = n->tail
#define _DOCX_FOREACH_NODE(n) \
	for (const Node* n = m_list; n; n = n->tail)
#define _DOCX_FOREACH_NODE2(n, lst) \
	for (const Node* n = (lst).m_list; n; n = n->tail)

public:
	List() : m_list(NULL)
	{
	}

	template <class AllocT>
	List(AllocT& alloc, const Element& e)
	{
		m_list = STD_NEW(alloc, Node)(e);
	}

	template <class AllocT>
	List(AllocT& alloc, const String& name, const Object& value)
	{
		m_list = STD_NEW(alloc, Node)(name, value);
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

	size_type winx_call size() const
	{
		size_type count = 0;
		_DOCX_FOREACH_NODE(n) { ++count; }
		return count;
	}

	bool winx_call empty() const
	{
		return m_list != NULL;
	}

	bool winx_call operator==(const List& lst) const
	{
		const Node* n2 = lst.m_list;
		_DOCX_FOREACH_NODE(n)
		{
			if (n2 == NULL || *n != *n2)
				return false;
			_DOCX_NEXT_NODE(n2);
		}
		return n2 == NULL;
	}

	bool winx_call operator!=(const List& lst) const
	{
		return !operator==(lst);
	}

	bool winx_call contains(const Element& e) const
	{
		_DOCX_FOREACH_NODE(n)
		{
			if (n->contains(e))
				return true;
		}
		return false;
	}

	bool winx_call contains(const List& lst) const
	{
		_DOCX_FOREACH_NODE2(n, lst)
		{
			if (!contains(*n))
				return false;
		}
		return true;
	}

	template <class LogT>
	void winx_call trace(LogT& log) const
	{
		log.print("\n[");
		_DOCX_FOREACH_NODE(n)
		{
			log.print('\t').printObj(*n).newline();
		}
		log.print("]");
	}

public:
	template <class AllocT>
	void winx_call reverse(AllocT& alloc)
	{
		Node* lst = NULL;
		_DOCX_FOREACH_NODE(n)
		{
			lst = STD_NEW(alloc, Node)(*n, lst);
		}
		m_list = lst;
	}

	void winx_call pop_front()
	{
		m_list = m_list->tail;
	}

	template <class AllocT>
	void winx_call push_front(AllocT& alloc, const Element& e)
	{
		m_list = STD_NEW(alloc, Node)(e, m_list);
	}

	template <class AllocT>
	void winx_call push_front(AllocT& alloc, const String& name, const Object& value)
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

	template <class AllocT, class CondT>
	List winx_call select(AllocT& alloc, CondT cond) const
	{
		const Node* lstRet;
		_selectRetTail(alloc, cond, &lstRet);
		return lstRet;
	}
	
private:
	template <class AllocT, class CondT>
	const Node*& winx_call _selectRetTail(
		AllocT& alloc, CondT cond, const Node** ptail) const
	{
		_DOCX_FOREACH_NODE(n)
		{
			const Element& e = *n;
			if (cond(e))
			{
				Node* node = STD_NEW(alloc, Node)(e);
				*ptail = node;
				ptail = &node->tail;
			}
		}
		return *ptail;
	}

	template <class AllocT>
	const Node*& winx_call _dupRetTail(AllocT& alloc, const Node** ptail) const
	{
		_DOCX_FOREACH_NODE(n)
		{
			Node* node = STD_NEW(alloc, Node)(*n);
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
// class NameIs, Equals, Contains

__DOCX_BEGIN

class NameIs
{
private:
	const String& m_name;

public:
	NameIs(const String& n) : m_name(n) {}

	bool operator()(const Element& e) const {
		return e.name == m_name;
	}
};

class Equals
{
private:
	const Element& m_e;

public:
	Equals(const Element& e) : m_e(e) {}

	bool operator()(const Element& e) const {
		return e == m_e;
	}
};

class Contains
{
private:
	const Element& m_e;

public:
	Contains(const Element& e) : m_e(e) {}

	bool operator()(const Element& e) const {
		return e.contains(m_e);
	}
};

__DOCX_END

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
		using docx::String;
		using docx::Object;

		std::BlockPool recycle;
		std::ScopeAlloc alloc(recycle);
		
		docx::List lst;
		lst.push_front(alloc, String(alloc, "Class"), Object(alloc, "TestDocxBasic"));
		lst.push_front(alloc, String(alloc, "Method"), Object(alloc, "testList"));
		lst.push_front(alloc, String(alloc, "Allocator"), Object(alloc, "ScopeAlloc"));
		log.newline().printObj(lst);
		AssertExp(lst.size() == 3);
		docx::List list0 = lst;

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

		list2.push_front(alloc, String(alloc, "SubList"), list0);
		lst.push_front(alloc, String(alloc, "SubList"), list2);
		log.newline().printObj(lst);
		AssertExp(lst.size() == 9);

		List list3 = lst.select(alloc, docx::NameIs(String(alloc, "List")));
		log.newline().printObj(list3);
		AssertExp(list3.size() == 2);

		docx::Element e(String(alloc, "Class"), Object(alloc, "TestDocxBasic"));
		List list4 = lst.select(alloc, docx::Equals(e));
		log.newline().printObj(list4);
		AssertExp(list4.size() == 2);

		docx::List list5 = list2;
		docx::Element e2(String(alloc, "List"), 5);
		list5.push_front(alloc, e2);
		lst.push_front(alloc, String(alloc, "SubList"), list5);
		log.newline().printObj(lst);

		docx::Element e3(String(alloc, "Allocator"), Object(alloc, "ScopeAlloc"));
		docx::List sub(alloc, String(alloc, "SubList"), docx::List(alloc, e3));
		docx::Element e4(String(alloc, "SubList"), sub);
		log.newline(2).printObj(e4);

		docx::List list6 = lst.select(alloc, docx::Contains(e4));
		log.newline().printObj(list6);
		AssertExp(list6.size() == 2);

		log.newline();
	}
};

__DOCX_END

// -------------------------------------------------------------------------
// $Log: Basic.h,v $

#endif /* __DOCX_BASIC_H__ */
