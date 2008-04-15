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
// Module: stdext/ThreadModel.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-3 2:18:33
// 
// $Id: ThreadModel.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_THREADMODEL_H__
#define __STDEXT_THREADMODEL_H__

#ifndef __STDEXT_BASIC_H__
#include "Basic.h"
#endif

#ifndef __STDEXT_THREADMODEL_SINGLETHREAD_H__
#include "threadmodel/SingleThread.h"
#endif

#ifndef __STDEXT_THREADMODEL_MULTITHREAD_H__
#include "threadmodel/MultiThread.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class AutoLock

template <class LockT>
class AutoLock
{
private:
	LockT& m_lock;

private:
	AutoLock(const AutoLock&);
	void operator=(const AutoLock&);

public:
	AutoLock(LockT& lock) : m_lock(lock)
	{
		m_lock.acquire();
	}
	~AutoLock()
	{
		m_lock.release();
	}
};

// -------------------------------------------------------------------------
// class MultiThreadModel

class MT
{
public:
	typedef RefCountMT RefCount;
	typedef CriticalSectionMT CriticalSection;

public:
	typedef CriticalSection CS;
	typedef AutoLock<CS> CSLock;
};

typedef MT MultiThreadModel;

// -------------------------------------------------------------------------
// class SingleThreadModel

class ST
{
public:
	typedef RefCountST RefCount;
	typedef CriticalSectionST CriticalSection;

public:
	typedef CriticalSection CS;
	typedef AutoLock<CS> CSLock;
};

typedef ST SingleThreadModel;

// -------------------------------------------------------------------------
// class DefaultThreadModel

#if defined(_MT)
typedef MultiThreadModel DefaultThreadModel;
#else
typedef SingleThreadModel DefaultThreadModel;
#endif

// -------------------------------------------------------------------------
//	$Log: $

__NS_STD_END

#endif /* __STDEXT_THREADMODEL_H__ */
