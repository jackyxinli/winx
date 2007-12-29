// ExampleDlg.h : header file
//

#if !defined(AFX_EXAMPLEDLG_H__BB62A9D7_9B38_4812_9A77_89A506216333__INCLUDED_)
#define AFX_EXAMPLEDLG_H__BB62A9D7_9B38_4812_9A77_89A506216333__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CExampleDlg dialog

class CExampleDlg : public CDialog
{
// Construction
public:
	CExampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CExampleDlg)
	enum { IDD = IDD_EXAMPLE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CExampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMPLEDLG_H__BB62A9D7_9B38_4812_9A77_89A506216333__INCLUDED_)
