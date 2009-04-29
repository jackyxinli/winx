// HellowtlDialog.h : interface of the CHellowtlDialog class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CHellowtlDialog : 
	public CAppStdDialogImpl<CHellowtlDialog>,
	public CUpdateUI<CHellowtlDialog>,
	public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_APP_DLG_CLASS(NULL, IDR_MAINFRAME, L"Software\\WTL\\Hellowtl")

	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

// CAppWindow operations
	bool AppHibernate( bool bHibernate)
	{
		// Insert your code here or delete member if not relevant
		return bHibernate;
	}

	bool AppNewInstance( LPCTSTR lpstrCmdLine)
	{
		// Insert your code here or delete member if not relevant
		return false;
	}

	void AppSave()
	{
		CAppInfo info;
		// Insert your code here or delete member if not relevant
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CHellowtlDialog)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CHellowtlDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		CHAIN_MSG_MAP(CAppStdDialogImpl<CHellowtlDialog>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{

		HWND hMenuBar = AtlCreateMenuBar(m_hWnd, ATL_IDM_MENU_DONECANCEL);
		UIAddToolBar(hMenuBar);
		UIAddChildWindowContainer(m_hWnd);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		return bHandled = FALSE;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		StdCloseDialog(IDOK);
		return 0;
	}

};
