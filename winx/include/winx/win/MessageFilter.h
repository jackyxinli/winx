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

__WINX_BEGIN

// =========================================================================
// class SimpleHook

template <class HookClass, int idHook>
class SimpleHook
{
private:
	struct _HookData
	{
		HHOOK hhk;
		HookClass* doIt;
	};
	static _HookData m_data;

	static LRESULT CALLBACK _HookProc(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code == HC_ACTION)
		{
			m_data.doIt->ProcessMessage(wParam, lParam);
		}
		return CallNextHookEx(m_data.hhk, code, wParam, lParam);
	}

public:
	typedef SimpleHook<HookClass, idHook> HookBase;

#if defined(_DEBUG)
	~SimpleHook()
	{
		WINX_ASSERT(m_data.hhk == NULL);
	}
#endif

	HHOOK winx_call Hook(HINSTANCE hInst)
	{
		WINX_ASSERT(m_data.hhk == NULL);
		WINX_ASSERT(m_data.doIt == NULL);
	
		m_data.doIt = (HookClass*)this;
		return m_data.hhk = ::SetWindowsHookEx(
			idHook, _HookProc, hInst, ::GetCurrentThreadId());
	}

	static BOOL winx_call IsHooking()
	{
		return (m_data.hhk != NULL);
	}

	static VOID winx_call Unhook()
	{
		WINX_ASSERT(m_data.hhk != NULL);

		if (m_data.hhk != NULL)
		{
			::UnhookWindowsHookEx(m_data.hhk);
			m_data.hhk = NULL;
		}
	}
};

template <class HookClass, int idHook>
typename SimpleHook<HookClass, idHook>::_HookData SimpleHook<HookClass, idHook>::m_data;

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
	public SimpleHook<AppIconHook, WH_CALLWNDPROC>,
	public AppIconImpl
{
public:
	AppIconHook(HINSTANCE hInst, RESID uIconID, BOOL fAutoHook = TRUE) 
		: AppIconImpl(hInst, uIconID)
	{
		if (fAutoHook)
			Hook(hInst);
	}
	~AppIconHook()
	{
		if (IsHooking())
			Unhook();
	}

public:
	VOID winx_call ProcessMessage(WPARAM wParam, LPARAM lParam)
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
	}
};

// 注意: 所有WINX_APP_XXX宏，均定义为Application一级属性。
// 而Application一级的属性，实际上通常是一个语句。故此一般出现在WinMain函数中。
#ifndef WINX_APP_ICON
#define WINX_APP_ICON	winx::AppIconHook _winx_appIcon
#endif

// =========================================================================
// class SingleTheadHook

enum HookLeaveRetType
{
	leaveError = -1,
	leaveDoNothing = 0,
	leaveUnhook = 1,
};

template <class HookClass, int idHook>
class SingleTheadHook
{
public:
	struct _HookData
	{
		HHOOK hhk;
		HookClass doIt;
	};
	static _HookData m_data;

	static LRESULT CALLBACK _HookProc(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code == HC_ACTION)
		{
			m_data.doIt.ProcessMessage(wParam, lParam);
		}
		return CallNextHookEx(m_data.hhk, code, wParam, lParam);
	}

public:
	static VOID winx_call Enter(HINSTANCE hInst, LPVOID extraParam)
	{
		if (m_data.hhk == NULL)
		{
			m_data.hhk = ::SetWindowsHookEx(
				idHook, _HookProc, hInst, ::GetCurrentThreadId());
		}
		m_data.doIt.OnEnter(extraParam);
	}

	static VOID winx_call Leave(LPVOID extraParam)
	{
		WINX_ASSERT(m_data.hhk != NULL);
		
		HookLeaveRetType result = m_data.doIt.OnLeave(extraParam);
		WINX_ASSERT(result != leaveError);
		if (result == leaveUnhook)
		{
			::UnhookWindowsHookEx(m_data.hhk);
			m_data.hhk = NULL;
		}
	}
};

// =========================================================================
// class MultiTheadHook

template <class HookClass, int idHook>
class MultiTheadHook
{
public:
	struct _HookData
	{
		HHOOK hhk;
		HookClass doIt;
	};
	static __declspec(thread) _HookData* m_data;

