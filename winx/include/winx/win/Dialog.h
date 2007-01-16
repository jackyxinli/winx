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
// Module: winx/win/Dialog.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 18:07:41
// 
// $Id: Dialog.h,v 1.13 2006/09/18 05:39:11 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_DIALOG_H__
#define __WINX_WIN_DIALOG_H__

#if defined(WINX_USE_DDX_FLOAT)
#define _ATL_USE_DDX_FLOAT
#endif

#ifndef __WINX_WIN_MESSAGE_H__
#include "Message.h"
#endif

#ifndef __ATLDDX_H__
#include "../../../../wtl/include/atlddx.h"
#endif

__WINX_BEGIN

// =========================================================================
// SetDlgMsgResult

#ifndef SetDlgMsgResult
#define SetDlgMsgResult(hwnd, msg, result) (( \
        (msg) == WM_CTLCOLORMSGBOX      || \
        (msg) == WM_CTLCOLOREDIT        || \
        (msg) == WM_CTLCOLORLISTBOX     || \
        (msg) == WM_CTLCOLORBTN         || \
        (msg) == WM_CTLCOLORDLG         || \
        (msg) == WM_CTLCOLORSCROLLBAR   || \
        (msg) == WM_CTLCOLORSTATIC      || \
        (msg) == WM_COMPAREITEM         || \
        (msg) == WM_VKEYTOITEM          || \
        (msg) == WM_CHARTOITEM          || \
        (msg) == WM_QUERYDRAGICON       || \
        (msg) == WM_INITDIALOG             \
    ) ? (BOOL)(result) : (:SetWindowLong((hwnd), DWL_MSGRESULT, (LPARAM)(LRESULT)(result)), TRUE))
#endif

// =========================================================================
// WINX DDX - DataExchange

template <class T>
class WinDataExchange : public WTL::CWinDataExchange<T>
{
};

#define WINX_DDX_BEGIN()						BEGIN_DDX_MAP(void)
#define WINX_DDX_TEXT(nID, var)					DDX_TEXT(nID, var)
#define WINX_DDX_TEXT_LEN(nID, var, len)		DDX_TEXT_LEN(nID, var, len)
#define WINX_DDX_INT(nID, var)					DDX_INT(nID, var)
#define WINX_DDX_INT_RANGE(nID, var, min, max)	DDX_INT_RANGE(nID, var, min, max)
#define WINX_DDX_UINT(nID, var)					DDX_UINT(nID, var)
#define WINX_DDX_UINT_RANGE(nID, var, min, max)	DDX_UINT_RANGE(nID, var, min, max)
#define WINX_DDX_CHECK(nID, var)				DDX_CHECK(nID, var)
#define WINX_DDX_RADIO(nID, var)				DDX_RADIO(nID, var)
#define WINX_DDX_END()							END_DDX_MAP()

#ifdef _ATL_USE_DDX_FLOAT
#define WINX_DDX_FLOAT(nID, var)								DDX_FLOAT(nID, var)
#define WINX_DDX_FLOAT_RANGE(nID, var, min, max)				DDX_FLOAT_RANGE(nID, var, min, max)
#define WINX_DDX_FLOAT_P(nID, var, precision)					DDX_FLOAT_P(nID, var, precision)
#define WINX_DDX_FLOAT_P_RANGE(nID, var, min, max, precision)	DDX_FLOAT_P_RANGE(nID, var, min, max, precision)
#endif

// =========================================================================
// WINX_DLG_ACCEL - 对话框支持Accelerator

#define WINX_DLG_ACCEL()													\
public:																		\
	VOID winx_msg OnEnterAccelFrame(HWND hWnd) {							\
		WINX_ENTERACCELFRAME(_winx_oldFrame, hWnd);							\
	}																		\
	VOID winx_msg OnLeaveAccelFrame(HWND hWnd) {							\
		WINX_LEAVEACCELFRAME(_winx_oldFrame);								\
	}																		\
private:																	\
	winx::ModalFrame _winx_oldFrame

// -------------------------------------------------------------------------
// WINX_DLG_FWDMSG - 对话框支持ForwardMessage

#define WINX_DLG_FWDMSG()	WINX_DLG_ACCEL()

// =========================================================================
// class DialogBase

