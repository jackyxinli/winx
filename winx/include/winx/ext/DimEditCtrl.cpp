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
// Module: winx/ext/DimEditCtrl.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 20:26:15
// 
// $Id: DimEditCtrl.cpp,v 1.2 2006/09/16 18:26:20 xushiwei Exp $
// -----------------------------------------------------------------------*/

/*|*\
|*|  File:      DimEditCtrl.cpp
|*|  
|*|  By:        James R. Twine, TransactionWorks, Inc.
|*|             Copyright 2000, TransactionWorks, inc.
|*|  Date:      Thursday, September 21, 2000
|*|             
|*|  Notes:     This Is The Implementation Of A "Dim Edit Control".
|*|             It Provides Visual Instructions Within The Edit
|*|             Control Itself.  It Can Be Used To Indicate Special
|*|             Properties Of A Edit Control Used On A Crowded
|*|             Interface
|*|             
|*|             May Be Freely Incorporated Into Projects Of Any Type
|*|             Subject To The Following Conditions:
|*|             
|*|             o This Header Must Remain In This File, And Any
|*|               Files Derived From It
|*|             o Do Not Misrepresent The Origin Of This Code
|*|               (IOW, Do Not Claim You Wrote It)
|*|             
|*|             A "Mention In The Credits", Or Similar Acknowledgement,
|*|             Is *NOT* Required.  It Would Be Nice, Though! :)
\*|*/

/*
#include "stdafx.h"
#include "DimEdit.h"
#include "DimEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/
//--> @@code delete

/////////////////////////////////////////////////////////////////////////////
// CDimEditCtrl

inline CDimEditCtrl::CDimEditCtrl() :
	m_bShowDimText( true ),									// Set The Dim Flag
//	m_cRedOS( -0x40 ), 										// Set The Default Dim Offset Colors
//	m_cGreenOS( -0x40 ),									// Set The Default Dim Offset Colors
//	m_cBlueOS( -0x40 ),										// Set The Default Dim Offset Colors
//	m_bUseDimOffset( true ),								// Use The Offset Colors
	m_iDimTextLen( 0 ),										// No Dim Text Set Yet
	m_crDimTextColor( RGB( 0x00, 0x00, 0x00 ) )				// No "Hard" Dim Text Color
{
	m_caDimText[ 0 ] = _T( '\0' );							// Terminate The Buffer
	SetDimOffset( -0x40, -0x40, -0x40 );					// Set The Dim Offset

	return;													// Done!
}

/*
BEGIN_MESSAGE_MAP(CDimEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CDimEditCtrl)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnSetfocus)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()
*/
//--> @@code delete


/////////////////////////////////////////////////////////////////////////////
// CDimEditCtrl message handlers

//void	CDimEditCtrl::PreSubclassWindow() 
inline LRESULT CDimEditCtrl::OnSubclass(HWND hWnd)
{
	//CEdit::PreSubclassWindow();							// Do Default...

	SetShowDimControl( true );								// Default To Show The Dim Control
		
	return 0;												// Done!
}


inline void	CDimEditCtrl::SetDimText( LPCTSTR cpDimText )
{
	if( cpDimText )											// If Dim Text Specified
	{
		_tcsncpy( m_caDimText, cpDimText, DIM_TEXT_LEN );	// Copy Over The Text
		m_caDimText[DIM_TEXT_LEN-1] = _T( '\0' );			// Enforce Termination (I Am Paranoid, I Know!) 
		m_iDimTextLen = _tcslen( m_caDimText );				// Store Length Of The Dim Text
	}
	else													// If No Dim Text
	{
		m_caDimText[ 0 ] = _T( '\0' );						// Just Terminate The Buffer (No Text)
		m_iDimTextLen = 0;									// No Dim Text
	}
	if( m_bShowDimText )									// If Showing Any Dim Text
	{
		DrawDimText();										// Draw The Dim Text
	}
	return;													// Done!
}


inline void	CDimEditCtrl::SetShowDimControl( bool bShow )
{
	m_bShowDimText = bShow;									// Set The Dim Flag
	if( bShow )												// If Showing Any Dim Text
	{
		DrawDimText();										// Draw The Dim Text
	}
	return;													// Done!
}

/*
inline BOOL	CDimEditCtrl::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
				DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, 
				CCreateContext* pContext ) 
{
	BOOL	bCreated = CWnd::Create( lpszClassName, 
					lpszWindowName, dwStyle, rect, 
					pParentWnd, nID, pContext );			// Try To Create Ourselves...

	if( bCreated )											// If We Got Created
	{
		SetShowDimControl( true );							// Show The Dim Control
	}
	return( bCreated );										// Return Creation Status
}
*/
//--> @@code delete

