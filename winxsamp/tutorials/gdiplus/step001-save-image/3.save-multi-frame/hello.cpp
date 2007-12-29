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
// $Id: hello.cpp,v 1.2 2006/09/13 13:55:21 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx/Config.h>
#include <winx/WinCore.h>
#include <winx/CommonDialogs.h>
#include <winx/Gdiplus.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
public:
	void OnOK(HWND hWnd)
	{
		winx::SaveFileDialog dlg(
			_T("TIFF files(*.tif;*.tiff)\0*.tif;*.tiff\0"), _T("*.tif"));
		
		if (IDOK != dlg.DoModal())
			return;
		
		TCHAR szFrame[] = _T("Frame1");

		USES_CONVERSION;
		CLSID clsid;
		Gdiplus::GetEncoderClsid(L"image/tiff", &clsid);
		Gdiplus::MultiFrameImageCreator multi(T2CW(dlg.lpstrFile), clsid);
		
		for (UINT i = 1; i < 10; ++i)
		{
			szFrame[5] = '0' + i;
			::SetDlgItemText(hWnd, IDC_PROMPT, szFrame);
			
			HBITMAP hbm = winx::ScreenCapture(hWnd);
			{
				Gdiplus::Bitmap bitmap(hbm, NULL);
				WINX_ASSERT(bitmap.GetLastStatus() == Gdiplus::Ok);
				multi.Add(bitmap);
			}
			::DeleteObject(hbm);
		}
		CloseDialog(hWnd);
	}
};

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	GdiplusAppInit gdiplus;
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.2  2006/09/13 13:55:21  xushiwei
// CloseDialog
//
// Revision 1.1  2006/08/27 19:20:49  xushiwei
// WINX-Extension-Gdiplus:
//   image-encoders / image-decoders / MultiFrameImageCreator
//
