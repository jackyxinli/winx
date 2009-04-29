// maindlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"
#include "maindlg.h"

/////////////////////////////////////////////////////////////////////////////
// Construction/destruction

CMainDlg::CMainDlg() : 
    m_uDownloadEvents(0), m_nLogBeforeNav(1), m_nLogNavComplete(1),
    m_nLogStatusTextChange(1), m_nLogDownloadBegin(1),
    m_nLogDownloadComplete(1), m_nLogCmdStateChange(1)
{
}

CMainDlg::~CMainDlg()
{
}


/////////////////////////////////////////////////////////////////////////////
// Message handlers

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
    // This was stolen from an SDI app using a form view.
    //
    // Pass keyboard messages along to the child window that has the focus.
    // When the browser has the focus, the message is passed to its containing
    // CAxWindow, which lets the control handle the message.

    if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
       (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
        return FALSE;

HWND hWndCtl = ::GetFocus();

    if(IsChild(hWndCtl))
        {
        // find a direct child of the dialog from the window that has focus
        while(::GetParent(hWndCtl) != m_hWnd)
            hWndCtl = ::GetParent(hWndCtl);

        // give control a chance to translate this message
        if(::SendMessage(hWndCtl, WM_FORWARDMSG, 0, (LPARAM)pMsg) != 0)
            return TRUE;
        }

    // A normal control has the focus, so call IsDialogMessage() so that
    // the dialog shortcut keys work (TAB, etc.)
    return IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
    m_wndOK.EnableWindow ( m_wndEdit.GetWindowTextLength() > 0 );
    
    UIUpdateChildWindows();
    return FALSE;
}

BOOL CMainDlg::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
    // center the dialog on the screen
    CenterWindow();

    // Init resizing
    DlgResize_Init();

    // Init DDX variables
    DoDataExchange(false);
    
    m_wndOK.Attach ( GetDlgItem(IDOK) );

    // set icons
    HICON hIcon = AtlLoadIconImage ( IDR_MAINFRAME, LR_DEFAULTCOLOR, 32, 32 );
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage ( IDR_MAINFRAME, LR_DEFAULTCOLOR, 32, 32 );
    SetIcon(hIconSmall, FALSE);

    // register object for message filtering and idle updates
CMessageLoop* pLoop = _Module.GetMessageLoop();

    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    UIAddChildWindowContainer(m_hWnd);

    // Set up the various controls in the dialog...

    // The list that shows the events we listen for.
    m_wndEventList.InsertColumn ( 0, _T(""), LVCFMT_LEFT, 0, 0 );

    // Get an IWebBrowser2 interface on the browser control and start listening
    // to events.
CAxWindow   wndIE = GetDlgItem(IDC_IE);
CComVariant v;

    wndIE.QueryControl ( &m_pWB2 );

    if ( m_pWB2 )
        {
        DispEventAdvise ( m_pWB2 );
        m_pWB2->Navigate ( CComBSTR("about:mozilla"), &v, &v, &v, &v );
        }

    // Set up the bitmap buttons.
CImageList iml;

    iml.CreateFromImage ( IDB_BACK_BTN, 17, 1, CLR_NONE, IMAGE_BITMAP, 
                          LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS | LR_CREATEDIBSECTION );
    m_btnBack.SetImageList ( iml.Detach() );
    m_btnBack.SetImages ( 0, 3, 1, 2 );
    m_btnBack.SetBitmapButtonExtendedStyle ( BMPBTN_HOVER, BMPBTN_HOVER );

    iml.CreateFromImage ( IDB_FWD_BTN, 17, 1, CLR_NONE, IMAGE_BITMAP, 
                          LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS | LR_CREATEDIBSECTION );
    m_btnFwd.SetImageList ( iml.Detach() );
    m_btnFwd.SetImages ( 0, 3, 1, 2 );
    m_btnFwd.SetBitmapButtonExtendedStyle ( BMPBTN_HOVER, BMPBTN_HOVER );

    iml.CreateFromImage ( IDB_STOP_BTN, 17, 1, CLR_NONE, IMAGE_BITMAP, 
                          LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS | LR_CREATEDIBSECTION );
    m_btnStop.SetImageList ( iml.Detach() );
    m_btnStop.SetImages ( 0, 3, 1, 2 );
    m_btnStop.SetBitmapButtonExtendedStyle ( BMPBTN_HOVER, BMPBTN_HOVER );

    iml.CreateFromImage ( IDB_RELOAD_BTN, 17, 1, CLR_NONE, IMAGE_BITMAP, 
                          LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS | LR_CREATEDIBSECTION );
    m_btnReload.SetImageList ( iml.Detach() );
    m_btnReload.SetImages ( 0, 3, 1, 2 );
    m_btnReload.SetBitmapButtonExtendedStyle ( BMPBTN_HOVER, BMPBTN_HOVER );

    m_wndEdit.SetFocus();
    m_wndEdit.SetSel ( 0, -1 );

    return FALSE;
}

void CMainDlg::OnDestroy()
{
    // Stop listening to events from the browser control.
    if ( m_pWB2 )
        {
        DispEventUnadvise ( m_pWB2 );
        m_pWB2.Release();
        }

    SetMsgHandled(false);
}


/////////////////////////////////////////////////////////////////////////////
// Command handlers

void CMainDlg::OnAppAbout(UINT uCode, int nID, HWND hWndCtl)
{
CAboutDlg dlg;

    dlg.DoModal();
}

void CMainDlg::OnOK(UINT uCode, int nID, HWND hWndCtl)
{
CComVariant v;

    if ( !DoDataExchange(true) )
        return;

    m_pWB2->Navigate ( CComBSTR(m_sAddress), &v, &v, &v, &v );
}

