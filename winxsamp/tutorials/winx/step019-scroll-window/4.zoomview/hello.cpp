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
// Date: 2007-1-28 18:17:50
// 
// $Id: hello.cpp,v 1.1 2006/08/22 09:02:17 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx/ScrollWindow.h>
#include "resource.h"

// -------------------------------------------------------------------------
// class CMyView

class CMyView : public winx::ZoomScrollWindow<CMyView>
{
	WINX_CLASS("MyView");

public:
	BOOL OnSetCursor(HWND hWnd, HWND hWndContainsCursor, UINT nHitTest, UINT uMouseMsg)
	{
		::SetCursor(::LoadCursor(NULL, nHitTest == HTCLIENT ? IDC_UPARROW : IDC_ARROW));
		return TRUE;
	}

	LRESULT OnCreate(HWND hWnd, LPCREATESTRUCT lpCS)
	{
		SetScrollSize(400, 400);
		SetZoomMode(ZOOMMODE_IN);
		return 0;
	}

	void DoPaint(winx::DCHandle dc)
	{
		dc.TextOut(1, 1, _T("Hello, ZoomScrollWindow! Please click left mouse button."));
		dc.TextOut(1, 100, _T("You are welcome!"));
	}
};

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
};

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CMyView::RegisterClass();

	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
