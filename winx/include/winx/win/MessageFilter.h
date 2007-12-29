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
// Module: winx/win/MessageFilter.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 18:03:20
// 
// $Id: MessageFilter.h,v 1.2 2006/08/23 05:47:06 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_MESSAGEFILTER_H__
#define __WINX_WIN_MESSAGEFILTER_H__

#if (0)
#define WINX_MT_HOOK
#endif

#if (0)
#define WINX_APPICON_TRACE	WINX_TRACE
#else
#define WINX_APPICON_TRACE	__noop
#endif

#ifndef __WINX_WIN_BASICTYPES_H__
#include "BasicTypes.h"
#endif

#ifndef _VECTOR_
#include <vector>
#endif

__WINX_BEGIN

// =========================================================================
// HookCodeTraits

template <int idHook>
struct HookCodeTraits
{
	enum { HasRetVal = 0 };
	static BOOL winx_call IsAction(int code) {
		return code == HC_ACTION;
	}
};

template <>
struct HookCodeTraits<WH_MSGFILTER>
{
	enum { HasRetVal = 1 };
	// MSGF_DDEMGR, MSGF_MAINLOOP (you can define) >= MSGF_USER
	// MSGF_DIALOGBOX, MSGF_MESSAGEBOX, MSGF_MENU, MSGF_SCROLLBAR, MSGF_NEXTWINDOW
	static BOOL winx_call IsAction(int code) {
		return code >= 0;
	}
};

template <>
struct HookCodeTraits<WH_SYSMSGFILTER>
{
	enum { HasRetVal = 1 };
	static BOOL winx_call IsAction(int code) {
		return code >= 0;
	}
};

// =========================================================================
// class MessageHook

typedef HHOOK HookHandle;

struct _NullHookData {};

template <class _HookClass, int idHook, class _DataClass = _NullHookData>
class MessageHook
{
public:
	typedef MessageHook HookBase;
	typedef _HookClass HookClass;
	typedef _DataClass HookDataClass;

	struct _HookData
	{
		HookHandle hhk;
		HookDataClass info;
	};

protected:
	typedef HookCodeTraits<idHook> _HookCodeTraits;
	static _HookData m_data;

	static LRESULT CALLBACK _HookProc(int code, WPARAM wParam, LPARAM lParam)
	{
		if (_HookCodeTraits::IsAction(code))
		{
			LRESULT res = _HookClass::ProcessHookMessage(code, wParam, lParam);
			if (_HookCodeTraits::HasRetVal)
			{
				if (res != 0)
					return res;
			}
		}
		return CallNextHookEx(m_data.hhk, code, wParam, lParam);
	}

public:
	static HookHandle winx_call Hook(HINSTANCE hInst)
	{
		WINX_ASSERT(m_data.hhk == NULL);
	
		_HookClass::OnInitHook(m_data.info);
		return m_data.hhk = ::SetWindowsHookEx(
			idHook, _HookProc, hInst, ::GetCurrentThreadId()
			);
	}

	static HookHandle winx_call Hook(HWND hWnd)
	{
		HINSTANCE hInst = (HINSTANCE)::GetWindowLong(hWnd, GWL_HINSTANCE);
		return Hook(hInst);
	}

	static VOID winx_call HookOnce(HINSTANCE hInst)
	{
		if (m_data.hhk == NULL)
			Hook(hInst);
	}

	static VOID winx_call HookOnce(HWND hWnd)
	{
		if (m_data.hhk == NULL)
			Hook(hWnd);
	}

	static VOID winx_call Unhook()
	{
		if (m_data.hhk != NULL)
		{
			::UnhookWindowsHookEx(m_data.hhk);
			m_data.hhk = NULL;
		}
	}

public:
	static VOID winx_call OnInitHook(HookDataClass& info)
	{
	}

	static HookDataClass& winx_call Data()
	{
		return m_data.info;
	}

	static BOOL winx_call IsHooking()
	{
		return m_data.hhk != NULL;
	}
};

template <class HookClass, int idHook, class _DataClass>
typename MessageHook<HookClass, idHook, _DataClass>::_HookData 
	MessageHook<HookClass, idHook, _DataClass>::m_data;

