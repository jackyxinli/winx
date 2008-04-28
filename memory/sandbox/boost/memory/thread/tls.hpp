//
//  boost/memory/thread/tls.hpp (*)
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef __BOOST_MEMORY_THREAD_TLS_HPP__
#define __BOOST_MEMORY_THREAD_TLS_HPP__

#ifndef __BOOST_MEMORY_BASIC_HPP__
#include "../basic.hpp"
#endif

#ifndef __BOOST_MEMORY_THREADMODEL_HPP__
#include "../threadModel.hpp"
#endif

// -------------------------------------------------------------------------
// class win_tls_key

#if defined(_WIN32)

__NS_BOOST_BEGIN

typedef DWORD TLSINDEX;

class win_tls_key
{
private:
	TLSINDEX m_key;

public:
	void BOOST_MEMORY_CALL create() {
		m_key = TlsAlloc();
	}

	void BOOST_MEMORY_CALL clear() {
		TlsFree(m_key);
	}

	void BOOST_MEMORY_CALL put(void* p) const {
		TlsSetValue(m_key, p);
	}

	void* BOOST_MEMORY_CALL get() const {
		return TlsGetValue(m_key);
	}
};

__NS_BOOST_END

#endif // defined(_WIN32)

// -------------------------------------------------------------------------
// class pthread_tls_key

#if !defined(_WIN32)

#ifndef _PTHREAD_H
#include <pthread.h>
#endif

__NS_BOOST_BEGIN

class pthread_tls_key
{
private:
	pthread_key_t m_key;

public:
	void BOOST_MEMORY_CALL create() {
		pthread_key_create(&m_key, NULL);
	}

	void BOOST_MEMORY_CALL clear() const {
		pthread_key_delete(m_key);
	}

	void BOOST_MEMORY_CALL put(void* p) const {
		pthread_setspecific(m_key, p);
	}

	void* BOOST_MEMORY_CALL get() const {
		return pthread_getspecific(m_key);
	}
};

__NS_BOOST_END

#endif // !defined(_WIN32)

// -------------------------------------------------------------------------
// class tls_key

__NS_BOOST_BEGIN

#if defined(_WIN32)

typedef win_tls_key tls_key;

#else

typedef pthread_tls_key tls_key;

#endif

__NS_BOOST_END

// -------------------------------------------------------------------------
// class tls_ptr

__NS_BOOST_BEGIN

template <class Type>
class tls_ptr
{
private:
	tls_ptr(const tls_ptr&);
	void operator=(const tls_ptr&);

public:
	tls_key p;

public:
	typedef Type* pointer;
	typedef Type& reference;

public:
	explicit tls_ptr(const tls_key& key) : p(key) {}

	operator pointer() const {
		return (pointer)p.get();
	}

	pointer BOOST_MEMORY_CALL operator->() const {
		return (pointer)p.get();
	}

	pointer BOOST_MEMORY_CALL operator=(pointer lp) {
		p.put(lp);
		return lp;
	}

	bool BOOST_MEMORY_CALL operator!() const {
		return p.get() == NULL;
	}

	reference BOOST_MEMORY_CALL operator*() const {
		return *(pointer)p.get();
	}
};

__NS_BOOST_END

// -------------------------------------------------------------------------
// class tls_object

__NS_BOOST_BEGIN

template <class Type>
class tls_factory
{
public:
	enum { has_cleanup = 1 };
	
	static Type* BOOST_MEMORY_CALL create() {
		return new Type;
	}
	static void cleanup(void* p) {
		delete (Type*)p;
	}
};

#pragma warning(disable:4786)

template <
	class Type, 
	class Factory = tls_factory<Type>,
	class ThreadModel = initializer_threadmodel>
class tls_object
{
private:
	typedef typename ThreadModel::refcount refcount;

	tls_key m_key;
	refcount m_ref;

public:
	tls_object() : m_ref(0) {}

	void BOOST_MEMORY_CALL init()
	{
		if (m_ref.acquire() == 1) {
			m_key.create();
		}
	}

	void BOOST_MEMORY_CALL term()
	{
		if (m_ref.release() == 0)
		{
			if (Factory::has_cleanup)
			{
				void* p = m_key.get();
				if (p)
					Factory::cleanup(p);
			}
			m_key.clear();
		}
	}

	const tls_key& BOOST_MEMORY_CALL storage() const
	{
		return m_key;
	}

	Type& BOOST_MEMORY_CALL get()
	{
		void* p = m_key.get();
		if (p == NULL) {
			m_key.put(p = Factory::create());
		}
		return *(Type*)p;
	}
};

#pragma warning(default:4786)

__NS_BOOST_END

// -------------------------------------------------------------------------
//	$Log: $

#endif /* __BOOST_MEMORY_THREAD_TLS_HPP__ */
