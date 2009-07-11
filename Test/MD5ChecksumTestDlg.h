/*****************************************************************************************

***		MD5ChecksumTestDlg.h : Interface for the CMD5ChecksumTestDlg class. 

***		Developed by Langfine Ltd. 
***		Released to the public domain 12/Nov/2001.
***		Please visit our website www.langfine.com

***		Any modifications must be clearly commented to distinguish them from Langfine's 
***		original source code. Please advise Langfine of useful modifications so that we 
***		can make them generally available. 

*****************************************************************************************/


#if !defined(AFX_MD5CHECKSUMTESTDLG_H__2BC79286_4C15_11D3_B2EE_A4A60E20D2C3__INCLUDED_)
#define AFX_MD5CHECKSUMTESTDLG_H__2BC79286_4C15_11D3_B2EE_A4A60E20D2C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PerformanceLogDlg.h"

/*****************************************************************************************
CLASS:			CMD5ChecksumTestDlg
DESCRIPTION:	Demonstrates the use of the CMD5Checksum class
NOTES:			None
*****************************************************************************************/
class CMD5ChecksumTestDlg : public CDialog
{
	//{{AFX_VIRTUAL(CMD5ChecksumTestDlg)
	public:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUALpublic:

	//{{AFX_MSG(CMD5ChecksumTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSelfTest();
	afx_msg void OnChangeEditStr();
	afx_msg void OnButtonSelFile();
	afx_msg void OnButtonAbout();
	afx_msg void OnButtonPerformanceTest();
	afx_msg void OnButtonShowLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//constructor
	CMD5ChecksumTestDlg(CWnd* pParent = NULL);

protected:

	//data initialisation
	bool LoadTestDataPath();

	//button handling functions
	virtual void OnOK();
	virtual void OnCancel();
	void ChecksumSelectedFile();

	//automatic self test functions
	bool SelfTest();
	bool SelfTest01();
	bool SelfTest02();
	bool SelfTest03();
	bool SelfTest04();
	bool SelfTest05();
	bool SelfTest06();
	bool SelfTest07();
	bool SelfTest08();
	bool SelfTest09();
	bool SelfTest10();
	bool SelfTest11();
	bool SelfTest12();
	bool SelfTest13();
	bool SelfTest14();
	bool SelfTest15();
	bool SelfTest15a();
	bool SelfTest16();
	bool SelfTest17();
	bool SelfTest18();
	bool SelfTest18a();
	bool SelfTest19();
	bool SelfTest20();
	bool SelfTest21();
	bool SelfTest21a();
	bool SelfTest22();
	bool SelfTest23();

	//user interface function
	void EnableCtrls(bool bEnable = true);

	//utility functions
	BOOL UpdateDataNow(BOOL bSaveAndValidate = TRUE);
	int LoadLargeTestFile();
	void UnloadLargeTestFile();

private:
	//{{AFX_DATA(CMD5ChecksumTestDlg)
	enum { IDD = IDD_MD5CHECKSUMTEST_DIALOG };
	CString	m_strSelfTestStatus;
	CString	m_strChecksum;
	CString	m_strEntry;
	CString	m_strFileChecksum;
	CString	m_strSelectedFile;
	double m_dPerformanceSecs;
	double m_dPerformanceCycles;
	//}}AFX_DATA
	
	CPerformanceLogDlg m_PerformanceLogDlg;	//display of performance data
	BYTE* m_pTestFileBuffer;		//points to large buffer of test data 
	bool m_bTestDataFolderFound;	//true if the test data folder has been found, false otherwise
	CString m_strTestDataPath;		//pathname of the test data folder
	HICON m_hIcon;					//see MFC
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MD5CHECKSUMTESTDLG_H__2BC79286_4C15_11D3_B2EE_A4A60E20D2C3__INCLUDED_)
