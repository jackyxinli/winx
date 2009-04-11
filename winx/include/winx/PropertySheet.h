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
// Module: winx/PropertySheet.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-2 21:26:41
// 
// $Id: PropertySheet.h,v 1.3 2006/09/13 17:05:11 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_WIN_PROPERTYSHEET_H
#define WINX_WIN_PROPERTYSHEET_H

#ifndef WINX_WIN_DIALOG_H
#include "win/Dialog.h"
#endif

// -------------------------------------------------------------------------
// Reference Win32 SDK

#if defined(WINX_USE_WINSDK)

#ifndef _INC_COMMDLG
#include "../../../winsdk/include/commdlg.h"
#endif

#ifndef _SHLOBJ_H_
#include "../../../winsdk/include/shlobj.h"
#endif

#else // defined(WINX_USE_DEFSDK)

#ifndef _INC_COMMDLG
#include <commdlg.h>
#endif

#ifndef _SHLOBJ_H_
#include <shlobj.h>
#endif

#endif

// -------------------------------------------------------------------------
// Reference WTL

#ifndef __ATLDLGS_H__
#include "../../../wtl/include/atldlgs.h"
#endif

NS_WINX_BEGIN

// =========================================================================
// class PropertySheetHandle

class PropertySheetHandle : public HandleT<WTL::CPropertySheetWindow>
{
	WINX_HANDLE_CLASS(PropertySheetHandle);
};

// -------------------------------------------------------------------------
// class PropertyPageHandle

class PropertyPageHandle : public HandleT<WTL::CPropertyPageWindow>
{
	WINX_HANDLE_CLASS(PropertyPageHandle);
};

// =========================================================================
// class StaticPageContainer

template <UINT nPages>
class StaticPageContainer
{
private:
	HPROPSHEETPAGE m_hPages[nPages];
	UINT m_nPages;

public:
	StaticPageContainer() : m_nPages(0) {}

	BOOL winx_call AddPage(HPROPSHEETPAGE hPage)
	{
		WINX_ASSERT(m_nPages < nPages);
		if (m_nPages < nPages)
		{
			m_hPages[m_nPages++] = hPage;
			return TRUE;
		}
		return FALSE;
	}

	BOOL winx_call AddPage(LPCPROPSHEETPAGE pPage)
	{
		WINX_ASSERT(pPage != NULL);
		HPROPSHEETPAGE hPage = ::CreatePropertySheetPage(pPage);
		if (hPage == NULL)
			return FALSE;
		return AddPage(hPage);
	}

	VOID winx_call ClearPages(BOOL fDestroy)
	{
		if (fDestroy)
		{
			for (UINT i = 0; i < m_nPages; ++i)
				::DestroyPropertySheetPage(m_hPages[i]);
		}
		m_nPages = 0;
	}

	HPROPSHEETPAGE* winx_call GetPages(UINT* pnPages)
	{
		*pnPages = m_nPages;
		return m_hPages;
	}

	BOOL winx_call IsEmpty() const
	{
		return m_nPages == 0;
	}
};

// -------------------------------------------------------------------------
// class DynamicPageContainer

class DynamicPageContainer
{
private:
	std::TypedPtrArray<HPROPSHEETPAGE> m_pages;
	
public:
	BOOL winx_call AddPage(HPROPSHEETPAGE hPage)
	{
		m_pages.push_back(hPage);
		return TRUE;
	}
	
	BOOL winx_call AddPage(LPCPROPSHEETPAGE pPage)
	{
		WINX_ASSERT(pPage != NULL);
		HPROPSHEETPAGE hPage = ::CreatePropertySheetPage(pPage);
		if (hPage == NULL)
			return FALSE;
		return AddPage(hPage);
	}
	
	VOID winx_call ClearPages(BOOL fDestroy)
	{
		if (fDestroy)
		{
			UINT nPages = m_pages.size();
			for (UINT i = 0; i < nPages; ++i)
				::DestroyPropertySheetPage(m_pages.at(i));
		}
		m_pages.clear();
	}

	HPROPSHEETPAGE* winx_call GetPages(UINT* pnPages)
	{
		*pnPages = m_pages.size();
		return &m_pages.at(0);
	}

	BOOL winx_call IsEmpty() const
	{
		return m_pages.empty();
	}
};

// =========================================================================
// class PropertySheetT

