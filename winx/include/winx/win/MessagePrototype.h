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
// Module: winx/win/MessagePrototype.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 17:59:09
// 
// $Id: MessagePrototype.h,v 1.15 2006/12/11 05:19:08 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_MESSAGEPROTOTYPE_H__
#define __WINX_WIN_MESSAGEPROTOTYPE_H__

#ifndef __WINX_WIN_BASICTYPES_H__
#include "BasicTypes.h"
#endif

__WINX_BEGIN

// =========================================================================
// Winx Extended Messages

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif

#ifndef WM_THEMECHANGED
#define WM_THEMECHANGED	0x031A // WindowsXP: 用户修改了XP风格的主题。
#endif

#ifndef WM_SETTINGCHANGE
#define WM_SETTINGCHANGE WM_WININICHANGE
#endif

// WM_FORWARDMSG - used to forward a message to another window for processing
// WPARAM - DWORD dwUserData - defined by user
// LPARAM - LPMSG pMsg - a pointer to the MSG structure
// return value - 0 if the message was not processed, nonzero if it was
#ifndef WM_FORWARDMSG
#define WM_FORWARDMSG 0x037F // MFC/ATL 定义的消息
#endif

#define WINX_WM_BASE					(0xDB00)
#define WINX_WM_REFLECT_BASE			(WM_USER+0x1c00) // 同ATL中的OCM__BASE
// basic message id

#define WINX_WM_POSTINIT				(WINX_WM_BASE + 0x01)
//	[in]  WPARAM	reserved
//	[in]  LPARAM	reserved
//	[out] LRESULT	void;
//   --> 窗口初始化后。目前的实现特指对话框OnPostInitDialog消息。

#define WINX_WM_CTLCOLOR_BASE			(WM_CTLCOLORMSGBOX)
#define WINX_WM_CTLCOLOR				(WINX_WM_BASE + WINX_WM_CTLCOLOR_BASE)
// 子窗口向父窗口询问自己使用的背景颜色是什么？
//	[in]  WPARAM	UINT nCtlColor;				--> WM_CTLCOLORMSGBOX, WM_CTLCOLORBTN, etc.
//	[in]  LPARAM	QueryCtlColorParam* query;	--> QueryCtlColorParam*
//	[out] LRESULT	void;

#define WINX_WM_REFLECT_NOTIFY			(WINX_WM_REFLECT_BASE + WM_NOTIFY) // OnReflectNotify消息
// reflect notification to child window control
//   --> 父窗口将notification消息重新发回子窗口。消息参数完全同WM_NOTIFY。

#define WINX_WM_REFLECT_COMMAND			(WINX_WM_REFLECT_BASE + WM_COMMAND) // OnReflectCommand消息
// reflect notification to child window control
//   --> 父窗口将command消息重新发回子窗口。消息参数完全同WM_COMMAND。

#define WINX_WM_REFLECT_DRAWITEM		(WINX_WM_REFLECT_BASE + WM_DRAWITEM) // DrawItem消息
// reflect notification to child window control
//   --> 父窗口将draw item消息重新发回子窗口。消息参数完全同WM_DRAWITEM。

#define WINX_WM_REFLECT_MEASUREITEM		(WINX_WM_REFLECT_BASE + WM_MEASUREITEM) // MeasureItem消息
// reflect notification to child window control
//   --> 父窗口将measure item消息重新发回子窗口。消息参数完全同WM_MEASUREITEM。

struct QueryCtlColorParam
{
	HWND	hWndCtl;	// [in] handle to control
	HBRUSH	hbrCtl;		// [out] handle to brush
	HDC		hCtlDC;		// [in] handle to dc
};

// -------------------------------------------------------------------------
// GetDragListMessage

inline UINT winx_call GetDragListMessage()
{
	static UINT uDragListMessage = ::RegisterWindowMessage(DRAGLISTMSGSTRING);
	return uDragListMessage;
}

// -------------------------------------------------------------------------
// BroadcastSendMessage

inline VOID winx_call BroadcastSendMessage(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	for (
		HWND hWndChild = ::GetTopWindow(hWnd);
		hWndChild != NULL;
		hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT)
		)
	{
		::SendMessage(hWndChild, message, wParam, lParam);
	}
}

// -------------------------------------------------------------------------
// ForwardCommand

inline BOOL winx_call ForwardCommand(HWND hWndModal, WPARAM wParam)
{
	HWND hWndCtl = ::GetFocus();
    if (::IsChild(hWndModal, hWndCtl))
	{
        // find a direct child of the dialog from the window that has focus
        while(::GetParent(hWndCtl) != hWndModal)
            hWndCtl = ::GetParent(hWndCtl);
		
		return (BOOL)::SendMessage(hWndCtl, WM_COMMAND, wParam, 0);
	}
	return FALSE;
}

// -------------------------------------------------------------------------
// ForwardMessage

inline BOOL winx_call ForwardMessage(HWND hWndModal, LPMSG pMsg)
{
	HWND hWndCtl = ::GetFocus();
    if (::IsChild(hWndModal, hWndCtl))
	{
        // find a direct child of the dialog from the window that has focus
        while(::GetParent(hWndCtl) != hWndModal)
            hWndCtl = ::GetParent(hWndCtl);
		
        // give control a chance to translate this message
        if (::SendMessage(hWndCtl, WM_FORWARDMSG, 0, (LPARAM)pMsg))
			return TRUE;
	}
	return (BOOL)::SendMessage(hWndModal, WM_FORWARDMSG, 0, (LPARAM)pMsg);
}

// -------------------------------------------------------------------------
// ReflectNotifications - ReflectMessage Support
//	see ATL::CWindowImplRoot<TBase>::ReflectNotifications