// =========================================================================
// class AppIconImpl

class AppIconImpl
{
private:
	HICON m_hAppIcon;
	
public:
	AppIconImpl() : m_hAppIcon(NULL)
	{
	}
	~AppIconImpl()
	{
		if (m_hAppIcon)
			::DestroyIcon(m_hAppIcon);
	}
	
public:
	VOID winx_call InitIcon(HINSTANCE hInst, RESID uIconID)
	{
		m_hAppIcon = ::LoadIcon(hInst, (LPCTSTR)(uIconID));
	}

	VOID winx_call SetWindowIcon(HWND hWnd)
	{
#if defined(_DEBUG)
		TCHAR szClass[128];
		::GetClassName(hWnd, szClass, countof(szClass));
		WINX_APPICON_TRACE(_T("---> SetWindowIcon [Class:%s]\n"), szClass);
#endif
		WINX_ASSERT(m_hAppIcon);
		winx::SetWindowIcon(hWnd, m_hAppIcon);
	}
};

// -------------------------------------------------------------------------
// class AppIconHook

class AppIconHook : 
	public MessageHook<AppIconHook, WH_CALLWNDPROC, AppIconImpl>
{
public:
	AppIconHook(HINSTANCE hInst, RESID uIconID) 
	{
		Data().InitIcon(hInst, uIconID);
		Hook(hInst);
	}
	AppIconHook(HINSTANCE hInst, RESID uIconID, int zeroForNoAutoHook) 
	{
		Data().InitIcon(hInst, uIconID);
	}

	static VOID winx_call OnInitDlgFrame(HWND hWnd)
	{
		Data().SetWindowIcon(hWnd);
	}

	static LRESULT winx_call ProcessHookMessage(int code, WPARAM wParam, LPARAM lParam)
	{
		CWPSTRUCT* lpCWP = (CWPSTRUCT*)lParam;
		if (lpCWP->message == WM_CREATE)
		{
			LPCREATESTRUCT lpCS = (LPCREATESTRUCT)lpCWP->lParam;
			if (WS_DLGFRAME & lpCS->style)
			{
				Data().SetWindowIcon(lpCWP->hwnd);
			}
		}
		return 0;
	}
};

// 注意: 所有WINX_APP_XXX宏，均定义为Application一级属性。
// 而Application一级的属性，实际上通常是一个语句。故此一般出现在WinMain函数中。
#ifndef WINX_APP_ICON
#define WINX_APP_ICON	winx::AppIconHook _winx_appIcon
#endif

// =========================================================================
// class AccelFrame

struct ModelFrame
{
	HWND m_hWndModal;
};

class AccelFrameHook : 
	public MessageHook<AccelFrameHook, WH_MSGFILTER, ModelFrame>
{
public:
	static LRESULT winx_call ProcessHookMessage(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code < MSGF_MENU || code > MSGF_MAX)
		{
			MSG* lpMsg = (MSG*)lParam;
			if (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST)
				return ForwardMessage(Data().m_hWndModal, lpMsg);
		}
		return FALSE;
	}

	static VOID winx_call Enter(HWND hWndModal, ModelFrame& oldFrame)
	{
		ModelFrame& data = Data();
		oldFrame.m_hWndModal = data.m_hWndModal;
		data.m_hWndModal = hWndModal;
		HookOnce(hWndModal);
	}

	static VOID winx_call Leave(const ModelFrame& oldFrame)
	{
		if (oldFrame.m_hWndModal == NULL)
			Unhook();
		Data().m_hWndModal = oldFrame.m_hWndModal;
	}
};

// -------------------------------------------------------------------------
// WINX_ACCELFRAME - DialogBox/MainFrame支持Accelerator

#define WINX_ACCELFRAME()													\
public:																		\
	VOID winx_msg OnEnterAccelFrame(HWND hWnd) {							\
		winx::AccelFrameHook::Enter(hWnd, _winx_oldFrame);					\
	}																		\
	VOID winx_msg OnLeaveAccelFrame(HWND hWnd) {							\
		winx::AccelFrameHook::Leave(_winx_oldFrame);						\
	}																		\
private:																	\
	winx::ModelFrame _winx_oldFrame

// =========================================================================
// class MessageFilter

