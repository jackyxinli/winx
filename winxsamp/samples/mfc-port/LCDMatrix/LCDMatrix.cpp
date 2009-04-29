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
// Module: LCDMatrix.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-2 22:23:39
// 
// $Id: LCDMatrix.cpp,v 1.1 2006/12/02 15:23:45 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "LCDMatrixDlg.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	InitCommonControls();
	
	CLCDMatrixDlg dlg;
	return dlg.DoModal();
}

// -------------------------------------------------------------------------
// $Log: LCDMatrix.cpp,v $
// Revision 1.1  2006/12/02 15:23:45  xushiwei
// Port MFC application: LCDMatrix to WINX
//
