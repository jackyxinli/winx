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
// Module: winx/win/Frame.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-22 15:04:15
// 
// $Id: Frame.h,v 1.5 2006/09/18 05:16:27 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_FRAME_H__
#define __WINX_WIN_FRAME_H__

#ifndef __WINX_WIN_WINDOW_H__
#include "Window.h"
#endif

#ifdef __ATLFRAME_H__
#error "Don't include atlframe.h first"
#endif

#include "../wtl/WrapperMessage.h"
#include "../../../../wtl/include/atlframe.h"
#include "../wtl/UnWrapperMessage.h"

__WINX_BEGIN

// =========================================================================
// class MainFrame

template <class WindowClass, class HandleClass = DefaultWindowHandle>
class MainFrame : public Window<WindowClass, HandleClass>
{
	WINX_DEFAULT_STYLE(WS_OVERLAPPEDWINDOW|WS_VISIBLE);
	WINX_DEFAULT_EXSTYLE(0);
	WINX_STACK_OBJECT(TRUE);
	
public:
	void OnDestroy(HWND hWnd)
	{
		::PostQuitMessage(0);
	}
};

// =========================================================================
// class MDIMainFrame

template <class WindowClass, class HandleClass = DefaultWindowHandle>
class MDIMainFrame : public Window<WindowClass, HandleClass>
{
	WINX_DEFAULT_STYLE(WS_OVERLAPPEDWINDOW|WS_VISIBLE);
	WINX_DEFAULT_EXSTYLE(0);
	WINX_STACK_OBJECT(TRUE);

public:
	LRESULT winx_msg InternalDefault(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ::DefFrameProc(hWnd, NULL, message, wParam, lParam);
	}

	void OnDestroy(HWND hWnd)
	{
		::PostQuitMessage(0);
	}
};

// =========================================================================
// class MDIChildFrame

template <class WindowClass, class HandleClass = DefaultWindowHandle>
class MDIChildFrame : public Window<WindowClass, HandleClass>
{
public:
	static LRESULT winx_msg InternalDefault(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ::DefMDIChildProc(hWnd, message, wParam, lParam);
	}
};

// =========================================================================
// Command State - UpdateUI

#define __WINX_PROCESS_UPDATEUI()											\
BOOL winx_call ProcessUpdateUIMessage(										\
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)	\
{																			\
	return UpdateUIClass::ProcessWindowMessage(								\
		hWnd, uMsg, wParam, lParam, lResult);								\
}

typedef WTL::CUpdateUIBase UpdateUIBase;

template <class WindowClass>
class UpdateUI : public WTL::CUpdateUI<WindowClass>
{
public:
	typedef UpdateUI<WindowClass> UpdateUIClass;
};

template <class WindowClass>
class DynamicUpdateUI : public WTL::CDynamicUpdateUI<WindowClass>
{
public:
	typedef DynamicUpdateUI<WindowClass> UpdateUIClass;
};

#define WINX_UPDATEUI_BEGIN()		public: BEGIN_UPDATE_UI_MAP(void)
#define WINX_UPDATEUI(nID, wType)	UPDATE_ELEMENT(nID, wType)
#define WINX_UPDATEUI_END()			END_UPDATE_UI_MAP()	__WINX_PROCESS_UPDATEUI()

// =========================================================================
// Layout - DialogResize

template <class WindowImplClass>
class _DialogResize : public ATL::CWindow, public WTL::CDialogResize<WindowImplClass>
{
	typedef WTL::CDialogResize<WindowImplClass> BaseClass;
	
public:
	using BaseClass::DLSZ_SIZE_X;
	using BaseClass::DLSZ_SIZE_Y;
	using BaseClass::DLSZ_MOVE_X;
	using BaseClass::DLSZ_MOVE_Y;
	
	enum { AddGripper = TRUE };
	enum { UseMinTrackSize = TRUE };
	enum { ForceStyle = WS_CLIPCHILDREN };

