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
// Module: stdext/aop/Interface.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// Description: AOP - Aspect Oriented Programming
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_AOP_INTERFACE_H__
#define __STDEXT_AOP_INTERFACE_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// interface Connection

interface Connection
{
	virtual void __stdcall disconnect() = 0;
};

// -------------------------------------------------------------------------
// class HConnection - Connection HandleClass

class HConnection
{
private:
	HConnection(const HConnection&);
	void operator=(const HConnection&);

private:
	Connection* m_conn;

public:
	HConnection(Connection* conn = NULL) : m_conn(conn) {}
	~HConnection() {
		if (m_conn)
			m_conn->disconnect();
	}

	void winx_call operator=(Connection* conn)
	{
		WINX_ASSERT(m_conn == NULL);
		m_conn = conn;
	}

	void winx_call disconnect()
	{
		if (m_conn)
		{
			m_conn->disconnect();
			m_conn = NULL;
		}
	}
};

// -------------------------------------------------------------------------
// interface Event

class FakeTarget
{
};

template <class EventTag>
interface Event
{
	template <class Target, class ParametersList>
	/* virtual */ Connection* __stdcall addListener(
		Target* target,
		void (__stdcall Target::*method)(ParametersList)
		);
};

#define DEFINE_EVENT(Event, ParametersList)									\
interface Event																\
{																			\
	virtual std::Connection* __stdcall _addListener(						\
		std::FakeTarget* target,											\
		void (__stdcall std::FakeTarget::*method) ParametersList) = 0;		\
																			\
	template <class Target>													\
	__forceinline std::Connection* winx_call addListener(					\
		Target* target,														\
		void (__stdcall Target::*method) ParametersList)					\
	{																		\
		typedef void (__stdcall std::FakeTarget::*FakeMethod) ParametersList; \
		return _addListener((std::FakeTarget*)target, (FakeMethod)method);	\
	}																		\
}

// -------------------------------------------------------------------------
// interface EventContainer

typedef IID EventName;
typedef const EventName& EventNameRef;

interface EventContainer
{
	virtual HRESULT __stdcall find(EventNameRef name, void** event) = 0;
};

// -------------------------------------------------------------------------
// $Log: $

__NS_STD_END

#endif /* __STDEXT_AOP_INTERFACE_H__ */
