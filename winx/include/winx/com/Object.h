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
// Module: winx/com/Object.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 16:04:11
// 
// $Id: Object.h,v 1.3 2006/08/26 03:53:34 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_COM_OBJECT_H__
#define __WINX_COM_OBJECT_H__

#ifndef __WINX_BASIC_H__
#include "../Basic.h"
#endif

#ifndef __WINX_COM_ATL_ATLCOMX_H__
#include "atl/atlcomx.h"
#endif

__WINX_BEGIN

// -------------------------------------------------------------------------
// interface - COM support

#ifndef STDPROC
#define STDPROC					virtual HRESULT __stdcall
#define STDPROC_(Type)			virtual Type __stdcall
#endif

#if defined(WINX_USE_DEFINE_IID)
#define WINX_DEFINE_IID(Interface, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8) \
	DEFINE_IID(Interface, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8)
#else
#define WINX_DEFINE_IID(Interface, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8) \
	interface __declspec(uuid(sz)) Interface;
#endif

#if defined(WINX_USE_DEFINE_IID)
#define WINX_UUID(Interface)	__uuid(Interface)
#else
#define WINX_UUID(Interface)	__uuidof(Interface)
#endif

// -------------------------------------------------------------------------
// class Object - COM support

template <class Interface, class Base = Interface>
class Object : public Base
{
private:
	ULONG m_nRef;

public:
	Object() : m_nRef(1) {}
	virtual ~Object() {}

	STDMETHODIMP_(ULONG) AddRef()
	{
		return ++m_nRef;
	}
	
	STDMETHODIMP_(ULONG) Release()
	{
		ULONG l = --m_nRef;
		if (l == 0)
			delete this;
		return l;
	}

	STDMETHODIMP QueryInterface(
		IN REFIID riid,
		OUT void** ppvObject)
	{
		if (riid == WINX_UUID(Interface) || riid == IID_IUnknown)
		{
			*ppvObject = static_cast<Interface*>(this);
			AddRef();
			return S_OK;
		}
		else
		{
			return E_NOINTERFACE;
		}
	}
};

template <class Interface, class Base = Interface>
class StackObject : public Base
{
public:
	STDMETHODIMP_(ULONG) AddRef()  { return 2; }
	STDMETHODIMP_(ULONG) Release() { return 1; }

	STDMETHODIMP QueryInterface(
		IN REFIID riid,
		OUT void** ppvObject)
	{
		if (riid == WINX_UUID(Interface) || riid == IID_IUnknown)
		{
			*ppvObject = static_cast<Interface*>(this);
			AddRef();
			return S_OK;
		}
		else
		{
			return E_NOINTERFACE;
		}
	}
};

template <class Interface, class Base = Interface>
class FakeObject : public Base
{
public:
	STDMETHODIMP_(ULONG) AddRef()  { return 2; }
	STDMETHODIMP_(ULONG) Release() { return 1; }
	STDMETHODIMP QueryInterface(
		IN REFIID riid,
		OUT void** ppvObject)
	{
		return E_NOINTERFACE;
	}
};

// -------------------------------------------------------------------------
// class EnumeratorPtr

template <class Interface>
struct EnumItemTypeTraits {
};

#define WINX_ENUMITEMTYPE(Interface, AItemType)								\
template <>																	\
struct EnumItemTypeTraits<Interface> {										\
	typedef AItemType ItemType;												\
}

WINX_ENUMITEMTYPE(IEnumString, LPOLESTR);
WINX_ENUMITEMTYPE(IEnumMoniker, IMoniker*);
WINX_ENUMITEMTYPE(IEnumFORMATETC, FORMATETC);

template <class Interface, class AItemType = EnumItemTypeTraits<Interface>::ItemType>
class EnumeratorPtr : public CComPtr<Interface>
{
public:
	typedef AItemType ItemType;

public:
	HRESULT winx_call Next(
		OUT ItemType* rgelt,
		IN ULONG celt = 1,
		OUT ULONG* pceltFetched = NULL)
	{
		WINX_ASSERT(p && rgelt);
		if (p == NULL)
			return S_FALSE;
		else
			return p->Next(celt, rgelt, pceltFetched);
	}
};

// -------------------------------------------------------------------------
// class IEnumStringImpl

using ATLX::IEnumOnSTLImpl;
using ATLX::_Copy;

template <class CollType, class Base = Object<IEnumString> >
class EnumStringSTLImpl : 
	public IEnumOnSTLImpl<Base, &IID_IEnumString, LPOLESTR, _Copy<LPOLESTR>, CollType>
{
public:
	EnumStringSTLImpl(CollType& coll) {
		m_pcollection = &coll;
		m_iter = coll.begin();
	}
	EnumStringSTLImpl(CollType& coll, typename CollType::iterator it) {
		m_pcollection = &coll;
		m_iter = it;
	}
	STDMETHOD(Clone)(IEnumString** pp) {
		*pp = new EnumStringSTLImpl(*this);
		return S_OK;
	}
};

// -------------------------------------------------------------------------
// $Log: Object.h,v $
// Revision 1.3  2006/08/26 03:53:34  xushiwei
// WINX-Basic:
//    IEnumOnSTLImpl, EnumStringSTLImpl
//
// Revision 1.2  2006/08/26 03:47:04  xushiwei
// Winx-Extension:
//    AutoComplete support
//    DropFileHandle(simple wrapper for HDROP)
//
// Revision 1.1  2006/08/19 09:42:10  xushiwei
// WINX-Basic:
//   BasicTypes(CString, CFindFile, CRecentDocumentList, CComBSTR, etc)
//   Helper(MsgBox, GetThisModule, InitCommonControls, etc)
//   Init(CComAppInit, COleAppInit, CDebugAppInit, CComModuleInit, etc)
//   Resource(GetModuleIniPath, Bitmap, Icon, Menu, Brush, Accelerator, etc)
//   Gdi(Rgn, Pen, Brush, Font, Bitmap, DC, DoubleBuffer, etc)
//   WindowHandle, WindowMap, MessageMap
//   COM Support(Object, StackObject, FakeObject, COLESTR, etc)
//

__WINX_END

#endif /* __WINX_COM_OBJECT_H__ */
