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
// Module: mfcport/afxtempl.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-26 11:19:51
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __MFCPORT_AFXTEMPL_H__
#define __MFCPORT_AFXTEMPL_H__

#ifndef __MFCPORT_AFX_H__
#include "afx.h"
#endif

#ifndef _VECTOR_
#include <vector>
#endif

#ifdef _AFX_PACKING
#pragma pack(push, _AFX_PACKING)
#endif

/////////////////////////////////////////////////////////////////////////////
// CopyElements, DumpElements, CompareElements

template<class TYPE>
AFX_INLINE void AFXAPI CopyElements(TYPE* pDest, const TYPE* pSrc, INT_PTR nCount)
{
	ENSURE(nCount == 0 || pDest != 0 && pSrc != 0);
	ASSERT(nCount == 0 ||
		AfxIsValidAddress(pDest, (size_t)nCount * sizeof(TYPE)));
	ASSERT(nCount == 0 ||
		AfxIsValidAddress(pSrc, (size_t)nCount * sizeof(TYPE)));

	// default is element-copy using assignment
	while (nCount--)
		*pDest++ = *pSrc++;
}

#ifdef _DEBUG
template<class TYPE>
void AFXAPI DumpElements(CDumpContext& dc, const TYPE* pElements, INT_PTR nCount)
{
	ENSURE(nCount == 0 || pElements != NULL);
	ASSERT(nCount == 0 ||
		AfxIsValidAddress(pElements, (size_t)nCount * sizeof(TYPE), FALSE));
	&dc; // not used
	pElements;  // not used
	nCount; // not used

	// default does nothing
}
#endif

template<class TYPE, class ARG_TYPE>
BOOL AFXAPI CompareElements(const TYPE* pElement1, const ARG_TYPE* pElement2)
{
	ENSURE(pElement1 != NULL && pElement2 != NULL);
	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));
	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	return *pElement1 == *pElement2;
}

/////////////////////////////////////////////////////////////////////////////
// HashKey

template<class ARG_KEY>
AFX_INLINE UINT AFXAPI HashKey(ARG_KEY key)
{
	// default identity hash - works for most primitive values
	return (DWORD)(((DWORD_PTR)key)>>4);
}

// special versions for CString
#if _MSC_VER >= 1100
template<> UINT AFXAPI HashKey<LPCWSTR> (LPCWSTR key);
template<> UINT AFXAPI HashKey<LPCSTR> (LPCSTR key);
#else // _MSC_VER >= 1100
UINT AFXAPI HashKey(LPCWSTR key);
UINT AFXAPI HashKey(LPCSTR key);
#endif // _MSC_VER >= 1100

// special versions for CComBSTR
template<> UINT AFXAPI HashKey<CComBSTR>(CComBSTR key);

struct tagVARIANT;

#if _MSC_VER >= 1100
template<> UINT AFXAPI HashKey<const struct tagVARIANT&> (const struct tagVARIANT& var);
#else // _MSC_VER >= 1100
UINT AFXAPI HashKey(const struct tagVARIANT& var);
#endif // _MSC_VER >= 1100

/////////////////////////////////////////////////////////////////////////////
// CArray<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE = const TYPE&>
class CArray
{
private:
	typedef std::vector<TYPE> Impl;
	Impl m_impl;

public:
// Attributes
	BOOL IsEmpty() const	{ return m_impl.empty(); }
	INT_PTR GetSize() const { return m_impl.size(); }
	INT_PTR GetCount() const { return m_impl.size(); }
	INT_PTR GetUpperBound() const { return m_impl.size() - 1; }

	void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1) {
		m_impl.resize(nNewSize);
	}

// Operations

	// Clean up
	void FreeExtra() {
		Impl vec = m_impl;
		vec.swap(*this);
	}

	void RemoveAll() { m_impl.clear(); }

	// Accessing elements
	const TYPE& GetAt(INT_PTR nIndex) const { return m_impl[nIndex]; }
	TYPE& GetAt(INT_PTR nIndex) { return m_impl[nIndex]; }

	void SetAt(INT_PTR nIndex, ARG_TYPE newElement) {
		ASSERT(nIndex >= 0);
		m_impl[nIndex] = newElement;
	}
	const TYPE& ElementAt(INT_PTR nIndex) const {
		ASSERT(nIndex >= 0);
		return m_impl[nIndex];
	}
	TYPE& ElementAt(INT_PTR nIndex) {
		ASSERT(nIndex >= 0);
		return m_impl[nIndex];
	}

	// Direct Access to the element data (may return NULL)
	const TYPE* GetData() const {
		if (m_impl.empty())
			return NULL;
		return &m_impl[0];
	}
	TYPE* GetData() {
		if (m_impl.empty())
			return NULL;
		return &m_impl[0];
	}

	// Potentially growing the array
	void SetAtGrow(INT_PTR nIndex, ARG_TYPE newElement) {
		ASSERT(nIndex >= 0);
		if (nIndex >= m_impl.size())
			m_impl.resize(nIndex + 1);
		m_impl[nIndex] = newElement;
	}

	INT_PTR Add(ARG_TYPE newElement) {
		m_impl.push_back(newElement);
	}

	INT_PTR Append(const CArray& src) {
		m_impl.insert(m_impl.end(), src.m_impl.begin(), src.m_impl.end());
	}

	void Copy(const CArray& src) {
		m_impl = src.m_impl;
	}

	// overloaded operator helpers
	const TYPE& operator[](INT_PTR nIndex) const {
		return m_impl[nIndex];
	}
	TYPE& operator[](INT_PTR nIndex) {
		return m_impl[nIndex];
	}

	// Operations that move elements around
	void InsertAt(INT_PTR nIndex, ARG_TYPE newElement, INT_PTR nCount = 1) {
		if (nIndex >= m_impl.size()) {
			m_impl.reserve(nIndex + nCount);
			m_impl.resize(nIndex);
		}
		m_impl.insert(m_impl.begin()+nIndex, nCount, newElement);
	}

	void RemoveAt(INT_PTR nIndex, INT_PTR nCount = 1) {
		ASSERT_VALID(this);
		ASSERT(nIndex >= 0);
		ASSERT(nCount >= 0);
		INT_PTR nUpperBound = nIndex + nCount;
		ASSERT(nUpperBound <= m_nSize && nUpperBound >= nIndex && nUpperBound >= nCount);
		if(nIndex < 0 || nCount < 0 || (nUpperBound > m_nSize) || (nUpperBound < nIndex) || (nUpperBound < nCount))
			AfxThrowInvalidArgException();
		m_impl.erase(m_impl.begin()+nIndex, m_impl.begin()+(nIndex+nCount));
	}

	void InsertAt(INT_PTR nStartIndex, CArray* pNewArray) {
		ASSERT_VALID(this);
		ASSERT(pNewArray != NULL);
		ASSERT_VALID(pNewArray);
		ASSERT(nStartIndex >= 0);
		if(pNewArray == NULL || nStartIndex < 0)
			AfxThrowInvalidArgException();
		if (pNewArray->GetSize() > 0) {
			InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
			for (INT_PTR i = 0; i < pNewArray->GetSize(); i++)
				SetAt(nStartIndex + i, pNewArray->GetAt(i));
		}
	}
};

