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
// Module: winx/WebBrowser.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:22:03
// 
// $Id: WebBrowser.h,v 1.2 2006/08/26 03:44:52 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WEBBROWSER_H__
#define __WINX_WEBBROWSER_H__

#ifndef __WINX_DISPATCHEVENT_H__
#include "DispatchEvent.h"
#endif

#ifndef __WINX_WINSDK_IE_H__
#include "winsdk/IE.h"
#endif

// -------------------------------------------------------------------------
// IID_ITravelLogStg

#ifndef IID_ITravelLogStg
#define IID_ITravelLogStg	__uuidof(ITravelLogStg)
#endif

// -------------------------------------------------------------------------
// Event Dispatch

template <class Type>
class DWebBrowserEvents2Disp :
	public DispEventSimpleImpl<DWebBrowserEvents2Disp<Type>, &DIID_DWebBrowserEvents2, Type>
{
	//
	// Event Prototype
	//

#if (0)
	//
	//@Brief
	//	- Fires before navigation occurs in the given object(on either a window or frameset element).
	//pDisp
	//	- Pointer to the IDispatch interface for the WebBrowser object that represents the 
	//	  window or frame. This interface can be queried for the IWebBrowser2 interface.
	//URL
	//	- Pointer to a VARIANT structure of type VT_BSTR that contains the URL to be navigated to.
	//Flags
	//	- Reserved.
	//TargetFrameName
	//	- Pointer to a VARIANT structure of type VT_BSTR that contains the name of the frame in 
	//	  which to display the resource, or NULL if no named frame is targeted for the resource.
	//PostData
	//	- Pointer to a VARIANT structure of type VT_BYREF|VT_VARIANT that contains the data to 
	//	  send to the server if the HTTP POST transaction is being used.
	//Headers
	//	- Pointer to a VARIANT structure of type VT_BSTR that contains additional HTTP headers
	//	  to send to the server (HTTPURLs only). The headers can specify things such as the 
	//	  action required of the server, the type of data being passed to the server, or a 
	//	  status code. 
	//Cancel
	//	- Pointer to a VARIANT structure of type VARIANT_BOOL that contains the cancel flag. 
	//	  An application can set this parameter to VARIANT_TRUE to cancel the navigation 
	//	  operation, or to VARIANT_FALSE to allow it to proceed.
	VOID __stdcall WebBrowser_OnBeforeNavigate2(
		IN IDispatch* pDisp, IN VARIANT* URL, IN VARIANT* Flags, IN VARIANT* TargetFrameName,
		IN VARIANT* PostData, IN VARIANT* Headers, IN OUT VARIANT_BOOL* Cancel)
	{
	}
	
	//
	//@Brief
	//	- Fires to request that the client window size be converted to the host window size.
	//CX, CY
	//	- Pointer to a long integer that receives and specifies the width/height of the 
	//	  client window.
	VOID __stdcall WebBrowser_OnClientToHostWindow(IN OUT long* CX, IN OUT long* CY)
	{
	}

	//
	//@Brief
	//	- Fires when a document has been completely loaded and initialized.
	//pDisp
	//	- Pointer to the IDispatch interface of the window or frame in which the document 
	//	  has loaded. This IDispatch interface can be queried for the IWebBrowser2 interface.
	//URL
	//	- Pointer to a VARIANT structure of type VT_BSTR that specifies the URL, Universal 
	//	  Naming Convention (UNC) file name, or pointer to an item identifier list (PIDL) 
	//	  of the loaded document.
	VOID __stdcall WebBrowser_OnDocumentComplete(
		IN IDispatch* pDisp, IN VARIANT* URL)
	{
	}

	//
	//@Brief
	//	- Fires when a navigation operation is beginning.
	VOID __stdcall WebBrowser_OnDownloadBegin()
	{
	}

	//
	//@Brief
	//	- Fires when a navigation operation finishes, is halted, or fails.
	VOID __stdcall WebBrowser_OnDownloadComplete()
	{
	}

	//
	//@Brief
	//	- Fires to indicate that a file download is about to occur.
	//	  If a file download dialog is to be displayed, this event is fired prior to 
	//	  the display of the dialog.
	//ActiveDocument
	//	- specifies whether the file is an Active Document.
	//Cancel
	//	- specifies whether to continue the download process and display the download dialog.
	VOID __stdcall WebBrowser_OnFileDownload(
		OUT VARIANT_BOOL* ActiveDocument, IN OUT VARIANT_BOOL* Cancel)
	{
	}

	//
	//@Brief
	//	- Fires after a navigation to a link is completed on either a window or frameSet element.
	//pDisp
	//	- Pointer to the IDispatch interface for the WebBrowser object that represents the 
	//	  window or frame. This interface can be queried for the IWebBrowser2 interface.
	//URL
	//	- Pointer to a VARIANT structure of type VT_BSTR that contains the URL, UNC file name,
	//	  or PIDL that was navigated to. Note that this URL can be different from the URL that
	//	  the browser was told to navigate to. One reason is that this URL is the canonicalized
	//	  and qualified URL. For example, if an application specified a URL of 
	//	  "www.microsoft.com" in a call to the Navigate or Navigate2 method, the URL passed by
	//	  IWebBrowser2::Navigate2 would be "http://www.microsoft.com/". Also, if the server has 
	//	  redirected the browser to a different URL, the redirected URL will be reflected here.
	VOID __stdcall WebBrowser_OnNavigateComplete2(
		IN IDispatch* pDisp, IN VARIANT* URL)
	{
	}

	//
	//@Brief
	//	- Fires when the status bar text of the object has changed.
	//Text
	//	- specifies the new status bar text.
	VOID __stdcall WebBrowser_OnStatusTextChange(IN BSTR Text)
	{
	}

	//
	//@Brief
	//	- Fires when the enabled state of a command changes.
	//Command
	//	- CommandStateChangeConstants enumeration value that specifies the command that changed.
	//Enable
	//	- specifies the enabled state.
	VOID __stdcall WebBrowser_OnCommandStateChange(
		IN long Command, VARIANT_BOOL Enable)
	{
	}

#endif

	//
	// Event Sink Map
	//

	WINX_SINK_BEGIN();
		WINX_SINK_DEFI2(DISPID_COMMANDSTATECHANGE, WebBrowser_OnCommandStateChange, long, VARIANT_BOOL);
		WINX_SINK_DEFI1(DISPID_STATUSTEXTCHANGE, WebBrowser_OnStatusTextChange, BSTR);
		WINX_SINK_DEFI7(DISPID_BEFORENAVIGATE2, WebBrowser_OnBeforeNavigate2,
			IDispatch*, VARIANT*, VARIANT*, VARIANT*, VARIANT*, VARIANT*, VARIANT_BOOL*);
		WINX_SINK_DEFI2(DISPID_NAVIGATECOMPLETE2, WebBrowser_OnNavigateComplete2, IDispatch*, VARIANT*);
		WINX_SINK_DEFI2(DISPID_DOCUMENTCOMPLETE, WebBrowser_OnDocumentComplete, IDispatch*, VARIANT*);
		WINX_SINK_DEFI2(DISPID_CLIENTTOHOSTWINDOW, WebBrowser_OnClientToHostWindow, long*, long*);
		WINX_SINK_DEFI2(DISPID_FILEDOWNLOAD, WebBrowser_OnFileDownload, VARIANT_BOOL*, VARIANT_BOOL*);
		WINX_SINK_DEFI0(DISPID_DOWNLOADBEGIN, WebBrowser_OnDownloadBegin);
		WINX_SINK_DEFI0(DISPID_DOWNLOADCOMPLETE, WebBrowser_OnDownloadComplete);
	WINX_SINK_END();

public:
	typedef DWebBrowserEvents2Disp<Type> DWebBrowserEvents2DispType;

	//
	// 注意: 我们推荐Sink的事件名有通用前缀，以避免重名。
	// 例如: WebBrowser_OnDownloadComplete，不要把事件名取为OnDownloadComplete。
	//

	WINX_NULL_SINK(WebBrowser_OnBeforeNavigate2);
	WINX_NULL_SINK(WebBrowser_OnClientToHostWindow);
	WINX_NULL_SINK(WebBrowser_OnDocumentComplete);
	WINX_NULL_SINK(WebBrowser_OnNavigateComplete2);
	WINX_NULL_SINK(WebBrowser_OnFileDownload);
	WINX_NULL_SINK(WebBrowser_OnStatusTextChange);
	WINX_NULL_SINK(WebBrowser_OnCommandStateChange);
	WINX_NULL_SINK(WebBrowser_OnDownloadBegin);
	WINX_NULL_SINK(WebBrowser_OnDownloadComplete);
};

// -------------------------------------------------------------------------
// $Log: WebBrowser.h,v $
// Revision 1.2  2006/08/26 03:44:52  xushiwei
// WINX-Core:
//    Dispatch Event Implement(Connect-Point support)
//
// Revision 1.1  2006/08/25 15:13:18  xushiwei
// Winx-Extension:
//    ActiveX, WebBrowser(IE), Theme(XPStyle) support
//

#endif /* __WINX_WEBBROWSER_H__ */
