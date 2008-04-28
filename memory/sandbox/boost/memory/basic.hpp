//
//  boost/memory/basic.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef __BOOST_MEMORY_BASIC_HPP__
#define __BOOST_MEMORY_BASIC_HPP__

#ifndef __BOOST_MEMORY_CONFIG_HPP__
#include "config.hpp"
#endif

// -------------------------------------------------------------------------

#if !defined(_NEW_) && !defined(_NEW)
#include <new>	// new
#endif

#if !defined(_CSTDIO_) && !defined(_CSTDIO)
#include <cstdio>
#endif

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H)
#include <malloc.h>	// _alloca
#endif

// =========================================================================
// BOOST_MEMORY_ASSERT - diagnost

#if defined(ASSERT)
#define BOOST_MEMORY_ASSERT(e)		ASSERT(e)
#elif defined(_ASSERTE)
#define BOOST_MEMORY_ASSERT(e)		_ASSERTE(e)
#else
#ifdef _DEBUG
#ifndef assert
#include <cassert>
#endif
#define BOOST_MEMORY_ASSERT(e)		assert(e)
#else
#define BOOST_MEMORY_ASSERT(e)
#endif
#endif

// =========================================================================
// ConstructorTraits, DestructorTraits

__NS_BOOST_BEGIN

struct ArrayDestructHeader
{
	size_t count;
};

typedef void BOOST_MEMORY_CALL __FnDestructor(void* data);
typedef __FnDestructor* DestructorType;

template <class Type>
struct ConstructorTraits
{
	static Type* BOOST_MEMORY_CALL construct(void* data)
	{
		return new(data) Type;
	}

	static Type* BOOST_MEMORY_CALL constructArray(Type* array, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			new(array + i) Type;
		return array;
	}
};

template <class Type>
struct DestructorTraits
{
	typedef ArrayDestructHeader HeaderT;
	typedef DestructorType destructor_type;
	
	static void BOOST_MEMORY_CALL destruct(void* data)
	{
		((Type*)data)->~Type();
	}

	static void BOOST_MEMORY_CALL destructArrayN(Type* array, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			array[i].~Type();
	}

	static void BOOST_MEMORY_CALL destructArray(void* data)
	{
		HeaderT* hdr = (HeaderT*)data;
		destructArrayN((Type*)(hdr + 1), hdr->count);
	}

	template <class AllocT>
	static void* BOOST_MEMORY_CALL allocArrayBuf(AllocT& alloc, size_t count)
	{
		HeaderT* hdr = (HeaderT*)alloc.allocate(
			sizeof(HeaderT)+sizeof(Type)*count, destructArray);
		hdr->count = count;
		return hdr + 1;
	}
};

template <class Type>
inline void BOOST_MEMORY_CALL destroyArray(Type* array, size_t count)
{
	DestructorTraits<Type>::destructArrayN(array, count);
}

__NS_BOOST_END

// =========================================================================
// STD_NO_DESTRUCTOR

#define STD_NO_DESTRUCTOR(Type)												\
__NS_BOOST_BEGIN															\
template <>																	\
struct DestructorTraits< Type >												\
{																			\
	typedef int destructor_type;											\
																			\
	enum { destruct = 0 };													\
	enum { destructArray = 0 };												\
																			\
	static void BOOST_MEMORY_CALL destructArrayN(Type* array, size_t count) {}		\
																			\
	template <class AllocT>													\
	static void* BOOST_MEMORY_CALL allocArrayBuf(AllocT& alloc, size_t count) {		\
		return alloc.allocate(sizeof(Type)*count);							\
	}																		\
};																			\
__NS_BOOST_END

#define STD_INT_NO_DESTRUCTOR(Type)											\
	STD_NO_DESTRUCTOR(unsigned Type);										\
	STD_NO_DESTRUCTOR(signed Type)

// -------------------------------------------------------------------------
// STD_NO_CONSTRUCTOR

#define STD_NO_CONSTRUCTOR(Type)											\
__NS_BOOST_BEGIN															\
template <>																	\
struct ConstructorTraits< Type >											\
{																			\
	static Type* BOOST_MEMORY_CALL construct(void* data) {							\
		return (Type*)data;													\
	}																		\
	static Type* BOOST_MEMORY_CALL constructArray(Type* array, size_t count) {		\
		return array;														\
	}																		\
};																			\
__NS_BOOST_END

#define STD_INT_NO_CONSTRUCTOR(Type)										\
	STD_NO_CONSTRUCTOR(unsigned Type);										\
	STD_NO_CONSTRUCTOR(signed Type)

// -------------------------------------------------------------------------
// C Standard Types Support

#define STD_DECL_CTYPE(Type)												\
	STD_NO_CONSTRUCTOR(Type);												\
	STD_NO_DESTRUCTOR(Type)

#define STD_DECL_INT_CTYPE(Type)											\
	STD_NO_CONSTRUCTOR(Type);												\
	STD_INT_NO_DESTRUCTOR(Type)

// -------------------------------------------------------------------------

STD_DECL_CTYPE(bool);
STD_DECL_CTYPE(float);
STD_DECL_CTYPE(double);

STD_DECL_INT_CTYPE(int);
STD_DECL_INT_CTYPE(char);
STD_DECL_INT_CTYPE(short);
STD_DECL_INT_CTYPE(long);

// =========================================================================
// MEMORY_DBG_NEW_ARG

#if defined(_DEBUG)
#define MEMORY_FILE_LINE_ARG		,__FILE__, __LINE__
#else
#define MEMORY_FILE_LINE_ARG
#endif

#define MEMORY_NEW_ARG(Type)					sizeof(Type), std::DestructorTraits<Type>::destruct
#define MEMORY_DBG_NEW_ARG(Type)				MEMORY_NEW_ARG(Type) MEMORY_FILE_LINE_ARG

#define MEMORY_NEW_ARRAY_ARG(Type, count)		(count), (Type*)0
#define MEMORY_DBG_NEW_ARRAY_ARG(Type, count)	MEMORY_NEW_ARRAY_ARG(Type, count) MEMORY_FILE_LINE_ARG

#define MEMORY_DBG_ALLOC_ARG(Type)				sizeof(Type) MEMORY_FILE_LINE_ARG
#define MEMORY_DBG_ALLOC_ARRAY_ARG(Type, count)	sizeof(Type)*(count) MEMORY_FILE_LINE_ARG

// =========================================================================
// NEW, NEW_ARRAY, ALLOC, ALLOC_ARRAY

#define STD_NEW(alloc, Type)					::new((alloc).allocate(MEMORY_DBG_NEW_ARG(Type))) Type
#define STD_NEW_ARRAY(alloc, Type, count) 		(alloc).newArray(MEMORY_DBG_NEW_ARRAY_ARG(Type, count))

#define STD_ALLOC(alloc, Type)					((Type*)(alloc).allocate(MEMORY_DBG_ALLOC_ARG(Type)))
#define STD_ALLOC_ARRAY(alloc, Type, count)		((Type*)(alloc).allocate(MEMORY_DBG_ALLOC_ARRAY_ARG(Type, count)))

// =========================================================================

#ifndef _CrtSetDbgFlag

#if defined(_MSC_VER)
#include <crtdbg.h>
#endif

#endif

// -------------------------------------------------------------------------

__NS_BOOST_BEGIN

inline void BOOST_MEMORY_CALL enableMemoryLeakCheck()
{
#ifdef _CrtSetDbgFlag
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
#endif
}

__NS_BOOST_END

// =========================================================================
// $Log: Basic.h,v $

#endif /* __BOOST_MEMORY_BASIC_HPP__ */
