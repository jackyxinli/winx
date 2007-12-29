// IEHost.cpp : main source file for IEHost.exe
//

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"
#include "maindlg.h"

CComModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, 
					 HINSTANCE /*hPrevInstance*/,
					 LPTSTR lpstrCmdLine,
					 int nCmdShow)
{
    InitCommonControls(ICC_WIN95_CLASSES);
	CComModuleInit module;
	CComAppInit com;
	CMainDlg dlg;
	dlg.DoModal();
	return 0;
}
