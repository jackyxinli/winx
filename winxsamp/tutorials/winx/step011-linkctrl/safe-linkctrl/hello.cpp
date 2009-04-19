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
// Date: 2006-8-25 10:17:54
// 
// $Id: hello.cpp,v 1.2 2006/08/25 15:15:26 xushiwei Exp $
// -----------------------------------------------------------------------*/

#define _WIN32_WINNT 0x0501
#include <winx/Config.h>
#include <winx.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
public:
	WINX_NOTIFY_BEGIN();
		WINX_NOTIFY_RANGE(IDC_CUSTOM1, IDC_CUSTOM2, NM_RETURN, OnNotifyClickLink);
		WINX_NOTIFY_RANGE(IDC_CUSTOM1, IDC_CUSTOM2, NM_CLICK, OnNotifyClickLink);
	WINX_NOTIFY_END();

	VOID OnNotifyClickLink(HWND hWnd, LPNMHDR pnmh, LRESULT* pResult)
	{
		PNMLINK pNMLink = (PNMLINK)pnmh;
		winx::MsgBox(hWnd, pNMLink->item.szUrl, L"Information");
	}
};

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	InitCommonControls(ICC_LINK_CLASS);
	winx::SafeLinkCtrl::RegisterClass();
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.2  2006/08/25 15:15:26  xushiwei
// Demo: ActiveX, WebBrowser(IE), Theme(XPStyle), SafeCtrl(of LinkCtrl), MouseWheel event
//
