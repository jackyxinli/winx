// LCDMatrixDlg.h : header file
//

#if !defined(AFX_LCDMATRIXDLG_H__2C47D633_B675_4039_9B1C_6E2178EF2315__INCLUDED_)
#define AFX_LCDMATRIXDLG_H__2C47D633_B675_4039_9B1C_6E2178EF2315__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MatrixStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CLCDMatrixDlg dialog

class CLCDMatrixDlg : public CDialog
{
// Construction
public:
	CLCDMatrixDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
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

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLCDMatrixDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LCDMATRIXDLG_H__2C47D633_B675_4039_9B1C_6E2178EF2315__INCLUDED_)
