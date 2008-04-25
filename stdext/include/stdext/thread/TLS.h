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
// Date: 2006-9-3 2:18:33
// 
// $Id: TLS.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_THREAD_TLS_H__
#define __STDEXT_THREAD_TLS_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

#ifndef __STDEXT_THREADMODEL_H__
#include "../ThreadModel.h"
#endif

#if defined(_WIN32) && !defined(STD_NO_WINSDK)
typedef DWORD TLSINDEX;
#endif

// -------------------------------------------------------------------------
// class WinTlsKey

__NS_STD_BEGIN

class WinTlsKey
{
private:
	TLSINDEX m_key;

public:
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

__NS_STD_END

// -------------------------------------------------------------------------
// class PthreadTlsKey

#if !defined(_WIN32) || defined(PTW32_VERSION)

#ifndef __STDEXT_PTHREAD_H__
#include "../pthread.h"
#endif

__NS_STD_BEGIN

class PthreadTlsKey
{
private:
	pthread_key_t m_key;

public:
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

__NS_STD_END

#endif // !defined(_WIN32)

// -------------------------------------------------------------------------
// class TlsKey

__NS_STD_BEGIN

#if defined(_WIN32)

typedef WinTlsKey TlsKey;

#else

typedef PthreadTlsKey TlsKey;

#endif

__NS_STD_END

// -------------------------------------------------------------------------
// class TlsPtr

__NS_STD_BEGIN

template <class Type>
class TlsPtr
{
private:
	TlsPtr(const TlsPtr&);
	void operator=(const TlsPtr&);

public:
	TlsKey p;

public:
	typedef Type* pointer;
	typedef Type& reference;

public:
	explicit TlsPtr(const TlsKey& key) : p(key) {}

	operator pointer() const {
		return (pointer)p.get();
	}

	pointer winx_call operator->() const {
		return (pointer)p.get();
	}

	pointer winx_call operator=(pointer lp) {
		p.put(lp);
		return lp;
	}

	bool winx_call operator!() const {
		return p.get() == NULL;
	}

	reference winx_call operator*() const {
		return *(pointer)p.get();
	}
};

__NS_STD_END

// -------------------------------------------------------------------------
// class TlsObject

__NS_STD_BEGIN

template <class Type>
class TlsFactory
{
public:
	enum { has_cleanup = 1 };
	
	static Type* winx_call create() {
		return new Type;
	}
	static void cleanup(void* p) {
		delete (Type*)p;
	}
};

template <
	class Type, 
	class Factory = TlsFactory<Type>,
	class ThreadModel = InitializerThreadModel>
class TlsObject
{
private:
	typedef typename ThreadModel::RefCount RefCount;

	TlsKey m_key;
	RefCount m_ref;

public:
	TlsObject() : m_ref(0) {}
	
	bool winx_call good() const {
		return (typename RefCount::value_type)m_ref > 0;
	}

	void winx_call init()
	{
		if (m_ref.acquire() == 1) {
			m_key.create();
		}
	}

	void winx_call term()
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

	const TlsKey& winx_call storage() const
	{
		return m_key;
	}

	Type& winx_call get()
	{
		void* p = m_key.get();
		if (p == NULL) {
			m_key.put(p = Factory::create());
		}
		return *(Type*)p;
	}
};

__NS_STD_END

// -------------------------------------------------------------------------

#if defined(STD_UNITTEST)

__NS_STD_BEGIN

template <class LogT>
class TestTLS : public TestCase
{
	WINX_TEST_SUITE(TestTLS);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	class Obj
	{
	private:
		int m_val;
	public:
		Obj(int arg = 0) {
			m_val = arg;
			printf("construct Obj: %d\n", m_val);
		}
		~Obj() {
			printf("destruct Obj: %d\n", m_val);
		}
		
		void trace(LogT& log) {
			log.print(m_val).newline();
		}

		static void cleanup(void* p) {
			delete (Obj*)p;
		}
	};

	void test(LogT& log)
	{
		TlsKey key;
		key.create();

		TlsPtr<Obj> obj(key);

		obj = new Obj(1);
		obj->trace(log);
		delete obj;

		obj = new Obj(2);
		obj->trace(log);
		delete obj;

		key.clear();
	}
};

__NS_STD_END

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
//	$Log: $

#endif /* __STDEXT_THREAD_TLS_H__ */