inline BOOL winx_call ReflectNotifications(
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
	HWND hWndChild = NULL;

	switch(uMsg)
	{
	case WM_COMMAND:
		if(lParam != NULL)	// not from a menu
			hWndChild = (HWND)lParam;
		break;
	case WM_NOTIFY:
		hWndChild = ((LPNMHDR)lParam)->hwndFrom;
		break;
#if (0) // 不考虑支持WM_PARENTNOTIFY的反射
	case WM_PARENTNOTIFY:
		switch(LOWORD(wParam))
		{
		case WM_CREATE:
		case WM_DESTROY:
			hWndChild = (HWND)lParam;
			break;
		default:
			hWndChild = ::GetDlgItem(hWnd, HIWORD(wParam));
			break;
		}
		break;
#endif
	case WM_DRAWITEM:
		if(wParam)	// not from a menu
			hWndChild = ((LPDRAWITEMSTRUCT)lParam)->hwndItem;
		break;
	case WM_MEASUREITEM:
	case WM_COMPAREITEM:
	case WM_DELETEITEM:
		if(wParam)	// not from a menu
			hWndChild = ::GetDlgItem(hWnd, ((LPMEASUREITEMSTRUCT)lParam)->CtlID);
		break;
#if (0) // 不考虑支持WM_VKEYTOITEM、WM_CHARTOITEM、WM_SCROLL等消息的反射
	case WM_VKEYTOITEM:
	case WM_CHARTOITEM:
	case WM_HSCROLL:
	case WM_VSCROLL:
		hWndChild = (HWND)lParam;
		break;
#endif
#if defined(WINX_REFLECT_CTLCOLOR)
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
		hWndChild = (HWND)lParam;
		break;
#endif
	default:
		break;
	}
	
	if (hWndChild == NULL)
	{
		return FALSE;
	}
	else
	{
		WINX_ASSERT(::IsWindow(hWndChild));
		lResult = ::SendMessage(hWndChild, WINX_WM_REFLECT_BASE + uMsg, wParam, lParam);
		return TRUE;
	}
}

// =========================================================================
// WINX_INITIAL - 取得窗口的初始大小，保存于私有变量m_ptOrgSize。

#define WINX_INITIAL(m_ptOrgSize)											\
private:																	\
	POINT m_ptOrgSize;														\
public:																		\
	VOID winx_msg OnGetInitialInfo(HWND hWnd)								\
	{																		\
		RECT rcOrg;															\
		::GetWindowRect(hWnd, &rcOrg);										\
		m_ptOrgSize.x = rcOrg.right - rcOrg.left;							\
		m_ptOrgSize.y = rcOrg.bottom - rcOrg.top;							\
	}

// =========================================================================
// WINX_MININFO - 控制最小窗口SIZE

#define WINX_MININFO(cx, cy)												\
public:																		\
	VOID winx_msg OnGetMinMaxInfo(HWND hWnd, LPMINMAXINFO lpMM)				\
	{																		\
		lpMM->ptMinTrackSize.x = (cx);										\
		lpMM->ptMinTrackSize.y = (cy);										\
	}

#define WINX_MININFO_PT(ptSizeLimit)										\
public:																		\
	VOID winx_msg OnGetMinMaxInfo(HWND hWnd, LPMINMAXINFO lpMM)				\
	{																		\
		lpMM->ptMinTrackSize = (ptSizeLimit);								\
	}

// 一种比较智能的限制窗口最小SIZE的方式：窗口大小不能小于初始大小！
#define WINX_MININFO_DEFAULT()												\
	WINX_INITIAL(_winx_ptOrgSize);											\
	WINX_MININFO_PT(_winx_ptOrgSize)

// =========================================================================
// WINX_CTLBKGND - Background Style Support

// 取值：mask可取CtlDialog, CtlStatic, etc；而brush可以是任意的HBRUSH。
#define WINX_CTLBKGND_EX(mask, brush)										\
public:																		\
	HBRUSH OnCtlColor(HWND hWnd, HDC hDC, HWND hWndCtrl, UINT nCtlColor)	\
	{																		\
		const int _winx_mask = (1 << (nCtlColor - WINX_WM_CTLCOLOR_BASE));	\
		if ( _winx_mask & (mask) )											\
			return (brush);													\
		else																\
			return NULL;													\
	}

#define WINX_CTLBKGND_NULL(mask)											\
public:																		\
	HBRUSH OnCtlColor(HWND hWnd, HDC hDC, HWND hWndCtrl, UINT nCtlColor)	\
	{																		\
		const int _winx_mask = (1 << (nCtlColor - WINX_WM_CTLCOLOR_BASE));	\
		if ( _winx_mask & (mask) )											\
		{																	\
			if (hDC)														\
				::SetBkMode(hDC, TRANSPARENT);								\
			return (HBRUSH)::GetStockObject(NULL_BRUSH);					\
		}																	\
		else																\
			return NULL;													\
	}

#define WINX_CTLBKGND_NULL_ALL()											\
public:																		\
	HBRUSH OnCtlColor(HWND hWnd, HDC hDC, HWND hWndCtrl, UINT nCtlColor)	\
	{																		\
		if (hDC)															\
			::SetBkMode(hDC, TRANSPARENT);									\
		return (HBRUSH)::GetStockObject(NULL_BRUSH);						\
	}

// 取值：Windows定义的StockBrush，即XXX_BRUSH宏。
// 例如：NULL_BRUSH, WHITE_BRUSH, LTGRAY_BRUSH, GRAY_BRUSH, DKGRAY_BRUSH, etc.
#define WINX_CTLBKGND_BRUSH(mask, brush)									\
		WINX_CTLBKGND_EX(mask, (HBRUSH)::GetStockObject(brush))

// 取值：Windows定义的COLOR_XXX宏。
// 例如：COLOR_WINDOW, COLOR_WINDOWFRAME, etc.
#define WINX_CTLBKGND_COLOR(mask, color)									\
		WINX_CTLBKGND_EX(mask, (HBRUSH)::GetSysColorBrush(color))

// 取值：可取资源中的任意Bitmap的ID。
// 例如：IDB_BKGND, IDB_XXX, etc.
#define WINX_CTLBKGND_PATTERN(mask, nBitmapId)									\
		WINX_CTLBKGND_EX(mask, WINX_CACHED_PATTERN(nBitmapId))

// =========================================================================
// WINX_BKGND - Background Support

#define WINX_BKGND_EX(brush)												\
public:																		\
	HBRUSH winx_call GetBkgndBrush() { return brush; }						\
	BOOL winx_msg OnEraseBkgnd(HWND hWnd, HDC hDC) {						\
		return winx::ProcessBkgndMessage(hWnd, hDC, brush);					\
	}