	enum {
		ResizeX = DLSZ_SIZE_X,
		ResizeY = DLSZ_SIZE_Y,
		ResizeAll = DLSZ_SIZE_X|DLSZ_SIZE_Y,
		AnchorRightTop = DLSZ_MOVE_X,
		AnchorLeftBottom = DLSZ_MOVE_Y,
		AnchorRightBottom = DLSZ_MOVE_X|DLSZ_MOVE_Y,
		AnchorAll = DLSZ_SIZE_X|DLSZ_SIZE_Y,
	};

public:
	void winx_call Initialize(HWND hWnd)
	{
		m_hWnd = hWnd;
		DlgResize_Init(
			(bool)_WINX_PWND->AddGripper,
			(bool)_WINX_PWND->UseMinTrackSize,
			_WINX_PWND->ForceStyle);
	}
};

#define __WINX_PROCESS_DLGRESIZE(resizer)									\
BOOL winx_call ProcessDialogResizeMessage(									\
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)	\
{																			\
	return (resizer).ProcessWindowMessage(									\
		hWnd, uMsg, wParam, lParam, lResult);								\
}

#define __WINX_INIT_DLGRESIZE(resizer)										\
VOID winx_msg OnDialogResizeInit(HWND hWnd)									\
{																			\
	(resizer).Initialize(hWnd);												\
}

#define WINX_DLGRESIZE_BEGIN_EX(fMinSizeLimit)								\
public:																		\
	struct DialogResizeClass : public winx::_DialogResize<DialogResizeClass>\
	{																		\
		enum { UseMinTrackSize = (fMinSizeLimit) };

#define WINX_DLGRESIZE_ADDGRIPPER(fAddGripper)								\
		enum { AddGripper = (fAddGripper) };

#define WINX_DLGRESIZE_FORCE_STYLE(style)									\
		enum { ForceStyle = (style) };

#define WINX_DLGRESIZE_MAP_BEGIN()											\
		BEGIN_DLGRESIZE_MAP(void)

#define WINX_DLGRESIZE(id, flags)											\
		DLGRESIZE_CONTROL(id, flags)

#define WINX_DLGRESIZE_GROUP_BEGIN()										\
		BEGIN_DLGRESIZE_GROUP()

#define WINX_DLGRESIZE_GROUP_END()											\
		END_DLGRESIZE_GROUP()

#define WINX_DLGRESIZE_MAP_END()											\
		END_DLGRESIZE_MAP()

#define WINX_DLGRESIZE_END_EX(resizer)										\
	} resizer;																\
	__WINX_INIT_DLGRESIZE(resizer)											\
	__WINX_PROCESS_DLGRESIZE(resizer)

#define WINX_DLGRESIZE_BEGIN(fMinSizeLimit)									\
	WINX_DLGRESIZE_BEGIN_EX(fMinSizeLimit)									\
	WINX_DLGRESIZE_MAP_BEGIN()

#define WINX_DLGRESIZE_END()												\
	WINX_DLGRESIZE_MAP_END()												\
	WINX_DLGRESIZE_END_EX(_winx_resizer)

// =========================================================================
// $Log: Frame.h,v $
// Revision 1.5  2006/09/18 05:16:27  xushiwei
// UpdateUI - interface changed
//
// Revision 1.3  2006/09/13 17:05:11  xushiwei
// WINX-Basic: DefaultWindowHandle(see macro WINX_NULL_WINDOW_HANDLE)
// WINX-Core: AxModalDialog/AxModalessDialog
//
// Revision 1.2  2006/08/23 05:47:06  xushiwei
// WINX-Core:
//   Property(Icon, AppIcon), Layout(DialogResize, MinTrackSize)
// WINX-CommonDialogs:
//   OpenFileDialog/SaveFileDialog, OpenMultiFilesDialog/SaveMultiFilesDialog
//
// Revision 1.1  2006/08/22 10:36:53  xushiwei
// WINX-Core:
//   Property(Bkgrnd, Accel, Layout), MainFrame support
//   CommandDispatch, CommandState, Demo: User-defined-control(Subclass, Superclass, SuperDialog)
//

__WINX_END

#endif /* __WINX_WIN_FRAME_H__ */
