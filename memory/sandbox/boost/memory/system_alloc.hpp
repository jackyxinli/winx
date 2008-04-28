//
//  boost/memory/system_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef __BOOST_MEMORY_SYSTEM_ALLOC_HPP__
#define __BOOST_MEMORY_SYSTEM_ALLOC_HPP__

#ifndef __BOOST_MEMORY_BASIC_HPP__
#include "basic.hpp"
#endif

#ifndef __BOOST_MEMORY_THREADMODEL_HPP__
#include "threadmodel.hpp"
#endif

#if !defined(_ALGORITHM_) && !defined(_ALGORITHM)
#include <algorithm> // std::swap
#endif

#if !defined(_CSTDLIB_) && !defined(_CSTDLIB)
#include <cstdlib> // malloc, free
#endif

__NS_BOOST_BEGIN

// -------------------------------------------------------------------------
// __BOOST_FAKE_DBG_ALLOCATE

#if defined(_DEBUG)

#define __BOOST_FAKE_DBG_ALLOCATE()														\
	void* BOOST_MEMORY_CALL allocate(size_t cb, LPCSTR szFile, int nLine)				\
		{ return allocate(cb); }														\
	void* BOOST_MEMORY_CALL allocate(size_t cb, destructor_t fn, LPCSTR szFile, int nLine)	\
		{ return allocate(cb, fn); }													\
	void* BOOST_MEMORY_CALL allocate(size_t cb, int fnZero, LPCSTR szFile, int nLine)	\
		{ return allocate(cb); }														\
	void* BOOST_MEMORY_CALL reallocate(void* p, size_t oldSize, size_t newSize,			\
		LPCSTR szFile, int nLine) { return reallocate(p, oldSize, newSize); }			\
	template <class Type>																\
	Type* BOOST_MEMORY_CALL newArray(size_t count, Type* zero, LPCSTR szFile, int nLine)\
		{ return newArray(count, zero); }

#else

#define __BOOST_FAKE_DBG_ALLOCATE()

#endif

// -------------------------------------------------------------------------
// class stdlib_alloc

#if defined(__GNUG__)
#define _msize	malloc_usable_size
#endif

class stdlib_alloc
{
public:
	static void* BOOST_MEMORY_CALL allocate(size_t cb)					{ return malloc(cb); }
	static void* BOOST_MEMORY_CALL allocate(size_t cb, destructor_t fn)	{ return malloc(cb); }
	static void* BOOST_MEMORY_CALL allocate(size_t cb, int fnZero)		{ return malloc(cb); }

	static void* BOOST_MEMORY_CALL reallocate(void* p, size_t oldSize, size_t newSize) {
		return realloc(p, newSize);
	}

	static void BOOST_MEMORY_CALL deallocate(void* p)			{ free(p); }
	static void BOOST_MEMORY_CALL deallocate(void* p, size_t)	{ free(p); }
	static void BOOST_MEMORY_CALL swap(stdlib_alloc& o)			{}

	static size_t BOOST_MEMORY_CALL alloc_size(void* p)
	{
		return _msize(p);
	}

	template <class Type>
	static void BOOST_MEMORY_CALL destroy(Type* obj)
	{
		obj->~Type();
		free(obj);
	}

	template <class Type>
	static Type* BOOST_MEMORY_CALL newArray(size_t count, Type* zero)
	{
		Type* array = (Type*)malloc(sizeof(Type) * count);
		return ConstructorTraits<Type>::constructArray(array, count);
	}

	template <class Type>
	static void BOOST_MEMORY_CALL destroyArray(Type* array, size_t count)
	{
		DestructorTraits<Type>::destructArrayN(array, count);
		free(array);
	}

#if defined(_malloc_dbg)
	static void* BOOST_MEMORY_CALL allocate(size_t cb, LPCSTR szFile, int nLine)
		{ return _malloc_dbg(cb, _NORMAL_BLOCK, szFile, nLine); }

	static void* BOOST_MEMORY_CALL allocate(size_t cb, destructor_t fn, LPCSTR szFile, int nLine)
		{ return _malloc_dbg(cb, _NORMAL_BLOCK, szFile, nLine); }
	
	static void* BOOST_MEMORY_CALL allocate(size_t cb, int fnZero, LPCSTR szFile, int nLine)
		{ return _malloc_dbg(cb, _NORMAL_BLOCK, szFile, nLine); }

	static void* BOOST_MEMORY_CALL reallocate(void* p, size_t oldSize, size_t newSize, LPCSTR szFile, int nLine)
		{ return _realloc_dbg(p, newSize, _NORMAL_BLOCK, szFile, nLine); }

	template <class Type>
	static Type* BOOST_MEMORY_CALL newArray(size_t count, Type* zero, LPCSTR szFile, int nLine)
	{
		Type* array = (Type*)_malloc_dbg(sizeof(Type) * count, _NORMAL_BLOCK, szFile, nLine);
		return ConstructorTraits<Type>::constructArray(array, count);
	}
#else
	__BOOST_FAKE_DBG_ALLOCATE()
#endif
};

// -------------------------------------------------------------------------
// class system_alloc

typedef stdlib_alloc system_alloc;

// -------------------------------------------------------------------------
// $Log: $

__NS_BOOST_END

#endif /* __BOOST_MEMORY_SYSTEM_ALLOC_HPP__ */
