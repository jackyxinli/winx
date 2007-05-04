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

template <class _HookClass, int idHook>
class MessageHook
{
public:
	typedef MessageHook HookBase;
	typedef _HookClass HookClass;

protected:
	typedef HookCodeTraits<idHook> _HookCodeTraits;
	struct _HookData
	{
		HookHandle hhk;
		HookClass* doIt;
	};
	static _HookData m_data;

	static LRESULT CALLBACK _HookProc(int code, WPARAM wParam, LPARAM lParam)
	{
		if (_HookCodeTraits::IsAction(code))
		{
			LRESULT res = m_data.doIt->ProcessHookMessage(code, wParam, lParam);
			if (_HookCodeTraits::HasRetVal)
			{
				if (res != 0)
					return res;
			}
		}
		return CallNextHookEx(m_data.hhk, code, wParam, lParam);
	}

public:
	HookHandle winx_call Hook(HINSTANCE hInst)
	{
		WINX_ASSERT(m_data.hhk == NULL);
		WINX_ASSERT(m_data.doIt == NULL);
	
		m_data.doIt = static_cast<HookClass*>(this);
		return m_data.hhk = ::SetWindowsHookEx(
			idHook, _HookProc, hInst, ::GetCurrentThreadId()
			);
	}

	HookHandle winx_call Hook(HWND hWnd)
	{
		HINSTANCE hInst = (HINSTANCE)::GetWindowLong(hWnd, GWL_HINSTANCE);
		return Hook(hInst);
	}

	VOID winx_call HookOnce(HINSTANCE hInst)
	{
		if (m_data.hhk == NULL)
			Hook(hInst);
	}

	VOID winx_call HookOnce(HWND hWnd)
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
			m_data.doIt = NULL;
		}
	}

	static HookClass* winx_call GetHook()
	{
		return m_data.doIt;
	}

	static BOOL winx_call IsHooking()
	{
		return m_data.hhk != NULL;
	}
};

template <class HookClass, int idHook>
typename MessageHook<HookClass, idHook>::_HookData MessageHook<HookClass, idHook>::m_data;

// =========================================================================
// class AutoMessageHook

template <class HookClass, int idHook>
class AutoMessageHook : public MessageHook<HookClass, idHook>
{
public:
	typedef AutoMessageHook HookBase;

	AutoMessageHook() {}
	AutoMessageHook(HINSTANCE hInst)
	{
		Hook(hInst);
	}
	~AutoMessageHook()
	{
		Unhook();
	}
};

// =========================================================================
// class AppIconImpl

class AppIconImpl
{
private:
	HICON m_hAppIcon;
	
public:
	AppIconImpl(HINSTANCE hInst, RESID uIconID)
		: m_hAppIcon(::LoadIcon(hInst, (LPCTSTR)(uIconID)))
	{
	}
	~AppIconImpl()
	{
		::DestroyIcon(m_hAppIcon);
	}
	
public:
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

	VOID winx_call OnInitDlgFrame(HWND hWnd)
	{
		SetWindowIcon(hWnd);
	}
};

// -------------------------------------------------------------------------
// class AppIconHook

class AppIconHook : 
	public AppIconImpl,
	public AutoMessageHook<AppIconHook, WH_CALLWNDPROC>
{
public:
	AppIconHook(HINSTANCE hInst, RESID uIconID) 
		: AppIconImpl(hInst, uIconID), HookBase(hInst)
	{
	}
	AppIconHook(HINSTANCE hInst, RESID uIconID, int zeroForNoAutoHook) 
		: AppIconImpl(hInst, uIconID)
	{
	}

	LRESULT winx_call ProcessHookMessage(int code, WPARAM wParam, LPARAM lParam)
	{
		CWPSTRUCT* lpCWP = (CWPSTRUCT*)lParam;
		if (lpCWP->message == WM_CREATE)
		{
			LPCREATESTRUCT lpCS = (LPCREATESTRUCT)lpCWP->lParam;
			if (WS_DLGFRAME & lpCS->style)
			{
				SetWindowIcon(lpCWP->hwnd);
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
	public ModelFrame,
	public MessageHook<AccelFrameHook, WH_MSGFILTER>
{
public:
	LRESULT winx_call ProcessHookMessage(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code < MSGF_MENU || code > MSGF_MAX)
		{
			MSG* lpMsg = (MSG*)lParam;
			if (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST)
				return ForwardMessage(m_hWndModal, lpMsg);
		}
		return FALSE;
	}

	static VOID winx_call Enter(HWND hWndModal, ModelFrame& oldFrame)
	{
		HookClass* data = GetHook();
		if (data == NULL)
		{
			oldFrame.m_hWndModal = NULL;
			data = new HookClass;
			data->m_hWndModal = hWndModal;
			data->Hook(hWndModal);
		}
		else
		{
			oldFrame.m_hWndModal = data->m_hWndModal;
			data->m_hWndModal = hWndModal;
		}
	}

	static VOID winx_call Leave(const ModelFrame& oldFrame)
	{
		HookClass* data = GetHook();
		if (oldFrame.m_hWndModal == NULL)
		{
			Unhook();
			delete data;
		}
		else
		{
			data->m_hWndModal = oldFrame.m_hWndModal;
		}
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

template <class WindowImplClass>
class MessageFilter : public winx::MessageHook<WindowImplClass, WH_MSGFILTER>
{
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
public:
	BOOL winx_msg PreTranslateMessage(MSG* lpMsg)
	{
		return FALSE;
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
	LRESULT winx_call ProcessHookMessage(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code < WINX_MSGF_BASE)
			return FALSE;
		return _WINX_PWND->PreTranslateMessage((MSG*)lParam);
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
		Hook(hWnd);															\
	}																		\
	VOID winx_msg OnMsgFilterTerm(HWND hWnd) {								\
		Unhook();															\
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
