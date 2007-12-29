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
// Module: MyTestAddin.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-6 22:53:04
// 
// $Id: MyTestAddin.h,v 1.1 2006/12/06 15:25:43 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __MYTESTADDIN_H__
#define __MYTESTADDIN_H__

#include "MyTestComApp_i.h"

// -------------------------------------------------------------------------
// class CMyTestAddin

class __declspec(uuid("367E9E30-2C68-435D-A1ED-A5CBF3D5533B")) CMyTestAddin;

using namespace AddInDesignerObjects;

class CMyTestAddin : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMyTestAddin, &__uuidof(CMyTestAddin)>,
	public IDispatchImpl<_IDTExtensibility2, &IID__IDTExtensibility2, &LIBID_AddInDesignerObjects>
{
private:
	_Application* m_app;

public:
	DECLARE_REGISTRY_RESOURCEID(ID_RGS_MyTestAddin)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CMyTestAddin)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(_IDTExtensibility2)
	END_COM_MAP()

	CMyTestAddin() : m_app(NULL) {}
	~CMyTestAddin() {}

public:
	STDMETHOD(OnConnection)(IDispatch * Application, 
		ext_ConnectMode ConnectMode, IDispatch * AddInInst, SAFEARRAY * * custom);

	STDMETHOD(OnDisconnection)(ext_DisconnectMode RemoveMode, SAFEARRAY * * custom);

	STDMETHOD(OnAddInsUpdate)(SAFEARRAY * * custom);

	STDMETHOD(OnStartupComplete)(SAFEARRAY * * custom);

	STDMETHOD(OnBeginShutdown)(SAFEARRAY * * custom);
};

// -------------------------------------------------------------------------
// $Log: MyTestAddin.h,v $
// Revision 1.1  2006/12/06 15:25:43  xushiwei
// ComAddin - Interact: How an com application interact with the addins.
//

#endif /* __MYTESTADDIN_H__ */