template <class WindowClass, int nDlgId, class HandleClass>
class DialogBase : 
	public WindowMessage<WindowClass>,
	public HandleClass
{
	WINX_ON_INITIAL();
	WINX_ON_TERMINATE();
	WINX_NO_DEFAULT(); // 对话框没有Default()过程。

public:
	WINX_MSG_NULL_HANDLER_IMPL(OnOK)
	{
		WindowClass::CloseDialog((HWND)hWnd, IDOK);
		return 0;
	}

public:
	enum { IDD = nDlgId };

	typedef WindowClass WindowImplClass;
	typedef WindowMessage<WindowClass> BaseClass;

public:
	static LPCTSTR winx_call GetOrgDlgClassName()
	{
		return _T("#32770");
	}

	static LPCTSTR winx_call GetDlgClassName()
	{
		return GetOrgDlgClassName();
	}

	static WindowClass* winx_call GetInstance(HWND hWnd)
	{
		typedef typename WindowClass::WindowMap WindowMap;

		WINX_ASSERT(
			IsWndClassName(hWnd, WindowClass::GetOrgDlgClassName()) ||
			IsWndClassName(hWnd, WindowClass::GetDlgClassName())
			);
		return (WindowClass*)WindowMap::GetWindow(hWnd);
	}

	static WindowClass* winx_call GetDlgItemInstance(HWND hDlg, UINT uID)
	{
		return GetInstance(::GetDlgItem(hDlg, uID));
	}

public:
	// OnCommand

	BOOL winx_msg OnCommand(
		HWND hWnd, WPARAM wParam, HWND hWndCtlFrom)
	{
		UINT uID = LOWORD(wParam);
		WINX_MSG_HAS(OnOK)
		{
			if (uID == IDOK)
			{
				_WINX_PWND->OnOK(hWnd);
				return TRUE;
			}
		}
		if (uID >= IDOK && uID <= IDNO)
		{
			WindowClass::CloseDialog(hWnd, uID);
			return TRUE;
		}
		return FALSE;
	}

public:
	// ProcessMessage - DWL_MSGRESULT

	LRESULT winx_msg ProcessMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lResult = BaseClass::ProcessMessage(hWnd, message, wParam, lParam);
		::SetWindowLong(hWnd, DWL_MSGRESULT, lResult);
		return lResult;
	}

public:
	// Default
	
	BOOL winx_call Default()
	{
		return FALSE;
	}
	
	BOOL winx_call Default(HWND hWnd)
	{
		return FALSE;
	}

public:
	// DefaultHandle

	BOOL winx_msg DefaultHandle(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WINX_MSG_HAS3(OnInitial, OnInitDialog, OnPostInitDialog)
		{
			if (message == WM_INITDIALOG) // 先构造父类，然后构造子类。
			{
				WINX_MSG_HAS(OnInitial)
					_WINX_PWND->OnInitial(hWnd);
				WINX_MSG_HAS(OnPostInitDialog)
					::PostMessage(hWnd, WINX_WM_POSTINIT, 0, 0);
				WINX_MSG_HAS(OnInitDialog)
					return _WINX_PWND->OnInitDialog(hWnd, (HWND)wParam);
				return TRUE;
			}
		}
		WINX_MSG_HAS(OnPostInitDialog)
		{
			if (message == WINX_WM_POSTINIT)
			{
				_WINX_PWND->OnPostInitDialog(hWnd);
				return TRUE;
			}
		}
		return FALSE;
	}
};

// =========================================================================
// class ModalessDialog

template <class WindowClass, int nDlgId = 0, class HandleClass = DefaultWindowHandle>
class ModalessDialog : public DialogBase<WindowClass, nDlgId, HandleClass>
{
	WINX_STACK_OBJECT(FALSE);
public:
	typedef DialogBase<WindowClass, nDlgId, HandleClass> BaseClass;
	typedef ModalessDialog<WindowClass, nDlgId, HandleClass> WindowBase;
	typedef NormalWindowMap WindowMap;
	typedef WindowClass WindowImplClass;
	
	using BaseClass::IDD;
	using BaseClass::GetOrgDlgClassName;

public:
	VOID winx_call ProcessTermMessage(HWND hWnd)
	{
		WindowMap::DestroyWindow(hWnd);
		WINX_DELETE_THIS_PWND();
	}
	
public:
	static BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowClass* pWnd = (WindowClass*)WindowMap::GetWindow(hDlg);
		if (pWnd == NULL)
		{
			if (message != WM_INITDIALOG)
				return FALSE;
			if (lParam) {
				pWnd = (WindowClass*)lParam;
				lParam = 0;
			}
			else {
				WINX_ASSERT(!WindowClass::StackWindowObject);
				if (!WindowClass::StackWindowObject)
					pWnd = WINX_NEW(WindowClass);
			}
			WindowMap::SetWindow(hDlg, pWnd);
		}
		return pWnd->ProcessMessage(hDlg, message, wParam, lParam);
	}

