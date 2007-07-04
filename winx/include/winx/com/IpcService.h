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
// Module: winx/com/IpcService.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-2-9 12:26:25
// Description: IPC - ServiceObject
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __WINX_COM_IPCSERVICE_H__
#define __WINX_COM_IPCSERVICE_H__

#ifndef __WINX_DISPATCH_H__
#include "../Dispatch.h"
#endif

#ifndef __WINX_DISPATCHEVENT_H__
#include "../DispatchEvent.h"
#endif

__WINX_BEGIN

// =========================================================================
// Ipc ServiceObject API

// -------------------------------------------------------------------------
// GetServiceObjectUUID

inline VOID winx_call GetServiceObjectUUID(
	IN HWND hWndService, DWORD dwService, GUID& uuid)
{
	WINX_ASSERT(sizeof(GUID)/sizeof(DWORD) == 4);
	DWORD* dw = (DWORD*)&uuid;
	dw[0] = dwService;
	dw[2] = ::GetWindowThreadProcessId(hWndService, &dw[1]);
	dw[3] = dwService;
}

// -------------------------------------------------------------------------
// RegisterServiceObject/RevokeServiceObject
// GetServiceObject/HasServiceObject

inline BOOL winx_call HasServiceObject(
	IN HWND hWndService, IN LPCTSTR szServiceName)
{
	return (BOOL)::GetProp(hWndService, szServiceName);
}

inline HRESULT winx_call RegisterServiceObject(
	IN HWND hWndService, IN LPCTSTR szServiceName, IN IUnknown* pService, OUT DWORD* pdwRegister)
{
	WINX_ASSERT(
		!HasServiceObject(hWndService, szServiceName)
		);

	GUID guidHost;
	GetServiceObjectUUID(hWndService, (DWORD)pService, guidHost);
	HRESULT hr = ::RegisterActiveObject(pService, guidHost, ACTIVEOBJECT_STRONG, pdwRegister);
	if (FAILED(hr))
		return hr;

	::SetProp(hWndService, szServiceName, (HANDLE)pService);
	return S_OK;
}

inline HRESULT winx_call RevokeServiceObject(
	IN HWND hWndService, IN LPCTSTR szServiceName, IN DWORD dwRegister)
{
	WINX_ASSERT(
		HasServiceObject(hWndService, szServiceName)
		);
	
	DWORD dwService = (DWORD)::RemoveProp(hWndService, szServiceName);
	if (dwService)
	{
		WINX_ASSERT(dwRegister);
		return ::RevokeActiveObject(dwRegister, NULL);
	}
	return S_FALSE;
}

inline HRESULT winx_call GetServiceObject(
	IN HWND hWndService, IN LPCTSTR szServiceName, IUnknown** ppUnk)
{
	DWORD dwService = (DWORD)::GetProp(hWndService, szServiceName);
	if (dwService)
	{
		GUID guidHost;
		GetServiceObjectUUID(hWndService, dwService, guidHost);
		return ::GetActiveObject(guidHost, NULL, ppUnk);
	}
	return E_INVALIDARG;
}

template <class Interface>
HRESULT winx_call GetServiceObject(
	IN HWND hWndService, IN LPCTSTR szServiceName, Interface** pp)
{
	IUnknown* pUnk;
	HRESULT hr = GetServiceObject(hWndService, szServiceName, &pUnk);
	if (SUCCEEDED(hr))
	{
		hr = pUnk->QueryInterface(WINX_UUID(Interface), (void**)pp);
		pUnk->Release();
	}
	return hr;
}

// =========================================================================
// Winx IpcTarget Service

#define WINX_IPCTARGET_SERVICE		WINX_TEXT("winx.ipc.target")

// -------------------------------------------------------------------------
// IpcTargetDisp

#define DISPID_TARGET_PUTDATA		0x01

template <class WindowImplClass>
class IpcTargetDisp : public DispEventBaseImpl<WindowImplClass>
{
private:
	DWORD m_dwRegister;

public:
	WINX_NULL_SINK(IpcTarget_OnPutData);

