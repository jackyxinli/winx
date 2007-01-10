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
// Module: winx/WinCore.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:22:03
// 
// $Id: WinCore.h,v 1.2 2006/08/22 10:36:53 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WINCORE_H__
#define __WINX_WINCORE_H__

#ifndef __WINX_BASIC_H__
#include "Basic.h"
#endif

// -------------------------------------------------------------------------

#ifndef __WINX_WIN_BASICTYPES_H__
#include "win/BasicTypes.h"
#endif

#ifndef __WINX_WIN_BASIC_H__
#include "win/Basic.h"
#endif

#ifndef __WINX_WIN_WINDOWHANDLE_H__
#include "win/WindowHandle.h"
#endif

#ifndef __WINX_WIN_WINDOWMAP_H__
#include "win/WindowMap.h"
#endif

#ifndef __WINX_WIN_RESOURCE_H__
#include "win/Resource.h"
#endif

#ifndef __WINX_WIN_GDI_H__
#include "win/Gdi.h"
#endif

#ifndef __WINX_WIN_MESSAGE_H__
#include "win/Message.h"
#endif

#ifndef __WINX_WIN_WINDOW_H__
#include "win/Window.h"
#endif

#ifndef __WINX_WIN_DIALOG_H__
#include "win/Dialog.h"
#endif

#ifndef __WINX_WIN_CONTROL_H__
#include "win/Control.h"
#endif

#ifndef __WINX_WIN_FRAME_H__
#include "win/Frame.h"
#endif

// -------------------------------------------------------------------------
// $Log: WinCore.h,v $
// Revision 1.2  2006/08/22 10:36:53  xushiwei
// WINX-Core:
//   Property(Bkgrnd, Accel, Layout), MainFrame support
//   CommandDispatch, CommandState, Demo: User-defined-control(Subclass, Superclass, SuperDialog)
//
// Revision 1.1  2006/08/20 07:33:15  xushiwei
// adjust the directory tree
//
// Revision 1.2  2006/08/19 10:50:55  xushiwei
// WINX-Core: Message, Window, Dialog, Control
//
// Revision 1.1  2006/08/19 09:42:10  xushiwei
// WINX-Basic:
//   BasicTypes(CString, CFindFile, CRecentDocumentList, CComBSTR, etc)
//   Helper(MsgBox, GetThisModule, InitCommonControls, etc)
//   Init(CComAppInit, COleAppInit, CDebugAppInit, CComModuleInit, etc)
//   Resource(GetModuleIniPath, Bitmap, Icon, Menu, Brush, Accelerator, etc)
//   Gdi(Rgn, Pen, Brush, Font, Bitmap, DC, DoubleBuffer, etc)
//   WindowHandle, WindowMap, MessageMap
//   COM Support(Object, StackObject, FakeObject, COLESTR, etc)
//

#endif /* __WINX_WINCORE_H__ */
