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
// Module: winx/wtl/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:36:01
// 
// $Id: Basic.h,v 1.11 2006/12/11 05:19:17 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WTL_BASIC_H__
#define __WINX_WTL_BASIC_H__

#if (0)
#define WINX_USE_APPMODULE
#define WINX_USE_ATLPORT
#define WINX_CREATE_AX_ON_DLGINIT
#endif

#if defined(WINX_GCC)
#define WINX_USE_ATLPORT
#endif

// -------------------------------------------------------------------------
// WINX_CREATE_AX_ON_DLGINIT
//	- @@bugfix: 高版本的ATL::CAxDialogImpl将ActiveX创建挪到了WM_INITDIALOG

#define _WTL_NO_AUTOMATIC_NAMESPACE
#define _WTL_USE_CSTRING

#ifndef __ATLBASE_H__
	#if defined(WINX_USE_ATLPORT)
	#include "../../../../atlport/include/atlbase.h"
	#else
	#include <atlbase.h>
	#endif
#endif

#if defined(_ATL_VER) && (_ATL_VER > 0x0300)
#define WINX_CREATE_AX_ON_DLGINIT
#endif

#if defined(_DEBUG)
#define _WINX_ASSERT_ONCE(e)												\
	do {																	\
		static int fReported = 0;											\
		if (!fReported && !(e))												\
		{																	\
			ATLASSERT(e);													\
			fReported = 1;													\
		}																	\
	} while(0)
#else
#define _WINX_ASSERT_ONCE(e)	0
#endif

#if defined(WINX_CREATE_AX_ON_DLGINIT)
	#pragma push_macro("ATLASSUME")
	#undef ATLASSUME
	#define ATLASSUME(e)	_WINX_ASSERT_ONCE(e)
#endif

// -------------------------------------------------------------------------

#if !defined(__ATLCOM_H__) && !defined(WINX_USE_APPMODULE)
extern ATL::CComModule _Module;
#endif

#ifndef __ATLAPP_H__
#define CDC _WTL_CDC
#include "../../../../wtl/include/atlapp.h"
#ifndef __ATLGDI_H__
#include "../../../../wtl/include/atlgdi.h"
#endif
#undef CDC
namespace WTL { typedef _WTL_CDC CDC; }
// --> 在VC++ 6.0下，奇怪虽然有namespace WTL，但WTL::CDC还是与MFC的CDC编译冲突。
#endif

#ifndef __ATLWIN_H__
	#if defined(WINX_USE_ATLPORT)
		#include "../../../../atlport/include/atlwin.h"
	#else
		#ifdef for
			#undef for
			#include <atlwin.h>
			#define for if (0); else for
		#else
			#include <atlwin.h>
		#endif
	#endif
#endif

// -------------------------------------------------------------------------

#if defined(WINX_CREATE_AX_ON_DLGINIT)
	#pragma pop_macro("ATLASSUME")
#endif

// -------------------------------------------------------------------------

#ifndef __ATLWINX_H__
#include "../../../../wtl/include/atlwinx.h"
#endif

#if !defined(WINX_VC_NET)
#define ATL	::ATL
#endif

#ifndef __ATLMISC_H__
#include "../../../../wtl/include/atlmisc.h"
#endif

#ifndef __ATLUSER_H__
#include "../../../../wtl/include/atluser.h"
#endif

#ifndef __ATLCTRLS_H__
#include "../../../../wtl/include/atlctrls.h"
#endif

#undef ATL
#undef IDC_STATIC

// -------------------------------------------------------------------------
// ToString

#ifndef _INC_TCHAR
#include <tchar.h>
#endif

__forceinline WTL::CString ToString(LPCTSTR psz)
{
	return WTL::CString(psz, (int)_tcslen(psz));
}

// -------------------------------------------------------------------------
// AtlCreateActiveXControls

#if defined(WINX_CREATE_AX_ON_DLGINIT) // see CAxDialogImpl::CreateActiveXControls