void CMainDlg::OnCancel(UINT uCode, int nID, HWND hWndCtl)
{
    CloseDialog(nID);
}

void CMainDlg::CloseDialog(int nVal)
{                        
    DestroyWindow();
    PostQuitMessage(nVal);
}

void CMainDlg::OnBack(UINT uCode, int nID, HWND hWndCtl)
{
    m_pWB2->GoBack();
}

void CMainDlg::OnForward(UINT uCode, int nID, HWND hWndCtl)
{
    m_pWB2->GoForward();
}

void CMainDlg::OnStop(UINT uCode, int nID, HWND hWndCtl)
{
    m_pWB2->Stop();
}

void CMainDlg::OnReload(UINT uCode, int nID, HWND hWndCtl)
{
CComVariant vLevel;

    if ( GetKeyState(VK_SHIFT) & 0x8000 )
        vLevel = REFRESH_COMPLETELY;
    else
        vLevel = REFRESH_IFEXPIRED;

    m_pWB2->Refresh2 ( &vLevel );
}

void CMainDlg::OnLogCheckbox(UINT uCode, int nID, HWND hWndCtl)
{
    DoDataExchange ( true, nID );
}


/////////////////////////////////////////////////////////////////////////////
// Other methods

void CMainDlg::AddToList ( LPCTSTR szMsg )
{
int nIndex = m_wndEventList.GetItemCount();

    m_wndEventList.InsertItem ( nIndex, szMsg );
    m_wndEventList.EnsureVisible ( nIndex, false );
    m_wndEventList.SetColumnWidth ( 0, LVSCW_AUTOSIZE );
}


/////////////////////////////////////////////////////////////////////////////
// Web browser event handlers

void __stdcall CMainDlg::OnBeforeNavigate2 (
    IDispatch* pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName,
    VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel )
{
USES_CONVERSION;
CString sFrame, sURL, sMsg;
    
    sFrame = TargetFrameName->bstrVal;
    sURL = URL->bstrVal;

    if ( m_nLogBeforeNav )
        {
        sMsg.Format ( _T("BeforeNavigate2: frame = %s, URL = %s"), 
                      (LPCTSTR) sFrame, (LPCTSTR) sURL );

        AddToList ( sMsg );
        }

    // You can set *Cancel to VARIANT_TRUE to stop the navigation from
    // happening. For example, to stop navigates to evil tracking companies
    // like doubleclick.net:
    if ( sURL.Find ( _T("doubleclick.net") ) > 0 )
        *Cancel = VARIANT_TRUE;

    if ( *Cancel && m_nLogBeforeNav )
        AddToList ( _T("** Cancelled **") );
}

void __stdcall CMainDlg::OnNavigateComplete2 ( IDispatch* pDisp, VARIANT * URL )
{
USES_CONVERSION;
CString sURL, sMsg;
    
    sURL = URL->bstrVal;

    if ( m_nLogNavComplete )
        {
        sMsg.Format ( _T("NavigateComplete2: URL = %s"), (LPCTSTR) sURL );
        AddToList ( sMsg );
        }

    if ( m_pWB2.IsEqualObject ( pDisp ) )   // top level browser? (ie not a frame)
        {
        m_sAddress = sURL;
        DoDataExchange ( false, IDC_ADDRESS );
        }
}

void __stdcall CMainDlg::OnStatusChange ( BSTR bsText )
{
USES_CONVERSION;
CString sText, sMsg;

    sText = bsText;

    if ( m_nLogStatusTextChange )
        {
        sMsg.Format ( _T("StatusChange: text = %s"), (LPCTSTR) sText );
        AddToList ( sMsg );
        }
    
    m_wndIEStatus.SetWindowText ( sText );
}

void __stdcall CMainDlg::OnCommandStateChange ( long lCmd, VARIANT_BOOL vbEnabled )
{
CString sMsg;
bool bEnable = (vbEnabled != VARIANT_FALSE);

    if ( CSC_NAVIGATEBACK == lCmd )
        {
        if ( m_nLogCmdStateChange )
            {
            sMsg.Format ( _T("CommandStateChange: command = back, enable = %s"),
                          bEnable ? _T("true") : _T("false") );

            AddToList ( sMsg );
            }

        UIEnable ( IDC_BACK, bEnable );
        }
    else if ( CSC_NAVIGATEFORWARD == lCmd )
        {
        if ( m_nLogCmdStateChange )
            {
            sMsg.Format ( _T("CommandStateChange: command = forward, enable = %s"),
                          bEnable ? _T("true") : _T("false") );

            AddToList ( sMsg );
            }

        UIEnable ( IDC_FORWARD, bEnable );
        }
}

void __stdcall CMainDlg::OnDownloadBegin()
{
    if ( 1 == ++m_uDownloadEvents )
        m_wndWait.ShowWindow ( SW_SHOW );

    if ( m_nLogDownloadBegin )
        {
        CString sMsg;

        sMsg.Format ( _T("DownloadBegin: count = %u"), m_uDownloadEvents );
        AddToList ( sMsg );
        }
}
 
void __stdcall CMainDlg::OnDownloadComplete()
{
    if ( 0 == m_uDownloadEvents )
        return;

    if ( 0 == --m_uDownloadEvents )
        m_wndWait.ShowWindow ( SW_HIDE );

    if ( m_nLogDownloadComplete )
        {
        CString sMsg;
        
        sMsg.Format ( _T("DownloadComplete: count = %u"), m_uDownloadEvents );
        AddToList ( sMsg );
        }
}
