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
// Module: stdext/winapi/posix/pthread.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-5 13:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_WINAPI_POSIX_PTHREAD_H__
#define __STDEXT_WINAPI_POSIX_PTHREAD_H__

#ifndef __STDEXT_PTHREAD_H__
#include "../../pthread.h"
#endif

#ifndef __STDEXT_WINAPI_WINDEF_H__
#include "../windef.h"
#endif

// -------------------------------------------------------------------------
// class PthreadRefCount

class PthreadRefCount
{
public:
	typedef long value_type;

private:
	volatile value_type m_nRef;
	pthread_mutex_t m_lock;

public:
	PthreadRefCount(value_type nRef) : m_nRef(nRef)
    {
		pthread_mutex_init(&m_lock, 0);
	}
	~PthreadRefCount()
	{
		pthread_mutex_destroy(&m_lock);
	}

	value_type winx_call acquire()
	{
		pthread_mutex_lock(&m_lock);
		value_type result = ++m_nRef;
		pthread_mutex_unlock(&m_lock);
		return result;
	}

	value_type winx_call release()
	{
		pthread_mutex_lock(&m_lock);
		value_type result = --m_nRef;
		pthread_mutex_unlock(&m_lock);
		return result;
	}

	operator value_type()
	{
		return m_nRef;
	}
};

// -------------------------------------------------------------------------
// CriticalSection

typedef pthread_mutex_t RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION PCRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;

__forceinline VOID WINAPI InitializeCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection)
{
	pthread_mutex_init(lpCriticalSection, NULL);
}

__forceinline VOID WINAPI EnterCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection)
{
	pthread_mutex_lock(lpCriticalSection);
}

__forceinline BOOL WINAPI TryEnterCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection)
{
	return pthread_mutex_trylock(lpCriticalSection) == 0;
}

__forceinline VOID WINAPI LeaveCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection)
{
	pthread_mutex_unlock(lpCriticalSection);
}

__forceinline VOID WINAPI DeleteCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection)
{
	pthread_mutex_destroy(lpCriticalSection);
}

// -------------------------------------------------------------------------

__forceinline DWORD WINAPI GetCurrentThreadId()
{
#if defined(PTW32_VERSION)
	return (DWORD)pthread_self().p;
#else
	return pthread_self();
#endif
}

// -------------------------------------------------------------------------

#ifndef TLS_OUT_OF_INDEXES
#define TLS_OUT_OF_INDEXES (DWORD)0xFFFFFFFF
#endif

typedef pthread_key_t TLSINDEX;

__forceinline TLSINDEX WINAPI TlsAlloc(void)
{
	pthread_key_t key;
	if (pthread_key_create(&key, NULL) != S_OK)
		return TLS_OUT_OF_INDEXES;
	else
		return key;
}

__forceinline BOOL WINAPI TlsFree(TLSINDEX key)
{
	return pthread_key_delete(key) == S_OK;
}

__forceinline BOOL WINAPI TlsSetValue(TLSINDEX key, LPVOID lpTlsValue)
{
	return pthread_setspecific(key, lpTlsValue) == S_OK;
}

__forceinline LPVOID WINAPI TlsGetValue(TLSINDEX key)
{
	return pthread_getspecific(key);
}

// -------------------------------------------------------------------------
// $Log: $

#endif /* __STDEXT_WINAPI_POSIX_PTHREAD_H__ */
