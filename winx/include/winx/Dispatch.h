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
// Module: winx/Dispatch.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-14 22:54:26
// 
// $Id: Dispatch.h,v 1.1 2007/01/12 20:27:53 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_DISPATCH_H__
#define __WINX_DISPATCH_H__

#ifndef __WINX_BASIC_H__
#include "Basic.h"
#endif

#ifndef _STRING_
#include <string>
#endif

__WINX_BEGIN

// -------------------------------------------------------------------------
// struct _DispRetTypeTraits

template <class Type>
struct _DispRetTypeTraits {};

template <>
struct _DispRetTypeTraits<long> {
	typedef long TYPE;
	enum { VT = VT_I4 };
	static TYPE winx_call DetachValue(VARIANT& vaResult) {
		vaResult.vt = VT_EMPTY;
		return vaResult.lVal;
	}
};

template <>
struct _DispRetTypeTraits<int> : public _DispRetTypeTraits<long> {
};

template <>
struct _DispRetTypeTraits<VARIANT_BOOL> {
	typedef VARIANT_BOOL TYPE;
	enum { VT = VT_BOOL };
	static TYPE winx_call DetachValue(VARIANT& vaResult) {
		vaResult.vt = VT_EMPTY;
		return vaResult.boolVal;
	}
};

template <>
struct _DispRetTypeTraits<BSTR> {
	typedef BSTR TYPE;
	enum { VT = VT_BSTR };
	static TYPE winx_call DetachValue(VARIANT& vaResult) {
		vaResult.vt = VT_EMPTY;
		return vaResult.bstrVal;
	}
};

template <>
struct _DispRetTypeTraits< std::basic_string<WCHAR> > {
	typedef std::basic_string<WCHAR> TYPE;
	enum { VT = VT_BSTR };
	static TYPE winx_call DetachValue(const VARIANT& vaResult) {
		return TYPE(vaResult.bstrVal, SysStringLen(vaResult.bstrVal));
	}
};

template <>
struct _DispRetTypeTraits<IDispatch*> {
	typedef IDispatch* TYPE;
	enum { VT = VT_DISPATCH };
	static TYPE winx_call DetachValue(VARIANT& vaResult) {
		vaResult.vt = VT_EMPTY;
		return vaResult.pdispVal;
	}
};

template <>
struct _DispRetTypeTraits<IUnknown*> {
	typedef IUnknown* TYPE;
	enum { VT = VT_UNKNOWN };
	static TYPE winx_call DetachValue(VARIANT& vaResult) {
		vaResult.vt = VT_EMPTY;
		return vaResult.punkVal;
	}
};

template <>
struct _DispRetTypeTraits<IUnknown**> {
	typedef IUnknown** TYPE;
	enum { VT = VT_UNKNOWN | VT_BYREF };
	static TYPE winx_call DetachValue(VARIANT& vaResult) {
		vaResult.vt = VT_EMPTY;
		return vaResult.ppunkVal;
	}
};

// -------------------------------------------------------------------------
// class DispatchHandle - see class ATL::CComDispatchDriver

class DispatchHandle
{
public:
	IDispatch* p;

public:
	DispatchHandle() : p(NULL) {}
	DispatchHandle(IDispatch* disp) : p(disp) {}

public:
	DispatchHandle& winx_call operator=(IDispatch* disp)
	{
		p = disp;
		return *this;
	}

	operator IDispatch*() const
	{
		return p;
	}

	HRESULT GetIDOfName(LPCOLESTR lpsz, DISPID* pdispid)
	{
		return p->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpsz, 1, LOCALE_USER_DEFAULT, pdispid);
	}

private:
	class _RetVariant : public VARIANT
	{
	public:
		_RetVariant() { ::VariantInit(this); }
		~_RetVariant() {
			if (vt != VT_EMPTY)
				::VariantClear(this);
		}
		
		template <class RetType>
		HRESULT CastTo(RetType* result)
		{
			enum { VT_NEED = _DispRetTypeTraits<RetType>::VT };
			if (vt != VT_NEED)
			{
				HRESULT hr = ::VariantChangeType(this, this, 0, VT_NEED);
				if (FAILED(hr))
					return hr;
			}
			*result = _DispRetTypeTraits<RetType>::DetachValue(*this);
			return S_OK;
		}
	};

