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
// Module: winx/winsdk/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:38:11
// 
// $Id: Basic.h,v 1.7 2006/12/11 05:19:16 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WINSDK_BASIC_H__
#define __WINX_WINSDK_BASIC_H__

#if (0)
#define WINX_USE_WINSDK
#define WINX_USE_DEFSDK
#endif

// =========================================================================
// Visual C++ Version

#if defined(_MSC_VER) 
#	if (_MSC_VER > 1200)
#		define WINX_VC_NET
#	else
#		define WINX_VC6
#	endif
#else
#endif

// =========================================================================
// Reference Win32 SDK

#if defined(WINX_USE_WINSDK)

#ifndef _WINDOWS_
#include "../../../../winsdk/include/windows.h"
#endif

#if !defined( _OBJBASE_H_ )
#include "../../../../winsdk/include/objbase.h"
#endif

#ifndef _INC_COMMCTRL
#include "../../../../winsdk/include/commctrl.h"
#endif

#ifndef _INC_SHLWAPI
#include "../../../../winsdk/include/shlwapi.h"
#endif

#ifndef _RICHEDIT_
#include "../../../../winsdk/include/richedit.h"
#endif

#ifndef _RICHOLE_
#include "../../../../winsdk/include/richole.h"
#endif

#else // defined(WINX_USE_DEFSDK)

#ifndef _WINDOWS_
#include <windows.h>
#endif

#if !defined( _OBJBASE_H_ )
#include <objbase.h>
#endif

#ifndef _INC_COMMCTRL
#include <commctrl.h>
#endif

#ifndef _INC_SHLWAPI
#include <shlwapi.h>
#endif

#ifndef _RICHEDIT_
#include <richedit.h>
#endif

#ifndef _RICHOLE_
#include <richole.h>
#endif

#endif

// =========================================================================
// BasicTypes

#ifndef _INC_TCHAR
#include <tchar.h>
#endif

#if !defined(_W64)
typedef signed char INT8, *PINT8;
typedef signed short INT16, *PINT16;
typedef unsigned char UINT8, *PUINT8;
typedef unsigned short UINT16, *PUINT16;
#endif

// =========================================================================
// DisableConflictSymbols

//
// disable SubclassWindow macro in <windowx.h>
// disable CreateDialog macro in <winuser.h>
//
#undef SubclassWindow
#undef CreateDialog

WINCOMMCTRLAPI HIMAGELIST WINAPI ImageList_Read(LPSTREAM pstm);
WINCOMMCTRLAPI BOOL       WINAPI ImageList_Write(HIMAGELIST himl, LPSTREAM pstm);

// =========================================================================
// InitCommonControls - Helper

#pragma comment(lib, "shlwapi")
#pragma comment(lib, "comctl32")

inline BOOL InitCommonControls(DWORD dwFlags)
{
	INITCOMMONCONTROLSEX iccx = { sizeof(INITCOMMONCONTROLSEX), dwFlags };
	return InitCommonControlsEx(&iccx);
}

// =========================================================================
// InitRichEditControl

inline BOOL InitRichEditControl(BOOL fInitRichEditV1 = TRUE)
{
#if (_RICHEDIT_VER >= 0x0200)
	if (fInitRichEditV1)
		return ::LoadLibrary(_T("RICHED32.DLL")) != NULL;
	else
		return ::LoadLibrary(_T("RICHED20.DLL")) != NULL;
#else
	return ::LoadLibrary(_T("RICHED32.DLL")) != NULL;
#endif
}

// =========================================================================
// $Log: Basic.h,v $
// Revision 1.7  2006/12/11 05:19:16  xushiwei
// vs2005 __w64 support
//
// Revision 1.6  2006/09/13 17:05:11  xushiwei
// WINX-Basic: DefaultWindowHandle(see macro WINX_NULL_WINDOW_HANDLE)
// WINX-Core: AxModalDialog/AxModalessDialog
//
// Revision 1.5  2006/08/25 15:13:19  xushiwei
// Winx-Extension:
//    ActiveX, WebBrowser(IE), Theme(XPStyle) support
//
// Revision 1.3  2006/08/21 18:45:48  xushiwei
// WINX-Core:
//    Window::Create, MainFrame, StackWindowObject(WINX_STACK_OBJECT) support
//
// Revision 1.2  2006/08/20 07:33:15  xushiwei
// adjust the directory tree
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

#endif /* __WINX_WINSDK_BASIC_H__ */
