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
// Module: stdext/memory/SystemAlloc.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2008-1-20 13:46:39
// 
// $Id: RecycleBuffer.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_SYSTEMALLOC_H__
#define __STDEXT_MEMORY_SYSTEMALLOC_H__

#ifndef __STDEXT_MEMORY_BASIC_H__
#include "Basic.h"
#endif

#ifndef __STDEXT_THREADMODEL_H__
#include "../ThreadModel.h"
#endif

#ifndef __STD_ALGORITHM_H__
#include "../../std/algorithm.h" // std::swap
#endif

#ifndef _INC_STDLIB
#include <stdlib.h> // malloc,free
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// __STD_FAKE_DBG_ALLOCATE

#if defined(_DEBUG)

#define __STD_FAKE_DBG_ALLOCATE()														\
	void* winx_call allocate(size_t cb, LPCSTR szFile, int nLine)						\
		{ return allocate(cb); }														\
	void* winx_call allocate(size_t cb, DestructorType fn, LPCSTR szFile, int nLine)	\
		{ return allocate(cb, fn); }													\
	void* winx_call allocate(size_t cb, int fnZero, LPCSTR szFile, int nLine)			\
		{ return allocate(cb); }														\
	template <class Type>																\
	Type* winx_call newArray(size_t count, Type* zero, LPCSTR szFile, int nLine)		\
		{ return newArray(count, zero); }

#else

#define __STD_FAKE_DBG_ALLOCATE()

#endif

// -------------------------------------------------------------------------
// StdLibAlloc

class StdLibAlloc
{
public:
	static void* winx_call allocate(size_t cb)						{ return malloc(cb); }
	static void* winx_call allocate(size_t cb, DestructorType fn)	{ return malloc(cb); }
	static void* winx_call allocate(size_t cb, int fnZero)			{ return malloc(cb); }

	static void winx_call deallocate(void* p)			{ free(p); }
	static void winx_call deallocate(void* p, size_t)	{ free(p); }
	static void winx_call swap(StdLibAlloc& o)			{}

	template <class Type>
	static void winx_call destroy(Type* obj)
	{
		obj->~Type();
		free(obj);
	}

	template <class Type>
	static Type* winx_call newArray(size_t count, Type* zero)
	{
		Type* array = (Type*)malloc(sizeof(Type) * count);
		return ConstructorTraits<Type>::constructArray(array, count);
	}

	template <class Type>
	static void winx_call destroyArray(Type* array, size_t count)
	{
		DestructorTraits<Type>::destructArrayN(array, count);
		free(array);
	}

#if defined(_DEBUG)
	static void* winx_call allocate(size_t cb, LPCSTR szFile, int nLine)
		{ return _malloc_dbg(cb, _NORMAL_BLOCK, szFile, nLine); }

	static void* winx_call allocate(size_t cb, DestructorType fn, LPCSTR szFile, int nLine)
		{ return _malloc_dbg(cb, _NORMAL_BLOCK, szFile, nLine); }
	
	static void* winx_call allocate(size_t cb, int fnZero, LPCSTR szFile, int nLine)
		{ return _malloc_dbg(cb, _NORMAL_BLOCK, szFile, nLine); }

	template <class Type>
	static Type* winx_call newArray(size_t count, Type* zero, LPCSTR szFile, int nLine)
	{
		Type* array = (Type*)_malloc_dbg(sizeof(Type) * count, _NORMAL_BLOCK, szFile, nLine);
		return ConstructorTraits<Type>::constructArray(array, count);
	}
#endif
};

// -------------------------------------------------------------------------
// CoTaskAlloc

#if !defined(STD_NO_WINSDK)

class CoTaskAlloc
{
public:
	static void* winx_call allocate(size_t cb)						{ return CoTaskMemAlloc(cb); }
	static void* winx_call allocate(size_t cb, DestructorType fn)	{ return CoTaskMemAlloc(cb); }
	static void* winx_call allocate(size_t cb, int fnZero)			{ return CoTaskMemAlloc(cb); }
	
	static void winx_call deallocate(void* p)			{ CoTaskMemFree(p); }
	static void winx_call deallocate(void* p, size_t)	{ CoTaskMemFree(p); }
	static void winx_call swap(CoTaskAlloc& o)			{}
	
	template <class Type>
	static void winx_call destroy(Type* obj)
	{
		obj->~Type();
		CoTaskMemFree(obj);
	}

