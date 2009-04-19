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
// Date: 2006-8-19 14:22:03
// 
// $Id: hello.cpp,v 1.4 2006/09/18 05:40:14 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx/Config.h>
#include <winx.h>
#include <winx/WebBrowser.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::AxModalDialog<CHelloDlg, IDD_HELLO>
{
public:
	BOOL winx_msg OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		winx::AxHandle activex;
		activex.DlgItem(hDlg, IDC_EXPLORER1);
		
		CComPtr<IWebBrowser2> spBrowser;
		activex.QueryControl(&spBrowser);

		CComVariant v;
		spBrowser->Navigate(CComBSTR("http://www.google.com"), &v, &v, &v, &v);
		return TRUE;
	}
};

// -------------------------------------------------------------------------

CComModule _Module;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	InitCommonControls();
	CComModuleInit module;
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.4  2006/09/18 05:40:14  xushiwei
// remove #include <winx/AxImpl.h>
//
// Revision 1.3  2006/09/14 14:44:46  xushiwei
// Winx-Extension-ActiveX support bugfix (Visual Studio 2005):
//  Failed to create ActiveX controls in dialog resource template.
//  (Reason: AtlAxDialogBox/AtlAxCreateDialog implementation was changed in high version ATL.)
//
// Revision 1.2  2006/08/25 15:15:26  xushiwei
// Demo: ActiveX, WebBrowser(IE), Theme(XPStyle), SafeCtrl(of LinkCtrl), MouseWheel event
//
