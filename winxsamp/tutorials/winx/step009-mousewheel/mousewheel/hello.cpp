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
// Date: 2006-8-25 9:23:00
// 
// $Id: hello.cpp,v 1.4 2006/11/22 12:16:21 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include "resource.h"

// -------------------------------------------------------------------------
// class MyView

class MyView : public winx::ListBox<MyView>
{
	WINX_CLASS("MyView");

public:
	void winx_msg OnLButtonDown(HWND hWnd, UINT uFlags, winx::CPoint pt)
	{
		::SetFocus(hWnd);
	}

	void winx_msg OnMouseWheel(HWND hWnd, UINT uFlags, INT zDelta, winx::CPoint pt)
	{
		winx::CString str;
		str.Format(_T("OnMouseWheel - zDelta: %d\n"), zDelta);
		SetCurSel(AddString(str));
	}
};

// -------------------------------------------------------------------------
// class CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
};

// -------------------------------------------------------------------------
// WinMain

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MyView::RegisterClass();

	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.4  2006/11/22 12:16:21  xushiwei
// MouseWheel event
//