class _MsgFilter
{
private:
	_MsgFilter* m_pNext;

public:
	virtual BOOL winx_msg PreTranslateMessage(MSG* lpMsg)
	{
		return FALSE;
	}

public:
	static BOOL winx_call _DoPreTranslateMessage(_MsgFilter* p, MSG* lpMsg)
	{
		for (; p; p = p->m_pNext)
		{
			if (p->PreTranslateMessage(lpMsg))
				return TRUE;
		}
		return FALSE;
	}

	static VOID winx_call _DoEnterChain(_MsgFilter*& pChain, _MsgFilter* pFilter)
	{
		pFilter->m_pNext = pChain;
		pChain = pFilter;
	}

	static VOID winx_call _DoLeaveChain(_MsgFilter** pp, _MsgFilter* pFilter)
	{
		_MsgFilter* p;
		while( (p = *pp) != NULL )
		{
			if (p == pFilter)
			{
				*pp = p->m_pNext;
				return;
			}
			pp = &p->m_pNext;	
		}
	}
};

template <class WindowImplClass, int MSGF_BASE = WINX_MSGF_BASE>
class MessageFilter : 
	public _MsgFilter,
	public winx::MessageHook<WindowImplClass, WH_MSGFILTER, _MsgFilter*>
{
public:
	typedef MessageFilter MessageFilterClass;

/*
 * Don't uncomment this. It's just an example.
 *
	BOOL winx_msg PreTranslateMessage(MSG* lpMsg)
	{
		// don't translate non-input events
		if ((lpMsg->message < WM_KEYFIRST || lpMsg->message > WM_KEYLAST) &&
			(lpMsg->message < WM_MOUSEFIRST || lpMsg->message > WM_MOUSELAST))
			return FALSE;

		return ::IsDialogMessage(_WINX_PWND->m_hWnd, lpMsg);
	}
 */

	VOID winx_call EnterMsgFilterChain(HWND hWnd)
	{
		_DoEnterChain(Data(), this);
		HookOnce(hWnd);
	}

	VOID winx_call LeaveMsgFilterChain()
	{
		_DoLeaveChain(&Data(), this);
	}

/*
 * Technical detail: why use condition: if (code < WINX_MSGF_BASE)?
 *
	MessageFilter::PreTranslateMessage mostly maybe call IsDialogMessage
	function to translate dialog messages. However, IsDialogMessage will
	call CallMsgFilter(with code=MSGF_DIALOGBOX) function, which recursively
	generate WH_MSGFILTER hook messages.
 */
public:
	static LRESULT winx_call ProcessHookMessage(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code < MSGF_BASE)
			return FALSE;
		return _DoPreTranslateMessage(Data(), (MSG*)lParam);
	}
};

// -------------------------------------------------------------------------
// WINX_MSGFILTER - 支持PreTranslateMessage

#define WINX_MSGFILTER_NULL()												\
	WINX_MSG_NULL_HANDLER(OnMsgFilterInit);									\
	WINX_MSG_NULL_HANDLER(OnMsgFilterTerm)

#define WINX_MSGFILTER()													\
public:																		\
	VOID winx_msg OnMsgFilterInit(HWND hWnd) {								\
		EnterMsgFilterChain(hWnd);											\
	}																		\
	VOID winx_msg OnMsgFilterTerm(HWND hWnd) {								\
		LeaveMsgFilterChain();												\
	}

#define WINX_MESSAGEFILTER()		WINX_MSGFILTER()
#define WINX_MESSAGEFILTER_NULL()	WINX_MSGFILTER_NULL()

// =========================================================================
// $Log: MessageFilter.h,v $
// Revision 1.2  2006/08/23 05:47:06  xushiwei
// WINX-Core:
//   Property(Icon, AppIcon), Layout(DialogResize, MinTrackSize)
// WINX-CommonDialogs:
//   OpenFileDialog/SaveFileDialog, OpenMultiFilesDialog/SaveMultiFilesDialog
//
// Revision 1.1  2006/08/19 10:50:56  xushiwei
// WINX-Core: Message, Window, Dialog, Control
//

__WINX_END

#endif /* __WINX_WIN_MESSAGEFILTER_H__ */