// 取值：Windows定义的COLOR_XXX宏。
// 例如：COLOR_WINDOW, COLOR_WINDOWFRAME, etc.
#define WINX_BKGND_COLOR(color)	WINX_BKGND_EX(::GetSysColorBrush(color))

// 取值：Windows定义的StockBrush，即XXX_BRUSH宏。
// 例如：NULL_BRUSH, WHITE_BRUSH, LTGRAY_BRUSH, GRAY_BRUSH, DKGRAY_BRUSH, etc.
#define WINX_BKGND_BRUSH(brush)	WINX_BKGND_EX((HBRUSH)::GetStockObject(brush))

// 取值：可取资源中的任意Bitmap的ID。
// 例如：IDB_BKGND, IDB_XXX, etc.
#define WINX_BKGND_PATTERN(nBitmapId) WINX_BKGND_EX(WINX_CACHED_PATTERN(nBitmapId))

// 含义：类同WINX_BKGND_BRUSH(NULL_BRUSH)。
// 提醒：虽然可以用WINX_BKGND_BRUSH(NULL_BRUSH)，但是性能上是WINX_BKGND_NULL()高。
#define WINX_BKGND_NULL()													\
public:																		\
	HBRUSH winx_call GetBkgndBrush() const { 								\
		return (HBRUSH)::GetStockObject(NULL_BRUSH);						\
	}																		\
	BOOL winx_msg OnEraseBkgnd(HWND hWnd, HDC hDC) {						\
		return TRUE;														\
	}

// -------------------------------------------------------------------------
// ProcessBkgndMessage - Background Support

inline BOOL winx_call ProcessBkgndMessage(HWND hWnd, HDC hDC, HBRUSH brush)
{
	if (brush) {
		FillClientRect(hWnd, hDC, brush);
		return TRUE;
	}
	return FALSE;
}

// =========================================================================
// Command Dispatcher

#define WINX_CMDS_BEGIN()													\
public:																		\
	BOOL winx_msg OnCommand(												\
		HWND hWnd, WPARAM wParam, HWND hWndCtlFrom)							\
	{

#define WINX_CMDS_BEGIN_EX()												\
	WINX_CMDS_BEGIN()

#define WINX_CMD_EX(nID, nCode, Function)									\
		if (wParam == MAKELONG(nID, nCode))	{								\
			Function(hWnd);	return TRUE;									\
		}

#define WINX_CMD_NOARG(nID, Function)										\
		if (LOWORD(wParam) == (nID)) {										\
			Function(); return TRUE;										\
		}

#define WINX_CMD(nID, Function)												\
		if (LOWORD(wParam) == (nID)) {										\
			Function(hWnd); return TRUE;									\
		}

#define WINX_CMD_RANGE(nIDFirst, nIDLast, Function)							\
		if (LOWORD(wParam) >= (nIDFirst) && LOWORD(wParam) <= (nIDLast)) {	\
			Function(hWnd, LOWORD(wParam)); return TRUE;					\
		}

#define WINX_CMD_NOTIFY(nID)												\
		if (LOWORD(wParam) == (nID)) {										\
			::SendMessage(::GetParent(hWnd),								\
				WM_COMMAND, wParam, (LPARAM)hWnd);							\
			return TRUE;													\
		}

#define WINX_CMDS_END()														\
		return BaseClass::OnCommand(hWnd, wParam, hWndCtlFrom);				\
	}

#define WINX_CMDS_END_EX()													\
		if (BaseClass::OnCommand(hWnd, wParam, hWndCtlFrom))				\
			return TRUE;													\
		else if (hWndCtlFrom == NULL) 										\
			return winx::ForwardCommand(hWnd, wParam);						\
		return FALSE;														\
	}

// =========================================================================
// SysCommand Dispatcher

#define WINX_SYSCMD_BEGIN()													\
public:																		\
	BOOL winx_msg OnSysCommand(												\
		HWND hWnd, WPARAM wParam, LPARAM lParam)							\
	{

#define WINX_SYSCMD(nID, Function)											\
		if ((wParam & 0xFFF0) == (nID))										\
			return Function(hWnd);

#define WINX_SYSCMD_END()													\
		return BaseClass::OnSysCommand(hWnd, wParam, lParam);				\
	}

// =========================================================================
// NotifyMessage Dispatcher

#define WINX_NOTIFY_BEGIN()													\
public:																		\
	BOOL winx_msg OnNotify(													\
		HWND hWnd, int idCtrl, LPNMHDR pnmh, LRESULT* pResult)				\
	{																		\
		UINT message = pnmh->code;

#define WINX_NOTIFY(id, nCode, Function)									\
		if (idCtrl == (id) && message == nCode)	{							\
			Function(hWnd, pnmh, pResult); return TRUE;						\
		}

#define WINX_NOTIFY_RANGE(idFirst, idLast, nCode, Function)					\
		if (idCtrl >= (idFirst) && idCtrl <= (idLast) && message == nCode){ \
			Function(hWnd, pnmh, pResult); return TRUE;						\
		}

#define WINX_NOTIFY_END()													\
		return BaseClass::OnNotify(hWnd, idCtrl, pnmh, pResult);			\
	}

// =========================================================================
// Accelerator Support

#define WINX_ACCEL_EX(hAccel)												\
public:																		\
	BOOL winx_msg OnTranslateAccel(HWND hWnd, LPMSG lpMsg)					\
	{																		\
		WINX_MSG_HAS(TestTranslateAccel)									\
			if (_WINX_PWND->TestTranslateAccel(hWnd, lpMsg))				\
				return FALSE;												\
		if (::TranslateAccelerator(hWnd, hAccel, lpMsg))					\
			return TRUE;													\
		return BaseClass::OnTranslateAccel(hWnd, lpMsg);					\
	}

#define WINX_ACCEL(idAccel)													\
private:																	\
	WINX_ACCEL_EX(_winx_hAccel);											\
	HACCEL _winx_hAccel;													\
public:																		\
	VOID winx_msg OnAccelInit(HWND hWnd) {									\
		_winx_hAccel = winx::LoadAccelerators(hWnd, idAccel);				\
	}																		\
	VOID winx_msg OnAccelTerm(HWND hWnd) {									\
		::DestroyAcceleratorTable(_winx_hAccel);							\
		_winx_hAccel = NULL;												\
	}

