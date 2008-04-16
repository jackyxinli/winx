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
// Module: stdext/threadmodel/MultiThread.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-3 2:18:33
// 
// $Id: MultiThread.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_THREADMODEL_MULTITHREAD_H__
#define __STDEXT_THREADMODEL_MULTITHREAD_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class RefCountMT

class RefCountMT
{
public:
	typedef LONG value_type;

private:
	value_type m_nRef;

public:
	RefCountMT(value_type nRef)
		: m_nRef(nRef)
	{
	}

	value_type winx_call acquire()
	{
		return InterlockedIncrement(&m_nRef);
	}

	value_type winx_call release()
	{
		return InterlockedDecrement(&m_nRef);
	}

	operator value_type()
	{
		return m_nRef;
	}
};

// -------------------------------------------------------------------------
// class CriticalSectionMT

class CriticalSectionMT
{
private:
	CRITICAL_SECTION m_cs;
	
private:
	CriticalSectionMT(const CriticalSectionMT&);
	void operator=(const CriticalSectionMT&);

public:
	CriticalSectionMT()
	{
		InitializeCriticalSection(&m_cs);
	}
	~CriticalSectionMT()
	{
		DeleteCriticalSection(&m_cs);
	}

	void winx_call acquire()
	{
		EnterCriticalSection(&m_cs);
	}

	void winx_call release()
	{
		LeaveCriticalSection(&m_cs);
	}

	BOOL winx_call good() const
	{
		return isInitialized(m_cs);
	}
};

// -------------------------------------------------------------------------
//	$Log: $

__NS_STD_END

#endif /* __STDEXT_THREADMODEL_MULTITHREAD_H__ */