	WINX_SINK_BEGIN();
		WINX_SINK_DEFI1_RET(DISPID_TARGET_PUTDATA, IpcTarget_OnPutData, IUnknown*);
	WINX_SINK_END();
	
#if (0)
	//
	// Event Prototype
	//
	HRESULT __stdcall IpcTarget_OnPutData(IUnknown* lpData)
	{
		return E_NOTIMPL;
	}
#endif

public:
	typedef IpcTargetDisp<WindowImplClass> IpcTargetDispType;

	HRESULT winx_call Register(HWND hWnd) {
		return RegisterServiceObject(
			hWnd, WINX_IPCTARGET_SERVICE, (IUnknown*)this, &m_dwRegister);
	}

	HRESULT winx_call Revoke(HWND hWnd) {
		return RevokeServiceObject(
			hWnd, WINX_IPCTARGET_SERVICE, m_dwRegister);
	}
};

inline BOOL winx_call HasIpcTargetService(HWND hWnd)
{
	return HasServiceObject(hWnd, WINX_IPCTARGET_SERVICE);
}

template <class IpcTargetType>
inline HRESULT winx_call GetIpcTargetObject(HWND hWnd, IpcTargetType** pp)
{
	return GetServiceObject(hWnd, WINX_IPCTARGET_SERVICE, pp);
}

inline HRESULT winx_call IpcTarget_PutData(HWND hWnd, IUnknown* pData)
{
	DispatchHandle pIpcTarget;
	HRESULT hr = GetServiceObject(hWnd, WINX_IPCTARGET_SERVICE, &pIpcTarget.p);
	if (SUCCEEDED(hr)) {
		pIpcTarget.CallMethod(DISPID_TARGET_PUTDATA, pData);
		pIpcTarget.Release();
	}
	return hr;
}

// -------------------------------------------------------------------------
// WINX_IPCTARGET

#define WINX_IPCTARGET()													\
public:																		\
	VOID winx_msg OnIpcTargetInit(HWND hWnd) {								\
		IpcTargetDispType::Register(hWnd);									\
	}																		\
	VOID winx_msg OnIpcTargetTerm(HWND hWnd) {								\
		IpcTargetDispType::Revoke(hWnd);									\
	}																		\

// =========================================================================
// Winx IpcHost Service

#define WINX_IPCHOST_SERVICE		WINX_TEXT("winx.ipc.host")

// -------------------------------------------------------------------------
// IHostWindowDisp

#define DISPID_HOST_LISTEN			0x01
#define DISPID_HOST_STOPLISTEN		0x02

// -------------------------------------------------------------------------
// IHostMessageDisp

#define DISPID_HOST_DESTROY			0x01
#define DISPID_HOST_SIZING			0x02
#define DISPID_HOST_MOVING			0x03

#pragma pack(1)

struct HostSizingParam
{
	BOOL bCancel;
	UINT nSide;
	RECT rcRect;
};

struct HostMovingParam
{
	BOOL bCancel;
	UINT nUnused;
	RECT rcRect;
};

#pragma pack()

// -------------------------------------------------------------------------
// class HostMessageDisp

template <class Type>
class HostMessageDisp : public DispEventSimpleImpl<HostMessageDisp<Type>, &GUID_NULL, Type>
{
public:
	typedef HostMessageDisp<Type> HostMessageDispType;

#if (0)
	//
	// Event Prototype
	//

	VOID __stdcall OnHostDestroy()
	{
	}

	LRESULT __stdcall OnHostSizing(
		IN UINT nSide, IN LPRECT lpRect, IN BOOL* pbEatThisMessage)
	{
		return FALSE;
	}

	LRESULT __stdcall OnHostMoving(
		IN UINT nUnused, IN LPRECT lpRect, IN BOOL* pbEatThisMessage)
	{
		return FALSE;
	}
#endif

	VOID __stdcall Impl_OnHostSizing(SABUFFER** saBuf)
	{
		Type* pThis = static_cast<Type*>(this);

		SABufferAccess<HostSizingParam> sa(*saBuf);
		HostSizingParam& pa = *sa;
		pThis->OnHostSizing(pa.nSide, &pa.rcRect, &pa.bCancel);
	}