// =========================================================================
// WINX_DROPTARGET - DropTarget - 拖放目标窗口

#define WINX_DROPTARGET_EX(pDropTarget)										\
public:																		\
	VOID winx_msg OnDropTargetInit(HWND hWnd)	{							\
		::RegisterDragDrop(hWnd, pDropTarget);								\
	}																		\
	VOID winx_msg OnDropTargetTerm(HWND hWnd)	{							\
		::RevokeDragDrop(hWnd);												\
	}

#define WINX_DROPTARGET()													\
	WINX_DROPTARGET_EX(this)

// =========================================================================
// WINX_ICON - Window Icon Support

#define WINX_ICON_EX(icon)													\
public:																		\
	VOID winx_msg OnIconInit(HWND hWnd)	{									\
		winx::SetWindowIcon(hWnd, (icon), ICON_BIG);						\
	}

#define WINX_ICONSM_EX(icon)												\
public:																		\
	VOID winx_msg OnIconSmInit(HWND hWnd) {									\
		winx::SetWindowIcon(hWnd, (icon), ICON_SMALL);						\
	}

// SYSICON，可取：IDI_APPLICATION, IDI_ASTERISK, IDI_ERROR, etc.
#define WINX_SYSICON(sysicon)												\
	WINX_ICON_EX(::LoadIcon(NULL, sysicon))

// SYSICON，可取：IDI_APPLICATION, IDI_ASTERISK, IDI_ERROR, etc.
#define WINX_SYSICONSM(sysicon)												\
	WINX_ICONSM_EX(::LoadIcon(NULL, sysicon))

// ICON，可取资源中的任意Icon的ID。
#define WINX_ICON(icon)														\
	WINX_ICON_EX(WINX_CACHED_ICON(icon))

// ICON，可取资源中的任意Icon的ID。
#define WINX_ICONSM(icon)													\
	WINX_ICONSM_EX(WINX_CACHED_ICON(icon))

// APPICON，可取exe资源中的任意Icon的ID。
#define WINX_APPICON(icon)													\
	WINX_ICON_EX(WINX_CACHED_APPICON(icon))

// APPICON，可取exe资源中的任意Icon的ID。
#define WINX_APPICONSM(icon)												\
	WINX_ICONSM_EX(WINX_CACHED_APPICON(icon))

// =========================================================================
// WINX_ON_DLGINIT_CREATE_AXCTRL - 对话框支持ActiveX Control

#if defined(WINX_CREATE_AX_ON_DLGINIT)

#define WINX_ON_DLGINIT_CREATE_AXCTRL()										\
public:																		\
	HRESULT winx_msg OnDlgInitCreateAxCtrl(HWND hDlg)						\
	{																		\
		HINSTANCE hInst = (HINSTANCE)::GetWindowLong(hDlg, GWL_HINSTANCE);	\
		return AtlCreateActiveXControls(hDlg, hInst, IDD);					\
	}

#define _WINX_DLGINIT_CREATE_AXCTRL()										\
	WINX_MSG_HAS(OnDlgInitCreateAxCtrl)										\
		_WINX_PWND->OnDlgInitCreateAxCtrl(hWnd)

#else // !defined(WINX_CREATE_AX_ON_DLGINIT)

#define WINX_ON_DLGINIT_CREATE_AXCTRL()
#define _WINX_DLGINIT_CREATE_AXCTRL()

#endif

// =========================================================================
// WINX_ON_INITIAL

#define WINX_ON_INITIAL()													\
public:																		\
	VOID winx_msg OnInitial(HWND hWnd)										\
	{																		\
		HandleClass::operator=(hWnd);										\
		_WINX_DLGINIT_CREATE_AXCTRL();										\
																			\
		WINX_MSG_HAS(OnGetInitialInfo)										\
			_WINX_PWND->OnGetInitialInfo(hWnd);								\
		WINX_MSG_HAS(OnIconInit)											\
			_WINX_PWND->OnIconInit(hWnd);									\
		WINX_MSG_HAS(OnIconSmInit)											\
			_WINX_PWND->OnIconSmInit(hWnd);									\
																			\
		WINX_MSG_HAS(OnEnterAccelFrame)										\
			_WINX_PWND->OnEnterAccelFrame(hWnd);							\
		WINX_MSG_HAS(OnAccelInit)											\
			_WINX_PWND->OnAccelInit(hWnd);									\
		WINX_MSG_HAS(OnXPThemeInit)											\
			_WINX_PWND->OnXPThemeInit(hWnd);								\
		WINX_MSG_HAS(OnIpcHostInit)											\
			_WINX_PWND->OnIpcHostInit(hWnd);								\
		WINX_MSG_HAS(OnIpcTargetInit)										\
			_WINX_PWND->OnIpcTargetInit(hWnd);								\
		WINX_MSG_HAS(OnDropTargetInit)										\
			_WINX_PWND->OnDropTargetInit(hWnd);								\
																			\
		WINX_MSG_HAS(OnDialogResizeInit)									\
			_WINX_PWND->OnDialogResizeInit(hWnd);							\
																			\
		_WINX_IDLE_TIMER_INIT();											\
	}

// =========================================================================
// WINX_ON_TERMINATE

#define WINX_ON_TERMINATE()													\
public:																		\
	VOID winx_msg OnTerminate(HWND hWnd)									\
	{																		\
		WINX_MSG_HAS(OnDropTargetTerm)										\
			_WINX_PWND->OnDropTargetTerm(hWnd);								\
		WINX_MSG_HAS(OnIpcTargetTerm)										\
			_WINX_PWND->OnIpcTargetTerm(hWnd);								\
		WINX_MSG_HAS(OnIpcHostTerm)											\
			_WINX_PWND->OnIpcHostTerm(hWnd);								\
		WINX_MSG_HAS(OnXPThemeTerm)											\
			_WINX_PWND->OnXPThemeTerm(hWnd);								\
		WINX_MSG_HAS(OnAccelTerm)											\
			_WINX_PWND->OnAccelTerm(hWnd);									\
		WINX_MSG_HAS(OnLeaveAccelFrame)										\
			_WINX_PWND->OnLeaveAccelFrame(hWnd);							\
	}