/////////////////////////////////////////////////////////////////////////////
// CList<TYPE, ARG_TYPE>
/*
template<class TYPE, class ARG_TYPE = const TYPE&>
class CList
{
protected:
	struct CNode
	{
		CNode* pNext;
		CNode* pPrev;
		TYPE data;
	};
public:
// Construction
	explicit CList(INT_PTR nBlockSize = 10);

// Attributes (head and tail)
	// count of elements
	INT_PTR GetCount() const;
	INT_PTR GetSize() const;
	BOOL IsEmpty() const;

	// peek at head or tail
	TYPE& GetHead();
	const TYPE& GetHead() const;
	TYPE& GetTail();
	const TYPE& GetTail() const;

// Operations
	// get head or tail (and remove it) - don't call on empty list !
	TYPE RemoveHead();
	TYPE RemoveTail();

	// add before head or after tail
	POSITION AddHead(ARG_TYPE newElement);
	POSITION AddTail(ARG_TYPE newElement);

	// add another list of elements before head or after tail
	void AddHead(CList* pNewList);
	void AddTail(CList* pNewList);

	// remove all elements
	void RemoveAll();

	// iteration
	POSITION GetHeadPosition() const;
	POSITION GetTailPosition() const;
	TYPE& GetNext(POSITION& rPosition); // return *Position++
	const TYPE& GetNext(POSITION& rPosition) const; // return *Position++
	TYPE& GetPrev(POSITION& rPosition); // return *Position--
	const TYPE& GetPrev(POSITION& rPosition) const; // return *Position--

	// getting/modifying an element at a given position
	TYPE& GetAt(POSITION position);
	const TYPE& GetAt(POSITION position) const;
	void SetAt(POSITION pos, ARG_TYPE newElement);
	void RemoveAt(POSITION position);

	// inserting before or after a given position
	POSITION InsertBefore(POSITION position, ARG_TYPE newElement);
	POSITION InsertAfter(POSITION position, ARG_TYPE newElement);

	// helper functions (note: O(n) speed)
	POSITION Find(ARG_TYPE searchValue, POSITION startAfter = NULL) const;
		// defaults to starting at the HEAD, return NULL if not found
	POSITION FindIndex(INT_PTR nIndex) const;
		// get the 'nIndex'th element (may return NULL)

// Implementation
protected:
	CNode* m_pNodeHead;
	CNode* m_pNodeTail;
	INT_PTR m_nCount;
	CNode* m_pNodeFree;
	struct CPlex* m_pBlocks;
	INT_PTR m_nBlockSize;

	CNode* NewNode(CNode*, CNode*);
	void FreeNode(CNode*);

public:
	~CList();
	void Serialize(CArchive&);
#ifdef _DEBUG
	void Dump(CDumpContext&) const;
	void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CList<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE>
AFX_INLINE INT_PTR CList<TYPE, ARG_TYPE>::GetCount() const
	{ return m_nCount; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE INT_PTR CList<TYPE, ARG_TYPE>::GetSize() const
	{ return m_nCount; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE BOOL CList<TYPE, ARG_TYPE>::IsEmpty() const
	{ return m_nCount == 0; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE TYPE& CList<TYPE, ARG_TYPE>::GetHead()
	{ ASSERT(m_pNodeHead != NULL);
		return m_pNodeHead->data; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE const TYPE& CList<TYPE, ARG_TYPE>::GetHead() const
	{ ASSERT(m_pNodeHead != NULL);
		return m_pNodeHead->data; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE TYPE& CList<TYPE, ARG_TYPE>::GetTail()
	{ ASSERT(m_pNodeTail != NULL);
		return m_pNodeTail->data; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE const TYPE& CList<TYPE, ARG_TYPE>::GetTail() const
	{ ASSERT(m_pNodeTail != NULL);
		return m_pNodeTail->data; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE POSITION CList<TYPE, ARG_TYPE>::GetHeadPosition() const
	{ return (POSITION) m_pNodeHead; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE POSITION CList<TYPE, ARG_TYPE>::GetTailPosition() const
	{ return (POSITION) m_pNodeTail; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE TYPE& CList<TYPE, ARG_TYPE>::GetNext(POSITION& rPosition) // return *Position++
	{ CNode* pNode = (CNode*) rPosition;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		rPosition = (POSITION) pNode->pNext;
		return pNode->data; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE const TYPE& CList<TYPE, ARG_TYPE>::GetNext(POSITION& rPosition) const // return *Position++
	{ CNode* pNode = (CNode*) rPosition;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		rPosition = (POSITION) pNode->pNext;
		return pNode->data; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE TYPE& CList<TYPE, ARG_TYPE>::GetPrev(POSITION& rPosition) // return *Position--
	{ CNode* pNode = (CNode*) rPosition;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		rPosition = (POSITION) pNode->pPrev;
		return pNode->data; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE const TYPE& CList<TYPE, ARG_TYPE>::GetPrev(POSITION& rPosition) const // return *Position--
	{ CNode* pNode = (CNode*) rPosition;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		rPosition = (POSITION) pNode->pPrev;
		return pNode->data; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE TYPE& CList<TYPE, ARG_TYPE>::GetAt(POSITION position)
	{ CNode* pNode = (CNode*) position;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		return pNode->data; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE const TYPE& CList<TYPE, ARG_TYPE>::GetAt(POSITION position) const
	{ CNode* pNode = (CNode*) position;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		return pNode->data; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE void CList<TYPE, ARG_TYPE>::SetAt(POSITION pos, ARG_TYPE newElement)
	{ CNode* pNode = (CNode*) pos;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		pNode->data = newElement; }

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::CList(INT_PTR nBlockSize)
{
	ASSERT(nBlockSize > 0);

	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::RemoveAll()
{
	ASSERT_VALID(this);

	// destroy elements
	CNode* pNode;
	for (pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
		pNode->data.~TYPE();

	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::~CList()
{
	RemoveAll();
	ASSERT(m_nCount == 0);
}

/////////////////////////////////////////////////////////////////////////////
// Node helpers
//
// Implementation note: CNode's are stored in CPlex blocks and
//  chained together. Free blocks are maintained in a singly linked list
//  using the 'pNext' member of CNode with 'm_pNodeFree' as the head.
//  Used blocks are maintained in a doubly linked list using both 'pNext'
//  and 'pPrev' as links and 'm_pNodeHead' and 'm_pNodeTail'
//   as the head/tail.
//
// We never free a CPlex block unless the List is destroyed or RemoveAll()
//  is used - so the total number of CPlex blocks may grow large depending
//  on the maximum past size of the list.
//

template<class TYPE, class ARG_TYPE>
typename CList<TYPE, ARG_TYPE>::CNode*
CList<TYPE, ARG_TYPE>::NewNode(CNode* pPrev, CNode* pNext)
{
	if (m_pNodeFree == NULL)
	{
		// add another block
		CPlex* pNewBlock = CPlex::Create(m_pBlocks, m_nBlockSize,
				 sizeof(CNode));

		// chain them into free list
		CNode* pNode = (CNode*) pNewBlock->data();
		// free in reverse order to make it easier to debug
		pNode += m_nBlockSize - 1;
		for (INT_PTR i = m_nBlockSize-1; i >= 0; i--, pNode--)
		{
			pNode->pNext = m_pNodeFree;
			m_pNodeFree = pNode;
		}
	}
	ENSURE(m_pNodeFree != NULL);  // we must have something

	CList::CNode* pNode = m_pNodeFree;
	m_pNodeFree = m_pNodeFree->pNext;
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
	m_nCount++;
	ASSERT(m_nCount > 0);  // make sure we don't overflow

#pragma push_macro("new")
#undef new
	::new( (void*)( &pNode->data ) ) TYPE;
#pragma pop_macro("new")
	return pNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::FreeNode(CNode* pNode)
{
	pNode->data.~TYPE();
	pNode->pNext = m_pNodeFree;
	m_pNodeFree = pNode;
	m_nCount--;
	ASSERT(m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::AddHead(ARG_TYPE newElement)
{
	ASSERT_VALID(this);

	CNode* pNewNode = NewNode(NULL, m_pNodeHead);
	pNewNode->data = newElement;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = pNewNode;
	else
		m_pNodeTail = pNewNode;
	m_pNodeHead = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::AddTail(ARG_TYPE newElement)
{
	ASSERT_VALID(this);

	CNode* pNewNode = NewNode(m_pNodeTail, NULL);
	pNewNode->data = newElement;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = pNewNode;
	else
		m_pNodeHead = pNewNode;
	m_pNodeTail = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AddHead(CList* pNewList)
{
	ASSERT_VALID(this);

	ENSURE(pNewList != NULL);
	ASSERT_VALID(pNewList);

	// add a list of same elements to head (maintain order)
	POSITION pos = pNewList->GetTailPosition();
	while (pos != NULL)
		AddHead(pNewList->GetPrev(pos));
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AddTail(CList* pNewList)
{
	ASSERT_VALID(this);
	ENSURE(pNewList != NULL);
	ASSERT_VALID(pNewList);

	// add a list of same elements
	POSITION pos = pNewList->GetHeadPosition();
	while (pos != NULL)
		AddTail(pNewList->GetNext(pos));
}

template<class TYPE, class ARG_TYPE>
TYPE CList<TYPE, ARG_TYPE>::RemoveHead()
{
	ASSERT_VALID(this);
	ASSERT(m_pNodeHead != NULL);  // don't call on empty list !!!
	ASSERT(AfxIsValidAddress(m_pNodeHead, sizeof(CNode)));

	CNode* pOldNode = m_pNodeHead;
	TYPE returnValue = pOldNode->data;

	m_pNodeHead = pOldNode->pNext;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = NULL;
	else
		m_pNodeTail = NULL;
	FreeNode(pOldNode);
	return returnValue;
}

template<class TYPE, class ARG_TYPE>
TYPE CList<TYPE, ARG_TYPE>::RemoveTail()
{
	ASSERT_VALID(this);
	ASSERT(m_pNodeTail != NULL);  // don't call on empty list !!!
	ASSERT(AfxIsValidAddress(m_pNodeTail, sizeof(CNode)));

	CNode* pOldNode = m_pNodeTail;
	TYPE returnValue = pOldNode->data;

	m_pNodeTail = pOldNode->pPrev;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = NULL;
	else
		m_pNodeHead = NULL;
	FreeNode(pOldNode);
	return returnValue;
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::InsertBefore(POSITION position, ARG_TYPE newElement)
{
	ASSERT_VALID(this);

	if (position == NULL)
		return AddHead(newElement); // insert before nothing -> head of the list

	// Insert it before position
	CNode* pOldNode = (CNode*) position;
	CNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
	pNewNode->data = newElement;

	if (pOldNode->pPrev != NULL)
	{
		ASSERT(AfxIsValidAddress(pOldNode->pPrev, sizeof(CNode)));
		pOldNode->pPrev->pNext = pNewNode;
	}
	else
	{
		ASSERT(pOldNode == m_pNodeHead);
		m_pNodeHead = pNewNode;
	}
	pOldNode->pPrev = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::InsertAfter(POSITION position, ARG_TYPE newElement)
{
	ASSERT_VALID(this);

	if (position == NULL)
		return AddTail(newElement); // insert after nothing -> tail of the list

	// Insert it before position
	CNode* pOldNode = (CNode*) position;
	ASSERT(AfxIsValidAddress(pOldNode, sizeof(CNode)));
	CNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
	pNewNode->data = newElement;

	if (pOldNode->pNext != NULL)
	{
		ASSERT(AfxIsValidAddress(pOldNode->pNext, sizeof(CNode)));
		pOldNode->pNext->pPrev = pNewNode;
	}
	else
	{
		ASSERT(pOldNode == m_pNodeTail);
		m_pNodeTail = pNewNode;
	}
	pOldNode->pNext = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::RemoveAt(POSITION position)
{
	ASSERT_VALID(this);

	CNode* pOldNode = (CNode*) position;
	ASSERT(AfxIsValidAddress(pOldNode, sizeof(CNode)));

	// remove pOldNode from list
	if (pOldNode == m_pNodeHead)
	{
		m_pNodeHead = pOldNode->pNext;
	}
	else
	{
		ASSERT(AfxIsValidAddress(pOldNode->pPrev, sizeof(CNode)));
		pOldNode->pPrev->pNext = pOldNode->pNext;
	}
	if (pOldNode == m_pNodeTail)
	{
		m_pNodeTail = pOldNode->pPrev;
	}
	else
	{
		ASSERT(AfxIsValidAddress(pOldNode->pNext, sizeof(CNode)));
		pOldNode->pNext->pPrev = pOldNode->pPrev;
	}
	FreeNode(pOldNode);
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::FindIndex(INT_PTR nIndex) const
{
	ASSERT_VALID(this);

	if (nIndex >= m_nCount || nIndex < 0)
		return NULL;  // went too far

	CNode* pNode = m_pNodeHead;
	while (nIndex--)
	{
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		pNode = pNode->pNext;
	}
	return (POSITION) pNode;
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::Find(ARG_TYPE searchValue, POSITION startAfter) const
{
	ASSERT_VALID(this);

	CNode* pNode = (CNode*) startAfter;
	if (pNode == NULL)
	{
		pNode = m_pNodeHead;  // start at head
	}
	else
	{
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		pNode = pNode->pNext;  // start after the one specified
	}

	for (; pNode != NULL; pNode = pNode->pNext)
		if (CompareElements<TYPE>(&pNode->data, &searchValue))
			return (POSITION)pNode;
	return NULL;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AssertValid() const
{
	CObject::AssertValid();

	if (m_nCount == 0)
	{
		// empty list
		ASSERT(m_pNodeHead == NULL);
		ASSERT(m_pNodeTail == NULL);
	}
	else
	{
		// non-empty list
		ASSERT(AfxIsValidAddress(m_pNodeHead, sizeof(CNode)));
		ASSERT(AfxIsValidAddress(m_pNodeTail, sizeof(CNode)));
	}
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CMap : public CObject
{
public:
	// CPair
	struct CPair
	{
		const KEY key;
		VALUE value;
	protected:
		CPair( ARG_KEY keyval ) : key( keyval )	{}
	};

protected:
	// Association
	class CAssoc : public CPair
	{
		friend class CMap<KEY,ARG_KEY,VALUE,ARG_VALUE>;
		CAssoc* pNext;
		UINT nHashValue;  // needed for efficient iteration
	public:
		CAssoc( ARG_KEY key ) : CPair( key ) {}
	};

public:
// Construction
	explicit CMap(INT_PTR nBlockSize = 10);

// Attributes
	// number of elements
	INT_PTR GetCount() const;
	INT_PTR GetSize() const;
	BOOL IsEmpty() const;

	// Lookup
	BOOL Lookup(ARG_KEY key, VALUE& rValue) const;
	const CPair *PLookup(ARG_KEY key) const;
	CPair *PLookup(ARG_KEY key);

// Operations
	// Lookup and add if not there
	VALUE& operator[](ARG_KEY key);

	// add a new (key, value) pair
	void SetAt(ARG_KEY key, ARG_VALUE newValue);

	// removing existing (key, ?) pair
	BOOL RemoveKey(ARG_KEY key);
	void RemoveAll();

	// iterating all (key, value) pairs
	POSITION GetStartPosition() const;

	const CPair *PGetFirstAssoc() const;
	CPair *PGetFirstAssoc();

	void GetNextAssoc(POSITION& rNextPosition, KEY& rKey, VALUE& rValue) const;

	const CPair *PGetNextAssoc(const CPair *pAssocRet) const;
	CPair *PGetNextAssoc(const CPair *pAssocRet);

	// advanced features for derived classes
	UINT GetHashTableSize() const;
	void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

// Implementation
protected:
	CAssoc** m_pHashTable;
	UINT m_nHashTableSize;
	INT_PTR m_nCount;
	CAssoc* m_pFreeList;
	struct CPlex* m_pBlocks;
	INT_PTR m_nBlockSize;

	CAssoc* NewAssoc(ARG_KEY key);
	void FreeAssoc(CAssoc*);
	CAssoc* GetAssocAt(ARG_KEY, UINT&, UINT&) const;

public:
	~CMap();
	void Serialize(CArchive&);
#ifdef _DEBUG
	void Dump(CDumpContext&) const;
	void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CMap<KEY, ARG_KEY, VALUE, ARG_VALUE> inline functions

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
AFX_INLINE INT_PTR CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetCount() const
	{ return m_nCount; }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
AFX_INLINE INT_PTR CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetSize() const
	{ return m_nCount; }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
AFX_INLINE BOOL CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::IsEmpty() const
	{ return m_nCount == 0; }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
AFX_INLINE void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::SetAt(ARG_KEY key, ARG_VALUE newValue)
	{ (*this)[key] = newValue; }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
AFX_INLINE POSITION CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetStartPosition() const
	{ return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
const typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CPair* CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::PGetFirstAssoc() const
{ 
	ASSERT_VALID(this);
	if(m_nCount == 0) return NULL;

	ASSERT(m_pHashTable != NULL);  // never call on empty map

	CAssoc* pAssocRet = (CAssoc*)BEFORE_START_POSITION;

	// find the first association
	for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
		if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
			break;
	ASSERT(pAssocRet != NULL);  // must find something

	return pAssocRet;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CPair* CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::PGetFirstAssoc()
{ 
	ASSERT_VALID(this);
	if(m_nCount == 0) return NULL;

	ASSERT(m_pHashTable != NULL);  // never call on empty map

	CAssoc* pAssocRet = (CAssoc*)BEFORE_START_POSITION;

	// find the first association
	for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
		if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
			break;
	ASSERT(pAssocRet != NULL);  // must find something

	return pAssocRet;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
AFX_INLINE UINT CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetHashTableSize() const
	{ return m_nHashTableSize; }

/////////////////////////////////////////////////////////////////////////////
// CMap<KEY, ARG_KEY, VALUE, ARG_VALUE> out-of-line functions

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CMap(INT_PTR nBlockSize)
{
	ASSERT(nBlockSize > 0);

	m_pHashTable = NULL;
	m_nHashTableSize = 17;  // default size
	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::InitHashTable(
	UINT nHashSize, BOOL bAllocNow)
//
// Used to force allocation of a hash table or to override the default
//   hash table size of (which is fairly small)
{
	ASSERT_VALID(this);
	ASSERT(m_nCount == 0);
	ASSERT(nHashSize > 0);

	if (m_pHashTable != NULL)
	{
		// free hash table
		delete[] m_pHashTable;
		m_pHashTable = NULL;
	}

	if (bAllocNow)
	{
		m_pHashTable = new CAssoc* [nHashSize];
		ENSURE(m_pHashTable != NULL);
		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
	}
	m_nHashTableSize = nHashSize;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveAll()
{
	ASSERT_VALID(this);

	if (m_pHashTable != NULL)
	{
		// destroy elements (values and keys)
		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			CAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext)
			{
				pAssoc->CAssoc::~CAssoc();
				//DestructElements<VALUE>(&pAssoc->value, 1);
				//DestructElements<KEY>((KEY*)&pAssoc->key, 1);
			}
		}
	}

	// free hash table
	delete[] m_pHashTable;
	m_pHashTable = NULL;

	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::~CMap()
{
	RemoveAll();
	ASSERT(m_nCount == 0);
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CAssoc*
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::NewAssoc(ARG_KEY key)
{
	if (m_pFreeList == NULL)
	{
		// add another block
		CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CMap::CAssoc));
		// chain them into free list
		CMap::CAssoc* pAssoc = (CMap::CAssoc*) newBlock->data();
		// free in reverse order to make it easier to debug
		pAssoc += m_nBlockSize - 1;
		for (INT_PTR i = m_nBlockSize-1; i >= 0; i--, pAssoc--)
		{
			pAssoc->pNext = m_pFreeList;
			m_pFreeList = pAssoc;
		}
	}
	ENSURE(m_pFreeList != NULL);  // we must have something

	CMap::CAssoc* pAssoc = m_pFreeList;

	// zero the memory
	CMap::CAssoc* pTemp = pAssoc->pNext;
	memset( pAssoc, 0, sizeof(CMap::CAssoc) );
	pAssoc->pNext = pTemp;

	m_pFreeList = m_pFreeList->pNext;
	m_nCount++;
	ASSERT(m_nCount > 0);  // make sure we don't overflow
#pragma push_macro("new")
#undef new
	::new(pAssoc) CMap::CAssoc(key);
#pragma pop_macro("new")
//	ConstructElements<KEY>(&pAssoc->key, 1);
//	ConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values
	return pAssoc;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::FreeAssoc(CAssoc* pAssoc)
{
	pAssoc->CAssoc::~CAssoc();
//	DestructElements<VALUE>(&pAssoc->value, 1);
//	DestructElements<KEY>(&pAssoc->key, 1);
	pAssoc->pNext = m_pFreeList;
	m_pFreeList = pAssoc;
	m_nCount--;
	ASSERT(m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CAssoc*
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetAssocAt(ARG_KEY key, UINT& nHashBucket, UINT& nHashValue) const
// find association (or return NULL)
{
	nHashValue = HashKey<ARG_KEY>(key);
	nHashBucket = nHashValue % m_nHashTableSize;

	if (m_pHashTable == NULL)
		return NULL;

	// see if it exists
	CAssoc* pAssoc;
	for (pAssoc = m_pHashTable[nHashBucket]; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (pAssoc->nHashValue == nHashValue && CompareElements(&pAssoc->key, &key))
			return pAssoc;
	}
	return NULL;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
BOOL CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::Lookup(ARG_KEY key, VALUE& rValue) const
{
	ASSERT_VALID(this);

	UINT nHashBucket, nHashValue;
	CAssoc* pAssoc = GetAssocAt(key, nHashBucket, nHashValue);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	rValue = pAssoc->value;
	return TRUE;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
const typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CPair* CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::PLookup(ARG_KEY key) const
{
	ASSERT_VALID(this);

	UINT nHashBucket, nHashValue;
	CAssoc* pAssoc = GetAssocAt(key, nHashBucket, nHashValue);
	return pAssoc;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CPair* CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::PLookup(ARG_KEY key)
{
	ASSERT_VALID(this);

	UINT nHashBucket, nHashValue;
	CAssoc* pAssoc = GetAssocAt(key, nHashBucket, nHashValue);
	return pAssoc;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
VALUE& CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::operator[](ARG_KEY key)
{
	ASSERT_VALID(this);

	UINT nHashBucket, nHashValue;
	CAssoc* pAssoc;
	if ((pAssoc = GetAssocAt(key, nHashBucket, nHashValue)) == NULL)
	{
		if (m_pHashTable == NULL)
			InitHashTable(m_nHashTableSize);

		ENSURE(m_pHashTable);
		// it doesn't exist, add a new Association
		pAssoc = NewAssoc(key);
		pAssoc->nHashValue = nHashValue;
		//'pAssoc->value' is a constructed object, nothing more

		// put into hash table
		pAssoc->pNext = m_pHashTable[nHashBucket];
		m_pHashTable[nHashBucket] = pAssoc;
	}
	return pAssoc->value;  // return new reference
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
BOOL CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveKey(ARG_KEY key)
// remove key - return TRUE if removed
{
	ASSERT_VALID(this);

	if (m_pHashTable == NULL)
		return FALSE;  // nothing in the table

	UINT nHashValue;
	CAssoc** ppAssocPrev;
	nHashValue = HashKey<ARG_KEY>(key);
	ppAssocPrev = &m_pHashTable[nHashValue%m_nHashTableSize];

	CAssoc* pAssoc;
	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if ((pAssoc->nHashValue == nHashValue) && CompareElements(&pAssoc->key, &key))
		{
			// remove it
			*ppAssocPrev = pAssoc->pNext;  // remove from list
			FreeAssoc(pAssoc);
			return TRUE;
		}
		ppAssocPrev = &pAssoc->pNext;
	}
	return FALSE;  // not found
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNextAssoc(POSITION& rNextPosition,
	KEY& rKey, VALUE& rValue) const
{
	ASSERT_VALID(this);
	ENSURE(m_pHashTable != NULL);  // never call on empty map

	CAssoc* pAssocRet = (CAssoc*)rNextPosition;
	ENSURE(pAssocRet != NULL);

	if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)
	{
		// find the first association
		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
		{
			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
			{
				break;
			}
		}
		ENSURE(pAssocRet != NULL);  // must find something
	}

	// find next association
	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));
	CAssoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		// go to next bucket
		for (UINT nBucket = (pAssocRet->nHashValue % m_nHashTableSize) + 1;
		  nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}

	rNextPosition = (POSITION) pAssocNext;

	// fill in return data
	rKey = pAssocRet->key;
	rValue = pAssocRet->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
const typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CPair*
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::PGetNextAssoc(const typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CPair* pPairRet) const
{
	ASSERT_VALID(this);

	CAssoc* pAssocRet = (CAssoc*)pPairRet;

	ASSERT(m_pHashTable != NULL);  // never call on empty map
	ASSERT(pAssocRet != NULL);
	
	if(m_pHashTable == NULL || pAssocRet == NULL)
		return NULL;
		
	ASSERT(pAssocRet != (CAssoc*)BEFORE_START_POSITION);

	// find next association
	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));
	CAssoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		// go to next bucket
		for (UINT nBucket = (pAssocRet->nHashValue % m_nHashTableSize) + 1;
		  nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}

	return pAssocNext;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CPair*
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::PGetNextAssoc(const typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CPair* pPairRet)
{
	ASSERT_VALID(this);

	CAssoc* pAssocRet = (CAssoc*)pPairRet;

	ASSERT(m_pHashTable != NULL);  // never call on empty map
	ASSERT(pAssocRet != NULL);
	
	if(m_pHashTable == NULL || pAssocRet == NULL)
		return NULL;
		
	ASSERT(pAssocRet != (CAssoc*)BEFORE_START_POSITION);

	// find next association
	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));
	CAssoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		// go to next bucket
		for (UINT nBucket = (pAssocRet->nHashValue % m_nHashTableSize) + 1;
		  nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}

	return pAssocNext;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::AssertValid() const
{
	CObject::AssertValid();

	ASSERT(m_nHashTableSize > 0);
	ASSERT(m_nCount == 0 || m_pHashTable != NULL);
		// non-empty map should have hash table
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrArray<BASE_CLASS, TYPE>

template<class BASE_CLASS, class TYPE>
class CTypedPtrArray : public BASE_CLASS
{
public:
	// Accessing elements
	TYPE GetAt(INT_PTR nIndex) const
		{ return (TYPE)BASE_CLASS::GetAt(nIndex); }
	TYPE& ElementAt(INT_PTR nIndex)
		{ return (TYPE&)BASE_CLASS::ElementAt(nIndex); }
	void SetAt(INT_PTR nIndex, TYPE ptr)
		{ BASE_CLASS::SetAt(nIndex, ptr); }

	// Potentially growing the array
	void SetAtGrow(INT_PTR nIndex, TYPE newElement)
	   { BASE_CLASS::SetAtGrow(nIndex, newElement); }
	INT_PTR Add(TYPE newElement)
	   { return BASE_CLASS::Add(newElement); }
	INT_PTR Append(const CTypedPtrArray<BASE_CLASS, TYPE>& src)
	   { return BASE_CLASS::Append(src); }
	void Copy(const CTypedPtrArray<BASE_CLASS, TYPE>& src)
		{ BASE_CLASS::Copy(src); }

	// Operations that move elements around
	void InsertAt(INT_PTR nIndex, TYPE newElement, INT_PTR nCount = 1)
		{ BASE_CLASS::InsertAt(nIndex, newElement, nCount); }
	void InsertAt(INT_PTR nStartIndex, CTypedPtrArray<BASE_CLASS, TYPE>* pNewArray)
	   { BASE_CLASS::InsertAt(nStartIndex, pNewArray); }

	// overloaded operator helpers
	TYPE operator[](INT_PTR nIndex) const
		{ return (TYPE)BASE_CLASS::operator[](nIndex); }
	TYPE& operator[](INT_PTR nIndex)
		{ return (TYPE&)BASE_CLASS::operator[](nIndex); }
};

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrList<BASE_CLASS, TYPE>

template<class BASE_CLASS, class TYPE>
class _CTypedPtrList : public BASE_CLASS
{
public:
// Construction
	_CTypedPtrList(INT_PTR nBlockSize = 10)
		: BASE_CLASS(nBlockSize) { }

	// peek at head or tail
	TYPE& GetHead()
		{ return (TYPE&)BASE_CLASS::GetHead(); }
	TYPE GetHead() const
		{ return (TYPE)BASE_CLASS::GetHead(); }
	TYPE& GetTail()
		{ return (TYPE&)BASE_CLASS::GetTail(); }
	TYPE GetTail() const
		{ return (TYPE)BASE_CLASS::GetTail(); }

	// get head or tail (and remove it) - don't call on empty list!
	TYPE RemoveHead()
		{ return (TYPE)BASE_CLASS::RemoveHead(); }
	TYPE RemoveTail()
		{ return (TYPE)BASE_CLASS::RemoveTail(); }

	// iteration
	TYPE& GetNext(POSITION& rPosition)
		{ return (TYPE&)BASE_CLASS::GetNext(rPosition); }
	TYPE GetNext(POSITION& rPosition) const
		{ return (TYPE)BASE_CLASS::GetNext(rPosition); }
	TYPE& GetPrev(POSITION& rPosition)
		{ return (TYPE&)BASE_CLASS::GetPrev(rPosition); }
	TYPE GetPrev(POSITION& rPosition) const
		{ return (TYPE)BASE_CLASS::GetPrev(rPosition); }

	// getting/modifying an element at a given position
	TYPE& GetAt(POSITION position)
		{ return (TYPE&)BASE_CLASS::GetAt(position); }
	TYPE GetAt(POSITION position) const
		{ return (TYPE)BASE_CLASS::GetAt(position); }
	void SetAt(POSITION pos, TYPE newElement)
		{ BASE_CLASS::SetAt(pos, newElement); }

	// inserting before or after a given position
	POSITION InsertBefore(POSITION position, TYPE newElement)
		{ return BASE_CLASS::InsertBefore(position, newElement); }
	POSITION InsertAfter(POSITION position, TYPE newElement)
		{ return BASE_CLASS::InsertAfter(position, newElement); }

	// transfer before or after a given position
	// Transfer semantics ensure no leakage by deleting the element in the case of an exception
	POSITION TransferInsertBefore(POSITION position, TYPE newElement)
	{
		try
		{
			return BASE_CLASS::InsertBefore(position, newElement); 
		}
		catch(...)
		{
			delete newElement;
			throw;
		}
	}

	POSITION TransferInsertAfter(POSITION position, TYPE newElement)
	{
		try
		{
			return BASE_CLASS::InsertAfter(position, newElement); 
		}
		catch(...)
		{
			delete newElement;
			throw;
		}
	}
};

template<class BASE_CLASS, class TYPE>
class CTypedPtrList : public _CTypedPtrList<BASE_CLASS, TYPE>
{
public:
// Construction
	CTypedPtrList(INT_PTR nBlockSize = 10)
		: _CTypedPtrList<BASE_CLASS, TYPE>(nBlockSize) { }

	// add before head or after tail
	POSITION AddHead(TYPE newElement)
		{ return BASE_CLASS::AddHead(newElement); }
	POSITION AddTail(TYPE newElement)
		{ return BASE_CLASS::AddTail(newElement); }

	// transfer add before head or tail
	POSITION TransferAddHead(TYPE newElement)
	{ 
		try
		{
			return BASE_CLASS::AddHead(newElement); 
		}
		catch(...)
		{
			delete newElement;
			throw;
		}
	}
	POSITION TransferAddTail(TYPE newElement)
	{ 
		try
		{
			return BASE_CLASS::AddTail(newElement); 
		}
		catch(...)
		{
			delete newElement;
			throw;
		}
	}

	// add another list of elements before head or after tail
	void AddHead(CTypedPtrList<BASE_CLASS, TYPE>* pNewList)
		{ BASE_CLASS::AddHead(pNewList); }
	void AddTail(CTypedPtrList<BASE_CLASS, TYPE>* pNewList)
		{ BASE_CLASS::AddTail(pNewList); }
};

// need specialized version for CObList because of AddHead/Tail ambiguity
template<> class CTypedPtrList<CObList, CObList*>
	: public _CTypedPtrList<CObList, CObList*>
{
public:
// Construction
	CTypedPtrList(INT_PTR nBlockSize = 10)
		: _CTypedPtrList<CObList, CObList*>(nBlockSize) { }

	// add before head or after tail
	POSITION AddHead(CObList* newElement)
		{ return _CTypedPtrList<CObList, CObList*>::AddHead((CObject*)newElement); }
	POSITION AddTail(CObList* newElement)
		{ return _CTypedPtrList<CObList, CObList*>::AddTail((CObject*)newElement); }

	// add another list of elements before head or after tail
	void AddHead(CTypedPtrList<CObList, CObList*>* pNewList)
		{ _CTypedPtrList<CObList, CObList*>::AddHead(pNewList); }
	void AddTail(CTypedPtrList<CObList, CObList*>* pNewList)
		{ _CTypedPtrList<CObList, CObList*>::AddTail(pNewList); }
};

// need specialized version for CPtrList because of AddHead/Tail ambiguity
template<> class CTypedPtrList<CPtrList, CPtrList*>
	: public _CTypedPtrList<CPtrList, CPtrList*>
{
public:
// Construction
	CTypedPtrList(INT_PTR nBlockSize = 10)
		: _CTypedPtrList<CPtrList, CPtrList*>(nBlockSize) { }

	// add before head or after tail
	POSITION AddHead(CPtrList* newElement)
		{ return _CTypedPtrList<CPtrList, CPtrList*>::AddHead((void*)newElement); }
	POSITION AddTail(CPtrList* newElement)
		{ return _CTypedPtrList<CPtrList, CPtrList*>::AddTail((void*)newElement); }

	// add another list of elements before head or after tail
	void AddHead(CTypedPtrList<CPtrList, CPtrList*>* pNewList)
		{ _CTypedPtrList<CPtrList, CPtrList*>::AddHead(pNewList); }
	void AddTail(CTypedPtrList<CPtrList, CPtrList*>* pNewList)
		{ _CTypedPtrList<CPtrList, CPtrList*>::AddTail(pNewList); }
};

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrMap<BASE_CLASS, KEY, VALUE>

template<class BASE_CLASS, class KEY, class VALUE>
class CTypedPtrMap : public BASE_CLASS
{
public:

// Construction
	CTypedPtrMap(INT_PTR nBlockSize = 10)
		: BASE_CLASS(nBlockSize) { }

	// Lookup
	BOOL Lookup(typename BASE_CLASS::BASE_ARG_KEY key, VALUE& rValue) const
		{ return BASE_CLASS::Lookup(key, (BASE_CLASS::BASE_VALUE&)rValue); }

	// Lookup and add if not there
	VALUE& operator[](typename BASE_CLASS::BASE_ARG_KEY key)
		{ return (VALUE&)BASE_CLASS::operator[](key); }

	// add a new key (key, value) pair
	void SetAt(KEY key, VALUE newValue)
		{ BASE_CLASS::SetAt(key, newValue); }

	// removing existing (key, ?) pair
	BOOL RemoveKey(KEY key)
		{ return BASE_CLASS::RemoveKey(key); }

	// iteration
	void GetNextAssoc(POSITION& rPosition, KEY& rKey, VALUE& rValue) const
		{ BASE_CLASS::GetNextAssoc(rPosition, (BASE_CLASS::BASE_KEY&)rKey,
			(BASE_CLASS::BASE_VALUE&)rValue); }
};
*/
/////////////////////////////////////////////////////////////////////////////
// $Log: $

#endif /* __MFCPORT_AFXTEMPL_H__ */
