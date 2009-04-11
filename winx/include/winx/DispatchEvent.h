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
// Module: winx/DispatchEvent.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:22:03
// 
// $Id: DispatchEvent.h,v 1.2 2006/08/26 16:27:53 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_DISPATCHEVENT_H
#define WINX_DISPATCHEVENT_H

#ifndef WINX_BASIC_H
#include "Basic.h"
#endif

#ifndef WINX_COM_SAFEARRAY_H
#include "com/SafeArray.h"
#endif

#if !defined( _OBJBASE_H_ )
#include <objbase.h>
#endif

#ifndef WINX_NO_VTABLE
	#if defined(ATL_NO_VTABLE)
	#define WINX_NO_VTABLE ATL_NO_VTABLE
	#else
	#define WINX_NO_VTABLE
	#endif
#endif

// =========================================================================
// Connection Point Helpers

inline HRESULT winx_call DispEvAdvise(IUnknown* pUnkCP, IUnknown* pUnk, const IID& iid, LPDWORD pdw)
{
	IConnectionPointContainer* pCPC;
	IConnectionPoint* pCP;
	HRESULT hRes = pUnkCP->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);
	if (SUCCEEDED(hRes))
	{
		hRes = pCPC->FindConnectionPoint(iid, &pCP);
		if (SUCCEEDED(hRes))
		{
			hRes = pCP->Advise(pUnk, pdw);
			pCP->Release();
		}
		pCPC->Release();
	}
	return hRes;
}

inline HRESULT winx_call DispEvUnadvise(IUnknown* pUnkCP, const IID& iid, DWORD dw)
{
	IConnectionPointContainer* pCPC;
	IConnectionPoint* pCP;
	HRESULT hRes = pUnkCP->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);
	if (SUCCEEDED(hRes))
	{
		hRes = pCPC->FindConnectionPoint(iid, &pCP);
		if (SUCCEEDED(hRes))
		{
			hRes = pCP->Unadvise(dw);
			pCP->Release();
		}
		pCPC->Release();
	}
	return hRes;
}

// =========================================================================
// struct CStdFuncInfo

#ifndef WINX_MAX_VARTYPES
#define WINX_MAX_VARTYPES 8
#endif

struct CStdFuncInfo
{
	CALLCONV cc;
	VARTYPE vtReturn;
	SHORT nParams;
	VARTYPE pVarTypes[WINX_MAX_VARTYPES];
};

// =========================================================================
// struct ClosurePtr

struct ClosurePtr
{
	typedef void (__stdcall *CallbackType)(LPVOID pThis);
	
	LPVOID pThis; // this pointer
	CallbackType pEvent; // method to invoke
	
	template <class FuncType>
	__forceinline void UnsafeInit(LPVOID pThisArg, FuncType fnArg)
	{
		pThis = pThisArg;
		pEvent = *(CallbackType*)&fnArg;
	}
};

// -------------------------------------------------------------------------
// class ClosureCallThunk

#if defined(_M_ALPHA)
#pragma message("ALPHA code for ClosureCallThunk needs to be verified!")

#pragma pack(push,4)
class ClosureCallThunk
{
public:
	void* pVtable;
	void* pFunc;
	DWORD ldah_at;      //  ldah    at, HIWORD(func)
	DWORD ldah_a0;      //  ldah    a0, HIWORD(this)
	DWORD lda_at;       //  lda     at, LOWORD(func)(at)
	DWORD lda_a0;       //  lda     a0, LOWORD(this)(a0)
	DWORD jmp;          //  jmp     zero,(at),0