	VOID __stdcall Impl_OnHostMoving(SABUFFER** saBuf)
	{
		Type* pThis = static_cast<Type*>(this);

		SABufferAccess<HostMovingParam> sa(*saBuf);
		HostMovingParam& pa = *sa;
		pThis->OnHostMoving(pa.nUnused, &pa.rcRect, &pa.bCancel);
	}

	//
	// Event Sink Map
	//

	WINX_NULL_SINK(OnHostDestroy);
	WINX_NULL_SINK(OnHostSizing);
	WINX_NULL_SINK(OnHostMoving);

	WINX_SINK_BEGIN();
		WINX_SINK_DEFI0(DISPID_HOST_DESTROY, OnHostDestroy);
		WINX_SINK_PSABUFFER(DISPID_HOST_MOVING, OnHostMoving, Impl_OnHostMoving);
		WINX_SINK_PSABUFFER(DISPID_HOST_SIZING, OnHostSizing, Impl_OnHostSizing);
	WINX_SINK_END();

public:
	//
	// Advise/Unadvise
	//

	HRESULT winx_call Advise(IN HWND hWndHost)
	{
		DispatchHandle pHost;
		HRESULT hr = GetServiceObject(hWndHost, WINX_IPCHOST_SERVICE, &pHost.p);
		if (SUCCEEDED(hr)) {
			hr = pHost.CallMethod(DISPID_HOST_LISTEN, (IDispatch*)this);
			pHost.Release();
		}
		return hr;
	}

	HRESULT winx_call Unadvise(IN HWND hWndHost)
	{
		DispatchHandle pHost;
		HRESULT hr = GetServiceObject(hWndHost, WINX_IPCHOST_SERVICE, &pHost.p);
		if (SUCCEEDED(hr)) {
			pHost.CallMethod(DISPID_HOST_STOPLISTEN, (IDispatch*)this);
			pHost.Release();
		}
		return hr;
	}
};

// -------------------------------------------------------------------------
// class HostWindowDisp

class HostWindowDisp : public DispEventBaseImpl<HostWindowDisp>
{
private:
	typedef std::vector<IDispatch*> _Listens;
	_Listens m_listens;
	DWORD m_dwRegister;
	
public:
	struct _ReleaseObj {
		void operator()(DispatchHandle p) {
			p.Release();
		}
	};
	struct _HostDestroy {
		void operator()(DispatchHandle p) {
			p.CallMethod(DISPID_HOST_DESTROY);
		}
	};
	template <DISPID dispid>
	struct _HostSizingT {
		VARIANT var;
		_HostSizingT(IN SABUFFER** saBuf) {
			::VariantInit(&var, saBuf);
		}
		void operator()(DispatchHandle p) {
			p.CallMethod(dispid, &var);
		}
	};
	typedef _HostSizingT<DISPID_HOST_SIZING> _HostSizing;
	typedef _HostSizingT<DISPID_HOST_MOVING> _HostMoving;

	~HostWindowDisp()
	{
		WINX_TRY {
			std::for_each(m_listens.begin(), m_listens.end(), _ReleaseObj());
		} WINX_TRY_END;
	}

	VOID winx_msg OnHostDestroy()
	{
		WINX_TRY {
			std::for_each(m_listens.begin(), m_listens.end(), _HostDestroy());
		} WINX_TRY_END;
	}

	VOID winx_msg OnHostMoveOrSizing(
		IN UINT message, IN UINT nSide, IN LPRECT lpRect, IN BOOL* pCancel)
	{
		WINX_TRY {
			SABuffer saBuf(sizeof(HostSizingParam));
			{
				SABufferAccess<HostSizingParam> sa(saBuf);
				HostSizingParam& pa = *sa;
				pa.nSide = nSide;
				pa.rcRect = *lpRect;
				pa.bCancel = FALSE;
			}
			if (message == WM_MOVING)
				std::for_each(m_listens.begin(), m_listens.end(), _HostMoving(&saBuf));
			else
				std::for_each(m_listens.begin(), m_listens.end(), _HostSizing(&saBuf));
			{
				SABufferAccess<HostSizingParam> sa(saBuf);
				HostSizingParam& pa = *sa;
				*lpRect = pa.rcRect;
				*pCancel = pa.bCancel;
			}
		}
		WINX_TRY_END;
	}

