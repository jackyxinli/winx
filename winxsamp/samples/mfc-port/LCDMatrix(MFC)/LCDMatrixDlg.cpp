// LCDMatrixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LCDMatrix.h"
#include "LCDMatrixDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLCDMatrixDlg dialog

CLCDMatrixDlg::CLCDMatrixDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLCDMatrixDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLCDMatrixDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLCDMatrixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLCDMatrixDlg)
	DDX_Control(pDX, IDC_MATRIXDOWN2, m_lcddown2);
	DDX_Control(pDX, IDC_LCDLEFT, m_lcdleft);
	DDX_Control(pDX, IDC_MATRIXDOWN, m_lcddown);
	DDX_Control(pDX, IDC_MATRIXSMALL, m_lcdstaticsmall);
	DDX_Control(pDX, IDC_LCDSTATIC, m_lcdstatic);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLCDMatrixDlg, CDialog)
	//{{AFX_MSG_MAP(CLCDMatrixDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLCDMatrixDlg message handlers

BOOL CLCDMatrixDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	CString tmpstr;
	// TODO: Add extra initialization here

	m_lcdstatic.SetNumberOfLines(1);
	m_lcdstatic.SetXCharsPerLine(18);
	m_lcdstatic.SetSize(CMatrixStatic::LARGE);
	m_lcdstatic.SetDisplayColors(RGB(0, 0, 0), RGB(0, 255, 50), RGB(0, 103, 30));
	m_lcdstatic.AdjustClientXToSize(18);
	m_lcdstatic.AdjustClientYToSize(1);
	m_lcdstatic.SetText(_T("Scroll Right .."));
	m_lcdstatic.SetAutoPadding(true);			//demonstrates auto padding with defalt character
	m_lcdstatic.DoScroll(300, CMatrixStatic::RIGHT);

	m_lcdleft.SetNumberOfLines(1);
	m_lcdleft.SetXCharsPerLine(18);
	m_lcdleft.SetSize(CMatrixStatic::LARGE);
	m_lcdleft.SetDisplayColors(RGB(0, 0, 0), RGB(255, 60, 0), RGB(103, 30, 0));
	m_lcdleft.AdjustClientXToSize(18);
	m_lcdleft.AdjustClientYToSize(1);
	m_lcdleft.SetText(_T("Scroll Left .."));
	m_lcdleft.SetAutoPadding(true, '!');			//demonstrates auto padding with different character
	m_lcdleft.DoScroll(150, CMatrixStatic::LEFT);

	m_lcdstaticsmall.SetNumberOfLines(3);
	m_lcdstaticsmall.SetXCharsPerLine(32);
	m_lcdstaticsmall.SetSize(CMatrixStatic::SMALL);
	m_lcdstaticsmall.AdjustClientXToSize(32);
	m_lcdstaticsmall.AdjustClientYToSize(3);
	m_lcdstaticsmall.SetText(_T(" !\"#$%&'()*+,-./0123456789;:<=>?@ABCDEFGHIJKLMNO PQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"));
	m_lcdstaticsmall.DoScroll(400, CMatrixStatic::RIGHT);
	
	m_lcddown.SetNumberOfLines(3);
	m_lcddown.SetXCharsPerLine(16);
	m_lcddown.SetSize(CMatrixStatic::TINY);
	m_lcddown.SetDisplayColors(RGB(165, 181, 66), RGB(0, 0, 0), RGB(148, 156, 66));
	m_lcddown.AdjustClientXToSize(16);
	m_lcddown.AdjustClientYToSize(3);
	m_lcddown.SetText(_T("    LCDMATRIX                          by                          Nic Wilson                   "));
	m_lcddown.DoScroll(600, CMatrixStatic::UP);

	m_lcddown2.SetNumberOfLines(3);
	m_lcddown2.SetXCharsPerLine(16);
	m_lcddown2.SetSize(CMatrixStatic::SMALL);
	m_lcddown2.SetDisplayColors(RGB(0, 0, 0), RGB(255, 181, 63), RGB(103, 64, 23));
	m_lcddown2.AdjustClientXToSize(16);
	m_lcddown2.AdjustClientYToSize(3);
	m_lcddown2.SetText(_T("                   Scroll Down                  "));
	m_lcddown2.DoScroll(600, CMatrixStatic::DOWN);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLCDMatrixDlg::OnPaint() 
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLCDMatrixDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CLCDMatrixDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_lcdstatic.StopScroll();
	m_lcdleft.StopScroll();
	m_lcddown.StopScroll();
	m_lcddown2.StopScroll();
CDialog::OnClose();
}
