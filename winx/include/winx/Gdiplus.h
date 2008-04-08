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
// Module: winx/Gdiplus.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-23 12:07:42
// 
// $Id: Gdiplus.h,v 1.1 2006/08/27 19:25:46 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_GDIPLUS_H__
#define __WINX_GDIPLUS_H__

#if (0)
#define WINX_DELAYLOAD_GDIPLUS
#endif

#ifndef __WINX_BASIC_H__
#include "Basic.h"
#endif

// -------------------------------------------------------------------------
// link gdiplus

#pragma comment(lib, "gdiplus")
#if defined(WINX_DELAYLOAD_GDIPLUS)
#	pragma comment(linker, "/delayload:gdiplus.dll")
#	pragma comment(lib, "delayimp.lib")
#endif

// -------------------------------------------------------------------------
// Reference Win32 SDK

#if defined(WINX_USE_WINSDK)

#ifndef _GDIPLUS_H
#include "../../../winsdk/include/GdiPlus.h"
#endif

#elif defined(WINX_USE_DEFSDK) || !defined(WINX_VC6)

#ifndef _GDIPLUS_H
#include <GdiPlus.h>
#endif

#endif

// -------------------------------------------------------------------------
// class GdiplusAppInit

class GdiplusAppInit
{
public:
	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartupOutput output;
	ULONG_PTR token;

public:
	GdiplusAppInit(
        Gdiplus::DebugEventProc debugEventCallback = NULL,
        BOOL suppressBackgroundThread = FALSE,
        BOOL suppressExternalCodecs = FALSE)
		: input(debugEventCallback, suppressBackgroundThread, suppressExternalCodecs)
	{
		Gdiplus::GdiplusStartup(&token, &input, &output);
	}
	~GdiplusAppInit()
	{
		Gdiplus::GdiplusShutdown(token);
	}
};

// -------------------------------------------------------------------------
// namespace Gdiplus

namespace Gdiplus {

#include "gdiplus/ImageCodec.h"

};

// -------------------------------------------------------------------------
// $Log: Gdiplus.h,v $
// Revision 1.1  2006/08/27 19:25:46  xushiwei
// WINX-Extension-Gdiplus:
//   image-encoders / image-decoders / MultiFrameImageCreator
//

#endif /* __WINX_GDIPLUS_H__ */