private:
	static LRESULT CALLBACK _MockWndProc(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message != WM_CREATE)
			return ::DefWindowProc(hWnd, message, wParam, lParam);

		LPCREATESTRUCT lpCS = (LPCREATESTRUCT)lParam;
		HWND hDlg = WindowClass::CreateDialog(lpCS->hInstance, IDD, lpCS->hwndParent, 0);
		
		::SetWindowLong(hWnd, GWL_ID, 0);
		::PostMessage(hWnd, WM_CLOSE, 0, 0);
		if (hDlg) {
			::MoveWindow(hDlg, lpCS->x, lpCS->y, lpCS->cx, lpCS->cy, TRUE);	
			::SetWindowLong(hDlg, GWL_ID, (LONG)lpCS->hMenu);
			return 0;
		}
		return -1;
	}
	
public:
	static LPCTSTR GetWndClassName()
	{
		return GetOrgDlgClassName();
	}

	static ATOM winx_call RegisterClass(
		IN HINSTANCE hInst = GetThisModule())
	{
		WINX_ASSERT(
			GetOrgDlgClassName() != WindowClass::GetWndClassName()
			);

		WNDCLASSEX wcex =
		{
			sizeof(WNDCLASSEX),				// cbSize
			0,								// style
			_MockWndProc,					// lpfnWndProc
			0,								// cbClsExtra
			0,								// cbWndExtra
			hInst,							// hInstance
			NULL,							// hIcon
			NULL,							// hCursor
			NULL,							// hbrBackground
			NULL,							// lpszMenuName
			WindowClass::GetWndClassName(),	// lpszClassName
			NULL							// hIconSm
		};
		return ::RegisterClassEx(&wcex);
	}

public:
	static HWND winx_call CreateDialog(
		HINSTANCE hInst, RESID nResId, HWND hWndParent, LPARAM lParam)
	{
		return ::CreateDialogParam(
			hInst, (LPCTSTR)nResId, hWndParent, WindowClass::DialogProc, lParam);
	}

	static void winx_call CloseDialog(HWND hDlg, int nResult = IDOK)
	{
		::DestroyWindow(hDlg);
	}

	void winx_call CloseDialog(int nResult = IDOK)
	{
		::DestroyWindow(BaseClass::m_hWnd);
	}

	HWND winx_call Create(
		HWND hWndParent, RESID nResId = IDD, HINSTANCE hInst = GetThisModule())
	{
		return WindowClass::CreateDialog(hInst, nResId, hWndParent, (LPARAM)_WINX_PWND);
	}

	HWND winx_call Create(
		HWND hWndParent, int x, int y, int nWidth, int nHeight,
		RESID nResId = IDD, HINSTANCE hInst = GetThisModule())
	{
		HWND hDlg = WindowClass::CreateDialog(hInst, nResId, hWndParent, (LPARAM)_WINX_PWND);
		if (hDlg)
			::MoveWindow(hDlg, x, y, nWidth, nHeight, TRUE);
		return hDlg;
	}
};

// -------------------------------------------------------------------------
// class AxModalessDialog

template <class WindowClass, int nDlgId = 0, class HandleClass = DefaultWindowHandle>
class AxModalessDialog : public ModalessDialog<WindowClass, nDlgId, HandleClass>
{
	WINX_ON_DLGINIT_CREATE_AXCTRL();
	WINX_DLG_FWDMSG(); // 默认就进行ForwardMessage。
	
public:
	static HWND winx_call CreateDialog(
		HINSTANCE hInst, RESID nResId, HWND hWndParent, LPARAM lParam)
	{
		return AtlAxCreateDialog(
			hInst, (LPCTSTR)nResId, hWndParent, WindowClass::DialogProc, lParam);
	}
};

// =========================================================================
// class ModalDialog

template <class WindowClass, int nDlgId = 0, class HandleClass = DefaultWindowHandle>
class ModalDialog : public DialogBase<WindowClass, nDlgId, HandleClass>
{
public:
	typedef NormalWindowMap WindowMap;
	typedef DialogBase<WindowClass, nDlgId, HandleClass> BaseClass;
	typedef ModalDialog<WindowClass, nDlgId, HandleClass> WindowBase;
	typedef WindowClass WindowImplClass;

