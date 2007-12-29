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
// $Id: hello.cpp,v 1.2 2006/08/21 18:46:50 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include <winx/ext/CreditStatic.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		winx::CreditStatic* pDyStatic;
		winx::SubclassDlgItem(&pDyStatic, hDlg, IDC_DYSTATIC);
		TCHAR arrCredit[] = 
			_T("黑白棋\t|"
				"1.6版||"
				"作者\r|上海财经大学98信息系|"
				"蔡莹||"    // this is a quoted bitmap resource 
				"声明\r"
				"|本软件属于自由免费软件"
				"|您可以任意拷贝"
				"|如有任何意见或建议"
				"|请给我发邮件"
				"|E-mail:  shuker@263.net"
				"|您的意见对我很重要！"
				"|||||");
		pDyStatic->SetCredits(arrCredit,'|');
		pDyStatic->SetSpeed(DISPLAY_FAST);
		pDyStatic->SetColor(BACKGROUND_COLOR, RGB(0,255,200));
		pDyStatic->SetTransparent();
		pDyStatic->SetGradient(GRADIENT_RIGHT_DARK);
		pDyStatic->StartScrolling();
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
	EnableMemoryLeakCheck(); // this is optional
	CComModuleInit module;
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.2  2006/08/21 18:46:50  xushiwei
// WINX-Core:
//    Window::Create, MainFrame, StackWindowObject(WINX_STACK_OBJECT) support
//
// Revision 1.1  2006/08/19 17:08:36  xushiwei
// MFC-Port:
//   Demo - port MFC controls to WINX (CreditStatic, RoundButton)
//
