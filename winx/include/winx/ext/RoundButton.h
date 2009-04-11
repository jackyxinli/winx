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
// Module: winx/ext/RoundButton.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 18:26:15
// 
// $Id: RoundButton.h,v 1.3 2006/08/26 09:12:37 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_EXT_ROUNDBUTTON_H
#define WINX_EXT_ROUNDBUTTON_H

#ifndef _INC_MATH
#include <math.h>
#endif

namespace winx {

/////////////////////////////////////////////////////////////////////////////
// Round Buttons!
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright (c) 1997,1998.
// 
// Modified: 2 Feb 1998 - Fix vis problem, CRgn resource leak,
//                        button reposition code redone. CJM.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
/////////////////////////////////////////////////////////////////////////////
	
// Calculate colour for a point at the given angle by performing a linear
// interpolation between the colours crBright and crDark based on the cosine
// of the angle between the light source and the point.
//
// Angles are measured from the +ve x-axis (i.e. (1,0) = 0 degrees, (0,1) = 90 degrees )
// But remember: +y points down!

inline COLORREF winx_call GetColour(double dAngle, COLORREF crBright, COLORREF crDark)
{
#define LIGHT_SOURCE_ANGLE	(-2.356) // -2.356 radians = -135 degrees, i.e. From top left

	WINX_ASSERT(dAngle > -3.1416 && dAngle < 3.1416);
	double dAngleDifference = LIGHT_SOURCE_ANGLE - dAngle;

	if (dAngleDifference < -3.1415) dAngleDifference = 6.293 + dAngleDifference;
	else if (dAngleDifference > 3.1415) dAngleDifference = 6.293 - dAngleDifference;

	double Weight = 0.5*(cos(dAngleDifference)+1.0);

	BYTE Red   = (BYTE) (Weight*GetRValue(crBright) + (1.0-Weight)*GetRValue(crDark));
	BYTE Green = (BYTE) (Weight*GetGValue(crBright) + (1.0-Weight)*GetGValue(crDark));
	BYTE Blue  = (BYTE) (Weight*GetBValue(crBright) + (1.0-Weight)*GetBValue(crDark));

	return RGB(Red, Green, Blue);
}

inline void winx_call DrawCircle(
	CDC* pDC, CPoint p, LONG lRadius, COLORREF crColour, BOOL bDashed = FALSE)
{
	const int nDashLength = 1;
	LONG lError, lXoffset, lYoffset;
	int  nDash = 0;
	BOOL bDashOn = TRUE;

	//Check to see that the coordinates are valid
	WINX_ASSERT( (p.x + lRadius <= LONG_MAX) && (p.y + lRadius <= LONG_MAX) );
	WINX_ASSERT( (p.x - lRadius >= LONG_MIN) && (p.y - lRadius >= LONG_MIN) );

	//Set starting values
	lXoffset = lRadius;
	lYoffset = 0;
	lError   = -lRadius;

	do {
		if (bDashOn) {
			pDC->SetPixelV(p.x + lXoffset, p.y + lYoffset, crColour);
			pDC->SetPixelV(p.x + lXoffset, p.y - lYoffset, crColour);
			pDC->SetPixelV(p.x + lYoffset, p.y + lXoffset, crColour);
			pDC->SetPixelV(p.x + lYoffset, p.y - lXoffset, crColour);
			pDC->SetPixelV(p.x - lYoffset, p.y + lXoffset, crColour);
			pDC->SetPixelV(p.x - lYoffset, p.y - lXoffset, crColour);
			pDC->SetPixelV(p.x - lXoffset, p.y + lYoffset, crColour);
			pDC->SetPixelV(p.x - lXoffset, p.y - lYoffset, crColour);
		}

		//Advance the error term and the constant X axis step
		lError += lYoffset++;

		//Check to see if error term has overflowed
		if ((lError += lYoffset) >= 0)
			lError -= --lXoffset * 2;

		if (bDashed && (++nDash == nDashLength)) {
			nDash = 0;
			bDashOn = !bDashOn;
		}

	} while (lYoffset <= lXoffset);	//Continue until halfway point
} 

inline void winx_call DrawCircle(
	CDC* pDC, CPoint p, LONG lRadius, COLORREF crBright, COLORREF crDark)
{
	LONG lError, lXoffset, lYoffset;

	//Check to see that the coordinates are valid
	WINX_ASSERT( (p.x + lRadius <= LONG_MAX) && (p.y + lRadius <= LONG_MAX) );
	WINX_ASSERT( (p.x - lRadius >= LONG_MIN) && (p.y - lRadius >= LONG_MIN) );

	//Set starting values
	lXoffset = lRadius;
	lYoffset = 0;
	lError   = -lRadius;

	do {
		const double Pi = 3.141592654, 
					 Pi_on_2 = Pi * 0.5,
					 Three_Pi_on_2 = Pi * 1.5;
		COLORREF crColour;
		double   dAngle = atan2((double)lYoffset, (double)lXoffset);

		//Draw the current pixel, reflected across all eight arcs
		crColour = GetColour(dAngle, crBright, crDark);
		pDC->SetPixelV(p.x + lXoffset, p.y + lYoffset, crColour);

		crColour = GetColour(Pi_on_2 - dAngle, crBright, crDark);
		pDC->SetPixelV(p.x + lYoffset, p.y + lXoffset, crColour);

		crColour = GetColour(Pi_on_2 + dAngle, crBright, crDark);
		pDC->SetPixelV(p.x - lYoffset, p.y + lXoffset, crColour);

		crColour = GetColour(Pi - dAngle, crBright, crDark);
		pDC->SetPixelV(p.x - lXoffset, p.y + lYoffset, crColour);

		crColour = GetColour(-Pi + dAngle, crBright, crDark);
		pDC->SetPixelV(p.x - lXoffset, p.y - lYoffset, crColour);

		crColour = GetColour(-Pi_on_2 - dAngle, crBright, crDark);
		pDC->SetPixelV(p.x - lYoffset, p.y - lXoffset, crColour);

		crColour = GetColour(-Pi_on_2 + dAngle, crBright, crDark);
		pDC->SetPixelV(p.x + lYoffset, p.y - lXoffset, crColour);

		crColour = GetColour(-dAngle, crBright, crDark);
		pDC->SetPixelV(p.x + lXoffset, p.y - lYoffset, crColour);

		//Advance the error term and the constant X axis step
		lError += lYoffset++;

		//Check to see if error term has overflowed
		if ((lError += lYoffset) >= 0)
			lError -= --lXoffset * 2;

	} while (lYoffset <= lXoffset);	//Continue until halfway point
} 

// -------------------------------------------------------------------------
// class RoundButton

class RoundButton : public Button<RoundButton>
{
	WINX_CLASS("WinxRoundButton");
	// 加上这一句只是出于完备性考虑。可以没有，只是这样就不能以超类方式创建该窗口类。

private:
	CRgn   m_rgn;
	CPoint m_ptCentre;
	int    m_nRadius;

public:
	enum {
		m_bDrawDashedFocusCircle = TRUE
	};
	
public:
	LRESULT winx_msg OnSubclass(HWND hWnd);

