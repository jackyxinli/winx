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
// Date: 2006-8-19 18:26:15
// 
// $Id: hello.cpp,v 1.1 2006/09/16 18:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include <wtlext/BitmapHyperLink.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_MAINDLG>
{
	WINX_ICON(IDR_MAINFRAME);

private:
	WTL::CBitmapHyperLink m_Link1;
	WTL::CBitmapHyperLink m_Link2;
	WTL::CBitmapHyperLink m_Link3;

public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		m_Link1.SetHyperLink(_T("http://msdn.microsoft.com"));
		m_Link1.SetLabel(_T("Have a question? Click here"));
		m_Link1.SubclassWindow(GetDlgItem(IDC_STATIC_URL1));
		m_Link1.SetExtendedStyle(HLINK_LEFTIMAGE | HLINK_NOTOOLTIP);
		HBITMAP hBmp = m_Link1.AddBitmap(IDB_ARROW);
		ATLASSERT(hBmp);
		
		m_Link2.SetHyperLink(_T("http://www.codeproject.com"));
		m_Link2.SetLabel(_T("Welcome to the Code Project"));
		m_Link2.SubclassWindow(GetDlgItem(IDC_STATIC_URL2));
		m_Link2.SetExtendedStyle(HLINK_RIGHTIMAGE | HLINK_NOTOOLTIP);
		hBmp = m_Link2.AddBitmap(IDB_ARROW);
		ATLASSERT(hBmp);
		
		m_Link3.SetHyperLink(_T("http://www.google.com"));
		m_Link3.SetLabel(_T("Looking for something?"));
		m_Link3.SubclassWindow(GetDlgItem(IDC_STATIC_URL3));
		m_Link3.SetExtendedStyle(HLINK_LEFTIMAGE | HLINK_UNDERLINEHOVER | HLINK_NOTOOLTIP);
		m_Link3.SetLinkColor(RGB(0, 0, 0));
		m_Link3.SetVisitedColor(RGB(0, 0, 0));
		m_Link3.SetHoverColor(RGB(0, 0, 255));
		hBmp = m_Link3.AddBitmap(IDB_ARROW);
		ATLASSERT(hBmp);

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
// Revision 1.1  2006/09/16 18:54:04  xushiwei
// WTL-Extension: BitmapButton, BitmapHyperLink, ColorButton, IconEdit
//
