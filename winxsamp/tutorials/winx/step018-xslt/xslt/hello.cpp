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
// $Id: hello.cpp,v 1.1 2006/09/25 02:31:33 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx/Config.h>
#include <winx.h>
#include <winx/CommonDialogs.h>
#include <winx/msxml/XSLT.h>

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CComAppInit com;
	
	winx::CString appPath = winx::GetAppPath();
	winx::OpenFileDialog dlg(_T("XML Files(*.xml)\0*.xml\0All Files(*.*)\0*.*\0"));
	dlg.lpstrTitle = _T("XSLTConvert: please choose an input file ...");
	dlg.lpstrInitialDir = appPath;
	if (IDOK != dlg.DoModal())
		return -1;

	TCHAR xslFile[MAX_PATH], htmFile[MAX_PATH];
	PathRenameExtension(_tcscpy(xslFile, dlg.lpstrFile), _T(".xsl"));
	PathRenameExtension(_tcscpy(htmFile, dlg.lpstrFile), _T(".htm"));
	if (S_OK == winx::XSLTConvert(xslFile, dlg.lpstrFile, htmFile))
	{
		ShellExecute(NULL, _T("Open"), htmFile, NULL, NULL, SW_SHOW);
	}
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.1  2006/09/25 02:31:33  xushiwei
// WINX-Extension: msxml (XSLT)
//