	VOID winx_msg DrawItem(HWND hWnd, LPDRAWITEMSTRUCT lpDrawItemStruct);
};

inline LRESULT winx_msg RoundButton::OnSubclass(HWND hWnd)
{
	ModifyStyle(0, BS_OWNERDRAW);
	
	CRect rect;
	GetClientRect(rect);
	
	// Resize the window to make it square
	rect.bottom = rect.right = min(rect.bottom,rect.right);
	
	// Get the vital statistics of the window
	m_ptCentre = rect.CenterPoint();
	m_nRadius  = rect.bottom/2-1;
	
	// Set the window region so mouse clicks only activate the round section 
	// of the button
	m_rgn.CreateEllipticRgnIndirect(rect);
	SetWindowRgn(m_rgn, TRUE);
	
	// Convert client coords to the parents client coords
	ClientToScreen(rect);

	HWND hWndParent = GetParent();
	winx::ScreenToClient(hWndParent, &rect);
	
	// Resize the window
	MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

	return 0;
}

inline VOID winx_msg RoundButton::DrawItem(HWND hWnd, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	WINX_ASSERT(lpDrawItemStruct != NULL);
	
	CDC dc(lpDrawItemStruct->hDC);
	CDC* pDC   = &dc;
	CRect rect = lpDrawItemStruct->rcItem;
	UINT state = lpDrawItemStruct->itemState;
	UINT nStyle = GetStyle();
	int nRadius = m_nRadius;
	
	int nSavedDC = pDC->SaveDC();
	
	pDC->SelectStockBrush(NULL_BRUSH);
	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));
	
	// Draw the focus circle around the button
	if ((state & ODS_FOCUS) && m_bDrawDashedFocusCircle)
		DrawCircle(pDC, m_ptCentre, nRadius--, RGB(0,0,0));
	
	// Draw the raised/sunken edges of the button (unless flat)
	if (nStyle & BS_FLAT) {
		DrawCircle(pDC, m_ptCentre, nRadius--, RGB(0,0,0));
		DrawCircle(pDC, m_ptCentre, nRadius--, ::GetSysColor(COLOR_3DHIGHLIGHT));
	} else {
		if ((state & ODS_SELECTED))	{
			DrawCircle(pDC, m_ptCentre, nRadius--, 
				::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DHIGHLIGHT));
			DrawCircle(pDC, m_ptCentre, nRadius--, 
				::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DLIGHT));
		} else {
			DrawCircle(pDC, m_ptCentre, nRadius--, 
				::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DDKSHADOW));
			DrawCircle(pDC, m_ptCentre, nRadius--, 
				::GetSysColor(COLOR_3DLIGHT), ::GetSysColor(COLOR_3DSHADOW));
		}
	}
	
	// draw the text if there is any
	CString strText;
	GetWindowText(strText);
	
	if (!strText.IsEmpty())
	{
		CRgn rgn;
		rgn.CreateEllipticRgn(m_ptCentre.x-nRadius, m_ptCentre.y-nRadius, 
			m_ptCentre.x+nRadius, m_ptCentre.y+nRadius);
		pDC->SelectClipRgn(rgn);
		
		CSize Extent = pDC->GetTextExtent(strText);
		CPoint pt = CPoint( m_ptCentre.x - Extent.cx/2, m_ptCentre.x - Extent.cy/2 );
		
		if (state & ODS_SELECTED) pt.Offset(1,1);
		
		pDC->SetBkMode(TRANSPARENT);
		
		if (state & ODS_DISABLED)
			pDC->DrawState(pt, Extent, strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		else
			pDC->TextOut(pt.x, pt.y, strText);
		
		pDC->SelectClipRgn(NULL);
		rgn.DeleteObject();
	}
	
	// Draw the focus circle on the inside of the button
	if ((state & ODS_FOCUS) && m_bDrawDashedFocusCircle)
		DrawCircle(pDC, m_ptCentre, nRadius-2, RGB(0,0,0), TRUE);
	
	pDC->RestoreDC(nSavedDC);
}

// -------------------------------------------------------------------------
//	$Log: RoundButton.h,v $
//	Revision 1.3  2006/08/26 09:12:37  xushiwei
//	vs2005 support
//	
//	Revision 1.2  2006/08/21 18:45:48  xushiwei
//	WINX-Core:
//	   Window::Create, MainFrame, StackWindowObject(WINX_STACK_OBJECT) support
//	
//	Revision 1.1  2006/08/19 17:07:15  xushiwei
//	MFC-Port:
//	  Demo - port MFC controls to WINX (CreditStatic, RoundButton)
//	

}; // namespace winx

#endif /* WINX_EXT_ROUNDBUTTON_H */
