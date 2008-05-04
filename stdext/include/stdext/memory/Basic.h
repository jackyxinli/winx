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
// Module: stdext/memory/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 12:25:29
// 
// $Id: Basic.h,v 1.2 2006/12/02 06:44:45 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_BASIC_H__
#define __STDEXT_MEMORY_BASIC_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

// -------------------------------------------------------------------------

#ifndef _NEW_
#include <new>	// new
#endif

#ifndef _INC_STDIO
#include <stdio.h>
#endif

#ifndef _INC_MALLOC
#include <malloc.h>	// _alloca
#endif

#pragma pack() // 为了不受其他头文件的字节对齐干扰

// =========================================================================
// Configurations

#ifndef MEMORY_ALLOC_PADDING
#define MEMORY_ALLOC_PADDING	32
#endif

#ifndef MEMORY_BLOCK_TOTAL
#define MEMORY_BLOCK_TOTAL		16384	// 16k
#endif

#ifndef MEMORY_BLOCK_SIZE
#define MEMORY_BLOCK_SIZE		(MEMORY_BLOCK_TOTAL - MEMORY_ALLOC_PADDING)
// 最佳的内存块大小，经验值
#endif

// =========================================================================
// Basic Functions

#define MEMORY_ASSERT(e)		WINX_ASSERT(e)
#define MEMORY_STATIC_ASSERT(e)	WINX_STATIC_ASSERT(e)
#define MEMORY_BREAK()			WINX_BREAK()

// =========================================================================
// ConstructorTraits, DestructorTraits

__NS_STD_BEGIN

struct ArrayDestructHeader
{
	size_t count;
};

typedef void winx_call __FnDestructor(void* data);
typedef __FnDestructor* DestructorType;

template <class Type>
struct ConstructorTraits
{
	static Type* winx_call construct(void* data)
	{
		return new(data) Type;
	}

	static Type* winx_call constructArray(Type* array, size_t count)
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
	
	static void winx_call destruct(void* data)
	{
		((Type*)data)->~Type();
	}

	static void winx_call destructArrayN(Type* array, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			array[i].~Type();
	}

	static void winx_call destructArray(void* data)
	{
		HeaderT* hdr = (HeaderT*)data;
		destructArrayN((Type*)(hdr + 1), hdr->count);
	}

	template <class AllocT>
	static void* winx_call allocArrayBuf(AllocT& alloc, size_t count)
	{
		HeaderT* hdr = (HeaderT*)alloc.allocate(
			sizeof(HeaderT)+sizeof(Type)*count, destructArray);
		hdr->count = count;
		return hdr + 1;
	}
};

__NS_STD_END

// =========================================================================
// STD_NO_DESTRUCTOR

#define STD_NO_DESTRUCTOR(Type)												\
__NS_STD_BEGIN																\
template <>																	\
struct DestructorTraits< Type >												\
{																			\
	typedef int destructor_type;											\
																			\
	enum { destruct = 0 };													\
	enum { destructArray = 0 };												\
																			\
	static void winx_call destructArrayN(Type* array, size_t count) {}		\
																			\
	template <class AllocT>													\
	static void* winx_call allocArrayBuf(AllocT& alloc, size_t count) {		\
		return alloc.allocate(sizeof(Type)*count);							\
	}																		\
};																			\
__NS_STD_END

// -------------------------------------------------------------------------
// STD_NO_CONSTRUCTOR

#define STD_NO_CONSTRUCTOR(Type)											\
__NS_STD_BEGIN																\
template <>																	\
struct ConstructorTraits< Type >											\
{																			\
	static Type* winx_call construct(void* data) {							\
		return (Type*)data;													\
	}																		\
	static Type* winx_call constructArray(Type* array, size_t count) {		\
		return array;														\
	}																		\
};																			\
__NS_STD_END

// -------------------------------------------------------------------------
// C Standard Types Support

#define STD_DECL_CTYPE(Type)												\
	STD_NO_CONSTRUCTOR(Type);												\
	STD_NO_DESTRUCTOR(Type)

// -------------------------------------------------------------------------

BOOST_DECL_CTYPE(bool);
BOOST_DECL_CTYPE(float);
BOOST_DECL_CTYPE(double);

BOOST_DECL_CTYPE(int);
BOOST_DECL_CTYPE(unsigned int);

BOOST_DECL_CTYPE(char);
BOOST_DECL_CTYPE(unsigned char);

BOOST_DECL_CTYPE(short);
BOOST_DECL_CTYPE(unsigned short);

BOOST_DECL_CTYPE(long);
BOOST_DECL_CTYPE(unsigned long);

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

__NS_STD_BEGIN

inline void winx_call enableMemoryLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

inline void winx_call swap(void* a, void* b, size_t cb)
{
	void* t = _alloca(cb);
	memcpy(t, a, cb);
	memcpy(a, b, cb);
	memcpy(b, t, cb);
}

__NS_STD_END

// =========================================================================
// $Log: Basic.h,v $

#endif /* __STDEXT_MEMORY_BASIC_H__ */
