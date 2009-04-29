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
// Date: 2006-8-22 18:20:56
// 
// $Id: hello.cpp,v 1.2 2006/08/23 05:49:54 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
	WINX_DLGRESIZE_BEGIN(FALSE) // --> have a try: FALSE -> TRUE
		WINX_DLGRESIZE(IDC_RIGHTTOP, AnchorRightTop)
		WINX_DLGRESIZE(IDC_RIGHTBOTTOM, AnchorRightBottom)
		WINX_DLGRESIZE(IDC_LEFTBOTTOM, AnchorLeftBottom)
		WINX_DLGRESIZE(IDC_ALL, AnchorAll)
	WINX_DLGRESIZE_END()
};

// -------------------------------------------------------------------------

CComModule _Module;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CComModuleInit module;
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.2  2006/08/23 05:49:54  xushiwei
// WINX-Core:
//   Property(Icon, AppIcon), Layout(DialogResize, MinTrackSize)
//
// Revision 1.1  2006/08/22 10:36:53  xushiwei
// WINX-Core:
//   Property(Bkgrnd, Accel, Layout), MainFrame support
//   CommandDispatch, CommandState, Demo: User-defined-control(Subclass, Superclass, SuperDialog)
//
