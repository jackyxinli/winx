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
// Module: hello.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-3-8 17:24:30
// 
// $Id: hello.cpp,v 1.2 2006/08/26 03:16:49 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include <winx/IPC.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : 
	public winx::ModelDialog<CHelloDlg, IDD_HELLO, winx::WindowHandle>,
	public winx::HostMessageDisp<CHelloDlg>
{
private:
	HWND hWndHost;
	winx::ListBoxHandle lb;

public:
	VOID __stdcall OnHostDestroy()
	{
		lb.AddString(_T("OnHostDestroy!!!\n"));
	}

	VOID __stdcall OnHostSizing(
		IN UINT nSide, IN LPRECT lpRect, IN BOOL* pbEatThisMessage)
	{
		lb.AddString(_T("OnHostSizing!!!\n"));
	}

	VOID __stdcall OnHostMoving(
		IN UINT nSide, IN LPRECT lpRect, IN BOOL* pbEatThisMessage)
	{
		winx::WindowRect rcWin(m_hWnd);
		::SetWindowPos(
			m_hWnd, NULL, 
			rcWin.left + lpRect->right - rcWin.left, rcWin.top, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	VOID OnDestroy(HWND hWnd)
	{
		HostMessageDispType::Unadvise(hWndHost);
	}
	
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		lb.DlgItem(hDlg, IDC_LIST1);
		hWndHost = ::FindWindow(NULL, _T("IpcHost"));
		if (!hWndHost)
			goto Fail;
		lb.AddString(_T("IpcHost window found!!!"));

		if (S_OK != HostMessageDispType::Advise(hWndHost))
			goto Fail;
		lb.AddString(_T("Connection succeed!!!\n"));
		return TRUE;
Fail:
		lb.AddString(_T("Connection failed!!!\n"));
		return TRUE;
	}
};

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	InitCommonControls();
	COleAppInit init;
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: $
