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
// Module: winx.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 14:22:51
// 
// $Id: winx.h,v 1.13 2006/11/23 05:13:42 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_H
#define WINX_H

#if (0)
#define WINX_USE_WINSDK
#define WINX_USE_DEFBOOST	// Uses Default Boost
#define WINX_USE_APPMODULE
#define WINX_USE_DDX_FLOAT
#define WINX_NULL_WINDOW_HANDLE
#define WINX_REFLECT_CTLCOLOR
#endif

// -------------------------------------------------------------------------

#ifndef WINX_BASIC_H
#include "winx/Basic.h"
#endif

#ifndef WINX_TRACE_H
#include "winx/Trace.h"
#endif

#ifndef WINX_DISPATCH_H
#include "winx/Dispatch.h"
#endif

#ifndef WINX_DISPATCHEVENT_H
#include "winx/DispatchEvent.h"
#endif

#ifndef WINX_WINCORE_H
#include "winx/WinCore.h"
#endif

#if !defined(_ATL_NO_HOSTING)
#ifndef WINX_AXIMPL_H
#include "winx/AxImpl.h"
#endif
#endif

#ifndef WINX_BEHAVIOR_WINDOWBEHAVIOR_H
#include "winx/behavior/WindowBehavior.h"
#endif

#ifndef WINX_BEHAVIOR_AUTOHIDDEN_H
#include "winx/behavior/AutoHidden.h"
#endif

// -------------------------------------------------------------------------
// $Log: winx.h,v $
// Revision 1.13  2006/11/23 05:13:42  xushiwei
// boost::pool, boost::object_pool
//
// Revision 1.12  2006/09/18 03:34:15  xushiwei
// WINX-Core: Message: OnIdle support
//
// Revision 1.11  2006/09/16 11:25:16  xushiwei
// WINX_REFLECT_CTLCOLOR
//
// Revision 1.10  2006/09/13 17:05:11  xushiwei
// WINX-Basic: DefaultWindowHandle(see macro WINX_NULL_WINDOW_HANDLE)
// WINX-Core: AxModelDialog/AxModelessDialog
//
// Revision 1.9  2006/09/13 13:57:55  xushiwei
// WINX-Core:
//  DDX(DataExchange) - class WinDataExchange, WINX_DDX_BEGIN, WINX_DDX_END, etc.
//
// Revision 1.8  2006/09/03 04:32:56  xushiwei
// WINX-Core: Behavior(BehaviorPermit, BehaviorSwitch, AutoHidden, etc)
// WINX-Extension: PropertySheet, PropertyPage
//
// Revision 1.7  2006/09/02 01:49:08  xushiwei
// WINX-Basic:
//   Log: TraceWindow, Storage: TraceStorage
// WINX-Extension-Gdiplus:
//   EncoderParameters: ColorDepth, Transformation, SaveFlag, Compression, Quality
//
// Revision 1.6  2006/08/21 18:45:48  xushiwei
// WINX-Core:
//    Window::Create, MainFrame, StackWindowObject(WINX_STACK_OBJECT) support
//
// Revision 1.4  2006/08/19 10:50:54  xushiwei
// WINX-Core: Message, Window, Dialog, Control
//
// Revision 1.3  2006/08/19 09:42:10  xushiwei
// WINX-Basic:
//   BasicTypes(CString, CFindFile, CRecentDocumentList, CComBSTR, etc)
//   Helper(MsgBox, GetThisModule, InitCommonControls, etc)
//   Init(CComAppInit, COleAppInit, CDebugAppInit, CComModuleInit, etc)
//   Resource(GetModuleIniPath, Bitmap, Icon, Menu, Brush, Accelerator, etc)
//   Gdi(Rgn, Pen, Brush, Font, Bitmap, DC, DoubleBuffer, etc)
//   WindowHandle, WindowMap, MessageMap
//   COM Support(Object, StackObject, FakeObject, COLESTR, etc)
//

#endif /* WINX_H */