inline void CDimEditCtrl::OnEnChange(HWND hWnd)
{
	int		iLen = GetWindowTextLength();					// Get Control's Text Length

	if( !iLen )												// If No Text
	{
		SetShowDimControl( true );							// Show The Dim Text
	}
	else													// If Text Now In The Control
	{
		SetShowDimControl( false );							// Disable The Dim Text
	}
	return;													// Done!
}

inline void CDimEditCtrl::OnEnSetfocus(HWND hWnd) 
{
	if( m_bShowDimText )									// If Showing Any Dim Text
	{
		DrawDimText();										// Draw The Dim Text
	}
	return;													// Done!
}

inline void	CDimEditCtrl::OnPaint(HWND hWnd) 
{
	Default();												// Do Default Control Drawing

	if( m_bShowDimText )									// If Showing Any Dim Text
	{
		DrawDimText();										// Draw The Dim Text
	}
	return;													// Done!
}


inline void	CDimEditCtrl::DrawDimText( void )
{
	if( !m_iDimTextLen )									// If No Dim Text
	{
		return;												// Stop Here
	}
	CClientDC	dcDraw( this );
	CRect		rRect;
	int			iState = dcDraw.SaveDC();					// Save The DC State
	
	GetClientRect( &rRect );								// Get Drawing Area
	rRect.OffsetRect( 1, 1 );								// Add Sanity Space
	
	dcDraw.SelectObject( GetFont() );					// Use The Control's Current Font
	dcDraw.SetTextColor( m_crDimTextColor );				// Set The Text Color
	dcDraw.SetBkColor( GetSysColor( COLOR_WINDOW ) );		// Set The Bk Color
	dcDraw.DrawText( m_caDimText, m_iDimTextLen, &rRect, 
			( DT_CENTER | DT_VCENTER ) );					// Draw The Dim Text
	
	dcDraw.RestoreDC( iState );								// Restore The DC State

	return;													// Done!
}


inline BOOL CDimEditCtrl::OnEraseBkgnd(HWND hWnd, HDC hDC) 
{
	//BOOL	bStatus = CEdit::OnEraseBkgnd(pDC);
	BOOL	bStatus = Default(hWnd);

	if( ( bStatus ) && ( m_bShowDimText ) )					// If All Good, And Showing Any Dim Text
	{
		DrawDimText();										// Draw The Dim Text
	}
	return( bStatus );										// Return Erase Status
}

/*
void CDimEditCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	TRACE( _T( "Click...\n" ) );
	CEdit::OnLButtonDown(nFlags, point);
}

void CDimEditCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	TRACE( _T( "DClick...\n" ) );
	CEdit::OnLButtonDblClk(nFlags, point);
}
*/

inline void	CDimEditCtrl::SetDimOffset( char cRedOS, char cGreenOS, char cBlueOS )
{
	COLORREF	crWindow = GetSysColor( COLOR_WINDOW );
	BYTE		btRedOS = ( GetRValue( crWindow ) + cRedOS );
	BYTE		btGreenOS = ( GetGValue( crWindow ) + cGreenOS );
	BYTE		btBlueOS = ( GetBValue( crWindow ) + cBlueOS );

	m_bUseDimOffset = true;									// Set The Flag
	m_cRedOS = cRedOS;										// Store Red Offset
	m_cGreenOS = cGreenOS;									// Store Green Offset
	m_cBlueOS = cBlueOS;									// Store Blue Offset
	m_crDimTextColor = RGB( (BYTE)btRedOS, (BYTE)btGreenOS, 
			(BYTE)btBlueOS );								// Build The New Dim Color

	return;													// Done!
}


inline void	CDimEditCtrl::SetDimColor( COLORREF crColor )
{
	m_bUseDimOffset = false;								// Unset The Flag
	m_crDimTextColor = crColor;								// Set The New Dim Color
	m_cRedOS = m_cGreenOS = m_cBlueOS = 0;					// No Offset

	return;													// Done!
}


inline void CDimEditCtrl::OnSettingChange(HWND hWnd, UINT uFlags, LPCTSTR lpszSection) 
{
	//CEdit::OnSettingChange(uFlags, lpszSection);
	//--> @@code modify
	Default(hWnd);
	
	if( m_bUseDimOffset )									// If Using An Offset For The Dim Color
	{
		COLORREF	crWindow = GetSysColor( COLOR_WINDOW );

		m_crDimTextColor = RGB( GetRValue( crWindow ) + 
				m_cRedOS, GetGValue( crWindow ) + 
				m_cGreenOS, GetBValue( crWindow ) + 
				m_cBlueOS );								// Rebuild The Dim Color
	}
	return;													// Done!
}

// --------------------------------------------------------------------------
// $Log: DimEditCtrl.cpp,v $
// Revision 1.2  2006/09/16 18:26:20  xushiwei
// EN_KILLFOCUS
//
// Revision 1.1  2006/09/16 18:07:10  xushiwei
// MFC-Port:
//   Demo - port MFC controls to WINX (DimEditCtrl)
//