	BOOL winx_msg ProcessHostMessage(
		UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		BOOL bCancel = FALSE;
		if (m_listens.size())
		{
			switch (message)
			{
			case WM_MOVING:
			case WM_SIZING:
				OnHostMoveOrSizing(message, wParam, (LPRECT)lParam, &bCancel);
				*pResult = TRUE;
				break;
			case WM_NCDESTROY:
				OnHostDestroy();
				break;
			}
		}
		return bCancel;
	}
	
public:
	WINX_SINK_BEGIN();
		WINX_SINK_DEFI1_RET(DISPID_HOST_LISTEN, Listen, IDispatch*);
		WINX_SINK_DEFI1(DISPID_HOST_STOPLISTEN, StopListen, IDispatch*);
	WINX_SINK_END();

public:
	HRESULT __stdcall Listen(IDispatch* pDisp)
	{
		WINX_TRACE("---> HostWindowDisp::Listen - %p\n", pDisp);

		_Listens::iterator it = std::find(m_listens.begin(), m_listens.end(), pDisp);
		if (it != m_listens.end())
			return S_FALSE;

		m_listens.push_back(pDisp);
		WINX_TRY {
			pDisp->AddRef();
		} WINX_TRY_END;

		return S_OK;
	}

	VOID __stdcall StopListen(IDispatch* pDisp)
	{
		WINX_TRACE("---> HostWindowDisp::StopListen - %p\n", pDisp);

		_Listens::iterator it = std::find(m_listens.begin(), m_listens.end(), pDisp);
		if (it != m_listens.end())
		{
			IDispatch* tmp = *it;
			m_listens.erase(it);
			WINX_TRY {
				tmp->Release();
			} WINX_TRY_END;
		}
	}

public:
	HRESULT winx_call Register(HWND hWndHost) {
		return RegisterServiceObject(
			hWndHost, WINX_IPCHOST_SERVICE, (IUnknown*)this, &m_dwRegister);
	}

	HRESULT winx_call Revoke(HWND hWndHost) {
		return RevokeServiceObject(
			hWndHost, WINX_IPCHOST_SERVICE, m_dwRegister);
	}
};

// -------------------------------------------------------------------------
// WINX_IPCHOST

#define WINX_IPCHOST()														\
public:																		\
	VOID winx_msg OnIpcHostInit(HWND hWndHost) {							\
		_winx_ipcHost.Register(hWndHost);									\
	}																		\
	VOID winx_msg OnIpcHostTerm(HWND hWndHost) {							\
		_winx_ipcHost.Revoke(hWndHost);										\
	}																		\
	BOOL winx_msg ProcessHostMessage(										\
		HWND, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)	\
	{																		\
		return _winx_ipcHost.ProcessHostMessage(							\
			message, wParam, lParam, pResult);								\
	}																		\
private:																	\
	winx::HostWindowDisp _winx_ipcHost;

// -------------------------------------------------------------------------
// class IpcHostWindow

class IpcHostWindow : 
	public SubclassWindow<IpcHostWindow, NullWindowHandle, cateHostWindow>
{
	WINX_IPCHOST();
};

inline BOOL winx_call HasIpcHostService(HWND hWndHost)
{
	return HasServiceObject(hWndHost, WINX_IPCHOST_SERVICE);
}

inline HRESULT winx_call RegisterIpcHost(HWND hWndHost)
{
	WINX_ASSERT(!HasIpcHostService(hWndHost));
	IpcHostWindow::SubclassOnce(hWndHost);
	return S_OK;
}

// =========================================================================
// $Log: IpcService.h,v $

__WINX_END

#endif /* __WINX_COM_IPCSERVICE_H__ */
