/*****************************************************************************************

***		MD5ChecksumTestDlg.cpp : Implementation of the CMD5ChecksumTestDlg class.

***		Developed by Langfine Ltd. 
***		Released to the public domain 12/Nov/2001.
***		Please visit our website www.langfine.com

***		Any modifications must be clearly commented to distinguish them from Langfine's 
***		original source code. Please advise Langfine of useful modifications so that we 
***		can make them generally available. 

*****************************************************************************************/


#include "stdafx.h"
#include "MD5ChecksumTest.h"
#include "MD5ChecksumTestDlg.h"
#include "MD5Checksum.h"
#include "MD5About.h"
#include "Timers.h"
#include "PerformanceLogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMD5ChecksumTestDlg dialog

/*****************************************************************************************
CONSTRUCTOR:	CMD5ChecksumTestDlg
DESCRIPTION:	Initialises member data
ARGUMENTS:		CWnd* pParent - see MFC
NOTES:			None
*****************************************************************************************/
CMD5ChecksumTestDlg::CMD5ChecksumTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMD5ChecksumTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMD5ChecksumTestDlg)
	m_strSelfTestStatus = _T("");
	m_strChecksum = _T("");
	m_strEntry = _T("");
	m_strFileChecksum = _T("");
	m_strSelectedFile = _T("");
	m_dPerformanceSecs = 0.0;
	m_dPerformanceCycles = 0.0;
	//}}AFX_DATA_INIT

	// initialise member data 
	m_pTestFileBuffer = NULL;

	//an empty string has a checksum, so calculate it
	m_strChecksum = CMD5Checksum::GetMD5( (BYTE*)(const char*)m_strEntry, m_strEntry.GetLength());

	//Note that LoadIcon does not require a subsequent DestroyIcon in Win32	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//find the folder containing test data file
	m_bTestDataFolderFound = LoadTestDataPath();
	if ( !m_bTestDataFolderFound )
	{
		//report an error if cannot find folder
		CString strErrMsg = CString("Could not find TestData Folder: ") +
							m_strTestDataPath +
							CString("\nSelf Test not available");
		AfxMessageBox(strErrMsg);
	}
}


/*****************************************************************************************
FUNCTION:		CMD5ChecksumTestDlg::DoDataExchange
DESCRIPTION:	see MFC
*****************************************************************************************/
void CMD5ChecksumTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMD5ChecksumTestDlg)
	DDX_Text(pDX, IDC_EDIT_SELF_TEST_STATUS, m_strSelfTestStatus);
	DDX_Text(pDX, IDC_EDIT_STR_CHECKSUM, m_strChecksum);
	DDX_Text(pDX, IDC_EDIT_STR, m_strEntry);
	DDX_Text(pDX, IDC_EDIT_FILE_CHECKSUM, m_strFileChecksum);
	DDX_Text(pDX, IDC_EDIT_SEL_FILE, m_strSelectedFile);
	DDX_Text(pDX, IDC_EDIT_PERF_TEST_RESULT, m_dPerformanceSecs);
	DDX_Text(pDX, IDC_EDIT_PERF_TEST_NUM_CYCLES, m_dPerformanceCycles );
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMD5ChecksumTestDlg, CDialog)
	//{{AFX_MSG_MAP(CMD5ChecksumTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SELFTEST, OnButtonSelfTest)
	ON_EN_CHANGE(IDC_EDIT_STR, OnChangeEditStr)
	ON_BN_CLICKED(IDC_BUTTON_SEL_FILE, OnButtonSelFile)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_PERFTEST, OnButtonPerformanceTest)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_LOG, OnButtonShowLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMD5ChecksumTestDlg message handlers

/*****************************************************************************************
FUNCTION:		CMD5ChecksumTestDlg::OnInitDialog
DETAILS:		overrides CDialog::OnInitDialog
DESCRIPTION:	see MFC
RETURNS:		see MFC
ARGUMENTS:		see MFC
NOTES:			see MFC
*****************************************************************************************/
BOOL CMD5ChecksumTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	//Set the icon for this dialog.  The framework does this automatically
	//when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//initialise dialog's controls
	EnableCtrls(true);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


/*****************************************************************************************
FUNCTION:		CMD5ChecksumTestDlg::OnSysCommand
DETAILS:		overrides CWnd::OnSysCommand
DESCRIPTION:	see MFC
RETURNS:		see MFC
ARGUMENTS:		see MFC
NOTES:			see MFC
*****************************************************************************************/
void CMD5ChecksumTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


