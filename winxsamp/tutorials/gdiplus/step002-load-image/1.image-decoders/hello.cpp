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
// $Id: hello.cpp,v 1.2 2006/08/27 19:30:22 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx/Config.h>
#include <winx/Gdiplus.h>

// -------------------------------------------------------------------------

int main()
{
	std::OutputLog log;
	GdiplusAppInit gdiplus;
	Gdiplus::ListImageDecoders(log);
	getchar();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.2  2006/08/27 19:30:22  xushiwei
// WINX-Extension-Gdiplus:
//   image-encoders / image-decoders / MultiFrameImageCreator
//