	void Init(const ClosurePtr& closure)
	{
		union {
			DWORD dwFunc;
			ClosurePtr::CallbackType pfn;
		} pfn;
		pfn.pfn = closure.pEvent;
		pVtable = &pFunc;
		pFunc = &ldah_at;
		thunk.ldah_at = (0x279f0000 | HIWORD(pFunc)) + (LOWORD(pFunc)>>15);
		thunk.ldah_a0 = (0x261f0000 | HIWORD(closure.pThis)) + (LOWORD(closure.pThis)>>15);
		thunk.lda_at = 0x239c0000 | LOWORD(pFunc);
		thunk.lda_a0 = 0x22100000 | LOWORD(closure.pThis);
		thunk.jmp = 0x6bfc0000;
		::FlushInstructionCache(::GetCurrentProcess(), this, sizeof(ClosureCallThunk));
	}
};
#pragma pack(pop)

#elif defined (_M_IX86)

#pragma pack(push,1)
class ClosureCallThunk
{
public:
	void* pVtable;
	void* pFunc;
	DWORD	m_mov;          // mov dword ptr [esp+4], pThis
	DWORD   m_this;         //
	BYTE    m_jmp;          // jmp func
	DWORD   m_relproc;      // relative jmp
	
	void Init(const ClosurePtr& closure)
	{
		union {
			DWORD dwFunc;
			ClosurePtr::CallbackType pfn;
		} pfn;
		pfn.pfn = closure.pEvent;
		pVtable = &pFunc;
		pFunc = &m_mov;
#pragma warning(disable:4311)
		m_mov = 0x042444C7;
		m_this = (DWORD)closure.pThis;
		m_jmp = 0xE9;
		m_relproc = (int)pfn.dwFunc - ((int)this+sizeof(ClosureCallThunk));
#pragma warning(default:4311)
		::FlushInstructionCache(::GetCurrentProcess(), this, sizeof(ClosureCallThunk));
	}
};
#pragma pack(pop)

#else
#error Only ALPHA and X86 supported
#endif

// -------------------------------------------------------------------------
// ClosureCall

/* Low-level helper for IDispatch::Invoke() provides machine independence
 * for customized Invoke().
 */
WINOLEAUTAPI DispCallFunc(void * pvInstance, ULONG oVft, CALLCONV cc,
            VARTYPE vtReturn, UINT  cActuals, VARTYPE * prgvt,
            VARIANTARG ** prgpvarg, VARIANT * pvargResult);

inline HRESULT winx_call ClosureCall(
	IN const CStdFuncInfo& info,
	IN const ClosurePtr& closure,
	IN DISPPARAMS* pdispparams,
	OUT VARIANT* pvarResult = NULL)
{
	VARIANTARG** pVarArgs = info.nParams ? 
		(VARIANTARG**)alloca(sizeof(VARIANTARG*)*info.nParams) : 0;

	for (int i=0; i<info.nParams; i++)
		pVarArgs[i] = &pdispparams->rgvarg[info.nParams - i - 1];

	ClosureCallThunk thunk;
	thunk.Init(closure);

	ATL::CComVariant tmpResult;
	if (pvarResult == NULL)
		pvarResult = &tmpResult;

	return ::DispCallFunc(
		&thunk,
		0,
		info.cc,
		info.vtReturn,
		info.nParams,
		(VARTYPE*)info.pVarTypes,
		pVarArgs,
		pvarResult);
}

// -------------------------------------------------------------------------
// Dispatch Event ArgType Traits

template <class ArgType>
struct _DispEvArgTypeTraits {};

#define WINX_DISPEV_ARGTYPE(Type, vtType)									\
template <>																	\
struct _DispEvArgTypeTraits<Type> { enum { VT = vtType }; }

struct SABUFFER;

WINX_DISPEV_ARGTYPE(long, VT_I4);
WINX_DISPEV_ARGTYPE(long*, VT_I4|VT_BYREF);

WINX_DISPEV_ARGTYPE(BSTR, VT_BSTR);
WINX_DISPEV_ARGTYPE(BSTR*, VT_BSTR|VT_BYREF);

WINX_DISPEV_ARGTYPE(VARIANT_BOOL, VT_BOOL);
WINX_DISPEV_ARGTYPE(VARIANT_BOOL*, VT_BOOL|VT_BYREF);