// =========================================================================
// WINX_MSG_NCDESTROY

#define WINX_MSG_NCDESTROY()												\
	WINX_MSG_HAS4(															\
		ProcessTermMessage, OnFinalMessage, OnNcDestroy, OnTerminate)		\
	{																		\
		if (message == WM_NCDESTROY)										\
		{																	\
			WINX_MSG_HAS(OnNcDestroy)										\
				_WINX_PWND->OnNcDestroy(hWnd);								\
			WINX_MSG_HAS(OnTerminate)										\
				_WINX_PWND->OnTerminate(hWnd);								\
			WINX_MSG_HAS(OnFinalMessage)									\
				_WINX_PWND->OnFinalMessage(hWnd);							\
			WINX_MSG_HAS(ProcessTermMessage)								\
				_WINX_PWND->ProcessTermMessage(hWnd);						\
			return TRUE;													\
		}																	\
	}

// =========================================================================
// macro _WINX_MSG_NULL_HANDLES

#define _WINX_MSG_NULL_HANDLES()											\
	WINX_MSG_NULL_HANDLER(PreHandleMessage);								\
	WINX_MSG_NULL_HANDLER(ProcessUserMessage);								\
	WINX_MSG_NULL_HANDLER(ProcessUpdateUIMessage);							\
	WINX_MSG_NULL_HANDLER(ProcessDialogResizeMessage);						\
	WINX_MSG_NULL_HANDLER(ProcessHostMessage);								\
	WINX_MSG_NULL_HANDLER(ProcessTermMessage);								\
	WINX_MSG_NULL_HANDLER(InternalDefault);									\
	WINX_MSG_NULL_HANDLER(HandleUpdateViews);								\
	WINX_MSG_DEFAULT_HANDLER(OnPaint);										\
	WINX_MSG_SIMPLE_HANDLER(OnDraw, 0);										\
	WINX_MSG_SIMPLE_HANDLER(OnDrawEx, 0);									\
	WINX_MSG_SIMPLE_HANDLER(OnDrawBkgnd, 0);								\
	WINX_MSG_DEFAULT_HANDLER(OnNcPaint);									\
	WINX_MSG_SIMPLE_HANDLER(OnXPThemeNcPaint, 0);							\
	WINX_MSG_DEFAULT_HANDLER(OnNcCalcSize);									\
	WINX_MSG_DEFAULT_HANDLER(OnMouseMove);									\
	WINX_MSG_DEFAULT_HANDLER(OnEraseBkgnd);									\
	WINX_MSG_DEFAULT_HANDLER(OnDrawItem);									\
	WINX_MSG_DEFAULT_HANDLER(OnMeasureItem);								\
	WINX_MSG_DEFAULT_HANDLER(OnLButtonDown);								\
	WINX_MSG_DEFAULT_HANDLER(OnRButtonDown);								\
	WINX_MSG_DEFAULT_HANDLER(OnLButtonUp);									\
	WINX_MSG_DEFAULT_HANDLER(OnRButtonUp);									\
	WINX_MSG_DEFAULT_HANDLER(OnLButtonDblClk);								\
	WINX_MSG_DEFAULT_HANDLER(OnRButtonDblClk);								\
	WINX_MSG_DEFAULT_HANDLER(OnMouseWheel);									\
	WINX_MSG_DEFAULT_HANDLER(OnSetCursor);									\
	WINX_MSG_DEFAULT_HANDLER(OnNcHitTest);									\
	WINX_MSG_DEFAULT_HANDLER(OnVScroll);									\
	WINX_MSG_DEFAULT_HANDLER(OnHScroll);									\
	WINX_MSG_DEFAULT_HANDLER(OnParentNotify);								\
	WINX_MSG_DEFAULT_HANDLER(OnCommand);									\
	WINX_MSG_SIMPLE_HANDLER(OnSysCommand, FALSE);							\
	WINX_MSG_DEFAULT_HANDLER(OnTranslateAccel);								\
	WINX_MSG_NULL_HANDLER(TestTranslateAccel);								\
	WINX_MSG_DEFAULT_HANDLER(OnNotify);										\
	WINX_MSG_DEFAULT_HANDLER(OnChar);										\
	WINX_MSG_DEFAULT_HANDLER(OnKeyDown);									\
	WINX_MSG_DEFAULT_HANDLER(OnKeyUp);										\
	WINX_MSG_DEFAULT_HANDLER(OnSysKeyDown);									\
	WINX_MSG_DEFAULT_HANDLER(OnSysKeyUp);									\
	WINX_MSG_DEFAULT_HANDLER(OnTimer);										\
	WINX_MSG_SIMPLE_HANDLER(OnIdle, 0);										\
	WINX_MSG_DEFAULT_HANDLER(OnWindowPosChanging);							\
	WINX_MSG_DEFAULT_HANDLER(OnWindowPosChanged);							\
	WINX_MSG_DEFAULT_HANDLER(OnGetMinMaxInfo);								\
	WINX_MSG_DEFAULT_HANDLER(OnContextMenu);								\
	WINX_MSG_DEFAULT_HANDLER(OnInitMenuPopup);								\
	WINX_MSG_DEFAULT_HANDLER(OnSetFocus);									\
	WINX_MSG_DEFAULT_HANDLER(OnKillFocus);									\
	WINX_MSG_DEFAULT_HANDLER(OnEnable);										\
	WINX_MSG_DEFAULT_HANDLER(OnThemeChanged);								\
	WINX_MSG_DEFAULT_HANDLER(OnSettingChange);								\
	WINX_MSG_DEFAULT_HANDLER(OnQueryDragIcon);								\
	WINX_MSG_DEFAULT_HANDLER(OnClose);										\
	WINX_MSG_SIMPLE_HANDLER(OnDestroy, 0);									\
	WINX_MSG_SIMPLE_HANDLER(OnNcDestroy, 0);								\
	WINX_MSG_SIMPLE_HANDLER(OnFinalMessage, 0);								\
	WINX_MSG_DEFAULT_HANDLER(OnCtlColor);									\
																			\
	WINX_MSG_NULL_HANDLER(OnGetInitialInfo);								\
	WINX_MSG_NULL_HANDLER(OnDialogResizeInit);								\
	WINX_MSG_NULL_HANDLER(OnIconInit);										\
	WINX_MSG_NULL_HANDLER(OnIconSmInit);									\
	WINX_MSG_NULL_HANDLER(OnAccelInit);										\
	WINX_MSG_NULL_HANDLER(OnAccelTerm);										\
	WINX_MSG_NULL_HANDLER(OnXPThemeInit);									\
	WINX_MSG_NULL_HANDLER(OnXPThemeTerm);									\
	WINX_MSG_NULL_HANDLER(OnIpcHostInit);									\
	WINX_MSG_NULL_HANDLER(OnIpcHostTerm);									\
	WINX_MSG_NULL_HANDLER(OnIpcTargetInit);									\
	WINX_MSG_NULL_HANDLER(OnIpcTargetTerm);									\
	WINX_MSG_NULL_HANDLER(OnDropTargetInit);								\
	WINX_MSG_NULL_HANDLER(OnDropTargetTerm);								\
	WINX_MSG_NULL_HANDLER(OnEnterAccelFrame);								\
	WINX_MSG_NULL_HANDLER(OnLeaveAccelFrame);								\
																			\
	WINX_MSG_SIMPLE_HANDLER(OnNcCreate, TRUE);								\
	WINX_MSG_SIMPLE_HANDLER(OnCreate, 0);									\
	WINX_MSG_SIMPLE_HANDLER(SuperOnCreate, 0);								\
	WINX_MSG_SIMPLE_HANDLER(OnSubclass, 0);									\
	WINX_MSG_SIMPLE_HANDLER(OnUnsubclass, 0);								\
	WINX_MSG_SIMPLE_HANDLER(OnInitDialog, TRUE);							\
	WINX_MSG_SIMPLE_HANDLER(OnPostInitDialog, TRUE);						\
	WINX_MSG_SIMPLE_HANDLER(OnDlgInitCreateAxCtrl, 0);						\
	WINX_MSG_SIMPLE_HANDLER(OnInitial, 0)									\
	WINX_MSG_SIMPLE_HANDLER(OnTerminate, 0)

