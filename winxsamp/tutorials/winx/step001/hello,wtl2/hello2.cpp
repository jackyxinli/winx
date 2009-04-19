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
// Date: 2006-8-19 18:24:19
// 
// $Id: hello2.cpp,v 1.1 2006/08/21 18:51:42 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <atlbase.h>
#include <atlapp.h>
extern WTL::CAppModule _Module;
#include <atlwin.h>
#include <atlframe.h>

// -------------------------------------------------------------------------
// CHelloMainFrame

class CHelloMainFrame : public CFrameWindowImpl<CHelloMainFrame>
{
public:
	BEGIN_MSG_MAP(CHelloMainFrame)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		CHAIN_MSG_MAP(CFrameWindowImpl<CHelloMainFrame>)
	END_MSG_MAP()

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CPaintDC dc(m_hWnd);
		dc.TextOut(1, 1, _T("Hello, WTL!"));
		return 0;
	}
};

// -------------------------------------------------------------------------

WTL::CAppModule _Module;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	_Module.Init(NULL, hInstance);
	{
		CMessageLoop theLoop;
		_Module.AddMessageLoop(&theLoop);

		CHelloMainFrame wndMain;
		wndMain.Create(NULL, NULL, _T("Hello"));
		wndMain.ShowWindow(nCmdShow);
		
		theLoop.Run();
	}
	_Module.Term();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello2.cpp,v $
// Revision 1.1  2006/08/21 18:51:42  xushiwei
// WINX-Core:
//    Window::Create, MainFrame, StackWindowObject(WINX_STACK_OBJECT) support
//
// Revision 1.1  2006/08/19 11:09:45  xushiwei
// tutorial-001: hello sdk, wtl, winx
//
