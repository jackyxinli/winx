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
// Module: stdext/msvc/posix/pthread.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-5 13:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MSVC_POSIX_PTHREAD_H__
#define __STDEXT_MSVC_POSIX_PTHREAD_H__

#ifndef _PTHREAD_H
#include <pthread.h>
#endif

// -------------------------------------------------------------------------

struct _Kern_Refcount
{
	typedef long _RC_t;

	volatile _RC_t _M_ref_count;
	pthread_mutex_t _M_ref_count_lock;

	_Kern_Refcount(_RC_t __n) : _M_ref_count(__n)
    {
		pthread_mutex_init(&_M_ref_count_lock, 0);
	}
	~_Kern_Refcount()
	{
		pthread_mutex_destroy(&_M_ref_count_lock);
	}
	STDMETHODIMP_(_RC_t) Increment()
	{
		pthread_mutex_lock(&_M_ref_count_lock);
		volatile _RC_t result = ++_M_ref_count;
		pthread_mutex_unlock(&_M_ref_count_lock);
		return result;
	}
	STDMETHODIMP_(_RC_t) Decrement()
	{
		pthread_mutex_lock(&_M_ref_count_lock);
		volatile _RC_t result = --_M_ref_count;
		pthread_mutex_unlock(&_M_ref_count_lock);
		return result;
	}
};

// -------------------------------------------------------------------------

struct _Kern_Mutex
{
	pthread_mutex_t _M_lock;

	_Kern_Mutex()
	{
		pthread_mutex_init(&_M_lock, 0);
	}
	~_Kern_Mutex()
	{
		pthread_mutex_destroy(&_M_lock);
	}
	STDMETHODIMP_(void) Lock()
	{
		pthread_mutex_lock(&_M_lock);
	}
	STDMETHODIMP_(void) Unlock()
	{
		pthread_mutex_unlock(&_M_lock);
	}
};

typedef _Kern_Mutex _Kern_CriticalSection;

// -------------------------------------------------------------------------
// $Log: $

#endif /* __STDEXT_MSVC_POSIX_PTHREAD_H__ */
