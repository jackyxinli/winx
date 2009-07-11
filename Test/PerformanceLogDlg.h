/*****************************************************************************************

***		PerformanceLogDlg.h : Interface for the  CPerformanceLogDlg class

***		Developed by Langfine Ltd. 
***		Released to the public domain 12/Nov/2001.
***		Please visit our website www.langfine.com

***		Any modifications must be clearly commented to distinguish them from Langfine's 
***		original source code. Please advise Langfine of useful modifications so that we 
***		can make them generally available. 

*****************************************************************************************/

#if !defined(AFX_PERFORMANCELOGDLG_H__898B6177_6154_4ADB_A57E_D73DCD692C61__INCLUDED_)
#define AFX_PERFORMANCELOGDLG_H__898B6177_6154_4ADB_A57E_D73DCD692C61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*****************************************************************************************
CLASS:			CPerformanceLogDlg
DESCRIPTION:	Displays a log of performance data. Each entry consists of a two values; 
				a "Seconds" value and a "Cycles" value. When displayed, the log shows each
				entry together with a final line that shows the average of all displayed 
				entries. The facility to save the log as a plain text file is provided. 
NOTES:			None
*****************************************************************************************/
class CPerformanceLogDlg : public CDialog
{
public:
	//constructor
	CPerformanceLogDlg(CWnd* pParent = NULL);

	//public functions
	void AddLogEntry(double dSecs, double dCycles);
	bool IsEmpty() const;
	bool IsDirty() const;

	//{{AFX_DATA(CPerformanceLogDlg)
	enum { IDD = IDD_PERF_LOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPerformanceLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPerformanceLogDlg)
	afx_msg void OnButtonClearLog();
	afx_msg void OnButtonSaveLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//implementation functions
	CString GetAverages();
	void Reset();

private:
		
	//log data 
	CString m_strLog;				//holds current log text for display
	bool m_bDirty;					//true if log data has not been saved since it was last changed
	int m_nNumOfEntries;			//the number of entries currently held in the log

	//used to calculate performance averages
	double m_dPerformanceSecsTotal;	
	double m_dPerformanceCyclesTotal;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERFORMANCELOGDLG_H__898B6177_6154_4ADB_A57E_D73DCD692C61__INCLUDED_)
