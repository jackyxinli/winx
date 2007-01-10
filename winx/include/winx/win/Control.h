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
// Module: winx/win/Control.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 18:33:44
// 
// $Id: Control.h,v 1.3 2006/12/11 05:19:08 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_CONTROL_H__
#define __WINX_WIN_CONTROL_H__

#ifndef __WINX_WIN_WINDOW_H__
#include "Window.h"
#endif

__WINX_BEGIN

// =========================================================================
// Handle Classes - Common Controls

class StaticHandle : public HandleT<WTL::CStatic>
{
	WINX_HANDLE_CLASS(StaticHandle);
};

class ButtonHandle : public HandleT<WTL::CButton>
{
	WINX_HANDLE_CLASS(ButtonHandle);
};

class EditHandle : public HandleT<WTL::CEdit>
{
	WINX_HANDLE_CLASS(EditHandle);
public:
	BOOL winx_call Redo()
		{ return (BOOL)::SendMessage(m_hWnd, EM_UNDO, 0, 0L); }
};

class ComboBoxHandle : public HandleT<WTL::CComboBox>
{
	WINX_HANDLE_CLASS(ComboBoxHandle);
};

class ScrollBarHandle : public HandleT<WTL::CScrollBar>
{
	WINX_HANDLE_CLASS(ScrollBarHandle);
};

class FlatScrollBarHandle : public HandleT<WTL::CFlatScrollBar>
{
	WINX_HANDLE_CLASS(FlatScrollBarHandle);
};

class ListBoxHandle : public HandleT<WTL::CListBox>
{
	WINX_HANDLE_CLASS(ListBoxHandle);
};

class DragListBoxHandle : public HandleT<WTL::CDragListBox>
{
	WINX_HANDLE_CLASS(DragListBoxHandle);
public:
	static UINT winx_call GetDragListMessage() {
		return winx::GetDragListMessage();
	}
};

// =========================================================================
// Handle Classes - Extended Common Controls

typedef WTL::CImageList ImageListHandle;
typedef WTL::CTreeItem TreeItem;
typedef WTL::CToolInfo ToolInfo;

class ListCtrlHandle : public HandleT<WTL::CListViewCtrl>
{
	WINX_HANDLE_CLASS(ListCtrlHandle);
};

class TreeCtrlHandle : public HandleT<WTL::CTreeViewCtrl>
{
	WINX_HANDLE_CLASS(TreeCtrlHandle);
};

class TreeCtrlHandleEx : public HandleT<WTL::CTreeViewCtrlEx>
{
	WINX_HANDLE_CLASS(TreeCtrlHandleEx);
};

class HeaderCtrlHandle : public HandleT<WTL::CHeaderCtrl>
{
	WINX_HANDLE_CLASS(HeaderCtrlHandle);
};

class ToolBarCtrlHandle : public HandleT<WTL::CToolBarCtrl>
{
	WINX_HANDLE_CLASS(ToolBarCtrlHandle);
};

class TabCtrlHandle : public HandleT<WTL::CTabCtrl>
{
	WINX_HANDLE_CLASS(TabCtrlHandle);
};

class ToolTipCtrlHandle : public HandleT<WTL::CToolTipCtrl>
{
	WINX_HANDLE_CLASS(ToolTipCtrlHandle);
};

class StatusBarCtrlHandle : public HandleT<WTL::CStatusBarCtrl>
{
	WINX_HANDLE_CLASS(StatusBarCtrlHandle);
};

class SliderCtrlHandle : public HandleT<WTL::CTrackBarCtrl>
{
	WINX_HANDLE_CLASS(SliderCtrlHandle);
};

class SpinButtonCtrlHandle : public HandleT<WTL::CUpDownCtrl>
{
	WINX_HANDLE_CLASS(SpinButtonCtrlHandle);
};

class ProgressCtrlHandle : public HandleT<WTL::CProgressBarCtrl>
{
	WINX_HANDLE_CLASS(ProgressCtrlHandle);
};

class HotKeyCtrlHandle : public HandleT<WTL::CHotKeyCtrl>
{
	WINX_HANDLE_CLASS(HotKeyCtrlHandle);
};

class AnimateCtrlHandle : public HandleT<WTL::CAnimateCtrl>
{
	WINX_HANDLE_CLASS(AnimateCtrlHandle);
};

