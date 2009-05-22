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
// Module: stdext/thread/TLS.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: TLS.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_THREAD_TLS_H
#define STDEXT_THREAD_TLS_H

#ifndef STDEXT_THREAD_BASIC_H
#include "Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class WinTlsKey

#if defined(STDEXT_THREAD_WINDOWS)

typedef DWORD TLSINDEX;

class WinTlsKey
{
private:
	TLSINDEX m_key;

public:
	WinTlsKey()
		: m_key(TLS_OUT_OF_INDEXES) {
	}

	void winx_call create() {
		m_key = TlsAlloc();
	}

	void winx_call clear() {
		TlsFree(m_key);
	}

	void winx_call put(void* p) const {
		TlsSetValue(m_key, p);
	}

	void* winx_call get() const {
		return TlsGetValue(m_key);
	}
};

#endif // defined(STDEXT_THREAD_WINDOWS)

// -------------------------------------------------------------------------
// class PthreadTlsKey

#if defined(STDEXT_HAS_PTHREAD)

class PthreadTlsKey
{
private:
	pthread_key_t m_key;

public:
	PthreadTlsKey()
		: m_key(TLS_OUT_OF_INDEXES) {
	}

	void winx_call create() {
		pthread_key_create(&m_key, NULL);
	}

	void winx_call clear() const {
		pthread_key_delete(m_key);
	}

	void winx_call put(void* p) const {
		pthread_setspecific(m_key, p);
	}

	void* winx_call get() const {
		return pthread_getspecific(m_key);
	}
};

#endif // !defined(_WIN32)

// -------------------------------------------------------------------------
// class TlsKey

#if defined(STDEXT_THREAD_WINDOWS)

typedef WinTlsKey TlsKey;

#else

typedef PthreadTlsKey TlsKey;

#endif

// -------------------------------------------------------------------------
// class TlsPtr - removed, use boost::thread_specific_ptr instead of TlsPtr

#if defined(BOOST_THREAD_TSS_HPP)
#define TlsPtr thread_specific_ptr

using boost::thread_specific_ptr;

#endif

// -------------------------------------------------------------------------
//	$Log: $

NS_STDEXT_END

#endif /* STDEXT_THREAD_TLS_H */