WINX_DISPEV_ARGTYPE(SABUFFER*, VT_ARRAY|VT_UI1);
WINX_DISPEV_ARGTYPE(SABUFFER**, VT_ARRAY|VT_UI1|VT_BYREF);

WINX_DISPEV_ARGTYPE(IUnknown*, VT_UNKNOWN);
WINX_DISPEV_ARGTYPE(IUnknown**, VT_UNKNOWN|VT_BYREF);

WINX_DISPEV_ARGTYPE(IDispatch*, VT_DISPATCH);
WINX_DISPEV_ARGTYPE(IDispatch**, VT_DISPATCH|VT_BYREF);

WINX_DISPEV_ARGTYPE(VARIANT*, VT_VARIANT|VT_BYREF);

// -------------------------------------------------------------------------
// DEFI - Dispatch Event Function Info

template <VARTYPE vtRet = VT_EMPTY>
struct DEFI0
{
	static const CStdFuncInfo& winx_call Prototype() {
		static CStdFuncInfo _g_info = {
			CC_STDCALL, vtRet, 0
		};
		return _g_info;
	}
};

template <class T1, VARTYPE vtRet = VT_EMPTY>
struct DEFI1 {
	static const CStdFuncInfo& winx_call Prototype() {
		static CStdFuncInfo _g_info = {
			CC_STDCALL, vtRet, 1,
			{ _DispEvArgTypeTraits<T1>::VT }
		};
		return _g_info;
	}
};

template <class T1, class T2, VARTYPE vtRet = VT_EMPTY>
struct DEFI2 {
	static const CStdFuncInfo& winx_call Prototype() {
		static CStdFuncInfo _g_info = {
			CC_STDCALL, vtRet, 2,
			{ _DispEvArgTypeTraits<T1>::VT, _DispEvArgTypeTraits<T2>::VT }
		};
		return _g_info;
	}
};

template <class T1, class T2, class T3, VARTYPE vtRet = VT_EMPTY>
struct DEFI3 {
	static const CStdFuncInfo& winx_call Prototype() {
		static CStdFuncInfo _g_info = {
			CC_STDCALL, vtRet, 3, {
			_DispEvArgTypeTraits<T1>::VT,
			_DispEvArgTypeTraits<T2>::VT,
			_DispEvArgTypeTraits<T3>::VT
			}
		};
		return _g_info;
	}
};

template <class T1, class T2, class T3, class T4, VARTYPE vtRet = VT_EMPTY>
struct DEFI4 {
	static const CStdFuncInfo& winx_call Prototype() {
		static CStdFuncInfo _g_info = {
			CC_STDCALL, vtRet, 4, {
			_DispEvArgTypeTraits<T1>::VT,
			_DispEvArgTypeTraits<T2>::VT,
			_DispEvArgTypeTraits<T3>::VT,
			_DispEvArgTypeTraits<T4>::VT
			}
		};
		return _g_info;
	}
};

template <class T1, class T2, class T3, class T4, class T5, VARTYPE vtRet = VT_EMPTY>
struct DEFI5 {
	static const CStdFuncInfo& winx_call Prototype() {
		static CStdFuncInfo _g_info = {
			CC_STDCALL, vtRet, 5, {
			_DispEvArgTypeTraits<T1>::VT,
			_DispEvArgTypeTraits<T2>::VT,
			_DispEvArgTypeTraits<T3>::VT,
			_DispEvArgTypeTraits<T4>::VT,
			_DispEvArgTypeTraits<T5>::VT
			}
		};
		return _g_info;
	}
};