/*****************************************************************************************
FUNCTION:		CMD5ChecksumTestDlg::OnSysCommand
DETAILS:		overrides CWnd::OnSysCommand
DESCRIPTION:	see MFC
RETURNS:		see MFC
ARGUMENTS:		see MFC
NOTES:			see MFC
				If you add a minimize button to your dialog, you will need the code below
				to draw the icon.  For MFC applications using the document/view model,
				this is automatically done for you by the framework.
*****************************************************************************************/
void CMD5ChecksumTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


/*****************************************************************************************
FUNCTION:		CMD5ChecksumTestDlg::OnQueryDragIcon
DETAILS:		overrides CWnd::OnQueryDragIcon
DESCRIPTION:	see MFC
RETURNS:		see MFC
ARGUMENTS:		see MFC
NOTES:			see MFC
				The system calls this to obtain the cursor to display while the user drags
				the minimized window.
*****************************************************************************************/
HCURSOR CMD5ChecksumTestDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}



/*****************************************************************************************
FUNCTION:		CMD5ChecksumTestDlg::UpdateDataNow
DETAILS:		protected.
DESCRIPTION:	Provides the dialog box with a message pump.
RETURNS:		Non Zero if the operation is successfull, otherwise zero
ARGUMENTS:		BOOL bSaveAndValidate - see MFC
NOTES:			By providing the dialog box with its own message pump, data displayed in
				the dialog can be updated immediately from within a CMD5ChecksumTestDlg 
				function. A normal UpdataData command is not properly processed until the 
				function that makes the UpdateData call is exited and the windows message 
				pump is entered.
*****************************************************************************************/
BOOL CMD5ChecksumTestDlg::UpdateDataNow(BOOL bSaveAndValidate)
{	
	BOOL bResult = CWnd::UpdateData(bSaveAndValidate);
	MSG msg;
	while ( PeekMessage ( &msg, (HWND) NULL, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return bResult;
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest01
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for the string "message digest"
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5("message digest") = f96b697d7cb7938d525a2f31aaf161d0
			 Tests function CMD5Checksum::GetMD5(BYTE* pBuf, UINT nLength)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest01()
{
	CString strMessageDigest("message digest");
	return CMD5Checksum::GetMD5(
				(BYTE*)(const char*)strMessageDigest,
				strMessageDigest.GetLength()
		   ) == "f96b697d7cb7938d525a2f31aaf161d0";																						    
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest02
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a file containing "message digest"
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5("message digest") = f96b697d7cb7938d525a2f31aaf161d0
			 Tests the function CMD5Checksum::GetMD5(const CString& strFilePath)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest02()
{
	CString strFile = m_strTestDataPath + CString("\\messagedigest.txt");
	return CMD5Checksum::GetMD5( strFile ) == "f96b697d7cb7938d525a2f31aaf161d0";
}



/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest03
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a file containing "message digest"
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5("message digest") = f96b697d7cb7938d525a2f31aaf161d0
			 Tests function CMD5Checksum::GetMD5(CFile& File)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest03()
{
	//test passing CFile file.
	CString strFile = m_strTestDataPath + CString("\\messagedigest.txt");
	CFile File(
		strFile,
		CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary
	);

	bool bResult = CMD5Checksum::GetMD5(File) == "f96b697d7cb7938d525a2f31aaf161d0";
	File.Close();

	return bResult;
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest04
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a file containing "abc"
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
			 Tests function CMD5Checksum::GetMD5(CFile& File)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest04()
{
	//test passing CFile file.
	CString strFile = m_strTestDataPath + CString("\\abc.txt");
	CFile File(
		strFile,
		CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary
	);

	bool bResult = CMD5Checksum::GetMD5(File) == "900150983cd24fb0d6963f7d28e17f72";
	File.Close();

	return bResult;
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest05
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for the string "abc"
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
			 Tests function CMD5Checksum::GetMD5(BYTE* pBuf, UINT nLength)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest05()
{
	CString strABC("abc");
	return CMD5Checksum::GetMD5( 
				(BYTE*)(const char*)strABC,strABC.GetLength()
		   ) == "900150983cd24fb0d6963f7d28e17f72";
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest06
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a file containing "abc"
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
			 Tests the function CMD5Checksum::GetMD5(const CString& strFilePath)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest06()
{
	CString strFile = m_strTestDataPath + CString("\\abc.txt");
	return CMD5Checksum::GetMD5( strFile ) == "900150983cd24fb0d6963f7d28e17f72";
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest07
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for an empty file
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5("") = d41d8cd98f00b204e9800998ecf8427e
			 Tests the function CMD5Checksum::GetMD5(const CString& strFilePath)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest07()
{
	CString strFile = m_strTestDataPath + CString("\\Empty.txt");
	return CMD5Checksum::GetMD5( strFile ) == "d41d8cd98f00b204e9800998ecf8427e";
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest08
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for an empty file
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5("") = d41d8cd98f00b204e9800998ecf8427e
			 Tests function CMD5Checksum::GetMD5(CFile& File)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest08()
{
	//test passing CFile file.
	CString strFile = m_strTestDataPath + CString("\\Empty.txt");
	CFile File(
		strFile,
		CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary
	);

	bool bResult = CMD5Checksum::GetMD5(File) == "d41d8cd98f00b204e9800998ecf8427e";
	File.Close();

	return bResult;
}

/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest09
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for an empty buffer of zero size
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5("") = d41d8cd98f00b204e9800998ecf8427e
			 Tests function CMD5Checksum::GetMD5(BYTE* pBuf, UINT nLength)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest09()
{
	CString strEmpty("");
	return CMD5Checksum::GetMD5( 
			(BYTE*)(const char*)strEmpty, 0
	   ) == "d41d8cd98f00b204e9800998ecf8427e";

}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest10
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a file containing "a"
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5("a") = 0cc175b9c0f1b6a831c399e269772661
			 Tests the function CMD5Checksum::GetMD5(const CString& strFilePath)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest10()
{
	CString strFile = m_strTestDataPath + CString("\\a.txt");
	return CMD5Checksum::GetMD5( strFile ) == "0cc175b9c0f1b6a831c399e269772661";
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest11
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a file containing "a"
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5("a") = 0cc175b9c0f1b6a831c399e269772661
			 Tests function CMD5Checksum::GetMD5(CFile& File)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest11()
{
	//test passing CFile file.
	CString strFile = m_strTestDataPath + CString("\\a.txt");
	CFile File(
		strFile,
		CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary
	);

	bool bResult = CMD5Checksum::GetMD5(File) == "0cc175b9c0f1b6a831c399e269772661";
	File.Close();

	return bResult;
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest12
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for the string "a"
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5("a") = 0cc175b9c0f1b6a831c399e269772661
			 Tests function CMD5Checksum::GetMD5(BYTE* pBuf, UINT nLength)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest12()
{
	CString stra("a");
	return CMD5Checksum::GetMD5( 
				(BYTE*)(const char*)stra, 1
		   ) == "0cc175b9c0f1b6a831c399e269772661";
}



/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest13
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a file containing the alphabet
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
			 Tests function CMD5Checksum::GetMD5(CFile& File)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest13()
{
	CString strFile = m_strTestDataPath + CString("\\Alpha.txt");
	//test passing CFile file.
	CFile File(
		strFile,
		CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary
	);

	bool bResult = CMD5Checksum::GetMD5(File) == "c3fcd3d76192e4007dfb496cca67e13b";
	File.Close();

	return bResult;
}



/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest14
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a file containing the alphabet
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
			 Tests the function CMD5Checksum::GetMD5(const CString& strFilePath)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest14()
{
	CString strFile = m_strTestDataPath + CString("\\Alpha.txt");
	return CMD5Checksum::GetMD5( strFile ) == "c3fcd3d76192e4007dfb496cca67e13b";
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest15
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a string containing the alphabet
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
			 Tests function CMD5Checksum::GetMD5(BYTE* pBuf, UINT nLength)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest15()
{
	CString strAlpha("abcdefghijklmnopqrstuvwxyz");
	return CMD5Checksum::GetMD5( 
				(BYTE*)(const char*)strAlpha, 26
		   ) == "c3fcd3d76192e4007dfb496cca67e13b";
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest15a
DETAILS:	 protected
DESCRIPTION: Checks the SelfTest15 calculation for a corrupted alphabet
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("abcdefghijklMnopqrstuvwxyz") != c3fcd3d76192e4007dfb496cca67e13b
			 Tests function CMD5Checksum::GetMD5(BYTE* pBuf, UINT nLength)
			 Checks that if one of the letters in the alphabet (in this case 'm') is
			 changed to uppercase, then a different checksum to that calculated in 
			 SelfTest15 is obtained.
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest15a()
{
	CString strAlpha("abcdefghijklMnopqrstuvwxyz");
	return CMD5Checksum::GetMD5( 
				(BYTE*)(const char*)strAlpha, 26
		   ) != "c3fcd3d76192e4007dfb496cca67e13b";
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest16
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a file containing the alphabet and numbers
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") =
			 d174ab98d277d9f5a5611c2c9f419d9f
			 Tests function CMD5Checksum::GetMD5(CFile& File)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest16()
{
	CString strFile = m_strTestDataPath + CString("\\AlphaNumeric.txt");
	//test passing CFile file.
	CFile File(
		strFile,
		CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary
	);

	bool bResult = CMD5Checksum::GetMD5(File) == "d174ab98d277d9f5a5611c2c9f419d9f";
	File.Close();

	return bResult;
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest17
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a file containing the alphabet and numbers
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") =
			 d174ab98d277d9f5a5611c2c9f419d9f
			 Tests the function CMD5Checksum::GetMD5(const CString& strFilePath)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest17()
{
	CString strFile = m_strTestDataPath + CString("\\AlphaNumeric.txt");
	return CMD5Checksum::GetMD5( strFile ) == "d174ab98d277d9f5a5611c2c9f419d9f";
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest18
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a string containing the alphabet and numbers
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") =
			 d174ab98d277d9f5a5611c2c9f419d9f
			 Tests function CMD5Checksum::GetMD5(BYTE* pBuf, UINT nLength)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest18()
{
	CString strAlphaNumeric("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	return CMD5Checksum::GetMD5( 
				(BYTE*)(const char*)strAlphaNumeric, strAlphaNumeric.GetLength()
		   ) == "d174ab98d277d9f5a5611c2c9f419d9f";
}

/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest18a
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a corrupted SelfTest18 string 
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345678") !=
			 d174ab98d277d9f5a5611c2c9f419d9f
			 Tests function CMD5Checksum::GetMD5(BYTE* pBuf, UINT nLength)
			 Checks that if the final digit in the SelfTest18 string is removed, then a
			 different checksum is calculated
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest18a()
{
	CString strAlphaNumeric("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345678");
	return CMD5Checksum::GetMD5( 
				(BYTE*)(const char*)strAlphaNumeric, strAlphaNumeric.GetLength()
		   ) != "d174ab98d277d9f5a5611c2c9f419d9f";
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest19
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a file containing  numbers
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("123456789012345678901234567890123456789012345678901234567890123456
			 78901234567890") = 57edf4a22be3c955ac49da2e2107b67a
			 Tests the function CMD5Checksum::GetMD5(const CString& strFilePath)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest19()
{	
	CString strFile = m_strTestDataPath + CString("\\Numeric.txt");
	return CMD5Checksum::GetMD5( strFile ) == "57edf4a22be3c955ac49da2e2107b67a";
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest20
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a file containing  numbers
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("123456789012345678901234567890123456789012345678901234567890123456
			 78901234567890") = 57edf4a22be3c955ac49da2e2107b67a
			 Tests function CMD5Checksum::GetMD5(CFile& File)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest20()
{
	CString strFile = m_strTestDataPath + CString("\\Numeric.txt");

	//test passing CFile file.
	CFile File(
		strFile,
		CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary
	);

	bool bResult = CMD5Checksum::GetMD5(File) == "57edf4a22be3c955ac49da2e2107b67a";
	File.Close();

	return bResult;
}

/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest21
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a string containing  numbers
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("123456789012345678901234567890123456789012345678901234567890123456
			 78901234567890") = 57edf4a22be3c955ac49da2e2107b67a
			 Tests function CMD5Checksum::GetMD5(BYTE* pBuf, UINT nLength)
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest21()
{
	CString strNumeric(
		"12345678901234567890123456789012345678901234567890123456789012345678901234567890"
	);

	return CMD5Checksum::GetMD5( 
				(BYTE*)(const char*)strNumeric, strNumeric.GetLength()
		   ) == "57edf4a22be3c955ac49da2e2107b67a";

}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest21a
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a corrupted SelfTest21 string
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("23456789012345678901234567890123456789012345678901234567890123456
			 78901234567890") != 57edf4a22be3c955ac49da2e2107b67a
			 Tests function CMD5Checksum::GetMD5(BYTE* pBuf, UINT nLength)
			 The string is corrupted by removing the first digit.
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest21a()
{
	CString strNumeric(
		"2345678901234567890123456789012345678901234567890123456789012345678901234567890"
	);

	return CMD5Checksum::GetMD5( 
				(BYTE*)(const char*)strNumeric, strNumeric.GetLength()
		   ) != "57edf4a22be3c955ac49da2e2107b67a";

}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest22
DETAILS:	 protected
DESCRIPTION: Checks the MD5 checksum calculation for a 50kb file
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 MD5 ("MD5 Test Data File.txt"") == eaf2d7238637e95bbf666fa0b98fa64f
			 Tests the function CMD5Checksum::GetMD5(const CString& strFilePath)
			 NB. Modifications to this implementation of the MD5 algorithm may work	
			 correctly for small files or strings, but not for larger amounts of data.
			 This test on a 50Kb file help checks correctness in this respect. 
******************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest22()
{
	CString strFile = m_strTestDataPath + CString("\\MD5 Test Data File.txt");
	return ( CMD5Checksum::GetMD5( strFile ) == "eaf2d7238637e95bbf666fa0b98fa64f" );
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::LoadLargeTestFile
DETAILS:	 protected
DESCRIPTION: Loads into a buffer the data from "MD5 Test Data File 1.txt" file
RETURNS:	 Size (in bytes) of the large test file
ARGUMENTS:	 none
NOTES:		 none
******************************************************************************************/
int CMD5ChecksumTestDlg::LoadLargeTestFile()
{
	// entry invariant
	ASSERT( m_pTestFileBuffer == NULL );

	//open the file as a binary file in readonly mode, denying write access 
	CString strFilePath = m_strTestDataPath + CString("\\MD5 Test Data File.txt");
	CFile File(strFilePath, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary);

	// read the file contents into a buffer
	const int knBufferSize = 51200;
	m_pTestFileBuffer = new BYTE[knBufferSize];
	int nLength = File.Read( m_pTestFileBuffer, knBufferSize );
	ASSERT( nLength == knBufferSize );
	return nLength;
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::UnloadLargeTestFile
DETAILS:	 protected
DESCRIPTION: Frees the memory allocated for the large test file.
RETURNS:	 none
ARGUMENTS:	 none
NOTES:		 none
******************************************************************************************/
void CMD5ChecksumTestDlg::UnloadLargeTestFile()
{
	delete m_pTestFileBuffer;
	m_pTestFileBuffer = NULL;
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest23
DETAILS:	 protected
DESCRIPTION: This is similar to SelfTest22. However, the large test file is first loaded 
			 into a buffer. A pointer to the buffer is passed to the MD5 function.
RETURNS:	 bool : true if checksum calculated correctly, false otherwise
ARGUMENTS:	 none
NOTES:		 none
******************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest23()
{
	const int nTestFileSize = LoadLargeTestFile();

	bool bOK = ( CMD5Checksum::GetMD5( m_pTestFileBuffer, nTestFileSize ) == "eaf2d7238637e95bbf666fa0b98fa64f" );

	UnloadLargeTestFile();

	return bOK;
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::SelfTest
DETAILS:	 private, message map function for Self Test Button
DESCRIPTION: Calls a series of test functions to check the correctness of CMD5Checksum
RETURNS:	 void
ARGUMENTS:	 none
NOTES:		 The following tests are performed:-
			 MD5 ("") = d41d8cd98f00b204e9800998ecf8427e
			 MD5 ("a") = 0cc175b9c0f1b6a831c399e269772661
			 MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
			 MD5 ("message digest") = f96b697d7cb7938d525a2f31aaf161d0
			 MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
			 MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") 
					= d174ab98d277d9f5a5611c2c9f419d9f
			 MD5 ("12345678901234567890123456789012345678901234567890123456789012345678901234567890") 
					= 57edf4a22be3c955ac49da2e2107b67a
			 MD5 ( MD5 Test Data File.txt ) = 9f9fb64291dbb5088abc465badb42937
			 All three of the CMD5Checksum::GetMD5 checksums are called for each of the
			 test strings. 
*****************************************************************************************/
bool CMD5ChecksumTestDlg::SelfTest()
{
	bool bOK = true;

	try
	{
		bOK = bOK && SelfTest01();
		bOK = bOK && SelfTest02();
		bOK = bOK && SelfTest03();
		bOK = bOK && SelfTest04();
		bOK = bOK && SelfTest05();
		bOK = bOK && SelfTest06();
		bOK = bOK && SelfTest07();
		bOK = bOK && SelfTest08();
		bOK = bOK && SelfTest09();
		bOK = bOK && SelfTest10();
		bOK = bOK && SelfTest11();
		bOK = bOK && SelfTest12();
		bOK = bOK && SelfTest13();
		bOK = bOK && SelfTest14();
		bOK = bOK && SelfTest15();
		bOK = bOK && SelfTest15a();
		bOK = bOK && SelfTest16();
		bOK = bOK && SelfTest17();
		bOK = bOK && SelfTest18();
		bOK = bOK && SelfTest18a();
		bOK = bOK && SelfTest19();
		bOK = bOK && SelfTest20();
		bOK = bOK && SelfTest21();
		bOK = bOK && SelfTest21a();
		bOK = bOK && SelfTest22();
		bOK = bOK && SelfTest23();
	}
	catch(CFileException* e)
	{
		CString strErrMsg = CString("Problem with file:\n") + e->m_strFileName;
		AfxMessageBox(strErrMsg);
		bOK = false;
	}
	
	return bOK;
}



/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::OnButtonSelfTest
DETAILS:	 private, message map function for Self Test Button
DESCRIPTION: Calls the selftest function to check the correctness of CMD5Checksum. 
			 Displays the result to the user. 
RETURNS:	 void
ARGUMENTS:	 none
NOTES:		 none
*****************************************************************************************/
void CMD5ChecksumTestDlg::OnButtonSelfTest() 
{
	ASSERT( m_bTestDataFolderFound );

	//tell the user to wait for the test to complete
	m_strSelfTestStatus.LoadString(IDS_PLEASE_WAIT);
	EnableCtrls(false);
	UpdateDataNow(FALSE);

	//perform the selftest and display the result to the user
	m_strSelfTestStatus = ( SelfTest() ? CString("All Pass") : CString("Fail") );
	EnableCtrls(true);
	UpdateDataNow(FALSE);
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::OnChangeEditStr
DETAILS:	 private, message map function 
DESCRIPTION: Gets and displays the MD5 checksum for a user entered string
RETURNS:	 void
ARGUMENTS:	 none
NOTES:		 This function is called whenever the user enters data in the 
			 "Enter a string and see the checksum" edit box. This function is called as
			 soon as the user enters any data, so the user will see the checksum change
			 as each individual character of the string is entered. 
*****************************************************************************************/
void CMD5ChecksumTestDlg::OnChangeEditStr() 
{
	//get the user entered data
	UpdateData();

	//calculate the checksum for the user entered string
	m_strChecksum = CMD5Checksum::GetMD5( (BYTE*)(const char*)m_strEntry, m_strEntry.GetLength() );

	//update the dialog box with the newly calculated checksum
	UpdateData(FALSE);
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::OnButtonSelFile
DETAILS:	 private, message map function for File Selection Button
DESCRIPTION: Gets and displays the MD5 checksum for a user selected file
RETURNS:	 void
ARGUMENTS:	 none
NOTES:		 Displays the Windows File Open Dialog to allow the user to select a file. The
			 MD5 checksum for the file is then calculated and displayed using the 
			 ChecksumSelectedFile function. Controls on the dialog are disabled while the 
			 checksum is being calculated. The ChecksumSelectedFile function can throw the 
			 CFileException (as it calls GetMD5 which tries open the user selected file.) 
			 Typically this will occur if the user selects a file which is already open in 
			 another application, in which case a sharing violation will occur. 
*****************************************************************************************/
void CMD5ChecksumTestDlg::OnButtonSelFile() 
{
	//display the file open dialog
	CFileDialog	FileDialog( TRUE );
	if ( FileDialog.DoModal() == IDOK )
	{
		//get the full path and name of the file and checksum it
		m_strSelectedFile = FileDialog.GetPathName();
		ChecksumSelectedFile();
	}
}

/*****************************************************************************************
FUNCTION:		CMD5ChecksumTestDlg::OnButtonAbout
DETAILS:		public	
DESCRIPTION:	Displays the Help-About dialog
RETURNS:		void
ARGUMENTS:		none
NOTES:			none
*****************************************************************************************/
void CMD5ChecksumTestDlg::OnButtonAbout() 
{
	CAboutDlg AboutDlg;
	AboutDlg.DoModal();
}


/*****************************************************************************************
FUNCTION:		CMD5ChecksumTestDlg::EnableCtrls
DETAILS:		protected
DESCRIPTION:	Enable or disables user interface ccontrols
RETURNS:		void
ARGUMENTS:		bool bEnable - enable or disable the controls
NOTES:			Controls need to be disabled when a checksum calculation is being performed 
				(because they will not respond while the calculation is being performed - 
				the program is not multi-threaded!)
*****************************************************************************************/
void CMD5ChecksumTestDlg::EnableCtrls(bool bEnable /*= true */)
{
	GetDlgItem(IDC_BUTTON_SELFTEST)->EnableWindow(bEnable && m_bTestDataFolderFound);
	GetDlgItem(IDC_BUTTON_SEL_FILE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_ABOUT)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_STR)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SEL_FILE)->EnableWindow(bEnable);
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::OnOK
DETAILS:	 protected, message map function for IDOK
DESCRIPTION: Checksums the entry in the "Select a File.." edit box when "Enter" is pressed.
RETURNS:	 void
ARGUMENTS:	 none
NOTES:		 Called whenever the Return (or Enter) key is pressed when the 
			 "IDC_EDIT_SEL_FILE"  edit box currently has focus. This function allows the 
			 user to enter a file name manually and then initiate the checksum on the file 
			 by pressing 'Enter'. 
*****************************************************************************************/
void CMD5ChecksumTestDlg::OnOK()
{

	//if there is an entry in the "Select a file..." edit box and it is the selected control
	UpdateData();
	if ( m_strSelectedFile.GetLength() > 0 && GetFocus() == GetDlgItem(IDC_EDIT_SEL_FILE) )
	{
		//get its checksum
		ChecksumSelectedFile();
	}
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::OnCancel
DETAILS:	 protected, virtual, overrides CDialog::OnCancel
DESCRIPTION: Exits the application after user confirmation
RETURNS:	 void
ARGUMENTS:	 none
NOTES:		 This function is called when the user attempts to exit the application.
			 Its main purpose is to prevent accidental termination should the user
			 mistakenly press the 'Esc' key. It also warns the user if the performance log
			 (if any) has not yet been saved. 
*****************************************************************************************/
void CMD5ChecksumTestDlg::OnCancel()
{
	bool bExit = false;

	if ( m_PerformanceLogDlg.IsDirty() )
	{
		bExit = (AfxMessageBox("The performance log has not been saved.\nExit without saving?", MB_YESNO) == IDYES );
	}
	else
	{
		bExit = ( AfxMessageBox("Do you want to exit?", MB_YESNO ) == IDYES );
	}

	if ( bExit )
	{
		CDialog::OnCancel();
	}
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::ChecksumSelectedFile
DETAILS:	 protected
DESCRIPTION: Calculates and displays the checksum for the file named in 'm_strSelectedFile'.
RETURNS:	 void
ARGUMENTS:	 none
NOTES:		 The function does not check that m_strSelectedFile represents a valid file
			 name before attempting to get the checksum. If the filename is not valid,
			 GetMD5 throws a CFileException that this function catches. 
*****************************************************************************************/
void CMD5ChecksumTestDlg::ChecksumSelectedFile()
{
	//check that a file has been selected
	ASSERT( m_strSelectedFile.GetLength() > 0 );

	//display a wait message while the checksum is calculated
	m_strFileChecksum.LoadString(IDS_PLEASE_WAIT);
	EnableCtrls(false);
	UpdateDataNow(FALSE);

	//calculate the checksum
	try 
	{
		m_strFileChecksum = CMD5Checksum::GetMD5( m_strSelectedFile );
	}

	//tell the user the calculation failed if a CFileException is thrown
	catch(CFileException* e) 
	{
		m_strFileChecksum = CString("Fail");
		EnableCtrls(true);
		UpdateData(FALSE);
		throw e;
	}

	//no exception was thrown - display the checksum result 
	EnableCtrls(true);
	UpdateData(FALSE);
}

/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::LoadTestDataPath
DETAILS:	 protected
DESCRIPTION: Loads 'm_strTestDataPath' with the pathname of the test data folder.
RETURNS:	 bool : true if the folder is found, false otherwise
ARGUMENTS:	 none
NOTES:		 The data files used in the Self Test function are held in a folder "TestData".
			 In the target environment (ie, a non-development environment), this folder 
			 should be located in the same folder as the executable. However, in a
			 development environment, the executable will be held in either the "debug" or
			 "release" folder, while the "TestData' folder will be held one level higher, ie,
			 with the programs source code. This function determines the path of the TestData
			 folder, taking account of three scenarios;
			 1) the release version of the program is being run standalone from the 
				"Release" folder
			 2) the debug version of the program is being run standalone from the 
				"Debug" folder
			 3) the release or debug program is being run from within the development 
				environment
*****************************************************************************************/
bool CMD5ChecksumTestDlg::LoadTestDataPath()
{
	const CString strTestDataFolder("\\TestData");

	//get the current folder
	char szCurDir[512];
	GetCurrentDirectory(512,szCurDir);
	CString strCurDir(szCurDir);

	//if the release version of the program is being run standalone from the "Release" folder
	if (strCurDir.Right(7) == "Release")
	{
		m_strTestDataPath = strCurDir.Left( strCurDir.GetLength() - 7 ) + strTestDataFolder;
	}
	else
	//if the debug version of the program is being run standalone from the "Debug" folder
	if (strCurDir.Right(5) == "Debug")
	{
		m_strTestDataPath = strCurDir.Left( strCurDir.GetLength() - 5 ) + strTestDataFolder;
	}
	else
	{
		//program is being run from same folder as \\TestData
		m_strTestDataPath = strCurDir + CString("\\TestData");
	}

	//check the folder exists (nb, no need to check it is an actual folder.)
	CFileFind FileFind;
	return ( FileFind.FindFile(m_strTestDataPath) ? true : false );
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::OnButtonPerformanceTest
DETAILS:	 public message map function
DESCRIPTION: Times the calculation of several MD5 checksum calculations
RETURNS:	 none
ARGUMENTS:	 none
NOTES:		 This function times a fixed sequence of MD5 checksum calculations. Its 
			 purpose is to facilitate fine tuning of the implementation for improved 
			 performance. Comparison of performance figures provided by this function
			 will indicate whether modifications have achieved a measurable performance
			 improvement.
			 Two types of 'timer' are used. CTimerSecs returns the elapsed time in seconds.
			 'CTimerCycles' returns the number of elapsed processor cycles. The tests are 
			 run twice, once for each type of timer.
			 Each test consists of a sequence of short tests followed by one long test. 
			 The short tests are performed 50 times to ensure that performance improvements
			 that relate mostly to short calculations are reflected in the test results (other-
			 wise the time taken to perform them would be insignificant compared to the long
			 test.) The long test is performed by first loading the contents of the test 
			 data into memory (this avoids the file read operation influencing the 
			 performance). After the long test, the relevant memory is freed. 
			 Test results are saved in a log and may be displayed on screen. The log 
			 may also be saved to disc as a text file. 
			 NB. This function makes no attempt to change its threads priority. Thus, it
			 could be interrupted by other processor activities. To ensure consistency and 
			 comparison of like with like, run tests in the same 'clean' environment. This 
			 is best achieved by rebooting windows, closing all unnecessary programs and 
			 services, and then running the test. Consecutive timings of the same sequence 
			 of individual tests can vary by up to 15%. It is therefore necessary to perform 
			 the tests repeatedly and average the results. The performance dialog provides 
			 an automatic summation and averaging facility. 
*****************************************************************************************/
void CMD5ChecksumTestDlg::OnButtonPerformanceTest() 
{
	// show the user a wait cursor while the performance test executes
	CWaitCursor WaitCursor;

	// if the algorithm passes self test
	if (SelfTest())
	{
		// read the test file into memory
		const int nTestFileSize = LoadLargeTestFile();

		// start the seconds timer
		CTimerSecs TimerSecs;
		TimerSecs.Start();	
	
		// perform a sequence of short tests
		for (int i=0; i<50; i++)
		{
			SelfTest01();
			SelfTest05();
			SelfTest09();
			SelfTest12();
			SelfTest15();
			SelfTest15a();
			SelfTest18();
			SelfTest18a();
			SelfTest21();
			SelfTest21a();
		}
		
		// perform the long test
		CMD5Checksum::GetMD5( m_pTestFileBuffer, nTestFileSize );
	
		// stop the seconds timer
		m_dPerformanceSecs = TimerSecs.Stop();
	
		// start the cycles timer
		CTimerCycles TimerCycles;
	    TimerCycles.Start();

		// perform a sequence of short tests
		for (int ii=0; i<50; i++);
		{
			SelfTest01();
			SelfTest05();
			SelfTest09();
			SelfTest12();
			SelfTest15();
			SelfTest15a();
			SelfTest18();
			SelfTest18a();
			SelfTest21();
			SelfTest21a();
		}
		
		// perform the long test
		CMD5Checksum::GetMD5( m_pTestFileBuffer, nTestFileSize );

		// stop the cycles timer
		m_dPerformanceCycles = TimerCycles.Stop();
	
		// add the results of both timers to the performance log
		m_PerformanceLogDlg.AddLogEntry(m_dPerformanceSecs, m_dPerformanceCycles);

		// free the allocated memory 
		UnloadLargeTestFile();
	}
	else
	{
		// if the algorithm failed its self test, report this 
		m_dPerformanceSecs = -1.0;
		AfxMessageBox("Self Test Failed.");
	}

	// refresh the display of results
	UpdateData(FALSE);
}


/*****************************************************************************************
FUNCTION:	 CMD5ChecksumTestDlg::OnButtonShowLog
DETAILS:	 public message map function
DESCRIPTION: Displays the performance log dialog
RETURNS:	 none
ARGUMENTS:	 none
NOTES:		 none 
*****************************************************************************************/
void CMD5ChecksumTestDlg::OnButtonShowLog() 
{
	m_PerformanceLogDlg.DoModal();
}



