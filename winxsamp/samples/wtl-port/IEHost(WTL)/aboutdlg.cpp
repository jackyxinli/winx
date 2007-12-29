// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow ( GetParent() );

CWindow wndPlaceholder = GetDlgItem ( IDC_IE_PLACEHOLDER );
CRect rc;
CAxWindow wndIE;

    // Get the rect of the placeholder group box, then destroy that window 
    // because we don't need it anymore.
    wndPlaceholder.GetWindowRect ( rc );
    ScreenToClient ( rc );
    wndPlaceholder.DestroyWindow();

    // Here are a couple of ways to create the browser control. Change the if(0)
    // to if(1) to switch between the two.
    if ( 0 )
        {
        CComPtr<IUnknown> punkCtrl;
        CComQIPtr<IWebBrowser2> pWB2;
        CComVariant v;

        // Create the AX host window.
        wndIE.Create ( *this, rc, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN );

        // Create the browser control using its GUID.
        wndIE.CreateControlEx ( L"{8856F961-340A-11D0-A96B-00C04FD705A2}", NULL,
                                NULL, &punkCtrl );

        /*
        NOTE: You could also use the control's ProgID: Shell.Explorer:
        wndIE.CreateControlEx ( L"Shell.Explorer", NULL,
                                NULL, &punkCtrl );
        */

        // Get an IWebBrowser2 interface on the control and navigate to a page.
        pWB2 = punkCtrl;

        if ( pWB2 )
            pWB2->Navigate ( CComBSTR("about:mozilla"), &v, &v, &v, &v );
        }
    else
        {
        // Create the AX host window. This uses the WS_EX_CLIENTEDGE extended
        // style to give the browser a sunken border.
        wndIE.Create ( *this, rc, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
                       WS_EX_CLIENTEDGE );

        // This overload treats the first parameter as the resource ID of an
        // HTML resource. It creates a browser control and navigates it to 
        // that resource.
        wndIE.CreateControl ( IDR_ABOUTPAGE );
        }

    return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return 0;
}