template <UINT nPages = 0, class PageContainer = StaticPageContainer<nPages> >
class PropertySheetT : public PageContainer
{
public:
	PROPSHEETHEADER m_psh;

public:
	PropertySheetT(
		RESID title = (LPCTSTR)NULL, UINT uStartPage = 0,
		HINSTANCE hInst = GetThisModule())
	{
		ZeroMemory(&m_psh, sizeof(PROPSHEETHEADER));
		m_psh.dwSize = sizeof(PROPSHEETHEADER);
		m_psh.hInstance = hInst;
		m_psh.pszCaption = title.m_lpstr;
		m_psh.nStartPage = uStartPage;
	}

#if defined(_DEBUG)
	~PropertySheetT()
	{
		WINX_ASSERT(IsEmpty());
	}
#endif

	int winx_call DoModal(HWND hWndParent = NULL)
	{
		m_psh.hwndParent = hWndParent;
		m_psh.phpage = GetPages(&m_psh.nPages);
		int result = ::PropertySheet(&m_psh);
		ClearPages(FALSE);
		return result;
	}
};

typedef PropertySheetT<0, DynamicPageContainer> DynamicPropertySheet;

// =========================================================================
// class PropertyPageInit

class PropertyPageInit : public PROPSHEETPAGE
{
public:
	void SetTitle(RESID title)
	{
		pszTitle = title.m_lpstr;
		dwFlags |= PSP_USETITLE;
	}
	
#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	void SetHeaderTitle(LPCTSTR lpstrHeaderTitle)
	{
		dwFlags |= PSP_USEHEADERTITLE;
		pszHeaderTitle = lpstrHeaderTitle;
	}
	
	void SetHeaderSubTitle(LPCTSTR lpstrHeaderSubTitle)
	{
		dwFlags |= PSP_USEHEADERSUBTITLE;
		pszHeaderSubTitle = lpstrHeaderSubTitle;
	}
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	
	void EnableHelp()
	{
		dwFlags |= PSP_HASHELP;
	}
};

// -------------------------------------------------------------------------
// class PropertySheetNotify

template <class WindowImplClass>
class PropertySheetNotify
{
public:
	BOOL winx_msg ProcessNotifyMessage(HWND hWnd, int idCtrl, LPNMHDR pnmh, LRESULT* pResult)
	{
		const UINT message = pnmh->code;

		WINX_MSG_CASE(PSN_SETACTIVE, PSN_OnSetActive)
		{
			*pResult = _WINX_PWND->PSN_OnSetActive(hWnd);
			return TRUE;
		}
		WINX_MSG_CASE(PSN_KILLACTIVE, PSN_OnKillActive)
		{
			*pResult = _WINX_PWND->PSN_OnKillActive(hWnd);
			return TRUE;
		}
		WINX_MSG_CASE(PSN_APPLY, PSN_OnApply)
		{
			*pResult = _WINX_PWND->PSN_OnApply(hWnd);
			return TRUE;
		}
		WINX_MSG_CASE(PSN_RESET, PSN_OnReset)
		{
			_WINX_PWND->PSN_OnReset(hWnd);
			return TRUE;
		}
		WINX_MSG_CASE(PSN_QUERYCANCEL, PSN_OnQueryCancel)
		{
			*pResult = _WINX_PWND->PSN_OnQueryCancel(hWnd);
			return TRUE;
		}
		WINX_MSG_CASE(PSN_WIZNEXT, PSN_OnWizardNext)
		{
			*pResult = _WINX_PWND->PSN_OnWizardNext(hWnd);
			return TRUE;
		}
		WINX_MSG_CASE(PSN_WIZBACK, PSN_OnWizardBack)
		{
			*pResult = _WINX_PWND->PSN_OnWizardBack(hWnd);
			return TRUE;
		}
		WINX_MSG_CASE(PSN_WIZFINISH, PSN_OnWizardFinish)
		{
			*pResult = _WINX_PWND->PSN_OnWizardFinish(hWnd);
			return TRUE;
		}
		WINX_MSG_CASE(PSN_HELP, PSN_OnHelp)
		{
			_WINX_PWND->PSN_OnHelp(hWnd);
			return TRUE;
		}

#ifndef _WIN32_WCE
#if (_WIN32_IE >= 0x0400)
		WINX_MSG_CASE(PSN_GETOBJECT, PSN_OnGetObject)
		{
			return _WINX_PWND->PSN_OnGetObject(hWnd, (LPNMOBJECTNOTIFY)pnmh);
		}
#endif // (_WIN32_IE >= 0x0400)
#if (_WIN32_IE >= 0x0500)
		WINX_MSG_CASE(PSN_TRANSLATEACCELERATOR, PSN_OnTranslateAccelerator)
		{
			LPPSHNOTIFY lpPSHNotify = (LPPSHNOTIFY)pnmh;
			*pResult = _WINX_PWND->PSN_OnTranslateAccelerator(hWnd, (LPMSG)lpPSHNotify->lParam);
			return TRUE;
		}
		WINX_MSG_CASE(PSN_QUERYINITIALFOCUS, PSN_OnQueryInitialFocus)
		{
			LPPSHNOTIFY lpPSHNotify = (LPPSHNOTIFY)pnmh;
			*pResult = (LRESULT)_WINX_PWND->PSN_OnQueryInitialFocus(hWnd, (HWND)lpPSHNotify->lParam);
			return TRUE;
		}
#endif // (_WIN32_IE >= 0x0500)
#endif // !_WIN32_WCE

		return FALSE;
	}

public:
	WINX_MSG_NULL_HANDLER(PSN_OnSetActive);
	WINX_MSG_NULL_HANDLER(PSN_OnKillActive);
	WINX_MSG_NULL_HANDLER(PSN_OnApply);
	WINX_MSG_NULL_HANDLER(PSN_OnReset);
	WINX_MSG_NULL_HANDLER(PSN_OnQueryCancel);
	WINX_MSG_NULL_HANDLER(PSN_OnWizardBack);
	WINX_MSG_NULL_HANDLER(PSN_OnWizardNext);
	WINX_MSG_NULL_HANDLER(PSN_OnWizardFinish);
	WINX_MSG_NULL_HANDLER(PSN_OnHelp);
	WINX_MSG_NULL_HANDLER(PSN_OnGetObject);
	WINX_MSG_NULL_HANDLER(PSN_OnTranslateAccelerator);
	WINX_MSG_NULL_HANDLER(PSN_OnQueryInitialFocus);

#if (0)
	int winx_msg PSN_OnSetActive(HWND hWnd)
	{
		// 0 = allow activate
		// -1 = go back that was active
		// page ID = jump to page
		return 0;
	}