	template <class Type>
	static Type* winx_call newArray(size_t count, Type* zero)
	{
		Type* array = (Type*)CoTaskMemAlloc(sizeof(Type) * count);
		return ConstructorTraits<Type>::constructArray(array, count);
	}

	template <class Type>
	static void winx_call destroyArray(Type* array, size_t count)
	{
		DestructorTraits<Type>::destructArrayN(array, count);
		CoTaskMemFree(array);
	}

	__STD_FAKE_DBG_ALLOCATE();
};

#endif

// -------------------------------------------------------------------------
// HeapMemAllocBase, HeapMemAlloc

template <DWORD uFlags = HEAP_GENERATE_EXCEPTIONS>
class HeapMemAllocBase
{
public:
	HANDLE m_hHeap;

public:
	HeapMemAllocBase(HANDLE hHeap) : m_hHeap(hHeap) {}

	void* winx_call allocate(size_t cb)					   { return HeapAlloc(m_hHeap, uFlags, cb); }
	void* winx_call allocate(size_t cb, DestructorType fn) { return HeapAlloc(m_hHeap, uFlags, cb); }
	void* winx_call allocate(size_t cb, int fnZero)		   { return HeapAlloc(m_hHeap, uFlags, cb); }
	
	void winx_call deallocate(void* p)					{ HeapFree(m_hHeap, uFlags, p); }
	void winx_call deallocate(void* p, size_t)			{ HeapFree(m_hHeap, uFlags, p); }	
	void winx_call swap(HeapMemAllocBase& o)			{ std::swap(m_hHeap, o.m_hHeap); }
	
	template <class Type>
	void winx_call destroy(Type* obj)
	{
		obj->~Type();
		HeapFree(m_hHeap, uFlags, obj);
	}

	template <class Type>
	Type* winx_call newArray(size_t count, Type* zero)
	{
		Type* array = (Type*)HeapAlloc(m_hHeap, uFlags, sizeof(Type) * count);
		return ConstructorTraits<Type>::constructArray(array, count);
	}

	template <class Type>
	void winx_call destroyArray(Type* array, size_t count)
	{
		DestructorTraits<Type>::destructArrayN(array, count);
		HeapFree(m_hHeap, uFlags, array);
	}

	__STD_FAKE_DBG_ALLOCATE();
};

template <class Unused>
class HeapAllocT
{
public:
	static HANDLE hProcessHeap;

	static void* winx_call allocate(size_t cb)					  { return HeapAlloc(hProcessHeap, 0, cb); }
	static void* winx_call allocate(size_t cb, DestructorType fn) { return HeapAlloc(hProcessHeap, 0, cb); }
	static void* winx_call allocate(size_t cb, int fnZero)		  { return HeapAlloc(hProcessHeap, 0, cb); }
	
	static void winx_call deallocate(void* p)				{ HeapFree(hProcessHeap, 0, p); }
	static void winx_call deallocate(void* p, size_t)		{ HeapFree(hProcessHeap, 0, p); }
	static void winx_call swap(HeapAllocT& o)				{}
	
	template <class Type>
	static void winx_call destroy(Type* obj)
	{
		obj->~Type();
		HeapFree(hProcessHeap, 0, obj);
	}

	template <class Type>
	static Type* winx_call newArray(size_t count, Type* zero)
	{
		Type* array = (Type*)HeapAlloc(hProcessHeap, 0, sizeof(Type) * count);
		return ConstructorTraits<Type>::constructArray(array, count);
	}

	template <class Type>
	static void winx_call destroyArray(Type* array, size_t count)
	{
		DestructorTraits<Type>::destructArrayN(array, count);
		HeapFree(hProcessHeap, 0, array);
	}

	__STD_FAKE_DBG_ALLOCATE();
};

template <class Unused>
HANDLE HeapAllocT<Unused>::hProcessHeap = ::GetProcessHeap();

typedef HeapAllocT<int> HeapMemAlloc;

// -------------------------------------------------------------------------
// Allocator Selector

typedef StdLibAlloc DefaultStaticAlloc;
typedef HeapMemAlloc DefaultDynamicAlloc;

typedef StdLibAlloc SystemAlloc;

// -------------------------------------------------------------------------
// $Log: $

__NS_STD_END

#endif /* __STDEXT_MEMORY_SYSTEMALLOC_H__ */
