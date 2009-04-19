// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "example.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	CString strCredits = "\tCAboutCtrl Example\n\n"
				   "\rProgrammed by:\n"
				   "Pablo van der Meer\n\n"
				   "\rSpecial thanks to:\nRoy Valkenhoff\nRuud van Gilst\nPatrick ten Hove\nLodewijk Wetsteijn\nNicole de Keijzer\nand Petra\n\n"
				   "\rCopyright © 2002\n\rPablo Software Solutions\n"
				   "\rAll right reserved.\n\n"
				   "http:\\www.pablovandermeer.nl\n";

	m_AboutCtrl.SetCredits(strCredits);
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_ABOUTCTRL, m_AboutCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers
