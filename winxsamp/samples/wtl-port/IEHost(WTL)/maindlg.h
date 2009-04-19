// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__B193DB4C_786C_4548_B5D8_A2A7FC0D6E33__INCLUDED_)
#define AFX_MAINDLG_H__B193DB4C_786C_4548_B5D8_A2A7FC0D6E33__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Info structs used by the event sink map

__declspec(selectany) _ATL_FUNC_INFO BeforeNavigate2Info =
    { CC_STDCALL, VT_EMPTY, 7,
        { VT_DISPATCH, VT_VARIANT|VT_BYREF, VT_VARIANT|VT_BYREF, 
          VT_VARIANT|VT_BYREF, VT_VARIANT|VT_BYREF, VT_VARIANT|VT_BYREF, 
          VT_BOOL|VT_BYREF }
};

__declspec(selectany) _ATL_FUNC_INFO NavigateComplete2Info =
    { CC_STDCALL, VT_EMPTY, 2, { VT_DISPATCH, VT_VARIANT|VT_BYREF } 
};

__declspec(selectany) _ATL_FUNC_INFO StatusChangeInfo =
    { CC_STDCALL, VT_EMPTY, 1, { VT_BSTR }
};

__declspec(selectany) _ATL_FUNC_INFO CommandStateChangeInfo =
    { CC_STDCALL, VT_EMPTY, 2, { VT_I4, VT_BOOL }
};

__declspec(selectany) _ATL_FUNC_INFO DownloadInfo =
    { CC_STDCALL, VT_EMPTY, 0 
};


/////////////////////////////////////////////////////////////////////////////

