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
// Module: winx/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:23:22
// 
// $Id: Basic.h,v 1.9 2006/12/22 10:22:28 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_BASIC_H__
#define __WINX_BASIC_H__

#if (0)
#define WINX_NO_DEBUG_NEW
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

// -------------------------------------------------------------------------

#ifndef __STLPATCH_H__
#include "../../../stdext/include/stlpatch.h"
#endif

#ifndef __WINX_WINSDK_BASIC_H__
#include "winsdk/Basic.h"
#endif

#ifndef __WINX_WTL_BASIC_H__
#include "wtl/Basic.h"
#endif

#ifndef __STDEXT_H__
#include "../../../stdext/include/stdext.h"
#endif

// -------------------------------------------------------------------------
// winx_msg

#ifndef winx_msg
#define winx_msg winx_call
#endif

// -------------------------------------------------------------------------
// __WINX

#ifndef __WINX
#define __WINX				winx::
#define __WINX_BEGIN		namespace winx {
#define __WINX_END			}
#endif

#ifndef WINX_VER
#define WINX_VER			0x0100	// Version 1.0
#endif

// -------------------------------------------------------------------------
// WINX_MALLOC/WINX_FREE

#define WINX_MALLOC(cb)		_malloc_dbg(cb, _NORMAL_BLOCK, __FILE__, __LINE__)
#define WINX_FREE(p)		_free_dbg(p, _NORMAL_BLOCK)

// -------------------------------------------------------------------------
// WINX_NEW/WINX_DELETE

#if !defined(WINX_NO_DEBUG_NEW) && defined(_DEBUG)
#define WINX_DEBUG_NEW
#endif

template <class WindowClass>
__forceinline void _WinxDbgDelete(WindowClass* pOb)
{
	pOb->~WindowClass();
	WINX_FREE(pOb);
}

#if defined(WINX_DEBUG_NEW)
#define WINX_NEW(WindowClass)			new(WINX_MALLOC(sizeof(WindowClass))) WindowClass
#define WINX_DELETE(pOb)				_WinxDbgDelete(pOb)
#else
#define WINX_NEW(WindowClass)			new WindowClass
#define WINX_DELETE(pOb)				delete pOb
#endif

// -------------------------------------------------------------------------
// class CComAppInit/COleAppInit - Helper

class CComAppInit
{
public:
	CComAppInit(LPVOID pvReserved = NULL) {
		::CoInitialize(pvReserved);
	}
	~CComAppInit() {
		::CoUninitialize();
	}
};

class COleAppInit
{
public:
	COleAppInit(LPVOID pvReserved = NULL) {
		::OleInitialize(pvReserved);
	}
	~COleAppInit() {
		::OleUninitialize();
	}
};

// -------------------------------------------------------------------------
// class CDebugAppInit - Debug Helper

#if defined(_DEBUG)
inline void winx_call EnableMemoryLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}
#else
inline void winx_call EnableMemoryLeakCheck() {}
#endif

class CDebugAppInit
{
public:

#if defined(_DEBUG)
	CDebugAppInit() { EnableMemoryLeakCheck(); }
#endif

};

// -------------------------------------------------------------------------
// class CComModuleInit - ATL Helper

#if (_ATL_VER > 0x0300)

class CComModuleInit
{
};

#else

class CComModuleInit
{
public:
	CComModuleInit(
		ATL::_ATL_OBJMAP_ENTRY* p = NULL,
		HINSTANCE hInst = GetThisModule(),
		const GUID* plibid = NULL)
	{
		_Module.Init(p, hInst, plibid);
	}
	~CComModuleInit()
	{
		_Module.Term();
	}
};

#endif

// -------------------------------------------------------------------------
// RunMsgLoop

#define WINX_MSGF_MODELDLG	0
#define WINX_MSGF_BASE		0x8100
#define WINX_MSGF_MAINLOOP	(WINX_MSGF_BASE + 1)

inline int winx_call RunMsgLoop(int codeMsgf = WINX_MSGF_MAINLOOP)
{
	for(;;)
	{
		MSG msg;
		BOOL bRet = ::GetMessage(&msg, NULL, 0, 0);
		if (bRet == -1)
		{
			WINX_TRACE("GetMessage returned -1 (error)\n");
			continue; // error, don't process
		}
		if (!bRet)
		{
			WINX_TRACE("RunMsgLoop - exiting\n");
			return (int)msg.wParam; // WM_QUIT, exit message loop
		}
		if (!::CallMsgFilter(&msg, codeMsgf))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

// -------------------------------------------------------------------------
// CAppModuleInit - WTL Helper

#if defined(WINX_USE_APPMODULE)

class CAppModuleInit : public WTL::CMessageLoop
{
public:
	CAppModuleInit(
		_ATL_OBJMAP_ENTRY* p = NULL,
		HINSTANCE hInst = GetThisModule(),
		const GUID* plibid = NULL)
	{
		_Module.Init(p, hInst, plibid);
		_Module.AddMessageLoop(this);
	}
	~CAppModuleInit()
	{
		_Module.Term();
	}

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		using namespace WTL;
		if (CallMsgFilter(pMsg, WINX_MSGF_MAINLOOP))
			return TRUE;
		return CMessageLoop::PreTranslateMessage(pMsg);
	}
};

#endif // defined(WINX_USE_APPMODULE)

// -------------------------------------------------------------------------
// $Log: Basic.h,v $
// Revision 1.9  2006/12/22 10:22:28  xushiwei
// STL-Patch: std::basic_string::_Split function bugfix (vc6)
//
// Revision 1.6  2006/08/22 10:36:53  xushiwei
// WINX-Core:
//   Property(Bkgrnd, Accel, Layout), MainFrame support
//   CommandDispatch, CommandState, Demo: User-defined-control(Subclass, Superclass, SuperDialog)
//
// Revision 1.5  2006/08/21 18:45:48  xushiwei
// WINX-Core:
//    Window::Create, MainFrame, StackWindowObject(WINX_STACK_OBJECT) support
//
// Revision 1.4  2006/08/20 05:14:47  xushiwei
// MFC-Compatibility:
//   GdiObject(CDC, CClientDC, CPaintDC, CBitmap, CPalette, etc),  Diagnost(ASSERT, VERIFY, etc)
//   CreditStatic - Demonstrate how to port MFC code to WINX --- see @@code in source code
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

#endif /* __WINX_BASIC_H__ */