template <class T1, class T2, class T3, class T4, class T5, class T6, VARTYPE vtRet = VT_EMPTY>
struct DEFI6 {
	static const CStdFuncInfo& winx_call Prototype() {
		static CStdFuncInfo _g_info = {
			CC_STDCALL, vtRet, 6, {
			_DispEvArgTypeTraits<T1>::VT,
			_DispEvArgTypeTraits<T2>::VT,
			_DispEvArgTypeTraits<T3>::VT,
			_DispEvArgTypeTraits<T4>::VT,
			_DispEvArgTypeTraits<T5>::VT,
			_DispEvArgTypeTraits<T6>::VT
			}
		};
		return _g_info;
	}
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, VARTYPE vtRet = VT_EMPTY>
struct DEFI7 {
	static const CStdFuncInfo& winx_call Prototype() {
		static CStdFuncInfo _g_info = {
			CC_STDCALL, vtRet, 7, {
			_DispEvArgTypeTraits<T1>::VT,
			_DispEvArgTypeTraits<T2>::VT,
			_DispEvArgTypeTraits<T3>::VT,
			_DispEvArgTypeTraits<T4>::VT,
			_DispEvArgTypeTraits<T5>::VT,
			_DispEvArgTypeTraits<T6>::VT,
			_DispEvArgTypeTraits<T7>::VT
			}
		};
		return _g_info;
	}
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, VARTYPE vtRet = VT_EMPTY>
struct DEFI8 {
	static const CStdFuncInfo& winx_call Prototype() {
		static CStdFuncInfo _g_info = {
			CC_STDCALL, vtRet, 8, {
			_DispEvArgTypeTraits<T1>::VT,
			_DispEvArgTypeTraits<T2>::VT,
			_DispEvArgTypeTraits<T3>::VT,
			_DispEvArgTypeTraits<T4>::VT,
			_DispEvArgTypeTraits<T5>::VT,
			_DispEvArgTypeTraits<T6>::VT,
			_DispEvArgTypeTraits<T7>::VT,
			_DispEvArgTypeTraits<T8>::VT
			}
		};
		return _g_info;
	}
};

// -------------------------------------------------------------------------
// WINX_DEFIn

#define WINX_DEFI0()										\
		DEFI0<VT_EMPTY>::Prototype()

#define WINX_DEFI1(T1)										\
		DEFI1<T1>::Prototype()

#define WINX_DEFI2(T1, T2)									\
		DEFI2<T1, T2>::Prototype()

#define WINX_DEFI3(T1, T2, T3)								\
		DEFI3<T1, T2, T3>::Prototype()

#define WINX_DEFI4(T1, T2, T3, T4)							\
		DEFI4<T1, T2, T3, T4>::Prototype()

#define WINX_DEFI5(T1, T2, T3, T4, T5)						\
		DEFI5<T1, T2, T3, T4, T5>::Prototype()

#define WINX_DEFI6(T1, T2, T3, T4, T5, T6)					\
		DEFI6<T1, T2, T3, T4, T5, T6>::Prototype()

#define WINX_DEFI7(T1, T2, T3, T4, T5, T6, T7)				\
		DEFI7<T1, T2, T3, T4, T5, T6, T7>::Prototype()

#define WINX_DEFI8(T1, T2, T3, T4, T5, T6, T7, T8)			\
		DEFI8<T1, T2, T3, T4, T5, T6, T7, T8>::Prototype()

// -------------------------------------------------------------------------
// WINX_DEFIn_RET

#define WINX_DEFI0_RET()									\
		DEFI0<VT_I4>::Prototype()

#define WINX_DEFI1_RET(T1)									\
		DEFI1<T1, VT_I4>::Prototype()

#define WINX_DEFI2_RET(T1, T2)								\
		DEFI2<T1, T2, VT_I4>::Prototype()

#define WINX_DEFI3_RET(T1, T2, T3)							\
		DEFI3<T1, T2, T3, VT_I4>::Prototype()

#define WINX_DEFI4_RET(T1, T2, T3, T4)						\
		DEFI4<T1, T2, T3, T4, VT_I4>::Prototype()

#define WINX_DEFI5_RET(T1, T2, T3, T4, T5)					\
		DEFI5<T1, T2, T3, T4, T5, VT_I4>::Prototype()

#define WINX_DEFI6_RET(T1, T2, T3, T4, T5, T6)				\
		DEFI6<T1, T2, T3, T4, T5, T6, VT_I4>::Prototype()

#define WINX_DEFI7_RET(T1, T2, T3, T4, T5, T6, T7)			\
		DEFI7<T1, T2, T3, T4, T5, T6, T7, VT_I4>::Prototype()

#define WINX_DEFI8_RET(T1, T2, T3, T4, T5, T6, T7, T8)		\
		DEFI8<T1, T2, T3, T4, T5, T6, T7, T8, VT_I4>::Prototype()

// -------------------------------------------------------------------------
// class DispEventBaseImpl

template <class EventEntry, const IID* pdiid = &GUID_NULL>
class WINX_NO_VTABLE DispEventBaseImpl
{
public:
	typedef EventEntry DispEvImplClass;

public:
	IDispatch* winx_call GetDispatch() {
		return (IDispatch*)this;
	}

public:
	virtual HRESULT STDMETHODCALLTYPE _LocDEQueryInterface(REFIID riid, void ** ppvObject)
	{
		if (riid == *pdiid || 
			riid == IID_IUnknown ||
			riid == IID_IDispatch)
		{
			if (ppvObject == NULL)
				return E_POINTER;
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
			return E_NOINTERFACE;
	}

	// These are here only to support use in non-COM objects
	virtual ULONG STDMETHODCALLTYPE AddRef()
		{ return 1; }
	virtual ULONG STDMETHODCALLTYPE Release()
		{ return 1; }

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo)
		{ return E_NOTIMPL; }

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
		{ return E_NOTIMPL; }

	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(
		REFIID riid, LPOLESTR* rgszNames, UINT cNames,
		LCID lcid, DISPID* rgdispid)
		{ return E_NOTIMPL; }

