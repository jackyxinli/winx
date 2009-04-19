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
// Date: 2006-8-23 13:18:38
// 
// $Id: hello.cpp,v 1.1 2006/08/23 05:26:34 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include <winx/CommonDialogs.h>

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	winx::OpenFileDialog dlg(_T("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0"));
	if (IDOK != dlg.DoModal())
		return -1;

	winx::CString str;
	str.Format(
		_T("szFile = %s;\nszFileTitle = %s;"),
		dlg.lpstrFile, (LPCTSTR)dlg.GetFileTitle());

	winx::MsgBox(str, _T("Information"));
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.1  2006/08/23 05:26:34  xushiwei
// WINX-CommonDialogs:
//   OpenFileDialog/SaveFileDialog, OpenMultiFilesDialog/SaveMultiFilesDialog
//