// =========================================================================
// class MessagePrototype

class MessagePrototype
{	
public:
	// OnInitial/OnNcCreate/OnCreate/OnDestroy/OnNcDestroy/OnFinalMessage

	VOID winx_msg OnGetInitialInfo(HWND hWnd)	{}
	VOID winx_msg OnDialogResizeInit(HWND hWnd)	{}
	
	VOID winx_msg OnIconInit(HWND hWnd)			{}
	VOID winx_msg OnIconSmInit(HWND hWnd)		{}

	VOID winx_msg OnAccelInit(HWND hWnd)		{}
	VOID winx_msg OnAccelTerm(HWND hWnd)		{}

	VOID winx_msg OnXPThemeInit(HWND hWnd)		{}
	VOID winx_msg OnXPThemeTerm(HWND hWnd)		{}
	
	VOID winx_msg OnIpcHostInit(HWND hWnd)		{}
	VOID winx_msg OnIpcHostTerm(HWND hWnd)		{}

	VOID winx_msg OnIpcTargetInit(HWND hWnd)	{}
	VOID winx_msg OnIpcTargetTerm(HWND hWnd)	{}

	VOID winx_msg OnDropTargetInit(HWND hWnd)	{}
	VOID winx_msg OnDropTargetTerm(HWND hWnd)	{}
		
	VOID winx_msg OnInitial(HWND hWnd)			{}
	VOID winx_msg OnTerminate(HWND hWnd)		{}
	
	VOID winx_msg OnClose(HWND hWnd)			{}
	VOID winx_msg OnDestroy(HWND hWnd)			{}
	VOID winx_msg OnNcDestroy(HWND hWnd)		{}
	VOID winx_msg OnFinalMessage(HWND hWnd)		{}

public:
	// OnNcCreate/OnCreate - Only For Normal Window
	BOOL winx_msg OnNcCreate(HWND hWnd, LPCREATESTRUCT lpCS) { return TRUE; }

	LRESULT winx_msg OnCreate(HWND hWnd, LPCREATESTRUCT lpCS) { return 0; }
	LRESULT winx_msg SuperOnCreate(HWND hWnd, LPCREATESTRUCT lpCS) { return 0; }

	// OnSubclass/OnUnsubclass - Only For SubclassWindow
	LRESULT winx_msg OnSubclass(HWND hWnd) { return 0; }
	VOID winx_msg OnUnsubclass(HWND hWnd) {}

	// OnInitDialog/OnPostInitDialog/OnDlgInitCreateAxCtrl - Only For Dialog Box
	HRESULT winx_msg OnDlgInitCreateAxCtrl(HWND hDlg) { return S_OK; }
	BOOL winx_msg OnInitDialog(HWND hDlg, HWND hDefaultFocus) { return TRUE; }
	VOID winx_msg OnPostInitDialog(HWND hDlg) {}

public:
	// OnEnterAccelFrame/OnLeaveAccelFrame - Only For Dialog Box

	VOID winx_msg OnEnterAccelFrame(HWND hWnd)	{}
	VOID winx_msg OnLeaveAccelFrame(HWND hWnd)	{}

public:
	// Reflect Support
	//  - 这些消息只有在父窗口（通常是对话框）定义WINX_REFLECT()宏才有效！！！

	// DrawItem/MeasureItem/OnReflectCommand
	//	- Only For CommCtrl, such as Static, Edit, Button, ListBox, etc.
	VOID winx_msg DrawItem(HWND hWnd, PDRAWITEMSTRUCT lpDI)	{}
	VOID winx_msg MeasureItem(HWND hWnd, LPMEASUREITEMSTRUCT lpMI) {}
	BOOL winx_msg OnReflectCommand(HWND hWnd, WPARAM wParam)
	{
		return FALSE;
	}

