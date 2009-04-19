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
// Module: MyTestComApp.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-6 16:45:48
// 
// $Id: MyTestComApp.cpp,v 1.1 2006/12/06 15:25:44 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "resource.h"
#include "idl/MyTestComApp_i.h"
#include "idl/MyTestComApp_i.c"
#include <winx/com/Addins.h>

// -------------------------------------------------------------------------
// class CMainDialog

class CMainDialog : 
	public winx::ModelDialog<CMainDialog, IDD_MAINDLG>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<_Application, &IID__Application>
{
private:
	winx::ComAddins m_addins;

public:
	BEGIN_COM_MAP(CMainDialog)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(_Application)
	END_COM_MAP()
	
public:
	// interface _Application

	STDMETHOD(get_Caption)(BSTR __RPC_FAR *prop)
	{
		winx::CString str = GetWindowText();
		*prop = str.AllocSysString();
		return S_OK;
	}

	STDMETHOD(MsgBox)(BSTR msg, BSTR title)
	{
		winx::MsgBox(m_hWnd, msg, title, MB_TOPMOST);
		return S_OK;
	}

public:
	// Message processing

	BOOL winx_msg OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		ShowWindow(SW_SHOW);
		UpdateWindow();
		
		m_addins.InitApplication(this);
		m_addins.ConnectAll(HKEY_CURRENT_USER, _T("Software\\winx\\MyTestComApp2\\Addins"));
		{
			//now, we should initialize the application...
		}
		m_addins.StartupComplete();
		return TRUE;
	}

	VOID OnDestroy(HWND hWnd)
	{
		m_addins.BeginShutdown();
		{
			//now, we start to shutdown the application...
		}
		m_addins.DisconnectAll();
	}
};

// -------------------------------------------------------------------------
// WinMain

CComModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int nShowCmd)
{
	CComAppInit com;
	CComModuleInit module(NULL, hInstance, &LIBID_MyTestComAppLib);
	CComObject<CMainDialog> dlg;
	dlg.AddRef(); // I'm a stack object, to release me will crash.
	dlg.DoModal();
    return 0;
}

// -------------------------------------------------------------------------
// $Log: MyTestComApp.cpp,v $
// Revision 1.1  2006/12/06 15:25:44  xushiwei
// ComAddin - Interact: How an com application interact with the addins.
//
