/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: winx/ext/CreditStatic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 20:26:15
// 
// $Id: CreditStatic.h,v 1.3 2006/09/16 18:06:12 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_EXT_CREDITSTATIC_H__
#define __WINX_EXT_CREDITSTATIC_H__

#ifndef __MFCPORT_AFX_H__
#include "../../mfcport/afx.h"
#endif

namespace winx {	//@@code insert

// CreditStatic.h : header file
//

#define DISPLAY_SLOW		0
#define DISPLAY_MEDIUM		1
#define DISPLAY_FAST		2

#define BACKGROUND_COLOR        0
#define TOP_LEVEL_TITLE_COLOR	1
#define TOP_LEVEL_GROUP_COLOR   2
#define GROUP_TITLE_COLOR       3
#define NORMAL_TEXT_COLOR		4

#define TOP_LEVEL_TITLE_HEIGHT	0		
#define TOP_LEVEL_GROUP_HEIGHT  1     
#define GROUP_TITLE_HEIGHT    	2     
#define	NORMAL_TEXT_HEIGHT		3

#define TOP_LEVEL_TITLE			0   // '\t'
#define TOP_LEVEL_GROUP         1   // '\n'
#define GROUP_TITLE           	2   // '\r'
#define DISPLAY_BITMAP			3   // '^'

#define GRADIENT_NONE			0
#define GRADIENT_RIGHT_DARK		1
#define GRADIENT_RIGHT_LIGHT	2
#define GRADIENT_LEFT_DARK		3
#define GRADIENT_LEFT_LIGHT		4

//class CCreditStatic : public CStatic //@@code modify -->
class CCreditStatic : public Static<CCreditStatic>
{
	WINX_CLASS("WinxCreditStatic"); //@@code insert
protected:
	typedef UINT POSITION; //@@code insert
	typedef std::vector<CString> CStringList; //@@code insert

	COLORREF    m_Colors[5];
	int         m_TextHeights[4];
    TCHAR       m_Escapes[4];
	int         m_DisplaySpeed[3],m_CurrentSpeed;
// 	CRect       m_ScrollRect;		   // rect of Static Text frame
	CStringList m_ArrCredit;
	CString		m_szWork;
	int         m_nCounter;		   // work ints
	POSITION    m_ArrIndex;
	BOOL        m_bFirstTime;
	BOOL        m_bDrawText;
	int         m_nClip,m_ScrollAmount;
	int         m_nCurrentFontHeight;

	CBitmap     m_bmpWork;                  // bitmap holder
	CBitmap		m_BmpMain;                  // bitmap holder

	CSize 		m_size;                     // drawing helpers
	CPoint 		m_pt;
	BOOL 		m_bProcessingBitmap;
	CPalette	m_pal;
	CBitmap		m_bitmap;
	int m_cxBitmap, m_cyBitmap;
	BOOL		m_bFirstTurn;
	UINT        m_Gradient;
	BOOL		m_bTransparent;
	int			n_MaxWidth;
	UINT        TimerOn;
// Construction
public:
	CCreditStatic();

// Attributes
public:

// Operations
public:
	BOOL StartScrolling();
	void EndScrolling();
	void SetCredits(LPCTSTR credits, char delimiter = '|');
	void SetCredits(UINT nID, char delimiter = '|');
	void SetSpeed(UINT index, int speed = 0);
	void SetColor(UINT index, COLORREF col);
	void SetTextHeight(UINT index, int height);
	void SetEscape(UINT index, char escape);
	void SetGradient(UINT value = GRADIENT_RIGHT_DARK);
	BOOL SetBkImage(UINT nIDResource);
	BOOL SetBkImage(LPCTSTR lpszResourceName);
	void SetTransparent(BOOL bTransparent = TRUE);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreditStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCreditStatic();

	// Generated message map functions
protected:
	void MoveCredit(CDC *pDC, CRect& r, CRect& r2, BOOL bCheck);
	void AddBackGround(CDC* pDC, CRect& m_ScrollRect, CRect& m_ClientRect);
	void DrawCredit(CDC* pDC, CRect& m_ScrollRect);
	void FillGradient(CDC *pDC, CRect *m_ScrollRect, CRect *m_FillRect, COLORREF color);
	void DrawBitmap(CDC* pDC, CDC* pDC2, CRect *rBitmap);

	//{{AFX_MSG(CCreditStatic)
	
	/*
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	*/
	
	//@@code modify -->
public:
	void winx_msg OnPaint(HWND hWnd);
	BOOL winx_msg OnEraseBkgnd(HWND hWnd, HDC hDC);
	void winx_msg OnTimer(HWND hWnd, UINT nIDEvent);
	void winx_msg OnDestroy(HWND hWnd);
	
	//}}AFX_MSG

	//DECLARE_MESSAGE_MAP() //@@code delete
};

/////////////////////////////////////////////////////////////////////////////

typedef CCreditStatic CreditStatic; //@@code insert

#include "CreditStatic.cpp"	//@@code insert

}; //@@code insert: namespace winx

// --------------------------------------------------------------------------
// $Log: CreditStatic.h,v $
// Revision 1.3  2006/09/16 18:06:12  xushiwei
// WINX_CLASS
//
// Revision 1.2  2006/08/20 04:49:57  xushiwei
// MFC-Compatibility:
//   GdiObject(CDC, CClientDC, CPaintDC, CBitmap, CPalette, etc),  Diagnost(ASSERT, VERIFY, etc)
//   CreditStatic - Demonstrate how to port MFC code to WINX --- see @@code in source code
//
// Revision 1.1  2006/08/19 17:07:15  xushiwei
// MFC-Port:
//   Demo - port MFC controls to WINX (CreditStatic, RoundButton)
//	

#endif /* __WINX_EXT_CREDITSTATIC_H__ */
