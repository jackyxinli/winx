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
// Module: stdext/memory/AutoFreeAlloc.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 12:50:46
// 
// $Id: AutoFreeAlloc.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_AUTOFREEALLOC_H__
#define __STDEXT_MEMORY_AUTOFREEALLOC_H__

#ifndef __STDEXT_MEMORY_BASIC_H__
#include "Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class AutoFreeAlloc

template <class _Alloc, int _MemBlockSize = MEMORY_BLOCK_SIZE>
class AutoFreeAllocT
{
public:
	enum { MemBlockSize = _MemBlockSize };
	enum { HeaderSize = sizeof(void*) };
	enum { BlockSize = MemBlockSize - HeaderSize };
	enum { IsAutoFreeAllocator = 1 };

private:
	struct _MemBlock
	{
		_MemBlock* pPrev;
		char buffer[BlockSize];
	};
	struct _DestroyNode
	{
		_DestroyNode* pPrev;
		DestructorType fnDestroy;
	};
	
	char* m_begin;
	char* m_end;
	_DestroyNode* m_destroyChain;
	_Alloc m_alloc;

private:
	_MemBlock* winx_call _ChainHeader() const
	{
		return (_MemBlock*)(m_begin - HeaderSize);
	}

	const AutoFreeAllocT& operator=(const AutoFreeAllocT& rhs);

public:
	AutoFreeAllocT() : m_destroyChain(NULL)
	{
		m_begin = m_end = (char*)HeaderSize;
	}
	explicit AutoFreeAllocT(_Alloc alloc) : m_alloc(alloc), m_destroyChain(NULL)
	{
		m_begin = m_end = (char*)HeaderSize;
	}
	explicit AutoFreeAllocT(AutoFreeAllocT& owner)
		: m_alloc(owner.m_alloc), m_destroyChain(NULL)
	{
		m_begin = m_end = (char*)HeaderSize;
	}

	~AutoFreeAllocT()
	{
		clear();
	}

	void winx_call swap(AutoFreeAllocT& o)
	{
		std::swap(m_begin, o.m_begin);
		std::swap(m_end, o.m_end);
		std::swap(m_destroyChain, o.m_destroyChain);
		m_alloc.swap(o.m_alloc);
	}

	void winx_call clear()
	{
		while (m_destroyChain)
		{
			_DestroyNode* curr = m_destroyChain;
			m_destroyChain = m_destroyChain->pPrev;
			curr->fnDestroy(curr + 1);
		}
		_MemBlock* pHeader = _ChainHeader();
		while (pHeader)
		{
			_MemBlock* pTemp = pHeader->pPrev;
			m_alloc.deallocate(pHeader);
			pHeader = pTemp;
		}
		m_begin = m_end = (char*)HeaderSize;
	}

	template <class Type>
	void winx_call destroy(Type* obj)
	{
		// no action
	}

	template <class Type>
	Type* winx_call newArray(size_t count, Type* zero)
	{
		Type* array = (Type*)DestructorTraits<Type>::allocArrayBuf(*this, count);
		return ConstructorTraits<Type>::constructArray(array, count);
	}

	template <class Type>
	void winx_call destroyArray(Type* array, size_t count)
	{
		// no action
	}

	void* winx_call allocate(size_t cb)
	{
		if ((size_t)(m_end - m_begin) < cb)
		{
			if (cb >= BlockSize)
			{
				_MemBlock* pHeader = _ChainHeader();
				_MemBlock* pNew = (_MemBlock*)m_alloc.allocate(HeaderSize + cb);
				if (pHeader)
				{
					pNew->pPrev = pHeader->pPrev;
					pHeader->pPrev = pNew;
				}
				else
				{
					m_end = m_begin = pNew->buffer;
					pNew->pPrev = NULL;
				}
				return pNew->buffer;
			}
			else
			{
				_MemBlock* pNew = (_MemBlock*)m_alloc.allocate(sizeof(_MemBlock));
				pNew->pPrev = _ChainHeader();
				m_begin = pNew->buffer;
				m_end = m_begin + BlockSize;
			}
		}
		return m_end -= cb;
	}

	void* winx_call reserve(size_t cb)
	{
		void* p = allocate(cb);
		m_end += cb;
		return p;
	}

	void* winx_call allocate(size_t cb, DestructorType fn)
	{
		_DestroyNode* pNode = (_DestroyNode*)allocate(sizeof(_DestroyNode) + cb);
		pNode->fnDestroy = fn;
		pNode->pPrev = m_destroyChain;
		m_destroyChain = pNode;
		return pNode + 1;
	}

	void* winx_call allocate(size_t cb, int fnZero)
	{
		return allocate(cb);
	}

	void winx_call deallocate(void* p, size_t cb)
	{
		// no action
	}

	__STD_FAKE_DBG_ALLOCATE();
};

typedef AutoFreeAllocT<DefaultStaticAlloc> AutoFreeAlloc;

// -------------------------------------------------------------------------
// class TestAutoFreeAlloc

template <class LogT>
class TestAutoFreeAlloc : public TestCase
{
	WINX_TEST_SUITE(TestAutoFreeAlloc);
		WINX_TEST(testBasic);
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
	};

	void testBasic(LogT& log)
	{
		std::AutoFreeAlloc alloc;
		
		Obj* o1 = STD_NEW(alloc, Obj)(1);
		Obj* o2 = STD_NEW_ARRAY(alloc, Obj, 8);
		Obj* o3 = STD_ALLOC_ARRAY(alloc, Obj, 10); // NOTE
		
		char* s1 = STD_ALLOC(alloc, char);
		char* s2 = STD_ALLOC_ARRAY(alloc, char, 100);
		memcpy(s2, "hello\n", 7);
		printf(s2);

		int* i1 = STD_NEW(alloc, int)(3);
		int* i2 = STD_NEW_ARRAY(alloc, int, 80);
	}
};

// -------------------------------------------------------------------------
// $Log: AutoFreeAlloc.h,v $

__NS_STD_END

#endif /* __STDEXT_MEMORY_AUTOFREEALLOC_H__ */