	using BaseClass::IDD;

public:
	VOID winx_call ProcessTermMessage(HWND hWnd)
	{
		WindowMap::DestroyWindow(hWnd);
	}

public:
	static BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowClass* pWnd = (WindowClass*)WindowMap::GetWindow(hDlg);
		if (pWnd == NULL)
		{
			if (message != WM_INITDIALOG)
				return FALSE;
			pWnd = (WindowClass*)lParam;
			WindowMap::SetWindow(hDlg, pWnd);
		}
		return pWnd->ProcessMessage(hDlg, message, wParam, lParam);
	}

public:
	static void winx_call CloseDialog(HWND hDlg, int nResult = IDOK)
	{
		::EndDialog(hDlg, nResult);
	}

	void winx_call CloseDialog(int nResult = IDOK)
	{
		::EndDialog(BaseClass::m_hWnd, nResult);
	}

	int winx_call DoModal(
		HWND hWndParent, RESID nResId = IDD, HINSTANCE hInst = GetThisModule())
	{
		return DialogBoxParam(
			hInst, (LPCTSTR)nResId,
			hWndParent, WindowClass::DialogProc, (LPARAM)_WINX_PWND);
	}

	int winx_call DoModal(
		RESID nResId = IDD, HINSTANCE hInst = GetThisModule())
	{
		return DialogBoxParam(
			hInst, (LPCTSTR)nResId,
			NULL, WindowClass::DialogProc, (LPARAM)_WINX_PWND);
	}
};

// -------------------------------------------------------------------------
// class AxModalDialog

template <class WindowClass, int nDlgId = 0, class HandleClass = DefaultWindowHandle>
class AxModalDialog : public ModalDialog<WindowClass, nDlgId, HandleClass>
{
	typedef ModalDialog<WindowClass, nDlgId, HandleClass> BaseClass;
	WINX_ON_DLGINIT_CREATE_AXCTRL();
	WINX_DLG_FWDMSG(); // 默认就进行ForwardMessage。

public:
	typedef WindowClass WindowImplClass;

	using BaseClass::IDD;

	int winx_call DoModal(
		HWND hWndParent, RESID nResId = IDD, HINSTANCE hInst = GetThisModule())
	{
		return AtlAxDialogBox(
			hInst, (LPCTSTR)nResId,
			hWndParent, WindowClass::DialogProc, (LPARAM)_WINX_PWND);
	}

	int winx_call DoModal(
		RESID nResId = IDD, HINSTANCE hInst = GetThisModule())
	{
		return AtlAxDialogBox(
			hInst, (LPCTSTR)nResId,
			NULL, WindowClass::DialogProc, (LPARAM)_WINX_PWND);
	}
};

// -------------------------------------------------------------------------
// class SimpleDialog - 简单对话框。
// class AxSimpleDialog - 支持ActiveX的简单对话框。

class SimpleDialog : public ModalDialog<SimpleDialog, 0, NullWindowHandle>
{
};

template <int nDlgId>
class AxSimpleDialog : public AxModalDialog<AxSimpleDialog<nDlgId>, nDlgId, NullWindowHandle>
{
};

// =========================================================================
// $Log: Dialog.h,v $
// Revision 1.13  2006/09/18 05:39:11  xushiwei
// WINX-Core: Message: OnIdle, OnSettingChange support
//
// Revision 1.11  2006/09/14 14:44:01  xushiwei
// Winx-Extension-ActiveX support bugfix (Visual Studio 2005):
//  Failed to create ActiveX controls in dialog resource template.
//  (Reason: AtlAxDialogBox/AtlAxCreateDialog implementation was changed in high version ATL.)
//
// Revision 1.10  2006/09/13 17:05:11  xushiwei
// WINX-Basic: DefaultWindowHandle(see macro WINX_NULL_WINDOW_HANDLE)
// WINX-Core: AxModalDialog/AxModalessDialog
//
// Revision 1.9  2006/09/13 13:54:36  xushiwei
// WINX-Core:
//  DDX(DataExchange) - class WinDataExchange, WINX_DDX_BEGIN, WINX_DDX_END, etc.
//
// Revision 1.8  2006/09/03 08:23:25  xushiwei
// WINX-Extension: Mfc-Together
//
// Revision 1.6  2006/09/03 06:32:13  xushiwei
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
// Revision 1.1  2006/08/19 10:50:55  xushiwei
// WINX-Core: Message, Window, Dialog, Control
//

__WINX_END

#endif /* __WINX_WIN_DIALOG_H__ */
