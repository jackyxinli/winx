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
// Module: winx/com/ROT.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-2-9 12:26:25
// Description: IPC - Running Object Table
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef WINX_COM_ROT_H
#define WINX_COM_ROT_H

#ifndef STDEXT_TCHAR_H
#include "../../../../stdext/include/stdext/tchar.h"
#endif

#ifndef WINX_COM_OBJECT_H
#include "Object.h"
#endif

#ifndef ROTFLAGS_DEFAULT
#define ROTFLAGS_DEFAULT	0
#endif

#ifndef ROTFLAGS_ALLOWANYCLIENT
#define ROTFLAGS_ALLOWANYCLIENT	2
#endif

NS_WINX_BEGIN

typedef CComPtr<IMoniker> MonikerPtr;

// -------------------------------------------------------------------------
// class MonikerObject

class MonikerObject : public CComPtr<IMoniker>
{
private:
	MonikerObject(const MonikerObject&);
	void operator=(const MonikerObject&);

public:
	MonikerObject(REFCLSID rclsid) {
		::CreateClassMoniker(rclsid, &p);
	}
	MonikerObject(LPCOLESTR lpszPathName) {
		::CreateFileMoniker(lpszPathName, &p);
	}
	MonikerObject(LPCOLESTR lpszDelim, LPCOLESTR lpszItem) {
		::CreateItemMoniker(lpszDelim, lpszItem, &p);
	}
	MonikerObject(LPUNKNOWN pUnk) {
		::CreatePointerMoniker(pUnk, &p);
	}
};

// -------------------------------------------------------------------------
// class BindCtxObject

class BindCtxObject : public CComPtr<IBindCtx>
{
public:
	BindCtxObject(DWORD reserved = 0) {
		::CreateBindCtx(reserved, &p);
	}
};

// -------------------------------------------------------------------------
// MonikerEnumerator

class MonikerEnumerator : public EnumeratorPtr<IEnumMoniker>
{
public:
	MonikerEnumerator(IRunningObjectTable* pROT) {
		pROT->EnumRunning(&p);
	}
};

// -------------------------------------------------------------------------
// class ROT

class ROT : public CComPtr<IRunningObjectTable>
{
private:
	ROT(const ROT&);
	void operator=(const ROT&);

public:
	ROT() {
		::GetRunningObjectTable(0, &p);
	}

public:
	HRESULT winx_call Register(
		OUT DWORD* pdwRegister,
		IN IUnknown* pObj,
		IN IMoniker* pMoniker,
		IN DWORD grfFlags = ROTFLAGS_DEFAULT)
	{
		if (p)
			return p->Register(grfFlags, pObj, pMoniker, pdwRegister);
		return E_ACCESSDENIED;
	}

	HRESULT winx_call Register(
		OUT DWORD* pdwRegister,
		IN IUnknown* pObj,
		IN MonikerObject objMoniker,
		IN DWORD grfFlags = ROTFLAGS_DEFAULT)
	{
		if (p)
			return p->Register(grfFlags, pObj, objMoniker, pdwRegister);
		return E_ACCESSDENIED;
	}

	static VOID CreateItemName(
		OUT LPWSTR szId,
		IN DWORD processId, IN CLSID clsid)
	{
		StringFromGUID2(clsid, szId, 64);
		szId = std::tchar::strecpy(wcschr(szId, '\0'), L"pid:");
		std::tchar::itoa(processId, szId, 16);
	}

	HRESULT winx_call RegisterItem(
		OUT DWORD* pdwRegister,
		IN IUnknown* pObj,
		IN LPCOLESTR lpszDelim, IN LPCOLESTR lpszItem,
		IN DWORD grfFlags = ROTFLAGS_DEFAULT)
	{
		HRESULT hr;
		if (p) {
			IMoniker* pMoniker;
			hr = ::CreateItemMoniker(lpszDelim, lpszItem, &pMoniker);
			if (SUCCEEDED(hr)) {
				hr = p->Register(grfFlags, pObj, pMoniker, pdwRegister);
				pMoniker->Release();
			}
		}
		else {
			hr = E_ACCESSDENIED;
		}
		return hr;
	}

	HRESULT winx_call RegisterItem(
		OUT DWORD* pdwRegister,
		IN IUnknown* pObj,
		IN CLSID clsid,
		IN DWORD grfFlags = ROTFLAGS_DEFAULT)
	{
		WCHAR szId[64];
		StringFromGUID2(clsid, szId, 64);
		return RegisterItem(pdwRegister, pObj, L"!", szId);
	}

	HRESULT winx_call RegisterItem(
		OUT DWORD* pdwRegister,
		IN IUnknown* pObj,
		IN DWORD processId, IN CLSID clsid,
		IN DWORD grfFlags = ROTFLAGS_DEFAULT)
	{
		WCHAR szId[64];
		CreateItemName(szId, processId, clsid);
		return RegisterItem(pdwRegister, pObj, L"!", szId);
	}

	HRESULT winx_call GetObject(
		IN IMoniker* pMoniker,
		IN REFIID iid, OUT void** ppObj)
	{
		HRESULT hr;
		if (p) {
			IUnknown* pUnk;
			hr = p->GetObject(pMoniker, &pUnk);
			if (SUCCEEDED(hr)) {
				hr = pUnk->QueryInterface(iid, ppObj);
				pUnk->Release();
			}
		}
		else {
			hr = E_ACCESSDENIED;
		}
		return hr;
	}

	template <class Interface>
	HRESULT winx_call GetObject(
		IN MonikerObject objMoniker,
		OUT Interface** ppObj)
	{
		return GetObject(objMoniker.p, WINX_UUID(Interface), (void**)ppObj);
	}

	template <class Interface>
	HRESULT winx_call GetItemObject(
		IN LPCOLESTR lpszDelim, IN LPCOLESTR lpszItem,
		OUT Interface** ppObj)
	{
		MonikerObject objMoniker(lpszDelim, lpszItem);
		return GetObject(objMoniker.p, WINX_UUID(Interface), (void**)ppObj);
	}

	template <class Interface>
	HRESULT winx_call GetItemObject(
		IN CLSID clsid,
		OUT Interface** ppObj)
	{
		WCHAR szId[64];
		StringFromGUID2(clsid, szId, 64);
		MonikerObject objMoniker(L"!", szId);
		return GetObject(objMoniker.p, WINX_UUID(Interface), (void**)ppObj);
	}

	template <class Interface>
	HRESULT winx_call GetItemObject(
		IN DWORD processId, IN CLSID clsid,
		OUT Interface** ppObj)
	{
		WCHAR szId[64];
		CreateItemName(szId, processId, clsid);
		MonikerObject objMoniker(L"!", szId);
		return GetObject(objMoniker.p, WINX_UUID(Interface), (void**)ppObj);
	}

	HRESULT winx_call Revoke(DWORD dwRegister)
	{
		if (p)
			return p->Revoke(dwRegister);
		return E_ACCESSDENIED;
	}
};

typedef ROT RunningObjectTable;

// -------------------------------------------------------------------------

inline HRESULT winx_call ExRegisterActiveObject(
	DWORD* pdwRegister, IUnknown* punk,
	DWORD processId, REFCLSID rclsid,
	DWORD dwFlags = ACTIVEOBJECT_STRONG)
{
	winx::ROT rot;
	rot.RegisterItem(
		pdwRegister, punk, processId, rclsid);
}

// -------------------------------------------------------------------------
// $Log: ROT.h,v $

NS_WINX_END

#endif /* WINX_COM_ROT_H */
