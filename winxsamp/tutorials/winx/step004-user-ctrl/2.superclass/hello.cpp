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
// Date: 2006-8-22 16:44:16
// 
// $Id: hello.cpp,v 1.1 2006/08/22 09:02:17 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include "resource.h"

// -------------------------------------------------------------------------
// class CMyEdit - 使用子类化(Subclass)技术

class CMyEdit : public winx::Edit<CMyEdit>
{
public:
	VOID OnContextMenu(HWND hWnd, winx::CPoint pt)
	{
		//禁止了右键菜单...
	}
};

// -------------------------------------------------------------------------
// class CMyEdit2 - 使用超类化(Superclass)技术

//注意:
//	实际上一个类可以同时既是超类，也是子类，完全没有必要有CMyEdit2。
//	但是我们为了展示子类化技术和超类化技术的不同点，使用了两个类。

class CMyEdit2 : public winx::Edit<CMyEdit2>
{
	WINX_CLASS("MyEdit");
public:
	VOID OnContextMenu(HWND hWnd, winx::CPoint pt)
	{
		//禁止了右键菜单...
	}
};

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		CMyEdit::DoSubclassDlgItem(hDlg, IDC_EDIT1);
		return TRUE;
	}
};

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CMyEdit2::RegisterClass();
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.1  2006/08/22 09:02:17  xushiwei
// tutorial-004: Demonstrate how to implement user-defined controls
//
