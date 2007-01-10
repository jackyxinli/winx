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
// Module: winx/com/atl/atlcomx.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-26 1:06:56
// 
// $Id: atlcomx.h,v 1.3 2006/08/26 09:12:37 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_COM_ATL_ATLCOMX_H__
#define __WINX_COM_ATL_ATLCOMX_H__

#ifdef _SECURE_ATL
#define ocscpy(dest, src)		wcscpy(dest, src)
#endif

namespace ATLX {

// -------------------------------------------------------------------------
// These _CopyXXX classes are used with enumerators in order to control
// how enumerated items are initialized, copied, and deleted

// Default is shallow copy with no special init or cleanup
template <class T>
class _Copy
{
public:
	static HRESULT copy(T* p1, T* p2) {memcpy(p1, p2, sizeof(T)); return S_OK;}
	static void init(T*) {}
	static void destroy(T*) {}
};

template<>
class _Copy<VARIANT>
{
public:
	static HRESULT copy(VARIANT* p1, VARIANT* p2) {return VariantCopy(p1, p2);}
	static void init(VARIANT* p) {p->vt = VT_EMPTY;}
	static void destroy(VARIANT* p) {VariantClear(p);}
};

template<>
class _Copy<LPOLESTR>
{
public:
	template <class WStringType> //add by xushiwei, for std::wstring support
	static HRESULT copy(LPOLESTR* p1, WStringType* p2)
	{
		UINT cb = sizeof(OLECHAR)*(p2->size()+1);
		(*p1) = (LPOLESTR)CoTaskMemAlloc(cb);
		if (*p1 == NULL)
			return E_OUTOFMEMORY;
		memcpy(*p1,p2->c_str(), cb);
		return S_OK;
	}
	static HRESULT copy(LPOLESTR* p1, LPOLESTR* p2)
	{
		HRESULT hr = S_OK;
		(*p1) = (LPOLESTR)CoTaskMemAlloc(sizeof(OLECHAR)*(ocslen(*p2)+1));
		if (*p1 == NULL)
			hr = E_OUTOFMEMORY;
		else
			ocscpy(*p1,*p2);
		return hr;
	}
	static void init(LPOLESTR* p) {*p = NULL;}
	static void destroy(LPOLESTR* p) { CoTaskMemFree(*p);}
};

template<>
class _Copy<OLEVERB>
{
public:
	static HRESULT copy(OLEVERB* p1, OLEVERB* p2)
	{
		HRESULT hr = S_OK;
		*p1 = *p2;
		if (p2->lpszVerbName == NULL)
			return S_OK;
		p1->lpszVerbName = (LPOLESTR)CoTaskMemAlloc(sizeof(OLECHAR)*(ocslen(p2->lpszVerbName)+1));
		if (p1->lpszVerbName == NULL)
			hr = E_OUTOFMEMORY;
		else
			ocscpy(p1->lpszVerbName,p2->lpszVerbName);
		return hr;
	}
	static void init(OLEVERB* p) { p->lpszVerbName = NULL;}
	static void destroy(OLEVERB* p) { if (p->lpszVerbName) CoTaskMemFree(p->lpszVerbName);}
};

template<>
class _Copy<CONNECTDATA>
{
public:
	static HRESULT copy(CONNECTDATA* p1, CONNECTDATA* p2)
	{
		*p1 = *p2;
		if (p1->pUnk)
			p1->pUnk->AddRef();
		return S_OK;
	}
	static void init(CONNECTDATA* ) {}
	static void destroy(CONNECTDATA* p) {if (p->pUnk) p->pUnk->Release();}
};

template <class T>
class _CopyInterface
{
public:
	static HRESULT copy(T** p1, T** p2)
	{
		*p1 = *p2;
		if (*p1)
			(*p1)->AddRef();
		return S_OK;
	}
	static void init(T** ) {}
	static void destroy(T** p) {if (*p) (*p)->Release();}
};

// -------------------------------------------------------------------------
// IEnumOnSTLImpl

template <class Base, const IID* piid, class T, class Copy, class CollType>
class ATL_NO_VTABLE IEnumOnSTLImpl : public Base
{
public:
	HRESULT Init(CollType& collection)
	{
		m_pcollection = &collection;
		m_iter = m_pcollection->begin();
		return S_OK;
	}
	STDMETHOD(Next)(ULONG celt, T* rgelt, ULONG* pceltFetched);
	STDMETHOD(Skip)(ULONG celt);
	STDMETHOD(Reset)(void)
	{
		if (m_pcollection == NULL)
			return E_FAIL;
		m_iter = m_pcollection->begin();
		return S_OK;
	}
//Data
	CollType* m_pcollection;
	typename CollType::iterator m_iter;
};

template <class Base, const IID* piid, class T, class Copy, class CollType>
STDMETHODIMP IEnumOnSTLImpl<Base, piid, T, Copy, CollType>::Next(ULONG celt, T* rgelt,
	ULONG* pceltFetched)
{
	if (rgelt == NULL || (celt != 1 && pceltFetched == NULL))
		return E_POINTER;
	if (m_pcollection == NULL)
		return E_FAIL;

	ULONG nActual = 0;
	HRESULT hr = S_OK;
	T* pelt = rgelt;
	while (SUCCEEDED(hr) && m_iter != m_pcollection->end() && nActual < celt)
	{
		hr = Copy::copy(pelt, &*m_iter);
		if (FAILED(hr))
		{
			while (rgelt < pelt)
				Copy::destroy(rgelt++);
			nActual = 0;
		}
		else
		{
			pelt++;
			m_iter++;
			nActual++;
		}
	}
	if (pceltFetched)
		*pceltFetched = nActual;
	if (SUCCEEDED(hr) && (nActual < celt))
		hr = S_FALSE;
	return hr;
}

template <class Base, const IID* piid, class T, class Copy, class CollType>
STDMETHODIMP IEnumOnSTLImpl<Base, piid, T, Copy, CollType>::Skip(ULONG celt)
{
	HRESULT hr = S_OK;
	while (celt--)
	{
		if (m_iter != m_pcollection->end())
			m_iter++;
		else
		{
			hr = S_FALSE;
			break;
		}
	}
	return hr;
}

// -------------------------------------------------------------------------
// $Log: atlcomx.h,v $
// Revision 1.3  2006/08/26 09:12:37  xushiwei
// vs2005 support
//
// Revision 1.2  2006/08/26 03:53:34  xushiwei
// WINX-Basic:
//    IEnumOnSTLImpl, EnumStringSTLImpl
//
// Revision 1.1  2006/08/26 03:47:05  xushiwei
// Winx-Extension:
//    AutoComplete support
//    DropFileHandle(simple wrapper for HDROP)
//

}; // namespace ATLX

#endif /* __WINX_COM_ATL_ATLCOMX_H__ */