	virtual HRESULT STDMETHODCALLTYPE Invoke(
		DISPID dispidMember, REFIID riid,
		LCID lcid, WORD /*wFlags*/, DISPPARAMS* pdispparams, VARIANT* pvarResult,
		EXCEPINFO* /*pexcepinfo*/, UINT* /*puArgErr*/)
	{
		EventEntry* pEntry = static_cast<EventEntry*>(this);

		CStdFuncInfo info;
		ClosurePtr closure;
		HRESULT hr = pEntry->FindEventEntry(dispidMember, lcid, &info, &closure);
		if (FAILED(hr))
			return hr;

		hr = ClosureCall(info, closure, pdispparams, pvarResult);
		WINX_ASSERT_OK(hr);
		return hr;
	}
};

// -------------------------------------------------------------------------
// class DispEventSimpleImpl

template <class EventEntry, const IID* pdiid, class DispEvClass>
class DispEventSimpleImpl : public DispEventBaseImpl<EventEntry, pdiid>
{
protected:
	DWORD m_dwEventCookie;
	
public:
	typedef DispEvClass DispEvImplClass;

	DispEventSimpleImpl()
		{ m_dwEventCookie = 0xFEFEFEFE; }

public:
	// Helpers for sinking events on random IUnknown*

	HRESULT winx_call DispEventAdvise(IUnknown* pUnk, const IID* piid)
	{
		WINX_ASSERT(m_dwEventCookie == 0xFEFEFEFE);
		return DispEvAdvise(pUnk, (IUnknown*)this, *piid, &m_dwEventCookie);
	}
	HRESULT winx_call DispEventUnadvise(IUnknown* pUnk, const IID* piid)
	{
		HRESULT hr = DispEvUnadvise(pUnk, *piid, m_dwEventCookie);
		m_dwEventCookie = 0xFEFEFEFE;
		return hr;
	}

public:
	// DispEventAdvise(或者Advise) / DispEventUnadvise(或者Unadvise)

	HRESULT winx_call DispEventAdvise(IUnknown* pUnk)
	{
		WINX_ASSERT(m_dwEventCookie == 0xFEFEFEFE);
		return DispEvAdvise(pUnk, (IUnknown*)this, *pdiid, &m_dwEventCookie);
	}
	HRESULT winx_call DispEventUnadvise(IUnknown* pUnk)
	{
		HRESULT hr = DispEvUnadvise(pUnk, *pdiid, m_dwEventCookie);
		m_dwEventCookie = 0xFEFEFEFE;
		return hr;
	}