	// OnReflectNotify - Only For ExCommCtrl, such as ListCtrl, TreeCtrl, etc.
	BOOL winx_msg OnReflectNotify(
		HWND hWnd, LPNMHDR pnmh, LRESULT* pResult)
	{
		return FALSE;
	}

public:
	HCURSOR winx_msg OnQueryDragIcon(HWND hWnd)
	{
		return NULL;
	}

public:
	// OnCommand/OnSysCommand/OnNotify/OnDrawItem(OwnerDraw)

	BOOL winx_msg OnSysCommand(
		HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	BOOL winx_msg OnCommand(
		HWND hWnd, WPARAM wParam, HWND hWndCtlFrom)
	{
		return FALSE;
	}

	VOID winx_msg OnOK(HWND hWnd)
	{
	}

	//
	//--> idCtrl
	//  Identifier of the common control sending the message. 
	//	This identifier is not guaranteed to be unique. An application should use the 
	//	hwndFrom or idFrom member of the NMHDR structure to identify the control. 
	//--> pnmh
	//	Pointer to an NMHDR structure that contains the notification code and additional 
	//	information. For some notification messages, this parameter points to a larger 
	//	structure that has the NMHDR structure as its first member. 
	//
	BOOL winx_msg OnNotify(
		HWND hWnd, int idCtrl, LPNMHDR pnmh, LRESULT* pResult)
	{
		return FALSE;
	}
	
	VOID winx_msg OnParentNotify(
		HWND hWnd, WPARAM wParam, LPARAM lParam) {}

	BOOL winx_msg OnDrawItem(HWND hWnd, int nIdCtrl, LPDRAWITEMSTRUCT lpDI)
		{ return FALSE; }

	BOOL winx_msg OnMeasureItem(HWND hWnd, int nIdCtrl, LPMEASUREITEMSTRUCT lpMI)
		{ return FALSE; }

public:
	// OnDragListNotify - DragListBox Notify
	
	BOOL winx_msg DragList_OnBeginDrag(HWND hWnd, int idCtrl, LPDRAGLISTINFO lpDLI)
	{
		return TRUE; // allow dragging
	}

	LONG winx_msg DragList_OnDragging(HWND hWnd, int idCtrl, LPDRAGLISTINFO lpDLI)
	{
		return DL_CURSORSET; // don't change cursor
	}
	
	VOID winx_msg DragList_OnCancelDrag(HWND hWnd, int idCtrl, LPDRAGLISTINFO lpDLI)
	{
	}
	
	VOID winx_msg DragList_OnDropped(HWND hWnd, int idCtrl, LPDRAGLISTINFO lpDLI)
	{
	}

	LRESULT winx_msg OnDragListNotify(HWND hWnd, int idCtrl, LPDRAGLISTINFO lpDLI)
	{
		return 0;
	}
	
public:
	// OnPaint

	VOID winx_msg OnDraw(HWND hWnd, HDC hDC) {}
	VOID winx_msg OnDrawEx(HWND hWnd, HDC hDC, const RECT& rcPaint) {}
	VOID winx_msg OnDrawBkgnd(HWND hWnd, HDC hDC, const RECT& rcPaint) {}
	VOID winx_msg OnPaint(HWND hWnd) {}
	VOID winx_msg OnNcPaint(HWND hWnd, HRGN hrgnClip) {}
	BOOL winx_msg OnXPThemeNcPaint(HWND hWnd, HRGN hrgnClip) { return FALSE; }
	VOID winx_msg OnNcCalcSize(HWND hWnd, BOOL bCalcValidRects, NCCALCSIZE_PARAMS& ncsp) {}
	
public:
	// OnEraseBkgnd - Background Support

	HBRUSH winx_call GetBkgndBrush() const { return NULL; }
	BOOL winx_msg OnEraseBkgnd(HWND hWnd, HDC hDC)
	{
		return FALSE;
	}

public:
	// OnContextMenu

	VOID winx_msg OnContextMenu(HWND hWnd, winx::CPoint pos)				{}
	VOID winx_msg OnInitMenuPopup(HWND hWnd, HMENU hMenu, LPARAM lParam)	{}

public:
	// OnWindowPosChanged/OnWindowPosChanging

	VOID winx_msg OnWindowPosChanged(HWND hWnd, const WINDOWPOS& wndPos)	{}
	VOID winx_msg OnWindowPosChanging(HWND hWnd, WINDOWPOS& wndPos)			{}

public:
	// OnGetMinMaxInfo - 限制Resizing窗口大小的行为

	VOID winx_msg OnGetMinMaxInfo(HWND hWnd, LPMINMAXINFO lpMM)	{}

public:
	// OnSettingChange
	
	//
	//uFlags
	//	When the system sends the message as a result of a SystemParametersInfo call,
	//	this parameter is a flag that indicates the system parameter that was changed.
	//	For a list of values, see the SystemParametersInfo function. 
	//	When an application sends the message, this parameter must be NULL. 
	//lpszSection
	//	Zero in WinCE.
	VOID winx_msg OnSettingChange(HWND hWnd, UINT uFlags, LPCTSTR lpszSection)
	{
	}
	
public:
	// OnThemeChanged - XPStyle Support

	VOID winx_msg OnThemeChanged(HWND hWnd)
	{
	}

public:
	// OnCtlColor - Background Style Support

	HBRUSH winx_msg OnCtlColor(
		HWND hWnd, HDC hDC, HWND hWndCtrl, UINT nCtlColor)
	{
		return NULL;
	}

public:
	// OnSetFocus - ActiveView Support

	//
	//--> hWndPrevFocus
	//	Handle to the window that has lost the keyboard focus. This parameter can be NULL. 
	//
	BOOL winx_msg OnSetFocus(HWND hWnd, HWND hWndPrevFocus)	{ return FALSE; }
	BOOL winx_msg OnKillFocus(HWND hWnd, HWND hWndNewFocus) { return FALSE; }
	
	VOID winx_msg OnEnable(HWND hWnd, BOOL fEnable)			{}

public:
	// MouseEvent Support

	VOID winx_msg OnMouseMove(HWND hWnd, UINT nFlags, winx::CPoint point)		{}
	VOID winx_msg OnLButtonDown(HWND hWnd, UINT nFlags, winx::CPoint point)		{}
	VOID winx_msg OnLButtonUp(HWND hWnd, UINT nFlags, winx::CPoint point)		{}
	VOID winx_msg OnLButtonDblClk(HWND hWnd, UINT nFlags, winx::CPoint point)	{}
	VOID winx_msg OnRButtonDown(HWND hWnd, UINT nFlags, winx::CPoint point)		{}
	VOID winx_msg OnRButtonUp(HWND hWnd, UINT nFlags, winx::CPoint point)		{}
	VOID winx_msg OnRButtonDblClk(HWND hWnd, UINT nFlags, winx::CPoint point)	{}
	VOID winx_msg OnMouseWheel(
		HWND hWnd, UINT nFlags, INT zDelta, winx::CPoint point) {}

	BOOL winx_msg OnSetCursor(
		HWND hWnd, HWND hWndContainsCursor, LPARAM lParam) { return FALSE; }

	UINT winx_msg OnNcHitTest(
		HWND hWnd, winx::CPoint point) { return HTNOWHERE; }

public:
	// Scroll Support

	VOID winx_msg OnHScroll(
		HWND hWnd, UINT nSBCode, INT nPos, HWND hWndScrollBar)			{}
	
	VOID winx_msg OnVScroll(
		HWND hWnd, UINT nSBCode, INT nPos, HWND hWndScrollBar)			{}
	
public:
	// Keyboard Support

	VOID winx_msg OnChar(HWND hWnd, UINT uChar, UINT uKeyData)			{}
	VOID winx_msg OnKeyDown(HWND hWnd, UINT uVKChar, UINT uKeyData)		{}
	VOID winx_msg OnKeyUp(HWND hWnd, UINT uVKChar, UINT uKeyData)		{}
	VOID winx_msg OnSysKeyDown(HWND hWnd, UINT uVKChar, UINT uKeyData)	{}
	VOID winx_msg OnSysKeyUp(HWND hWnd, UINT uVKChar, UINT uKeyData)	{}

public:
	// Timer Support

	VOID winx_msg OnTimer(HWND hWnd, UINT nIDEvent) {}
	VOID winx_msg OnIdle(HWND hWnd) {}

public:
	// Accelerator Support

	enum TranslateAccelState
	{
		CanTranslateAccel = 0,
		CannotTranslateAccel = 1,
	};

	BOOL winx_msg TestTranslateAccel(HWND hWnd, LPMSG lpMsg)
	{
		return CanTranslateAccel;
	}

	BOOL winx_msg OnTranslateAccel(HWND hWnd, LPMSG lpMsg)
	{
		return FALSE;
	}

public:
	// HandleUpdateViews - UpdateViews Support

	VOID winx_msg HandleUpdateViews(HWND hWnd)
	{
	}

public:
	// PreHandleMessage - Behavior Support

	BOOL winx_msg PreHandleMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		return FALSE;
	}
		
public:
	// ProcessUpdateUIMessage - WTL::CUpdateUI Support

