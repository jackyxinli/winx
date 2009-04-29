// LCDMatrixDlg.h : header file
//

#if !defined(AFX_LCDMATRIXDLG_H__2C47D633_B675_4039_9B1C_6E2178EF2315__INCLUDED_)
#define AFX_LCDMATRIXDLG_H__2C47D633_B675_4039_9B1C_6E2178EF2315__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MatrixStatic.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CLCDMatrixDlg dialog

//class CLCDMatrixDlg : public CDialog //@@code modify -->
class CLCDMatrixDlg : public winx::ModelDialog<CLCDMatrixDlg, IDD_LCDMATRIX_DIALOG>
{
	WINX_ICON(IDR_MAINFRAME); //@@code insert
// Construction
public:
	//CLCDMatrixDlg(CWnd* pParent = NULL);	//@@code delete

// Dialog Data
	/*
	//{{AFX_DATA(CLCDMatrixDlg)
	enum { IDD = IDD_LCDMATRIX_DIALOG };
	CMatrixStatic	m_lcddown2;
	CMatrixStatic	m_lcdleft;
	CMatrixStatic	m_lcddown;
	CMatrixStatic	m_lcdstaticsmall;
	CMatrixStatic	m_lcdstatic;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLCDMatrixDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	*/
	//@@code modify -->

	typedef winx::MatrixStatic CMatrixStatic;

	CMatrixStatic*	m_lcddown2;
	CMatrixStatic*	m_lcdleft;
	CMatrixStatic*	m_lcddown;
	CMatrixStatic*	m_lcdstaticsmall;
	CMatrixStatic*	m_lcdstatic;


	/* @@code delete
// Implementation
protected:
	HICON m_hIcon;
	*/

	/*
	// Generated message map functions
	//{{AFX_MSG(CLCDMatrixDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	*/
	//@@code modify -->

public:
	BOOL winx_msg OnInitDialog(HWND hDlg, HWND hWndDefaultFocus);
	void winx_msg OnClose(HWND hWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LCDMATRIXDLG_H__2C47D633_B675_4039_9B1C_6E2178EF2315__INCLUDED_)
