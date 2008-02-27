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
// Module: stdext/aop/Event.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// Description: AOP - Aspect Oriented Programming
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_AOP_EVENT_H__
#define __STDEXT_AOP_EVENT_H__

#ifndef __STDEXT_AOP_INTERFACE_H__
#include "Interface.h"
#endif

// -------------------------------------------------------------------------

__NS_STD_BEGIN

class BasicConnection;

class ConnectionNodeBase : public Connection
{
protected:
	ConnectionNodeBase* m_prev;
	ConnectionNodeBase* m_next;

	friend class BasicConnection;

public:
	ConnectionNodeBase()
	{
		m_prev = m_next = this;
	}

	ConnectionNodeBase(ConnectionNodeBase* aPrev, ConnectionNodeBase* aNext)
		: m_prev(aPrev), m_next(aNext)
	{
	}

	void __stdcall disconnect()
	{
		m_next->m_prev = m_prev;
		m_prev->m_next = m_next;
		m_prev = m_next = this;
	}
};

class BasicConnection : public ConnectionNodeBase
{
private:
	typedef void* FakeMethod;

public:
	FakeTarget* target;
	FakeMethod method;

public:
	BasicConnection(FakeTarget* aTarget, FakeMethod aMethod, ConnectionNodeBase& head)
		: target(aTarget), method(aMethod), ConnectionNodeBase(&head, head.m_next)
	{
		head.m_next->m_prev = this;
		head.m_next = this;
	}
	~BasicConnection()
	{
		ConnectionNodeBase::disconnect();
	}

	const BasicConnection* next() const
	{
		return (const BasicConnection*)m_next;
	}
};

class ConnectionList : public ConnectionNodeBase
{
public:
	const BasicConnection* winx_call first() const
	{
		return (const BasicConnection*)m_next;
	}

	bool winx_call done(const ConnectionNodeBase* connection) const
	{
		return connection == this;
	}
};

__NS_STD_END

// -------------------------------------------------------------------------
// IMPLEMENT_EVENT

#define _AOP_CASTTO_VOID(p)				(*(void**)&(p))
#define _AOP_METHOD_ASSIGN(dest, src)	(*(void**)&(dest) = (src))

#define EX_EVENT_TYPE(Alloc, Event, ParametersList, arguments_list)			\
class Event##Impl : public Event											\
{																			\
private:																	\
	typedef std::FakeTarget FakeTarget;										\
	typedef void (__stdcall FakeTarget::*FakeMethod) ParametersList;		\
	std::ConnectionList m_connections;										\
	Alloc& m_alloc;															\
																			\
public:																		\
	Event##Impl(Alloc& _winx_alloc) : m_alloc(_winx_alloc) {}				\
																			\
	std::Connection* __stdcall _addListener(								\
		FakeTarget* _winx_target, FakeMethod _winx_method)					\
	{																		\
		return STD_NEW(m_alloc, std::BasicConnection)(						\
			_winx_target, _AOP_CASTTO_VOID(_winx_method), m_connections);	\
	}																		\
																			\
	void winx_call fire ParametersList										\
	{																		\
		FakeMethod _winx_method;											\
		const std::BasicConnection* _winx_conn = m_connections.first();		\
		while (!m_connections.done(_winx_conn))								\
		{																	\
			const std::BasicConnection* _winx_next = _winx_conn->next();	\
			FakeTarget* _winx_target = (FakeTarget*)_winx_conn->target;		\
			_AOP_METHOD_ASSIGN(_winx_method, _winx_conn->method);			\
			(_winx_target->*_winx_method) arguments_list;					\
			_winx_conn = _winx_next;										\
		}																	\
	}																		\
}

#define EVENT_TYPE(Event, ParametersList, arguments_list)					\
	EX_EVENT_TYPE(std::ScopeAlloc, Event, ParametersList, arguments_list)

// -------------------------------------------------------------------------
// class TestEvent

#if defined(__STDEXT_TESTCASE_H__) && defined(__STDEXT_MEMORY_SCOPEALLOC_H__)

__NS_STD_BEGIN

namespace test_event {

//
// Event interface
// 

typedef int Text;

DEFINE_EVENT(TextChanged, (Text text)); // define event type 'TextChanged'

//
// Edit component
//

class Edit
{
private:
	typedef EVENT_TYPE(TextChanged, (Text text), (text)) TextChangedEvent;

	Text m_text;
	TextChangedEvent m_changed;

public:
	Edit(std::ScopeAlloc& alloc) : m_changed(alloc) {
	}

	TextChanged* textChanged() {
		return &m_changed;
	}

	void putText(Text text) {
		m_text = text;		
		m_changed.fire(text); // fire 'changed' event when the content of Edit is altered
	}
};

//
// Dialog component
//

class Dialog
{
	std::BlockPool m_recycle;
	std::ScopeAlloc m_alloc;

	Edit m_edit;
	std::Connection* m_editChanged; // is needed only if you want to disconnect

public:
	Dialog() 
		: m_alloc(m_recycle), m_edit(m_alloc)
	{
		m_editChanged = m_edit.textChanged()->addListener(this, &Dialog::onEditChanged);
	}

	void __stdcall onEditChanged(Text text)
	{
		printf("text in edit is: %d\n", text);
	}

	void input(Text text)
	{
		m_edit.putText(text);
	}

	void disconnectEditChanged()
	{
		m_editChanged->disconnect();
	}
};

} // namespace test_event

template <class LogT>
class TestEvent : public TestCase
{
	WINX_TEST_SUITE(TestEvent);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void test(LogT& log)
	{
		using namespace test_event;

		Dialog dlg;
		dlg.input(1);
		dlg.input(2);
		dlg.disconnectEditChanged();
		dlg.input(3);
	}
};

__NS_STD_END

#endif // defined(__STDEXT_TESTCASE_H__)

// -------------------------------------------------------------------------
// $Log: $

#endif /* __STDEXT_AOP_EVENT_H__ */