	BOOL winx_msg PSN_OnKillActive(HWND hWnd)
	{
		// FALSE = allow deactivate
		// TRUE = prevent deactivation
		return FALSE;
	}

	int winx_msg PSN_OnApply(HWND hWnd)
	{
		// PSNRET_NOERROR = apply OK
		// PSNRET_INVALID = apply not OK, return to this page
		// PSNRET_INVALID_NOCHANGEPAGE = apply not OK, don't change focus
		return PSNRET_NOERROR;
	}

	void winx_msg PSN_OnReset(HWND hWnd)
	{
	}

	BOOL winx_msg PSN_OnQueryCancel(HWND hWnd)
	{
		// FALSE = allow cancel
		// TRUE = prevent cancel
		return FALSE;
	}

	int winx_msg PSN_OnWizardBack(HWND hWnd)
	{
		// 0  = goto previous page
		// -1 = prevent page change
		// >0 = jump to page by dlg ID
		return 0;
	}

	int winx_msg PSN_OnWizardNext(HWND hWnd)
	{
		// 0  = goto next page
		// -1 = prevent page change
		// >0 = jump to page by dlg ID
		return 0;
	}

	INT_PTR winx_msg PSN_OnWizardFinish(HWND hWnd)
	{
		// FALSE = allow finish
		// TRUE = prevent finish
		// HWND = prevent finish and set focus to HWND (CommCtrl 5.80 only)
		return FALSE;
	}

	void winx_msg PSN_OnHelp(HWND hWnd)
	{
	}

	BOOL winx_msg PSN_OnGetObject(HWND hWnd, LPNMOBJECTNOTIFY lpObjectNotify)
	{
		return FALSE;   // not processed
	}

	int winx_msg PSN_OnTranslateAccelerator(HWND hWnd, LPMSG lpMsg)
	{
		// PSNRET_NOERROR - message not handled
		// PSNRET_MESSAGEHANDLED - message handled
		return PSNRET_NOERROR;
	}

	HWND winx_msg PSN_OnQueryInitialFocus(HWND hWnd, HWND hWndFocus)
	{
		// NULL = set focus to default control
		// HWND = set focus to HWND
		return NULL;
	}
#endif
};

// -------------------------------------------------------------------------
// class PropertyPage

