/*****************************************************************************************

***		PerformanceLogDlg.cpp : Implementation of the CPerformanceLogDlg class

***		Developed by Langfine Ltd. 
***		Released to the public domain 12/Nov/2001.
***		Please visit our website www.langfine.com

***		Any modifications must be clearly commented to distinguish them from Langfine's 
***		original source code. Please advise Langfine of useful modifications so that we 
***		can make them generally available. 

*****************************************************************************************/


#include "stdafx.h"
#include "md5checksumtest.h"
#include "PerformanceLogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*****************************************************************************************
CONSTRUCTOR:	CPerformanceLogDlg
DESCRIPTION:	Initialises member data
ARGUMENTS:		CWnd* pParent - see MFC
NOTES:			None
*****************************************************************************************/
CPerformanceLogDlg::CPerformanceLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPerformanceLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPerformanceLogDlg)
	//}}AFX_DATA_INIT

	Reset();
}


/*****************************************************************************************
FUNCTION:		CMD5ChecksumTestDlg::DoDataExchange
DESCRIPTION:	see MFC
*****************************************************************************************/
void CPerformanceLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CString strDisplay;	//the text to be displayed

	//add the averages to the log
	strDisplay = m_strLog + CString("\n") + GetAverages();

	//display the log data to the user
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPerformanceLogDlg)
	//}}AFX_DATA_MAP

	//display the performance log in the rich edit control
	DDX_Text(pDX, IDC_EDIT_PERF_LOG, strDisplay);
}


BEGIN_MESSAGE_MAP(CPerformanceLogDlg, CDialog)
	//{{AFX_MSG_MAP(CPerformanceLogDlg)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClearLog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSaveLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/*****************************************************************************************
FUNCTION:		CPerformanceLogDlg::OnButtonClearLog
DETAILS:		protected, message map function
DESCRIPTION:	Resets all log data to its initial empty state and updates the log display.
				Displays an "Are you sure" message first. 
RETURNS:		void
ARGUMENTS:		none
NOTES:			None
*****************************************************************************************/
void CPerformanceLogDlg::OnButtonClearLog()
{
	if ( AfxMessageBox("Are you sure that you want to clear the log?", MB_YESNO) == IDYES )
	{
		//reset all log data
		Reset();

		//update the display of log data
		UpdateData(FALSE);
	}
}


/*****************************************************************************************
FUNCTION:		CPerformanceLogDlg::AddLogEntry
DETAILS:		public
DESCRIPTION:	Adds an entry to the performance log
RETURNS:		void	
ARGUMENTS:		double dSecs - the 'timing' performance data
				double dCycles - the 'number of cycles' performance data
NOTES:			This is the main accessor function exported by this class. The user calls
				this whenever a performance test is performed which has results that
				should be logged. 
*****************************************************************************************/
void CPerformanceLogDlg::AddLogEntry(double dSecs, double dCycles)
{
	//convert the log data to a string
	CString strLogEntry;
	strLogEntry.Format("Secs: %f,  Cycles: %.0f\n", dSecs, dCycles);

	//add the log data to the data for display
	m_strLog += strLogEntry;

	//increment the totals (for later use when calculating averages)
	m_dPerformanceSecsTotal += dSecs;
	m_dPerformanceCyclesTotal += dCycles;
	m_nNumOfEntries++;

	//the log now needs to be saved so set the dirty flag
	m_bDirty = true;
}


/*****************************************************************************************
FUNCTION:		CPerformanceLogDlg::OnButtonSaveLog
DETAILS:		protected, message map function
DESCRIPTION:	Saves the performance log to a file
RETURNS:		void
ARGUMENTS:		none
NOTES:			Log files are saved as text, so the file dialog is provided with "txt" as
				the default file suffix. 
*****************************************************************************************/
void CPerformanceLogDlg::OnButtonSaveLog() 
{
	//get a filename for the log
	CFileDialog FileDialog(FALSE, "txt");
	if ( FileDialog.DoModal() == IDOK )
	{
		CString strFileName = FileDialog.GetPathName();

		//write the log to the file (appending the averages)
		CStdioFile StdioFile( strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText );
		StdioFile.WriteString( m_strLog + CString("\n") + GetAverages() );

		//the log has been saved so clear the dirty flag
		m_bDirty = false;
	}
}




/*****************************************************************************************
FUNCTION:		CPerformanceLogDlg::IsEmpty()
DETAILS:		public accessor function
DESCRIPTION:	Determines wether there is log data or not
RETURNS:		Returns true if there is now log data, false otherwise
ARGUMENTS:		None
NOTES:			None
*****************************************************************************************/
bool CPerformanceLogDlg::IsEmpty() const 
{ 
	return ( m_strLog.GetLength() == 0 );
} 



/*****************************************************************************************
FUNCTION:		CPerformanceLogDlg::IsDirty() 
DETAILS:		public accessor function
DESCRIPTION:	Determines wether the log data needs to be saved to a file or not
RETURNS:		True if the log is dirty, false otherwise
ARGUMENTS:		none
NOTES:			The log is dirty if log data has been added to it since it was last saved.
				The purpose of this function is to determine wether the log needs to be 
				saved before the application exits.
*****************************************************************************************/
bool CPerformanceLogDlg::IsDirty() const 
{ 
	return m_bDirty; 
}


/*****************************************************************************************
FUNCTION:		CPerformanceLogDlg::Reset()
DETAILS:		protected
DESCRIPTION:	Resets member data to initial (zero) values
RETURNS:		void
ARGUMENTS:		none
NOTES:			Clears the log of all data. 
*****************************************************************************************/
void CPerformanceLogDlg::Reset()
{
	m_bDirty = false;
	m_nNumOfEntries = 0;
	m_dPerformanceSecsTotal = 0;
	m_dPerformanceCyclesTotal = 0;
	m_strLog.Empty();
}


/*****************************************************************************************
FUNCTION:		CPerformanceLogDlg::GetAverages()
DETAILS:		protected
DESCRIPTION:	Calculates average performance data and returns it as a formatted string
RETURNS:		The CString of performance averages
ARGUMENTS:		none
NOTES:			The timing and cycle count data are averaged and returned within the 
				CString. This string is suitable for display within the log
*****************************************************************************************/
CString CPerformanceLogDlg::GetAverages()
{
	CString strAverages;

	//if there are any entries in the log 
	if ( m_nNumOfEntries > 0 )
	{
		//calculate the average time and cycle performance values
		double dPerformanceSecsAve = m_dPerformanceSecsTotal / m_nNumOfEntries;
		double dPerformanceCyclesAve = m_dPerformanceCyclesTotal / m_nNumOfEntries;

		//add the averages to the text for display
		strAverages.Format("Ave Secs: %f,  Ave Cycles: %.2f",dPerformanceSecsAve,dPerformanceCyclesAve);
	}
	
	//return the averages text, ready for display
	return strAverages;
}


