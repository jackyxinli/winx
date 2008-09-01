//
//  boost/memory/gc_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_TLS_GC_ALLOC_HPP
#define BOOST_MEMORY_TLS_GC_ALLOC_HPP

#ifndef BOOST_MEMORY_GC_ALLOC_HPP
#include "gc_alloc.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class tls_gc_alloc

typedef tls_object<gc_alloc> tls_gc_alloc_t;

STDAPI_(tls_gc_alloc_t*) _boost_TlsGcAlloc();

template <class Unused>
class tls_gc_alloc_imp
{
private:
	static tls_gc_alloc_t* _tls_gcAlloc;

public:
	typedef gc_alloc allocator_type;

	tls_gc_alloc_imp() {
		_tls_gcAlloc->init();
	}
	~tls_gc_alloc_imp() {
		_tls_gcAlloc->term();
	}

	static allocator_type& BOOST_MEMORY_CALL instance()
	{
		return _tls_gcAlloc->get();
	}

	static void BOOST_MEMORY_CALL clear()
	{
		_tls_gcAlloc->get().clear();
	}

	static void* BOOST_MEMORY_CALL allocate(const size_t cbData)
	{
		return _tls_gcAlloc->get().allocate(cbData);
	}

	static void* BOOST_MEMORY_CALL allocate(const size_t cbData0, destructor_t fn)
	{
		return _tls_gcAlloc->get().allocate(cbData0, fn);
	}

	static void* BOOST_MEMORY_CALL allocate(size_t cb, int fnZero)
	{
		return _tls_gcAlloc->get().allocate(cb);
	}

	static void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cbData0, destructor_t fn)
	{
		return _tls_gcAlloc->get().unmanaged_alloc(cbData0, fn);
	}

	static void* BOOST_MEMORY_CALL manage(void* p, destructor_t fn)
	{
		return _tls_gcAlloc->get().manage(p, fn);
	}

	static void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, int fnZero)
	{
		return _tls_gcAlloc->get().allocate(cb);
	}

	static void* BOOST_MEMORY_CALL manage(void* p, int fnZero)
	{
		return p;
	}

	static void BOOST_MEMORY_CALL deallocate(void* pData, const size_t cbData)
	{
		_tls_gcAlloc->get().deallocate(pData, cbData);
	}

	static void* BOOST_MEMORY_CALL reallocate(void* p, size_t oldSize, size_t newSize)
	{
		return _tls_gcAlloc->get().reallocate(p, oldSize, newSize);
	}

	template <class Type>
	static void BOOST_MEMORY_CALL destroy(Type* obj)
	{
		_tls_gcAlloc->get().destroy(obj);
	}

	template <class Type>
	static Type* BOOST_MEMORY_CALL newArray(size_t count, Type* zero)
	{
		return _tls_gcAlloc->get().newArray(count, zero);
	}

	template <class Type>
	static void BOOST_MEMORY_CALL destroyArray(Type* array, size_t count)
	{
		return _tls_gcAlloc->get().destroyArray(array, count);
	}

	BOOST_MEMORY_FAKE_DBG_ALLOCATE_();
};

template <class Unused>
tls_gc_alloc_t* tls_gc_alloc_imp<Unused>::_tls_gcAlloc = _boost_TlsGcAlloc();

typedef tls_gc_alloc_imp<int> tls_gc_alloc;

// -------------------------------------------------------------------------
// $Log: tls_gc_alloc.hpp,v $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_TLS_GC_ALLOC_HPP */