template <class WindowClass, int nDlgId = 0, class HandleClass = DefaultWindowHandle>
class PropertyPage : 
	public ModelessDialog<WindowClass, nDlgId, HandleClass>,
	public PropertySheetNotify<WindowClass>
{
public:
	static BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowClass* pWnd = (WindowClass*)WindowMap::GetWindow(hDlg);
		if (pWnd == NULL)
		{
			if (message != WM_INITDIALOG)
				return FALSE;
			if (lParam) {
				LPPROPSHEETPAGE ppsp = (LPPROPSHEETPAGE)lParam;
				pWnd = (WindowClass*)ppsp->lParam;
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

	static UINT CALLBACK PropertyPageProc(HWND hWnd, UINT uMsg, LPPROPSHEETPAGE ppsp)
	{
		WindowClass* pWnd = (WindowClass*)ppsp->lParam;
		switch(uMsg)
		{
		case PSPCB_CREATE:
			return pWnd->OnPageCreate(hWnd);
#if (_WIN32_IE >= 0x0500)
		case PSPCB_ADDREF:
			pWnd->OnPageAddRef(hWnd);
			break;
#endif // (_WIN32_IE >= 0x0500)
		case PSPCB_RELEASE:
			pWnd->OnPageRelease(hWnd);
			break;
		}
		return 0;
	}

	BOOL winx_msg OnNotify(HWND hWnd, int idCtrl, LPNMHDR pnmh, LRESULT* pResult)
	{
		return ProcessNotifyMessage(hWnd, idCtrl, pnmh, pResult);
	}

public:
	WINX_MSG_NULL_HANDLER(OnPagePreCreate);
	WINX_MSG_SIMPLE_HANDLER(OnPageCreate, TRUE);
	WINX_MSG_SIMPLE_HANDLER(OnPageAddRef, 0);
	WINX_MSG_SIMPLE_HANDLER(OnPageRelease, 0);
	
#if (0)
	BOOL winx_msg OnPagePreCreate(PropertyPageInit* pPsp, LPVOID pInitParam)
	{
		return TRUE;
	}

	BOOL winx_msg OnPageCreate(HWND hWndZero)	{ return TRUE; }
	void winx_msg OnPageAddRef(HWND hWndZero)	{}
	void winx_msg OnPageRelease(HWND hWndZero)	{}
#endif

public:
	HPROPSHEETPAGE winx_call Create(
		RESID title = (LPCTSTR)NULL, LPVOID pInitParam = NULL,
		HINSTANCE hInst = GetThisModule())
	{
		PropertyPageInit psp;
		ZeroMemory(&psp, sizeof(PropertyPageInit));	

		psp.dwSize = sizeof(PROPSHEETPAGE);
		psp.hInstance = hInst;
		psp.pszTemplate = MAKEINTRESOURCE(IDD);
		psp.pfnDlgProc = (DLGPROC)WindowClass::DialogProc;
		psp.lParam = (LPARAM)_WINX_PWND;

		WINX_MSG_HAS3(OnPageCreate, OnPageAddRef, OnPageRelease)
		{
			WINX_MSG_HAS(PropertyPageProc)
			{
				psp.dwFlags = PSP_USECALLBACK;
				psp.pfnCallback = WindowClass::PropertyPageProc;
			}
		}
		
		if (title.m_lpstr != NULL)
		{
			psp.dwFlags |= PSP_USETITLE;
			psp.pszTitle = title.m_lpstr;
		}

		WINX_MSG_HAS(OnPagePreCreate)
			_WINX_PWND->OnPagePreCreate(&psp, pInitParam);

		return ::CreatePropertySheetPage(&psp);
	}

public:
	template <class SheetClass>
	BOOL winx_call AddTo(
		SheetClass& sheet,
		RESID title = (LPCTSTR)NULL, LPVOID pInitParam = NULL,
		HINSTANCE hInst = GetThisModule())
	{
		HPROPSHEETPAGE hPage = Create(title, pInitParam, hInst);
		if (hPage == NULL)
		{
			WINX_DELETE_THIS_PWND();
			return FALSE;
		}
		return sheet.AddPage(hPage);
	}

public:
	int winx_call DoModal(
		HWND hWndParent = NULL,
		RESID titleSheet = (LPCTSTR)NULL,
		RESID titlePage = (LPCTSTR)NULL, LPVOID pInitParam = NULL,
		HINSTANCE hInst = GetThisModule())
	{
		PropertySheetT<1> sheet(titleSheet, 0, hInst);
		BOOL fRet = AddTo(sheet, titlePage, pInitParam, hInst);
		if (fRet)
			return sheet.DoModal(hWndParent);
		return -1;
	}
};

// =========================================================================
// $Log: PropertySheet.h,v $
// Revision 1.3  2006/09/13 17:05:11  xushiwei
// WINX-Basic: DefaultWindowHandle(see macro WINX_NULL_WINDOW_HANDLE)
// WINX-Core: AxModelDialog/AxModelessDialog
//
// Revision 1.2  2006/09/03 06:32:12  xushiwei
// WINX-Extension: PropertySheet, PropertyPage
//

NS_WINX_END

#endif /* WINX_WIN_PROPERTYSHEET_H */
