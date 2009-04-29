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
// $Id: hello.cpp,v 1.3 2006/09/03 06:35:37 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include <winx/PropertySheet.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloPage

class CHelloPage : public winx::PropertyPage<CHelloPage, IDD_HELLO>
{
	WINX_STACK_OBJECT(TRUE); // for convenience

private:
	LPCTSTR m_str;

public:
	CHelloPage(LPCTSTR str = _T(""))
		: m_str(str) {}

	void OnPaint(HWND hWnd)
	{
		winx::PaintDC dc(hWnd);
		dc.TextOut(10, 10, m_str);
	}

	int PSN_OnApply(HWND hWnd)
	{
		winx::MsgBox(hWnd, _T("OnApply"), _T("Information"));
		return PSNRET_NOERROR;
	}
};

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CHelloPage page1(_T("This is page1"));
	page1.DoModal(NULL, _T("PropertySheet"), _T("SinglePage"));

	CHelloPage page2(_T("This is page2"));
	winx::PropertySheetT<2> sheet(_T("PropertySheet"));
	sheet.AddPage(page1.Create(_T("Page1")));
	sheet.AddPage(page2.Create(_T("Page2")));
	sheet.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.3  2006/09/03 06:35:37  xushiwei
// WINX-Extension: PropertySheet, PropertyPage
//