public:
	// Invoke a method by DISPID with no parameters
	HRESULT Invoke0(DISPID dispid, VARIANT* pvarRet = NULL)
	{
		DISPPARAMS dispparams = { NULL, NULL, 0, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}
	// Invoke a method by name with no parameters
	HRESULT Invoke0(LPCOLESTR lpszName, VARIANT* pvarRet = NULL)
	{
		DISPID dispid;
		HRESULT hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = Invoke0(dispid, pvarRet);
		return hr;
	}
	// Invoke a method by DISPID with a single parameter
	HRESULT Invoke1(DISPID dispid, VARIANT* pvarParam1, VARIANT* pvarRet = NULL)
	{
		DISPPARAMS dispparams = { pvarParam1, NULL, 1, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}
	// Invoke a method by name with a single parameter
	HRESULT Invoke1(LPCOLESTR lpszName, VARIANT* pvarParam1, VARIANT* pvarRet = NULL)
	{
		DISPID dispid;
		HRESULT hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = Invoke1(dispid, pvarParam1, pvarRet);
		return hr;
	}
	// Invoke a method by DISPID with two parameters
	HRESULT Invoke2(DISPID dispid, VARIANT* pvarParam1, VARIANT* pvarParam2, VARIANT* pvarRet = NULL)
	{
		VARIANT varArgs[2];
		varArgs[0] = *pvarParam2;
		varArgs[1] = *pvarParam1;
		DISPPARAMS dispparams = { &varArgs[0], NULL, 2, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}
	// Invoke a method by name with two parameters
	HRESULT Invoke2(LPCOLESTR lpszName, VARIANT* pvarParam1, VARIANT* pvarParam2, VARIANT* pvarRet = NULL)
	{
		DISPID dispid;
		HRESULT hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = Invoke2(dispid, pvarParam1, pvarParam2, pvarRet);
		return hr;
	}
	// Invoke a method by DISPID with three parameters
	HRESULT Invoke3(DISPID dispid, VARIANT* pvarParam1, VARIANT* pvarParam2, VARIANT* pvarParam3, VARIANT* pvarRet = NULL)
	{
		VARIANT varArgs[3];
		varArgs[0] = *pvarParam3;
		varArgs[1] = *pvarParam2;
		varArgs[2] = *pvarParam1;
		DISPPARAMS dispparams = { &varArgs[0], NULL, 3, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}
	// Invoke a method by name with three parameters
	HRESULT Invoke3(LPCOLESTR lpszName, VARIANT* pvarParam1, VARIANT* pvarParam2, VARIANT* pvarParam3, VARIANT* pvarRet = NULL)
	{
		DISPID dispid;
		HRESULT hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = Invoke3(dispid, pvarParam1, pvarParam2, pvarParam3, pvarRet);
		return hr;
	}
	// Invoke a method by DISPID with N parameters
	HRESULT InvokeN(DISPID dispid, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL)
	{
		DISPPARAMS dispparams = { pvarParams, NULL, nParams, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}
	// Invoke a method by name with Nparameters
	HRESULT InvokeN(LPCOLESTR lpszName, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL)
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = InvokeN(dispid, pvarParams, nParams, pvarRet);
		return hr;
	}

public:
	static HRESULT GetProperty(IDispatch* pDisp, DISPID dwDispID, VARIANT* pVar)
	{
		DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
		return pDisp->Invoke(dwDispID, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
				&dispparamsNoArgs, pVar, NULL, NULL);
	}

	static HRESULT PutProperty(IDispatch* pDisp, DISPID dwDispID,
		VARIANT* pVar)
	{
		DISPPARAMS dispparams = {NULL, NULL, 1, 1};
		dispparams.rgvarg = pVar;
		DISPID dispidPut = DISPID_PROPERTYPUT;
		dispparams.rgdispidNamedArgs = &dispidPut;

		if (pVar->vt == VT_UNKNOWN || pVar->vt == VT_DISPATCH || 
			(pVar->vt & VT_ARRAY) || (pVar->vt & VT_BYREF))
		{
			HRESULT hr = pDisp->Invoke(dwDispID, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUTREF,
				&dispparams, NULL, NULL, NULL);
			if (SUCCEEDED(hr))
				return hr;
		}

		return pDisp->Invoke(dwDispID, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT,
				&dispparams, NULL, NULL, NULL);
	}

	template <class RetType>
	static HRESULT GetProperty(IDispatch* pDisp, DISPID dwDispID, RetType* result)
	{
		_RetVariant varRet;
		DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
		HRESULT hr = pDisp->Invoke(dwDispID, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
				&dispparamsNoArgs, &varRet, NULL, NULL);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

	template <class RetType>
	static HRESULT GetProperty(IDispatch* pDisp, LPCOLESTR lpsz, RetType* result)
	{
		DISPID dwDispID;
		HRESULT hr = pDisp->GetIDsOfNames(IID_NULL, 
			(LPOLESTR*)&lpsz, 1, LOCALE_USER_DEFAULT, &dwDispID);
		if (SUCCEEDED(hr))
			hr = GetProperty(pDisp, dwDispID, result);
		return hr;
	}

	template <class RetType>
	HRESULT GetProperty(DISPID dwDispID, RetType* pVar)
	{
		return GetProperty(p, dwDispID, pVar);
	}

	template <class RetType>
	HRESULT GetProperty(LPCOLESTR lpsz, RetType* pVar)
	{
		DISPID dwDispID;
		HRESULT hr = GetIDOfName(lpsz, &dwDispID);
		if (SUCCEEDED(hr))
			hr = GetProperty(p, dwDispID, pVar);
		return hr;
	}

	HRESULT PutProperty(DISPID dwDispID, VARIANT* pVar)
	{
		return PutProperty(p, dwDispID, pVar);
	}
	HRESULT PutProperty(LPCOLESTR lpsz, VARIANT* pVar)
	{
		DISPID dwDispID;
		HRESULT hr = GetIDOfName(lpsz, &dwDispID);
		if (SUCCEEDED(hr))
			hr = PutProperty(p, dwDispID, pVar);
		return hr;
	}

public:
	HRESULT winx_call CallMethod(LPCOLESTR name) { return Invoke0(name); }
	HRESULT winx_call CallMethod(LPCOLESTR name, VARIANT* arg1) { return Invoke1(name, arg1); }
	HRESULT winx_call CallMethod(LPCOLESTR name, VARIANT* arg1, VARIANT* arg2) { return Invoke2(name, arg1, arg2); }
	HRESULT winx_call CallMethod(LPCOLESTR name, VARIANT* arg1, VARIANT* arg2, VARIANT* arg3) { return Invoke3(name, arg1, arg2, arg3); }

	HRESULT winx_call CallMethod(DISPID name) { return Invoke0(name); }
	HRESULT winx_call CallMethod(DISPID name, VARIANT* arg1) { return Invoke1(name, arg1); }
	HRESULT winx_call CallMethod(DISPID name, VARIANT* arg1, VARIANT* arg2) { return Invoke2(name, arg1, arg2); }
	HRESULT winx_call CallMethod(DISPID name, VARIANT* arg1, VARIANT* arg2, VARIANT* arg3) { return Invoke3(name, arg1, arg2, arg3); }

	HRESULT winx_call CallMethod(LPCOLESTR name, CComVariant arg1) { return Invoke1(name, &arg1); }
	HRESULT winx_call CallMethod(LPCOLESTR name, CComVariant arg1, CComVariant arg2) { return Invoke2(name, &arg1, &arg2); }
	HRESULT winx_call CallMethod(LPCOLESTR name, CComVariant arg1, CComVariant arg2, CComVariant arg3) { return Invoke3(name, &arg1, &arg2, &arg3); }

	HRESULT winx_call CallMethod(DISPID name, CComVariant arg1) { return Invoke1(name, &arg1); }
	HRESULT winx_call CallMethod(DISPID name, CComVariant arg1, CComVariant arg2) { return Invoke2(name, &arg1, &arg2); }
	HRESULT winx_call CallMethod(DISPID name, CComVariant arg1, CComVariant arg2, CComVariant arg3) { return Invoke3(name, &arg1, &arg2, &arg3); }

public:
	template <class RetType>
	HRESULT winx_call CallFunction(LPCOLESTR name, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke0(name, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

	template <class RetType>
	HRESULT winx_call CallFunction(LPCOLESTR name, VARIANT* arg1, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke1(name, arg1, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

	template <class RetType>
	HRESULT winx_call CallFunction(LPCOLESTR name, VARIANT* arg1, VARIANT* arg2, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke2(name, arg1, arg2, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

	template <class RetType>
	HRESULT winx_call CallFunction(LPCOLESTR name, VARIANT* arg1, VARIANT* arg2, VARIANT* arg3, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke3(name, arg1, arg2, arg3, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

public:
	template <class RetType>
	HRESULT winx_call CallFunction(DISPID name, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke0(name, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

	template <class RetType>
	HRESULT winx_call CallFunction(DISPID name, VARIANT* arg1, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke1(name, arg1, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

	template <class RetType>
	HRESULT winx_call CallFunction(DISPID name, VARIANT* arg1, VARIANT* arg2, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke2(name, arg1, arg2, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

	template <class RetType>
	HRESULT winx_call CallFunction(DISPID name, VARIANT* arg1, VARIANT* arg2, VARIANT* arg3, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke3(name, arg1, arg2, arg3, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

public:
	template <class RetType>
	HRESULT winx_call CallFunction(LPCOLESTR name, CComVariant arg1, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke1(name, &arg1, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

	template <class RetType>
	HRESULT winx_call CallFunction(LPCOLESTR name, CComVariant arg1, CComVariant arg2, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke2(name, &arg1, &arg2, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

	template <class RetType>
	HRESULT winx_call CallFunction(LPCOLESTR name, CComVariant arg1, CComVariant arg2, CComVariant arg3, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke3(name, &arg1, &arg2, &arg3, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

public:
	template <class RetType>
	HRESULT winx_call CallFunction(DISPID name, CComVariant arg1, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke1(name, &arg1, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

	template <class RetType>
	HRESULT winx_call CallFunction(DISPID name, CComVariant arg1, CComVariant arg2, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke2(name, &arg1, &arg2, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

	template <class RetType>
	HRESULT winx_call CallFunction(DISPID name, CComVariant arg1, CComVariant arg2, CComVariant arg3, RetType* result) {
		_RetVariant varRet;
		HRESULT hr = Invoke3(name, &arg1, &arg2, &arg3, &varRet);
		if (SUCCEEDED(hr))
			hr = varRet.CastTo(result);
		return hr;
	}

public:
};

// -------------------------------------------------------------------------
// GetDispatchObj

struct DispatchObj
{
	DispatchHandle disp;
	DISPID dispid;
};

inline HRESULT winx_call GetDispatchObj(
	IN IDispatch* obj, IN LPOLESTR nameObj, OUT DispatchObj& info)
{
	if (obj == NULL)
		return E_ACCESSDENIED;

	HRESULT hr;
	IDispatch* objTmp;
	LPWSTR pos;
	obj->AddRef();
	while ( (pos = wcschr(nameObj, '.')) != NULL )
	{
		*pos = '\0';
		hr = DispatchHandle::GetProperty(obj, nameObj, &objTmp);
		obj->Release();
		if (FAILED(hr))
			return hr;
		obj = objTmp;
		nameObj = pos + 1;
	}
	info.obj = obj;
	return obj->GetIDsOfNames(
		IID_NULL, (LPOLESTR*)&nameObj, 1, LOCALE_USER_DEFAULT, &info.dispid);
}

inline HRESULT winx_call GetDispatchObjHelper(
	IN IDispatch* obj, IN LPCOLESTR nameObj, OUT DispatchObj& info)
{
	LPOLESTR name = wcsdup(nameObj);
	HRESULT hr = GetDispatchObj(obj, name, info);
	free(name);
	return hr;
}

// -------------------------------------------------------------------------
// class DispatchMethod

class DispatchMethod : public DispatchObj
{
public:
	DispatchMethod(IDispatch* obj, LPCWSTR nameObj) {
		GetDispatchObjHelper(obj, nameObj, *this);
	}
};

// -------------------------------------------------------------------------
// class DispatchFunction

class DispatchFunction
{
public:
};

// -------------------------------------------------------------------------
// $Log: Dispatch.h,v $
// Revision 1.1  2007/01/12 20:27:53  xushiwei
// WINX-Core:
//   Dispatch Helper
//

__WINX_END

#endif /* __WINX_DISPATCH_H__ */