	BOOL winx_call ProcessUpdateUIMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

public:
	// ProcessDialogResizeMessage - WTL::CDialogResize Support

	BOOL winx_call ProcessDialogResizeMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

public:
	// ProcessUserMessage - UserDefined Message Support

	BOOL winx_call ProcessUserMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);
	
public:
	// HandleMessage - DefaultHandle与HandleMessage关系，相当于父类与子类的关系。

	LRESULT winx_msg HandleMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WINX_REPORT("HandleMessage - Don't call me!!!\n");
		return 0;
	}

	LRESULT winx_msg DefaultHandle(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WINX_REPORT("DefaultHandle - Don't call me!!!\n");
		return 0;
	}
};

// =========================================================================
// $Log: MessagePrototype.h,v $
// Revision 1.14  2006/12/03 08:40:22  xushiwei
// Unify style of map macro, such as:
//  WINX_TEST_SUITE/WINX_TEST/WINX_TEST_SUITE_END;
//  WINX_CMDS_BEGIN/WINX_CMD/WINX_CMDS_END;
//  WINX_SYSCMD_BEGIN/WINX_SYSCMD/WINX_SYSCMD_END;
//  WINX_NOTIFY_BEGIN/WINX_NOTIFY/WINX_NOTIFY_END;
//  WINX_REFLECT_NOTIFY_BEGIN/WINX_REFLECT_NOTIFY/WINX_REFLECT_NOTIFY_END;
//
// Revision 1.13  2006/12/02 15:28:54  xushiwei
// WM_CLOSE, WM_QUERYDRAGICON
//
// Revision 1.12  2006/09/18 05:39:11  xushiwei
// WINX-Core: Message: OnIdle, OnSettingChange support
//
// Revision 1.8  2006/09/16 11:25:16  xushiwei
// WINX_REFLECT_CTLCOLOR
//
// Revision 1.7  2006/09/14 14:44:01  xushiwei
// Winx-Extension-ActiveX support bugfix (Visual Studio 2005):
//  Failed to create ActiveX controls in dialog resource template.
//  (Reason: AtlAxDialogBox/AtlAxCreateDialog implementation was changed in high version ATL.)
//
// Revision 1.6  2006/09/03 04:32:56  xushiwei
// WINX-Core: Behavior(BehaviorPermit, BehaviorSwitch, AutoHidden, etc)
// WINX-Extension: PropertySheet, PropertyPage
//
// Revision 1.4  2006/08/23 05:47:06  xushiwei
// WINX-Core:
//   Property(Icon, AppIcon), Layout(DialogResize, MinTrackSize)
// WINX-CommonDialogs:
//   OpenFileDialog/SaveFileDialog, OpenMultiFilesDialog/SaveMultiFilesDialog
//
// Revision 1.3  2006/08/22 10:36:53  xushiwei
// WINX-Core:
//   Property(Bkgrnd, Accel, Layout), MainFrame support
//   CommandDispatch, CommandState, Demo: User-defined-control(Subclass, Superclass, SuperDialog)
//
// Revision 1.2  2006/08/21 18:45:48  xushiwei
// WINX-Core:
//    Window::Create, MainFrame, StackWindowObject(WINX_STACK_OBJECT) support
//
// Revision 1.1  2006/08/19 10:50:56  xushiwei
// WINX-Core: Message, Window, Dialog, Control
//

__WINX_END

#endif /* __WINX_WIN_MESSAGEPROTOTYPE_H__ */
