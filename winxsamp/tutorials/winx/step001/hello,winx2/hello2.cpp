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
// Module: hello2.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 18:26:15
// 
// $Id: hello2.cpp,v 1.1 2006/08/21 18:51:42 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>

// -------------------------------------------------------------------------
// CHelloMainFrame

class CHelloMainFrame : public winx::MainFrame<CHelloMainFrame>
{
	WINX_CLASS("CHelloMainFrame");
public:
	void OnPaint(HWND hWnd)
	{
		winx::PaintDC dc(hWnd);
		dc.TextOut(1, 1, _T("Hello, WINX!"));
	}
};

// -------------------------------------------------------------------------
// WinMain

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CHelloMainFrame::RegisterClass();
	CHelloMainFrame wndMain;
	wndMain.Create(NULL, _T("Hello"));

	return RunMsgLoop();
}

// -------------------------------------------------------------------------
// $Log: hello2.cpp,v $
// Revision 1.1  2006/08/21 18:51:42  xushiwei
// WINX-Core:
//    Window::Create, MainFrame, StackWindowObject(WINX_STACK_OBJECT) support
//