inline HRESULT AtlCreateActiveXControls(HWND hWndParent, HINSTANCE hInst, UINT nID)
{
	using namespace ATL;

	// Load dialog template and InitData
	HRSRC hDlgInit = ::FindResource(hInst, MAKEINTRESOURCE(nID), (LPTSTR)_ATL_RT_DLGINIT);
	BYTE* pInitData = NULL;
	HGLOBAL hData = NULL;
	HRESULT hr = S_OK;
	if (hDlgInit != NULL)
	{
		hData = ::LoadResource(hInst, hDlgInit);
		if (hData != NULL)
			pInitData = (BYTE*)::LockResource(hData);
	}

	HRSRC hDlg = ::FindResource(hInst, MAKEINTRESOURCE(nID), (LPTSTR)RT_DIALOG);
	if (hDlg != NULL)
	{
		HGLOBAL hResource = ::LoadResource(hInst, hDlg);
		DLGTEMPLATE* pDlg = NULL;
		if (hResource != NULL)
		{
			pDlg = (DLGTEMPLATE*) ::LockResource(hResource);
			if (pDlg != NULL)
			{
				// Get first control on the template
				BOOL bDialogEx = _DialogSplitHelper::IsDialogEx(pDlg);
				WORD nItems = _DialogSplitHelper::DlgTemplateItemCount(pDlg);

				// Get first control on the dialog
				DLGITEMTEMPLATE* pItem = _DialogSplitHelper::FindFirstDlgItem(pDlg);
				HWND hWndPrev = ::GetWindow(hWndParent, GW_CHILD);

				// Create all ActiveX cotnrols in the dialog template and place them in the correct tab order (z-order)
				for (WORD nItem = 0; nItem < nItems; nItem++)
				{
					DWORD wID = bDialogEx ? ((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : pItem->id;
					if (_DialogSplitHelper::IsActiveXControl(pItem, bDialogEx))
					{
						BYTE* pData = NULL;
						DWORD dwLen = _DialogSplitHelper::FindCreateData(wID, pInitData, &pData);
						CComPtr<IStream> spStream;
						if (dwLen != 0)
						{
							HGLOBAL h = GlobalAlloc(GHND, dwLen);
							if (h != NULL)
							{
								BYTE* pBytes = (BYTE*) GlobalLock(h);
								BYTE* pSource = pData;
#if _ATL_VER <= 0x0710
								memcpy(pBytes, pSource, dwLen);
#else
								Checked::memcpy_s(pBytes, dwLen, pSource, dwLen);
#endif
								GlobalUnlock(h);
								CreateStreamOnHGlobal(h, TRUE, &spStream);
							}
							else
							{
								hr = E_OUTOFMEMORY;
								break;
							}
						}

						CComBSTR bstrLicKey;
						hr = _DialogSplitHelper::ParseInitData(spStream, &bstrLicKey.m_str);
						if (SUCCEEDED(hr))
						{
							CAxWindow2 wnd;
							// Get control caption.
							LPWSTR pszClassName = 
								bDialogEx ? 
									(LPWSTR)(((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem) + 1) :
									(LPWSTR)(pItem + 1);
							// Get control rect.
							RECT rect;
							rect.left = 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->x : 
									pItem->x;
							rect.top = 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->y : 
									pItem->y;
							rect.right = rect.left + 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->cx : 
									pItem->cx);
							rect.bottom = rect.top + 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->cy : 
									pItem->cy);

							// Convert from dialog units to screen units
							::MapDialogRect(hWndParent, &rect);

							// Create AxWindow with a NULL caption.
							wnd.Create(hWndParent, 
								&rect, 
								NULL, 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->style : 
									pItem->style) | WS_TABSTOP, 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->exStyle : 
									0,
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : 
									pItem->id,
								NULL);

							if (wnd != NULL)
							{
								// Set the Help ID
								if (bDialogEx && ((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->helpID != 0)
									wnd.SetWindowContextHelpId(((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->helpID);
								// Try to create the ActiveX control.
								hr = wnd.CreateControlLic(pszClassName, spStream, NULL, bstrLicKey);
								if (FAILED(hr))
									break;
								// Set the correct tab position.
								if (nItem == 0)
									hWndPrev = HWND_TOP;
								wnd.SetWindowPos(hWndPrev, 0,0,0,0,SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
								hWndPrev = wnd;
							}
							else
							{
								hr = AtlHresultFromLastError();
							}
						}
					}
					else
					{
						if (nItem != 0)
							hWndPrev = ::GetWindow(hWndPrev, GW_HWNDNEXT);
					}
					pItem = _DialogSplitHelper::FindNextDlgItem(pItem, bDialogEx);
				}
			}
			else
				hr = AtlHresultFromLastError();
		}
		else
			hr = AtlHresultFromLastError();
	}
	return hr;
}

#endif // defined(WINX_CREATE_AX_ON_DLGINIT)

// -------------------------------------------------------------------------
// $Log: Basic.h,v $
// Revision 1.11  2006/12/11 05:19:17  xushiwei
// vs2005 __w64 support
//
// Revision 1.10  2006/09/14 14:44:01  xushiwei
// Winx-Extension-ActiveX support bugfix (Visual Studio 2005):
//  Failed to create ActiveX controls in dialog resource template.
//  (Reason: AtlAxDialogBox/AtlAxCreateDialog implementation was changed in high version ATL.)
//
// Revision 1.8  2006/08/25 15:13:19  xushiwei
// Winx-Extension:
//    ActiveX, WebBrowser(IE), Theme(XPStyle) support
//
// Revision 1.6  2006/08/21 18:45:48  xushiwei
// WINX-Core:
//    Window::Create, MainFrame, StackWindowObject(WINX_STACK_OBJECT) support
//
// Revision 1.5  2006/08/20 14:17:52  xushiwei
// ToString
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

#endif /* __WINX_WTL_BASIC_H__ */
