// IEHost.cpp : main source file for IEHost.exe
//

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"
#include "maindlg.h"

CAppModule _Module;

// Override of CComObjectStack that doesn't freak out and assert when 
// the IUnknown methods are called.
template <class Base>
class CComObjectStack2 : public CComObjectStack<Base>
{
public:
    CComObjectStack2() : CComObjectStack<Base>()
    { }
    ~CComObjectStack2()
    { }

    STDMETHOD_(ULONG, AddRef)() { return 1; }
    STDMETHOD_(ULONG, Release)() { return 1; }

    STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
    { 
        return _InternalQueryInterface(iid, ppvObject); 
    }
};

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

CComObjectStack2<CMainDlg> dlgMain;

    if ( dlgMain.Create(NULL) == NULL )
        {
        ATLTRACE(_T("Main dialog creation failed!\n"));
        return 0;
        }

    dlgMain.ShowWindow(nCmdShow);

    int nRet = theLoop.Run();

    _Module.RemoveMessageLoop();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    HRESULT hRes = ::CoInitialize(NULL);
    ATLASSERT(SUCCEEDED(hRes));

    // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
    ::DefWindowProc(NULL, 0, 0, 0L);

    AtlInitCommonControls ( ICC_WIN95_CLASSES );

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    AtlAxWinInit();

    int nRet = Run(lpstrCmdLine, nCmdShow);

    _Module.Term();
    ::CoUninitialize();

    return nRet;
}
