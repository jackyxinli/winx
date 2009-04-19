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
// Module: MainDlg.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-13 20:37:20
// 
// $Id: maindlg.h,v 1.2 2006/12/03 08:56:02 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __MAINDLG_H__
#define __MAINDLG_H__

// -------------------------------------------------------------------------
// class CMainDlg

class CMainDlg : public winx::AxModalDialog<CMainDlg, IDD_MAINDLG>,
				 public winx::WinDataExchange<CMainDlg>,
				 public winx::UpdateUI<CMainDlg>,
                 public DWebBrowserEvents2Disp<CMainDlg>
{
	WINX_ICON(IDR_MAINFRAME);

    WINX_CMDS_BEGIN()
        WINX_CMD(ID_APP_ABOUT, OnAppAbout)
        WINX_CMD(IDCANCEL, OnCancel)
        WINX_CMD(IDC_BACK, OnBack)
        WINX_CMD(IDC_FORWARD, OnForward)
        WINX_CMD(IDC_RELOAD, OnReload)
        WINX_CMD(IDC_STOP, OnStop)
        WINX_CMD_RANGE(IDC_LOG_BEFORENAV, IDC_LOG_CMDSTATECHG, OnLogCheckbox)
    WINX_CMDS_END();

    WINX_UPDATEUI_BEGIN()
        WINX_UPDATEUI(IDC_BACK, UPDUI_CHILDWINDOW)
        WINX_UPDATEUI(IDC_FORWARD, UPDUI_CHILDWINDOW)
    WINX_UPDATEUI_END();

    WINX_DDX_BEGIN()
        DDX_TEXT(IDC_ADDRESS, m_sAddress)
        DDX_CHECK(IDC_LOG_BEFORENAV, m_nLogBeforeNav)
        DDX_CHECK(IDC_LOG_NAVCOMPLETE, m_nLogNavComplete)
        DDX_CHECK(IDC_LOG_STATUSCHG, m_nLogStatusTextChange)
        DDX_CHECK(IDC_LOG_DOWNLOADBEGIN, m_nLogDownloadBegin)
        DDX_CHECK(IDC_LOG_DOWNLOADEND, m_nLogDownloadComplete)
        DDX_CHECK(IDC_LOG_CMDSTATECHG, m_nLogCmdStateChange)
    WINX_DDX_END();

    WINX_DLGRESIZE_BEGIN(TRUE)
        DLGRESIZE_CONTROL(IDC_ADDRESS, DLSZ_SIZE_X)
        DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X)
        DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X)
        DLGRESIZE_CONTROL(ID_APP_ABOUT, DLSZ_MOVE_X)
        DLGRESIZE_CONTROL(IDC_IE, DLSZ_SIZE_X|DLSZ_SIZE_Y)
        DLGRESIZE_CONTROL(IDC_IE_STATUS, DLSZ_SIZE_X|DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_EVENT_LIST, DLSZ_SIZE_X|DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_LOG_BEFORENAV, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_LOG_NAVCOMPLETE, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_LOG_STATUSCHG, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_LOG_DOWNLOADBEGIN, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_LOG_DOWNLOADEND, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_LOG_CMDSTATECHG, DLSZ_MOVE_Y)
    WINX_DLGRESIZE_END();

public:
    // Construction/destruction
    CMainDlg();
    ~CMainDlg();

    // Web browser event handlers
    void __stdcall WebBrowser_OnBeforeNavigate2 (
        IDispatch* pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName,
        VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel );

    void __stdcall WebBrowser_OnNavigateComplete2 ( IDispatch* pDisp, VARIANT* URL );
    void __stdcall WebBrowser_OnStatusTextChange( BSTR bsText );
    void __stdcall WebBrowser_OnCommandStateChange ( long lCmd, VARIANT_BOOL vbEnabled );
    void __stdcall WebBrowser_OnDownloadBegin ();
    void __stdcall WebBrowser_OnDownloadComplete ();

    // Message handlers
    void OnIdle(HWND hWnd);

    BOOL OnInitDialog(HWND hWnd, HWND hWndDefaultFocus);
    void OnDestroy(HWND hWnd);

    // Command handlers
    void OnAppAbout(HWND hWnd);
    void OnOK(HWND hWnd);
    void OnCancel(HWND hWnd);

    void OnBack(HWND hWnd);
	void OnForward(HWND hWnd);
	void OnStop(HWND hWnd);
	void OnReload(HWND hWnd);
	void OnLogCheckbox(HWND hWnd, int nID);

protected:
	typedef winx::CString CString;

    // Interface to the web browser control
    CComPtr<IWebBrowser2> m_pWB2;

    // Counter used to show the "wait" text
    UINT m_uDownloadEvents;

    // Browser control buttons
	WTL::CBitmapButton m_btnBack, m_btnFwd, m_btnStop, m_btnReload;
	winx::EditHandle	m_wndEdit;
	winx::StaticHandle	m_wndWait;
    winx::StaticHandle	m_wndIEStatus;
    winx::WindowHandle	m_wndOK;
	winx::ListCtrlHandle m_wndEventList;

    // DDX vars
	CString m_sAddress;
    int m_nLogBeforeNav, m_nLogNavComplete, m_nLogStatusTextChange,
        m_nLogDownloadBegin, m_nLogDownloadComplete, m_nLogCmdStateChange;

    // Misc functions
    void AddToList ( LPCTSTR szMsg );
};

// -------------------------------------------------------------------------
// $Log: maindlg.h,v $
// Revision 1.2  2006/12/03 08:56:02  xushiwei
// Unify style of map macro, such as:
//  WINX_TEST_SUITE/WINX_TEST/WINX_TEST_SUITE_END;
//  WINX_CMDS_BEGIN/WINX_CMD/WINX_CMDS_END;
//  WINX_SYSCMD_BEGIN/WINX_SYSCMD/WINX_SYSCMD_END;
//  WINX_NOTIFY_BEGIN/WINX_NOTIFY/WINX_NOTIFY_END;
//  WINX_REFLECT_NOTIFY_BEGIN/WINX_REFLECT_NOTIFY/WINX_REFLECT_NOTIFY_END;
//
// Revision 1.1  2006/09/18 05:23:26  xushiwei
// port WTL application to winx
//

#endif /* __MAINDLG_H__ */
