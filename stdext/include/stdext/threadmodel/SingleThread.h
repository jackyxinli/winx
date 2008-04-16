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
// Module: stdext/threadmodel/SingleThread.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-3 2:18:33
// 
// $Id: SingleThread.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_THREADMODEL_SINGLETHREAD_H__
#define __STDEXT_THREADMODEL_SINGLETHREAD_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// struct RefCountST

class RefCountST
{
public:
	typedef long value_type;

private:
	value_type m_nRef;

public:
	RefCountST(value_type nRef) : m_nRef(nRef)
	{
	}

	value_type winx_call acquire() { return ++m_nRef; }
	value_type winx_call release() { return --m_nRef; }

	operator value_type()
	{
		return m_nRef;
	}
};

// -------------------------------------------------------------------------
// class CriticalSectionST

class CriticalSectionST
{
public:
	void winx_call acquire() {}
	void winx_call release() {}
	BOOL winx_call good() const {
		return TRUE;
	}
};

// -------------------------------------------------------------------------
//	$Log: $

__NS_STD_END

#endif /* __STDEXT_THREADMODEL_SINGLETHREAD_H__ */