class RichEditHandle : public HandleT<WTL::CRichEditCtrl>
{
	WINX_HANDLE_CLASS(RichEditHandle);

private:
	static DWORD CALLBACK _ExportStreamProc(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
	{
		IStream* pstm = (IStream*)ULongToPtr(dwCookie);
		return pstm->Write(pbBuff, cb, (ULONG*)pcb);
	}

	static DWORD CALLBACK _ExportFileProc(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
	{
		FILE* pstm = (FILE*)ULongToPtr(dwCookie);
		*pcb = (LONG)fwrite(pbBuff, 1, cb, pstm);
		return 0;
	}

public:
	LONG winx_call ExportRtf(IN IStream* pstm)
	{
		EDITSTREAM es = { (DWORD)PtrToUlong(pstm), 0, _ExportStreamProc };
		return StreamOut(SF_RTF, es);
	}

	LONG winx_call ExportRtf(IN FILE* pstm)
	{
		EDITSTREAM es = { (DWORD)PtrToUlong(pstm), 0, _ExportFileProc };
		return StreamOut(SF_RTF, es);
	}

	LONG winx_call ExportRtfFile(IN LPCWSTR szRtfFile)
	{
		USES_CONVERSION;
		return ExportRtfFile(W2CA(szRtfFile));
	}

	LONG winx_call ExportRtfFile(IN LPCSTR szRtfFile)
	{
		FILE* fp = fopen(szRtfFile, "w");
		if (fp != NULL)
		{
			EDITSTREAM es = { (DWORD)PtrToUlong(fp), 0, _ExportFileProc };
			LONG lCount = StreamOut(SF_RTF, es);
			fclose(fp);
			return lCount;
		}
		return 0;
	}

private:
	typedef const BYTE* LPCBYTE;

	struct _ImportDataCookie {
		LPCBYTE pData;
		LPCBYTE pEnd;
	};

	static DWORD CALLBACK _ImportDataProc(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
	{
		_ImportDataCookie& cookie = *(_ImportDataCookie*)ULongToPtr(dwCookie);
		if (cb > cookie.pEnd - cookie.pData)
			cb = (LONG)(cookie.pEnd - cookie.pData);
		CopyMemory(pbBuff, cookie.pData, cb);
		cookie.pData += cb;
		*pcb = cb;
		return 0;
	}

	template <class Archive>
	struct _Import
	{
		static DWORD CALLBACK Proc(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
		{
			Archive& ar = *(Archive*)dwCookie;
			*pcb = ar.get((char*)pbBuff, cb);
			return 0;
		}
	};

public:
	template <class Archive>
	UINT winx_call ImportData(Archive& ar, UINT uFormat)
	{
		EDITSTREAM es = {
			(DWORD)&ar, 0, _Import<Archive>::Proc
		};
		return StreamIn(uFormat, es);
	}

	UINT winx_call ImportData(LPCVOID pData, UINT cbData, UINT uFormat)
	{
		_ImportDataCookie cookie = {
			(LPCBYTE)pData,
			(LPCBYTE)pData + cbData
		};
		EDITSTREAM es = { 
			(DWORD)PtrToUlong(&cookie), 0, _ImportDataProc
		};
		return StreamIn(uFormat, es);
	}

	VOID winx_call ImportText(
		LPCWSTR szText, INT cchText = -1, UINT uFlags = SFF_SELECTION)
	{
		if (cchText < 0)
			cchText = (INT)wcslen(szText);
		ImportData(szText, cchText*sizeof(WCHAR), SF_TEXT|SF_UNICODE|uFlags);
	}

	VOID winx_call ImportText(
		LPCSTR szText, INT cchText = -1, UINT uFlags = SFF_SELECTION)
	{
		if (cchText < 0)
			cchText = (INT)strlen(szText);
		ImportData(szText, cchText, SF_TEXT|uFlags);
	}

	VOID winx_call ImportRtfText(
		LPCSTR szRtfText, INT cchText = -1, UINT uFlags = SFF_SELECTION)
	{
		if (cchText < 0)
			cchText = (INT)strlen(szRtfText);
		ImportData(szRtfText, cchText, SF_RTF|uFlags);
	}
};

class ReBarCtrlHandle : public HandleT<WTL::CReBarCtrl>
{
	WINX_HANDLE_CLASS(ReBarCtrlHandle);
};

class ComboBoxExHandle : public HandleT<WTL::CComboBoxEx>
{
	WINX_HANDLE_CLASS(ComboBoxExHandle);
};

class DateTimeCtrlHandle : public HandleT<WTL::CDateTimePickerCtrl>
{
	WINX_HANDLE_CLASS(DateTimeCtrlHandle);
};

class MonthCalCtrlHandle : public HandleT<WTL::CMonthCalendarCtrl>
{
	WINX_HANDLE_CLASS(MonthCalCtrlHandle);
};

class IPAddressCtrlHandle : public HandleT<WTL::CIPAddressCtrl>
{
	WINX_HANDLE_CLASS(IPAddressCtrlHandle);
};

class PagerCtrlHandle : public HandleT<WTL::CPagerCtrl>
{
	WINX_HANDLE_CLASS(PagerCtrlHandle);
};

class AxCtrlHandle : public HandleT<::ATL::CAxWindow> // ActiveX Control
{
	WINX_HANDLE_CLASS(AxCtrlHandle);
};

typedef AxCtrlHandle AxHandle;
typedef ListCtrlHandle ListViewCtrlHandle;
typedef TreeCtrlHandle TreeViewCtrlHandle;
typedef SliderCtrlHandle TrackBarCtrlHandle;
typedef SpinButtonCtrlHandle UpDownCtrlHandle;
typedef ProgressCtrlHandle ProgressBarCtrlHandle;
typedef DateTimeCtrlHandle DateTimePickerCtrlHandle;
typedef MonthCalCtrlHandle MonthCalendarCtrlHandle;

// =========================================================================
// HandleClass - LinkCtrlHandle, Windows CE common controls

#if (_WIN32_WINNT >= 0x0501) && !defined(_WIN32_WCE)

class LinkCtrlHandle : public HandleT<WTL::CLinkCtrl>
{
	WINX_HANDLE_CLASS(LinkCtrlHandle);
};

#endif // (_WIN32_WINNT >= 0x0501)

#ifdef _WIN32_WCE

class CECommandBarCtrlHandle : public HandleT<WTL::CCECommandBarCtrl>
{
	WINX_HANDLE_CLASS(CECommandBarCtrlHandle);
};

class CECommandBandsCtrlHandle : public HandleT<WTL::CCECommandBandsCtrl>
{
	WINX_HANDLE_CLASS(CECommandBandsCtrlHandle);
};

#endif

// =========================================================================
// WINX_REFLECT_CMDS

#define WINX_REFLECT_CMDS_BEGIN()											\
public:																		\
	BOOL winx_msg OnReflectCommand(HWND hWnd, WPARAM wParam)				\
	{

#define WINX_REFLECT_CMD(nCode, Function)									\
		if (HIWORD(wParam) == (nCode))	{									\
			Function(hWnd); return TRUE;									\
		}

#define WINX_REFLECT_CMDS_END()												\
		return BaseClass::OnReflectCommand(hWnd, wParam);					\
	}

// =========================================================================
// class CommCtrlT

template <class BaseWindowClass>
class CommCtrlT : public BaseWindowClass
{
public:
	typedef BaseWindowClass BaseClass;
	typedef CommCtrlT<BaseWindowClass> WindowBase;

	typedef typename BaseWindowClass::WindowImplClass WindowImplClass;
	typedef WindowImplClass WindowClass;

public:
	WINX_MSG_SIMPLE_HANDLER(DrawItem, 0);
	WINX_MSG_SIMPLE_HANDLER(MeasureItem, 0);
	WINX_MSG_SIMPLE_HANDLER(OnReflectCommand, FALSE);

public:
	LRESULT winx_msg DefaultHandle(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WINX_MSG_CASE(WINX_WM_REFLECT_DRAWITEM, DrawItem)
		{
			_WINX_PWND->DrawItem(hWnd, (LPDRAWITEMSTRUCT)lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WINX_WM_REFLECT_MEASUREITEM, MeasureItem)
		{
			_WINX_PWND->MeasureItem(hWnd, (LPMEASUREITEMSTRUCT)lParam);
			return TRUE;
		}
		WINX_MSG_CASE(WINX_WM_REFLECT_COMMAND, OnReflectCommand)
		{
			return _WINX_PWND->OnReflectCommand(hWnd, wParam);
		}
		return BaseWindowClass::DefaultHandle(hWnd, message, wParam, lParam);
	}
};

// =========================================================================
// class ListBox

template <class BaseWindowClass>
class ListBoxT : public CommCtrlT<BaseWindowClass>
{
};

template <
	class WindowClass,
	class HandleClass = ListBoxHandle,
	int nInst = cateSubclassWindow>
class ListBox : public CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class Edit

template <class BaseWindowClass>
class EditT : public CommCtrlT<BaseWindowClass>
{
};

template <
	class WindowClass,
	class HandleClass = EditHandle,
	int nInst = cateSubclassWindow>
class Edit : public CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class Button

template <class BaseWindowClass>
class ButtonT : public CommCtrlT<BaseWindowClass>
{
};

template <
	class WindowClass,
	class HandleClass = ButtonHandle,
	int nInst = cateSubclassWindow>
class Button : public CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class Static

template <class BaseWindowClass>
class StaticT : public CommCtrlT<BaseWindowClass>
{
};

template <
	class WindowClass,
	class HandleClass = StaticHandle,
	int nInst = cateSubclassWindow>
class Static : public CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef Static CStatic;
	typedef CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class ComboBox

template <
	class WindowClass,
	class HandleClass = ComboBoxHandle,
	int nInst = cateSubclassWindow>
class ComboBox : public CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class ScrollBar

template <
	class WindowClass,
	class HandleClass = ScrollBarHandle,
	int nInst = cateSubclassWindow>
class ScrollBar : public CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class FlatScrollBar - todo

template <
	class WindowClass,
	class HandleClass = FlatScrollBarHandle,
	int nInst = cateSubclassWindow>
class FlatScrollBar : public CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef CommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// WINX_REFLECT_NOTIFY

#define WINX_REFLECT_NOTIFY_BEGIN()											\
public:																		\
	BOOL winx_msg OnReflectNotify(											\
		HWND hWnd, LPNMHDR pnmh, LRESULT* pResult)							\
	{																		\
		UINT message = pnmh->code;

#define WINX_REFLECT_NOTIFY(nCode, Function)								\
		if (message == nCode) {												\
			Function(pnmh, pResult); return TRUE;							\
		}

#define WINX_REFLECT_NOTIFY_END()											\
		return BaseClass::OnReflectNotify(hWnd, pnmh, pResult);				\
	}

// =========================================================================
// class ExCommCtrlT

template <class BaseWindowClass>
class ExCommCtrlT : public BaseWindowClass
{
public:
	typedef BaseWindowClass BaseClass;
	typedef ExCommCtrlT<BaseWindowClass> WindowBase;
	
	typedef typename BaseWindowClass::WindowImplClass WindowImplClass;
	typedef WindowImplClass WindowClass;

public:
	WINX_MSG_SIMPLE_HANDLER(OnReflectNotify, FALSE);

public:
	LRESULT winx_msg DefaultHandle(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WINX_MSG_CASE(WINX_WM_REFLECT_NOTIFY, OnReflectNotify)
		{
			LRESULT nNotifyResult = 0;
			if (_WINX_PWND->OnReflectNotify(hWnd, (LPNMHDR)lParam, &nNotifyResult))
				return nNotifyResult;
			goto DefaultProcess;
		}
DefaultProcess:
		return BaseWindowClass::DefaultHandle(hWnd, message, wParam, lParam);
	}
};

// =========================================================================
// class ListCtrl

template <class BaseWindowClass>
class ListCtrlT : public ExCommCtrlT<BaseWindowClass>
{
};

template <
	class WindowClass,
	class HandleClass = ListCtrlHandle,
	int nInst = cateSubclassWindow>
class ListCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class HeaderCtrl

template <
	class WindowClass,
	class HandleClass = HeaderCtrlHandle,
	int nInst = cateSubclassWindow>
class HeaderCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class ToolBarCtrl

template <
	class WindowClass,
	class HandleClass = ToolBarCtrlHandle,
	int nInst = cateSubclassWindow>
class ToolBarCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class ReBarCtrl

template <
	class WindowClass,
	class HandleClass = ReBarCtrlHandle,
	int nInst = cateSubclassWindow>
class ReBarCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class TabCtrl

template <
	class WindowClass,
	class HandleClass = TabCtrlHandle,
	int nInst = cateSubclassWindow>
class TabCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class ToolTipCtrl

template <
	class WindowClass,
	class HandleClass = ToolTipCtrlHandle,
	int nInst = cateSubclassWindow>
class ToolTipCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class StatusBarCtrl

template <
	class WindowClass,
	class HandleClass = StatusBarCtrlHandle,
	int nInst = cateSubclassWindow>
class StatusBarCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class SliderCtrl/TrackBarCtrl - 例如：用于音量调整

template <
	class WindowClass,
	class HandleClass = SliderCtrlHandle,
	int nInst = cateSubclassWindow>
class SliderCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

template <
	class WindowClass,
	class HandleClass = TrackBarCtrlHandle,
	int nInst = cateSubclassWindow>
class TrackBarCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class ProgressCtrl - 进度条

template <
	class WindowClass,
	class HandleClass = ProgressCtrlHandle,
	int nInst = cateSubclassWindow>
class ProgressCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class SpinButtonCtrl/UpDownCtrl

template <
	class WindowClass,
	class HandleClass = SpinButtonCtrlHandle,
	int nInst = cateSubclassWindow>
class SpinButtonCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

template <
	class WindowClass,
	class HandleClass = UpDownCtrlHandle,
	int nInst = cateSubclassWindow>
class UpDownCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class TreeCtrl

template <class BaseWindowClass>
class TreeCtrlT : public ExCommCtrlT<BaseWindowClass>
{
};

template <
	class WindowClass,
	class HandleClass = TreeCtrlHandle,
	int nInst = cateSubclassWindow>
class TreeCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class HotKeyCtrl

template <
	class WindowClass,
	class HandleClass = HotKeyCtrlHandle,
	int nInst = cateSubclassWindow>
class HotKeyCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class AnimateCtrl

template <
	class WindowClass,
	class HandleClass = AnimateCtrlHandle,
	int nInst = cateSubclassWindow>
class AnimateCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class ComboBoxEx

template <
	class WindowClass,
	class HandleClass = ComboBoxExHandle,
	int nInst = cateSubclassWindow>
class ComboBoxEx : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class DateTimeCtrl

template <
	class WindowClass,
	class HandleClass = DateTimeCtrlHandle,
	int nInst = cateSubclassWindow>
class DateTimeCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class MonthCalCtrl

template <
	class WindowClass,
	class HandleClass = MonthCalCtrlHandle,
	int nInst = cateSubclassWindow>
class MonthCalCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class IPAddressCtrl

template <
	class WindowClass,
	class HandleClass = IPAddressCtrlHandle,
	int nInst = cateSubclassWindow>
class IPAddressCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class PagerCtrl

template <
	class WindowClass,
	class HandleClass = PagerCtrlHandle,
	int nInst = cateSubclassWindow>
class PagerCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class RichEdit

template <
	class WindowClass,
	class HandleClass = RichEditHandle,
	int nInst = cateSubclassWindow>
class RichEdit : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

// =========================================================================
// class LinkCtrl

#if (_WIN32_WINNT >= 0x0501) && !defined(_WIN32_WCE)

template <
	class WindowClass,
	class HandleClass = LinkCtrlHandle,
	int nInst = cateSubclassWindow>
class LinkCtrl : public ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> >
{
public:
	typedef ExCommCtrlT<SubclassWindow<WindowClass, HandleClass, nInst> > BaseClass;
};

#endif

// -------------------------------------------------------------------------
// class SafeLinkCtrl/AltLinkCtrl

class AltLinkCtrl : public LightSuperclassWindow<AltLinkCtrl, StaticHandle>
{
	WINX_SAFECLASS("WinxLink");
	WINX_CLASS("WinxAltLink");
public:
	LRESULT winx_msg SuperOnCreate(HWND hWnd, LPCREATESTRUCT lpCS)
	{
		CString str(lpCS->lpszName, (int)_tcslen(lpCS->lpszName));
		int nPos = str.Find(WINX_TEXT("<a"));
		if (nPos >= 0)
		{
			int nPos2 = str.Find('>', nPos+2);
			if (nPos2 > 0)
			{
				int nPos3 = str.Find(WINX_TEXT("</a>"), nPos2+1);
				if (nPos3 > 0)
				{
					str.Delete(nPos3, 4);
					str.Delete(nPos, nPos2 - nPos + 1);
					::SetWindowText(hWnd, str);
				}
			}
		}
		return 0;
	}
};

#if (_WIN32_WINNT >= 0x0501) && !defined(_WIN32_WCE)

//--> CLASS: "WinxLink"
typedef SafeCtrl<LinkCtrlHandle, AltLinkCtrl> SafeLinkCtrl;

#endif

// =========================================================================
// $Log: Control.h,v $
// Revision 1.3  2006/12/11 05:19:08  xushiwei
// vs2005 __w64 support
//
// Revision 1.2  2006/12/03 08:40:22  xushiwei
// Unify style of map macro, such as:
//  WINX_TEST_SUITE/WINX_TEST/WINX_TEST_SUITE_END;
//  WINX_CMDS_BEGIN/WINX_CMD/WINX_CMDS_END;
//  WINX_SYSCMD_BEGIN/WINX_SYSCMD/WINX_SYSCMD_END;
//  WINX_NOTIFY_BEGIN/WINX_NOTIFY/WINX_NOTIFY_END;
//  WINX_REFLECT_NOTIFY_BEGIN/WINX_REFLECT_NOTIFY/WINX_REFLECT_NOTIFY_END;
//
// Revision 1.1  2006/08/19 10:50:55  xushiwei
// WINX-Core: Message, Window, Dialog, Control
//

__WINX_END

#endif /* __WINX_WIN_CONTROL_H__ */