class CMainDlg : public CAxDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
                 public CMessageFilter, public CIdleHandler,
                 public CWinDataExchange<CMainDlg>,
                 public CDialogResize<CMainDlg>,
                 public CComObjectRootEx<CComSingleThreadModel>,
                 public CComCoClass<CMainDlg>,
                 public IDispEventSimpleImpl<37, CMainDlg, &DIID_DWebBrowserEvents2>
{
public:
    enum { IDD = IDD_MAINDLG };

    // Construction/destruction
    CMainDlg();
    ~CMainDlg();

    // Maps
    BEGIN_MSG_MAP(CMainDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(ID_APP_ABOUT, OnAppAbout)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER_EX(IDC_BACK, OnBack)
        COMMAND_ID_HANDLER_EX(IDC_FORWARD, OnForward)
        COMMAND_ID_HANDLER_EX(IDC_RELOAD, OnReload)
        COMMAND_ID_HANDLER_EX(IDC_STOP, OnStop)
        COMMAND_RANGE_HANDLER_EX(IDC_LOG_BEFORENAV, IDC_LOG_CMDSTATECHG, OnLogCheckbox)
        CHAIN_MSG_MAP(CUpdateUI<CMainDlg>)
        CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
    END_MSG_MAP()

    BEGIN_UPDATE_UI_MAP(CMainDlg)
        UPDATE_ELEMENT(IDC_BACK, UPDUI_CHILDWINDOW)
        UPDATE_ELEMENT(IDC_FORWARD, UPDUI_CHILDWINDOW)
    END_UPDATE_UI_MAP()

    BEGIN_DDX_MAP(CMainDlg)
        DDX_TEXT(IDC_ADDRESS, m_sAddress)
        DDX_CONTROL(IDC_ADDRESS, m_wndEdit)
        DDX_CONTROL(IDC_BACK, m_btnBack)
        DDX_CONTROL(IDC_FORWARD, m_btnFwd)
        DDX_CONTROL(IDC_STOP, m_btnStop)
        DDX_CONTROL(IDC_RELOAD, m_btnReload)
        DDX_CONTROL(IDC_WAITUP, m_wndWait)
        DDX_CONTROL(IDC_IE_STATUS, m_wndIEStatus)
        DDX_CONTROL(IDC_EVENT_LIST, m_wndEventList)
        DDX_CHECK(IDC_LOG_BEFORENAV, m_nLogBeforeNav)
        DDX_CHECK(IDC_LOG_NAVCOMPLETE, m_nLogNavComplete)
        DDX_CHECK(IDC_LOG_STATUSCHG, m_nLogStatusTextChange)
        DDX_CHECK(IDC_LOG_DOWNLOADBEGIN, m_nLogDownloadBegin)
        DDX_CHECK(IDC_LOG_DOWNLOADEND, m_nLogDownloadComplete)
        DDX_CHECK(IDC_LOG_CMDSTATECHG, m_nLogCmdStateChange)
    END_DDX_MAP()

    BEGIN_DLGRESIZE_MAP(CMainDlg)
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
    END_DLGRESIZE_MAP()

    BEGIN_COM_MAP(CMainDlg)
//      COM_INTERFACE_ENTRY2(IDispatch, IDispEventSimpleImpl)
    END_COM_MAP()

    BEGIN_SINK_MAP(CMainDlg)
        SINK_ENTRY_INFO(37, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2, &BeforeNavigate2Info)
        SINK_ENTRY_INFO(37, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2, OnNavigateComplete2, &NavigateComplete2Info)
        SINK_ENTRY_INFO(37, DIID_DWebBrowserEvents2, DISPID_STATUSTEXTCHANGE, OnStatusChange, &StatusChangeInfo)
        SINK_ENTRY_INFO(37, DIID_DWebBrowserEvents2, DISPID_COMMANDSTATECHANGE, OnCommandStateChange, &CommandStateChangeInfo)
        SINK_ENTRY_INFO(37, DIID_DWebBrowserEvents2, DISPID_DOWNLOADBEGIN, OnDownloadBegin, &DownloadInfo)
        SINK_ENTRY_INFO(37, DIID_DWebBrowserEvents2, DISPID_DOWNLOADCOMPLETE, OnDownloadComplete, &DownloadInfo)
    END_SINK_MAP()

    // Web browser event handlers
    void __stdcall OnBeforeNavigate2 (
        IDispatch* pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName,
        VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel );

    void __stdcall OnNavigateComplete2 ( IDispatch* pDisp, VARIANT* URL );
    void __stdcall OnStatusChange ( BSTR bsText );
    void __stdcall OnCommandStateChange ( long lCmd, VARIANT_BOOL vbEnabled );
    void __stdcall OnDownloadBegin();
    void __stdcall OnDownloadComplete();

    // Message handlers
    BOOL PreTranslateMessage(MSG* pMsg);
    BOOL OnIdle();

    BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
    void OnDestroy();

    // Command handlers
    void OnAppAbout(UINT uCode, int nID, HWND hWndCtl);
    void OnOK(UINT uCode, int nID, HWND hWndCtl);
    void OnCancel(UINT uCode, int nID, HWND hWndCtl);

    void OnBack(UINT uCode, int nID, HWND hWndCtl);
	void OnForward(UINT uCode, int nID, HWND hWndCtl);
	void OnStop(UINT uCode, int nID, HWND hWndCtl);
	void OnReload(UINT uCode, int nID, HWND hWndCtl);
	void OnLogCheckbox(UINT uCode, int nID, HWND hWndCtl);

    void CloseDialog(int nVal);

protected:
    // Interface to the web browser control
    CComPtr<IWebBrowser2> m_pWB2;

    // Counter used to show the "wait" text
    UINT m_uDownloadEvents;

    // Browser control buttons
    CWindow       m_wndOK;
    CEditImpl     m_wndEdit;
    CBitmapButton m_btnBack, m_btnFwd, m_btnStop, m_btnReload;

    // DDX vars
    CString           m_sAddress;
    CStaticImpl       m_wndWait;
    CStaticImpl       m_wndIEStatus;
    CListViewCtrlImpl m_wndEventList;

    int m_nLogBeforeNav, m_nLogNavComplete, m_nLogStatusTextChange,
        m_nLogDownloadBegin, m_nLogDownloadComplete, m_nLogCmdStateChange;

    // Misc functions
    void AddToList ( LPCTSTR szMsg );
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__B193DB4C_786C_4548_B5D8_A2A7FC0D6E33__INCLUDED_)