	static LRESULT CALLBACK _HookProc(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code == HC_ACTION)
		{
			m_data->doIt.ProcessMessage(wParam, lParam);
		}
		return CallNextHookEx(m_data->hhk, code, wParam, lParam);
	}

public:
	static VOID winx_call Enter(HINSTANCE hInst, LPVOID extraParam)
	{
		_HookData* data;
		if (m_data == NULL)
		{
			data = m_data = new _HookData;
			data->hhk = ::SetWindowsHookEx(
				idHook, _HookProc, hInst, ::GetCurrentThreadId());
		}
		else
		{
			data = m_data;
			WINX_ASSERT(data->hhk != NULL);
		}
		data->doIt.OnEnter(extraParam);
	}

	static VOID winx_call Leave(LPVOID extraParam)
	{
		WINX_ASSERT(m_data != NULL);
		WINX_ASSERT(m_data->hhk != NULL);
		
		_HookData* data = m_data;
		if (data)
		{
			HookLeaveRetType result = data->OnLeave(extraParam);
			WINX_ASSERT(result != leaveError);
			if (result == leaveUnhook)
			{
				m_data = NULL;
				::UnhookWindowsHookEx(data->hhk);
				delete data;
			}
		}
	}
};

// =========================================================================
// class AccelFrame

struct ModalFrame
{
	HWND m_hWndModal;
};

class AccelFrame : public ModalFrame
{
public:
	AccelFrame() { m_hWndModal = NULL; }

	ModalFrame winx_call EnterAccelFrame(HWND hWndModal)
	{
		WINX_ASSERT(::IsWindow(hWndModal));
		ModalFrame oldFrame = *static_cast<const ModalFrame*>(this);
		m_hWndModal = hWndModal;
		return oldFrame;
	}

	void winx_call LeaveAccelFrame(const ModalFrame& oldFrame)
	{
		*static_cast<ModalFrame*>(this) = oldFrame;
	}

public:
	struct EnterArgType
	{
		HWND hWndModal;
		ModalFrame oldFrame;
	};

	VOID winx_call ProcessMessage(WPARAM wParam, LPARAM lParam)
	{
		if (wParam != PM_REMOVE)
			return;

		LPMSG pMsg = (LPMSG)lParam;
		if (pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST)
			return;

		if (ForwardMessage(m_hWndModal, pMsg))
			pMsg->message = WM_NULL; // eaten this message
	}
	
	VOID winx_call OnEnter(LPVOID extraParam)
	{
		EnterArgType* p = (EnterArgType*)extraParam;
		p->oldFrame = EnterAccelFrame(p->hWndModal);
	}

	HookLeaveRetType winx_call OnLeave(LPVOID extraParam)
	{
		const ModalFrame& oldFrame = *(const ModalFrame*)extraParam;
		LeaveAccelFrame(oldFrame);
		return oldFrame.m_hWndModal ? leaveDoNothing : leaveUnhook;
	}
};

#if defined(WINX_MT_HOOK)
	typedef MultiTheadHook<AccelFrame, WH_GETMESSAGE> AccelFrameHook;
#else
	typedef SingleTheadHook<AccelFrame, WH_GETMESSAGE> AccelFrameHook;
#endif

// =========================================================================
// EnterAccelFrame/LeaveAccelFrame

#define WINX_ENTERACCELFRAME(oldFrameArg, hWndModalArg)						\
	do {																	\
		winx::AccelFrame::EnterArgType _winx_para;							\
		_winx_para.hWndModal = hWndModalArg;								\
		HINSTANCE _winx_hInst = (HINSTANCE)::GetWindowLong(					\
			_winx_para.hWndModal, GWL_HINSTANCE);							\
		winx::AccelFrameHook::Enter(_winx_hInst, &_winx_para);				\
		oldFrameArg = _winx_para.oldFrame;									\
	} while (0)

#define WINX_LEAVEACCELFRAME(oldFrameArg)									\
	winx::AccelFrameHook::Leave((LPVOID)&(oldFrameArg))

// =========================================================================

template <class HookClass, int idHook>
typename SingleTheadHook<HookClass, idHook>::_HookData
	SingleTheadHook<HookClass, idHook>::m_data;

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