	HRESULT winx_call Advise(IUnknown* pUnk)
	{
		WINX_ASSERT(m_dwEventCookie == 0xFEFEFEFE);
		return DispEvAdvise(pUnk, (IUnknown*)this, *pdiid, &m_dwEventCookie);
	}
	HRESULT winx_call Unadvise(IUnknown* pUnk)
	{
		HRESULT hr = DispEvUnadvise(pUnk, *pdiid, m_dwEventCookie);
		m_dwEventCookie = 0xFEFEFEFE;
		return hr;
	}
};

// =========================================================================
// Dispatch Event Sink Helper

typedef int (CDECL *DispEvNullHandler)(UINT, ...);

class _DispEvNo	 { int __unused; };
class _DispEvYes { _DispEvNo __no[2]; };

_DispEvNo  _DispEvValid(int, DispEvNullHandler);
_DispEvYes _DispEvValid(int, ...);

#define WINX_DISPEV_HANDLER_VALID(checkHandler)								\
	( sizeof(_DispEvValid(0, &DispEvImplClass::checkHandler)) == sizeof(_DispEvYes) )

#define WINX_DISPEV_NULL_HANDLER(fn)										\
public:																		\
	static int CDECL fn(UINT, ...) {										\
		WINX_REPORT("Don't call me!!!\n");									\
		return 0;															\
	}																		\
private:

// 注意: 我们推荐Sink的事件名有通用前缀，以避免重名。
// 例如: WebBrowser_OnDownloadComplete，不要把事件名取为OnDownloadComplete。
#define WINX_NULL_SINK(fn)			WINX_DISPEV_NULL_HANDLER(fn)

// -------------------------------------------------------------------------
// WINX_SINK_BEGIN/WINX_SINK_END

#define WINX_SINK_BEGIN()													\
public:																		\
	HRESULT winx_call FindEventEntry(										\
		IN DISPID dispidMember, LCID lcid,									\
		OUT CStdFuncInfo* pInfo, OUT ClosurePtr* pClosure)					\
	{

#define WINX_SINK_CLOSURE(dispid, pThis, fn, info)							\
		if (dispidMember == (dispid))										\
		{																	\
			*pInfo = (info);												\
			pClosure->UnsafeInit(pThis, fn);								\
			return S_OK;													\
		}

#define WINX_SINK_EX(dispid, Type, fn, info, fnRef)							\
		if (WINX_DISPEV_HANDLER_VALID(fnRef) && dispidMember == (dispid))	\
		{																	\
			*pInfo = (info);												\
			pClosure->UnsafeInit(											\
				static_cast<Type*>(this),									\
				&Type::fn);													\
			return S_OK;													\
		}

#define WINX_SINK(dispid, fn, info)											\
		WINX_SINK_EX(dispid, DispEvImplClass, fn, info, fn)

#define WINX_SINK_SABUFFER(dispid, fn, fnImpl)								\
		WINX_SINK_EX(dispid, DispEvImplClass, fnImpl, WINX_DEFI1(SABUFFER*), fn)

#define WINX_SINK_PSABUFFER(dispid, fn, fnImpl)								\
		WINX_SINK_EX(dispid, DispEvImplClass, fnImpl, WINX_DEFI1(SABUFFER**), fn)

#define WINX_SINK_DEFI0(dispid, fn)											\
		WINX_SINK(dispid, fn, WINX_DEFI0())

#define WINX_SINK_DEFI1(dispid, fn, T1)										\
		WINX_SINK(dispid, fn, WINX_DEFI1(T1))

#define WINX_SINK_DEFI2(dispid, fn, T1, T2)									\
		WINX_SINK(dispid, fn, WINX_DEFI2(T1, T2))

#define WINX_SINK_DEFI3(dispid, fn, T1, T2, T3)								\
		WINX_SINK(dispid, fn, WINX_DEFI3(T1, T2, T3))

#define WINX_SINK_DEFI4(dispid, fn, T1, T2, T3, T4)							\
		WINX_SINK(dispid, fn, WINX_DEFI4(T1, T2, T3, T4))

#define WINX_SINK_DEFI5(dispid, fn, T1, T2, T3, T4, T5)						\
		WINX_SINK(dispid, fn, WINX_DEFI5(T1, T2, T3, T4, T5))

#define WINX_SINK_DEFI6(dispid, fn, T1, T2, T3, T4, T5, T6)					\
		WINX_SINK(dispid, fn, WINX_DEFI6(T1, T2, T3, T4, T5, T6))

#define WINX_SINK_DEFI7(dispid, fn, T1, T2, T3, T4, T5, T6, T7)				\
		WINX_SINK(dispid, fn, WINX_DEFI7(T1, T2, T3, T4, T5, T6, T7))

#define WINX_SINK_DEFI8(dispid, fn, T1, T2, T3, T4, T5, T6, T7, T8)			\
		WINX_SINK(dispid, fn, WINX_DEFI8(T1, T2, T3, T4, T5, T6, T7, T8))

#define WINX_SINK_DEFI0_RET(dispid, fn)										\
		WINX_SINK(dispid, fn, WINX_DEFI0_RET())

#define WINX_SINK_DEFI1_RET(dispid, fn, T1)									\
		WINX_SINK(dispid, fn, WINX_DEFI1_RET(T1))

#define WINX_SINK_DEFI2_RET(dispid, fn, T1, T2)								\
		WINX_SINK(dispid, fn, WINX_DEFI2_RET(T1, T2))

#define WINX_SINK_DEFI3_RET(dispid, fn, T1, T2, T3)							\
		WINX_SINK(dispid, fn, WINX_DEFI3_RET(T1, T2, T3))

#define WINX_SINK_DEFI4_RET(dispid, fn, T1, T2, T3, T4)						\
		WINX_SINK(dispid, fn, WINX_DEFI4_RET(T1, T2, T3, T4))

#define WINX_SINK_DEFI5_RET(dispid, fn, T1, T2, T3, T4, T5)					\
		WINX_SINK(dispid, fn, WINX_DEFI5_RET(T1, T2, T3, T4, T5))

#define WINX_SINK_DEFI6_RET(dispid, fn, T1, T2, T3, T4, T5, T6)				\
		WINX_SINK(dispid, fn, WINX_DEFI6_RET(T1, T2, T3, T4, T5, T6))

#define WINX_SINK_DEFI7_RET(dispid, fn, T1, T2, T3, T4, T5, T6, T7)			\
		WINX_SINK(dispid, fn, WINX_DEFI7_RET(T1, T2, T3, T4, T5, T6, T7))

#define WINX_SINK_DEFI8_RET(dispid, fn, T1, T2, T3, T4, T5, T6, T7, T8)		\
		WINX_SINK(dispid, fn, WINX_DEFI8_RET(T1, T2, T3, T4, T5, T6, T7, T8))

#define WINX_SINK_END()														\
		return E_NOTIMPL;													\
	}																		\
private:

// =========================================================================
// namespace winx

NS_WINX_BEGIN

using ::DispEvAdvise;
using ::DispEvUnadvise;

using ::CStdFuncInfo;
using ::ClosurePtr;
using ::ClosureCallThunk;

using ::ClosureCall;

using ::DEFI0;
using ::DEFI1;
using ::DEFI2;
using ::DEFI3;
using ::DEFI4;
using ::DEFI5;
using ::DEFI6;
using ::DEFI7;
using ::DEFI8;

using ::DispEventBaseImpl;
using ::DispEventSimpleImpl;

NS_WINX_END

// =========================================================================
// $Log: DispatchEvent.h,v $
// Revision 1.2  2006/08/26 16:27:53  xushiwei
// WINX-Core:
//   Dispatch Event Implement(Connect-Point support)
//

#endif /* WINX_DISPATCHEVENT_H */
