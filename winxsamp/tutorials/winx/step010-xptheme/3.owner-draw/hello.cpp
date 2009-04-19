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
// Date: 2006-8-25 9:47:19
// 
// $Id: hello.cpp,v 1.3 2006/09/13 17:07:43 xushiwei Exp $
// -----------------------------------------------------------------------*/

#define _WIN32_WINNT 0x0501
#include <winx/Config.h>
#include <winx.h>
#include <winx/Theme.h>
#include "resource.h"

// -------------------------------------------------------------------------
// class MyView

class MyView : public winx::Window<MyView>, public winx::Theme<MyView>
{
	WINX_CLASS("MyView");
	WINX_THEME();

public:
	LRESULT winx_msg OnCreate(HWND hWnd, LPCREATESTRUCT lpCS)
	{
		OpenThemeData(L"Edit");
		m_dwExtendedStyle |= THEME_EX_THEMECLIENTEDGE;
		return 0;
	}

	void winx_msg OnPaint(HWND hWnd)
	{
		winx::PaintDC dc(hWnd);
		winx::ClientRect rect(hWnd);
		if(m_hTheme != NULL)
		{
			DrawThemeText(
				dc, BP_PUSHBUTTON, PBS_NORMAL,
				L"MyView", -1, DT_SINGLELINE | DT_CENTER | DT_VCENTER, 0, &rect);
		}
		else
		{
			dc.DrawText(_T("MyView"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}
};

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
};

// -------------------------------------------------------------------------
// WinMain

CComModule _Module;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	InitCommonControls();
	CComModuleInit module;
	MyView::RegisterClass();
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.3  2006/09/13 17:07:43  xushiwei
// WINX-Basic: DefaultWindowHandle(see macro WINX_NULL_WINDOW_HANDLE)
// WINX-Core: AxModalDialog/AxModalessDialog
//
// Revision 1.2  2006/08/25 15:15:26  xushiwei
// Demo: ActiveX, WebBrowser(IE), Theme(XPStyle), SafeCtrl(of LinkCtrl), MouseWheel event
//
