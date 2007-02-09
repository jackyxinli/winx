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
// Module: winx/win/menu/MenuItem.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Contributor: renfengxing@gmail.com
// Date: 2007-2-9 14:02:58
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_MENU_MENUITEM_H__
#define __WINX_WIN_MENU_MENUITEM_H__

#ifndef __WINX_WIN_BASIC_H__
#include "../Basic.h"
#endif

__WINX_BEGIN

// -------------------------------------------------------------------------

// From <winuser.h>
#ifndef ODS_HOTLIGHT
    #define ODS_HOTLIGHT 0x0040
#endif

#ifndef ODS_INACTIVE
    #define ODS_INACTIVE 0x0080
#endif

#ifndef ODS_NOACCEL
    #define ODS_NOACCEL 0x0100
#endif

// From <winuser.h>
#ifndef OBM_CHECK
    #define OBM_CHECK 32760
#endif

// From <winuser.h>, for DrawText
#ifndef DT_HIDEPREFIX
    #define DT_HIDEPREFIX 0x00100000
#endif

// From <winuser.h>, for SystemParametersInfo
#ifndef SPI_GETKEYBOARDCUES
    #define SPI_GETKEYBOARDCUES 0x100A
#endif

#ifndef MIIM_BITMAP
	#define MIIM_BITMAP      0x00000080
#endif

#ifndef MIIM_FTYPE
	#define MIIM_FTYPE       0x00000100
#endif

///////////////////////////////////////////////////////////////////////////////
// Menu item image management class
//

class CImgDesc
{
public:
    HIMAGELIST m_hImgList;
    int        m_nIndex;

    CImgDesc (HIMAGELIST hImgList = NULL, int nIndex = 0)
        : m_hImgList (hImgList), m_nIndex (nIndex)
    {
    }
};

///////////////////////////////////////////////////////////////////////////////
// Menu item management class
//

class MenuItemBase
{
protected:
	typedef DCHandle CDC;
	typedef BrushDC CBrushDC;
	typedef PenDC CPenDC;
	typedef BoldDC CBoldDC;

    MENUITEMINFO m_miInfo;
    CString      m_sCaption;
	HBITMAP		 m_hBitmap;

public:
    MenuItemBase();

// Properties
public:
    int   GetCaption   (CString& sCaption) const;
    int   GetShortCut  (CString& sShortCut) const;
    BOOL  GetSeparator () const;
    BOOL  GetChecked   () const;
    BOOL  GetRadio     () const;
    BOOL  GetDisabled  () const;
    BOOL  GetDefault   () const;
    HMENU GetPopup     () const;
    UINT  GetID        () const;

// Methods
public:
    int  GetCaptionWidth  (CDC* pDC) const;
    int  GetShortCutWidth (CDC* pDC) const;
    int  GetHeight        (CDC* pDC) const;

    BOOL Draw(CDC* pDC, LPCRECT pRect, 
		BOOL bSelected, BOOL bMenuBar = FALSE,
		BOOL bHotLight = FALSE,
		BOOL bInactive = FALSE,
		BOOL bNoAccel = FALSE) const;

	BOOL Draw(const DRAWITEMSTRUCT* pDrawItemStruct, BOOL bMenuBar = FALSE)
	{
		DCHandle dc(pDrawItemStruct->hDC);
		return Draw(&dc,
			&pDrawItemStruct->rcItem, 
			(pDrawItemStruct->itemState&ODS_SELECTED)!=0,
			bMenuBar,
			(pDrawItemStruct->itemState&ODS_HOTLIGHT)!=0,
			(pDrawItemStruct->itemState&ODS_INACTIVE)!=0,
			(pDrawItemStruct->itemState&ODS_NOACCEL));
	}
};

///////////////////////////////////////////////////////////////////////////////
// class class MenuItemT

template <class ImagePolicy>
class MenuItemT : public MenuItemBase
{
public:
    MenuItemT(MenuHandle hMenu, UINT uItemPos)//, UINT uFlag = MF_BYPOSITION)
	{
		m_miInfo.cbSize = sizeof(MENUITEMINFO);
		m_miInfo.fMask = 
			MIIM_STATE|MIIM_SUBMENU|MIIM_TYPE|MIIM_DATA|MIIM_ID|MIIM_CHECKMARKS;
		::GetMenuItemInfo(hMenu, uItemPos, MF_BYPOSITION, &m_miInfo);
		hMenu.GetMenuString(uItemPos, m_sCaption, MF_BYPOSITION);

		if ( !(m_miInfo.fType & MFT_SEPARATOR) )
		{
			m_hBitmap = ImagePolicy::GetMenuImage(&m_miInfo);
			if (m_hBitmap)
				WINX_TRACE("\nGet a bitmap");
		}
	}
};

namespace Policy
{
	struct MenuNoImage
	{
		static HBITMAP GetMenuImage(LPMENUITEMINFO lpMII)
		{
			return NULL;
		}
	};

	struct MenuImage
	{
		static HBITMAP GetMenuImage(LPMENUITEMINFO lpMII)
		{
			return lpMII->hbmpChecked;
		}
	};
}

typedef MenuItemT<Policy::MenuNoImage> MenuItemNoImage;
typedef MenuItemT<Policy::MenuImage> MenuItemImage;

///////////////////////////////////////////////////////////////////////////////

#define IMGWIDTH 16
#define IMGHEIGHT 16
#define IMGPADDING 6
#define TEXTPADDING 8
#define TEXTPADDING_MNUBR 4
#define SM_CXSHADOW 4

///////////////////////////////////////////////////////////////////////////////
inline MenuItemBase::MenuItemBase ()
{
    memset(&m_miInfo, 0, sizeof(MENUITEMINFO));
}

///////////////////////////////////////////////////////////////////////////////
inline int MenuItemBase::GetCaption (CString& sCaption) const
{
    WINX_ASSERT(m_miInfo.fMask & MIIM_TYPE);
    sCaption = m_sCaption;

    int nShortCutPos = sCaption.Find ('\t');

    if ( nShortCutPos == -1 )
    {
        return sCaption.GetLength();
    }
    sCaption = sCaption.Left (nShortCutPos);

    return nShortCutPos-1;
}

///////////////////////////////////////////////////////////////////////////////
inline int MenuItemBase::GetShortCut (CString& sShortCut) const
{
    WINX_ASSERT(m_miInfo.fMask & MIIM_TYPE);
    CString sCaption = m_sCaption;

    int nShortCutPos = sCaption.Find ('\t');

    if ( nShortCutPos == -1 )
    {
        sShortCut = "";
        return 0;
    }
    int nLength = sCaption.GetLength()-nShortCutPos-1;

    sShortCut = sCaption.Right (nLength);

    return nLength;
}

///////////////////////////////////////////////////////////////////////////////
inline BOOL MenuItemBase::GetSeparator () const
{
    WINX_ASSERT(m_miInfo.fMask & MIIM_TYPE);
    return (m_miInfo.fType & MFT_SEPARATOR) == MFT_SEPARATOR;
}

///////////////////////////////////////////////////////////////////////////////
inline BOOL MenuItemBase::GetChecked () const
{
    WINX_ASSERT(m_miInfo.fMask & MIIM_STATE);
    return (m_miInfo.fState & MFS_CHECKED) == MFS_CHECKED;
}

///////////////////////////////////////////////////////////////////////////////
inline BOOL MenuItemBase::GetRadio () const
{
    WINX_ASSERT(m_miInfo.fMask & MIIM_TYPE);
    return (m_miInfo.fType & MFT_RADIOCHECK) == MFT_RADIOCHECK;
}

///////////////////////////////////////////////////////////////////////////////
inline BOOL MenuItemBase::GetDisabled () const
{
    WINX_ASSERT(m_miInfo.fMask & MIIM_STATE);
    return (m_miInfo.fState & MFS_GRAYED) == MFS_GRAYED;
}

///////////////////////////////////////////////////////////////////////////////
inline BOOL MenuItemBase::GetDefault () const
{
    WINX_ASSERT(m_miInfo.fMask & MIIM_STATE);
    return (m_miInfo.fState & MFS_DEFAULT) == MFS_DEFAULT;
}

///////////////////////////////////////////////////////////////////////////////
inline HMENU MenuItemBase::GetPopup () const
{
    WINX_ASSERT(m_miInfo.fMask & MIIM_SUBMENU);
    return m_miInfo.hSubMenu;
}

///////////////////////////////////////////////////////////////////////////////
inline UINT MenuItemBase::GetID () const
{
    WINX_ASSERT(m_miInfo.fMask & MIIM_ID);
    return m_miInfo.wID;
}

///////////////////////////////////////////////////////////////////////////////
inline int MenuItemBase::GetCaptionWidth (CDC* pDC) const
{
    if ( GetSeparator() )
    {
        return 0;
    }
    CString sCaption;
    int nLength = 0;

    if ( GetCaption (sCaption) > 0 )
    {
        int nPos = sCaption.Find ('&');
        CBoldDC bold (*pDC, GetDefault());

        if ( nPos >= 0 )
        {
            sCaption = sCaption.Left (nPos) + sCaption.Right (sCaption.GetLength()-nPos-1);
        }
        nLength = pDC->GetTextExtent(sCaption).cx;
    }
    return nLength;
}

///////////////////////////////////////////////////////////////////////////////
inline int MenuItemBase::GetShortCutWidth (CDC* pDC) const
{
    if ( GetSeparator() )
    {
        return 0;
    }
    CString sShortCut;
    int nLength = 0;

    if ( GetShortCut (sShortCut) > 0 )
    {
        CBoldDC bold (*pDC, GetDefault());

        nLength = pDC->GetTextExtent (sShortCut).cx;
    }
    return nLength;
}

///////////////////////////////////////////////////////////////////////////////
inline int MenuItemBase::GetHeight (CDC* pDC) const
{
    TEXTMETRIC tm;

    pDC->GetTextMetrics (&tm);
    return GetSeparator() ? tm.tmHeight/2+2 : tm.tmHeight+4;
}

///////////////////////////////////////////////////////////////////////////////
inline BOOL MenuItemBase::Draw(
	CDC* pDC, LPCRECT pRect, BOOL bSelected, BOOL bMenuBar, BOOL bHotLight, BOOL bInactive, BOOL bNoAccel) const
{
    COLORREF crBackImg = CLR_NONE;
    bool bMenuBarItemSelected = false;

    if ( bMenuBar && bSelected )
    {
        CRect rc (pRect);
        CPenDC pen (*pDC, ::GetSysColor (COLOR_3DDKSHADOW));
        CBrushDC brush (*pDC, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +20, 0));

        rc.right -= TEXTPADDING_MNUBR;
        bMenuBarItemSelected = true;
        pDC->Rectangle (rc);
        rc.left = rc.right;
        rc.right += TEXTPADDING_MNUBR;
        pDC->FillSolidRect (rc, ::GetSysColor (COLOR_3DFACE));

        for ( int x = 0; x < SM_CXSHADOW; x++ )
        {
            for ( int y = ( x < 2 ) ? 2-x : 0; y < rc.Height()-x-((x>0)?1:2); y++ )
            {
                int nMakeSpec = 78+(3-(y==0?0:(y==1?(x<2?0:1):(y==2?(x<2?x:2):x))))*5;
                COLORREF cr = pDC->GetPixel (rc.right-x-1, rc.top+y+SM_CXSHADOW);
                COLORREF cr2 = RGB(((nMakeSpec * int(GetRValue(cr))) / 100),
			                       ((nMakeSpec * int(GetGValue(cr))) / 100),
			                       ((nMakeSpec * int(GetBValue(cr))) / 100));
			    pDC->SetPixel (rc.right-x-1, rc.top+y+SM_CXSHADOW, cr2);
            }
        }
    }
    else if ( bSelected || (bHotLight && !bInactive) )
    {
        COLORREF crHighLight = ::GetSysColor (COLOR_HIGHLIGHT);
        CPenDC pen (*pDC, crHighLight);
        CBrushDC brush (*pDC, crBackImg = GetDisabled() ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +73, 0) : HLS_TRANSFORM (crHighLight, +70, -57));

        if ( bMenuBar )
        {
            CRect rc (pRect);

            rc.right -= TEXTPADDING_MNUBR;
            pDC->Rectangle (rc);
            rc.left = rc.right;
            rc.right += TEXTPADDING_MNUBR;
            pDC->FillSolidRect(rc, ::GetSysColor (COLOR_3DFACE));
        }
        else
        {
            pDC->Rectangle(pRect);
        }
    }
    else if ( !bMenuBar )
    {
        CRect rc (pRect);

        rc.right = IMGWIDTH+IMGPADDING;
        pDC->FillSolidRect(rc, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +20, 0));
        rc.left = rc.right;
        rc.right = pRect->right;
        pDC->FillSolidRect(rc, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +75, 0));
    }
    else
    {
        pDC->FillSolidRect(pRect, ::GetSysColor (COLOR_3DFACE));
    }
    if ( GetSeparator() )
    {
        CPenDC pen(*pDC, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -18, 0));

        pDC->MoveTo(pRect->left+IMGWIDTH+IMGPADDING+TEXTPADDING,  (pRect->top+pRect->bottom)/2);
        pDC->LineTo(pRect->right-1, (pRect->top+pRect->bottom)/2);
    }
    else
    {
        CRect rc (pRect);
        CString sCaption;

        if ( GetCaption (sCaption) > 0 )
        {
            pDC->SetTextColor (bInactive ? ::GetSysColor (COLOR_3DSHADOW) : (GetDisabled() ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -18, 0) : ::GetSysColor (COLOR_MENUTEXT)));
            pDC->SetBkMode (TRANSPARENT);

            BOOL bKeyboardCues = true;
            ::SystemParametersInfo (SPI_GETKEYBOARDCUES, 0, &bKeyboardCues, 0);
            DWORD dwHidePrefix = ( bNoAccel && !bKeyboardCues ) ? DT_HIDEPREFIX : 0;

            if (bMenuBar)
            {
                rc.right -= TEXTPADDING_MNUBR;
                pDC->DrawText (sCaption, rc, DT_SINGLELINE|DT_VCENTER|DT_CENTER|dwHidePrefix);
            }
            else
            {
                rc.left = IMGWIDTH+IMGPADDING+TEXTPADDING;
                pDC->DrawText (sCaption, rc, DT_SINGLELINE|DT_VCENTER|DT_LEFT|dwHidePrefix);

                CString sShortCut;

                if ( GetShortCut (sShortCut) > 0 )
                {
                    rc.right -= TEXTPADDING+4;
                    pDC->DrawText (sShortCut, rc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
                }
                if ( GetChecked() )
                {
                    COLORREF crHighLight = ::GetSysColor (COLOR_HIGHLIGHT);
                    CPenDC pen (*pDC, crHighLight);
                    CBrushDC brush (*pDC, crBackImg = GetDisabled() ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +73, 0) :
                                                                      (bSelected ? HLS_TRANSFORM (crHighLight, +50, -50) : HLS_TRANSFORM (crHighLight, +70, -57)));

                    pDC->Rectangle (CRect (pRect->left+1, pRect->top+1, pRect->left+IMGWIDTH+4, pRect->bottom-1));
                }
                if (m_hBitmap)
                {
                    bool bOver = !GetDisabled() && bSelected;

                    if ( GetDisabled() || (bSelected && !GetChecked()) )
                    {
						COLORREF crBrush = (bOver ? 
							HLS_TRANSFORM (::GetSysColor (COLOR_HIGHLIGHT), +50, -66) : 
							HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -27, 0));
						pDC->DrawState(
							CPoint(pRect->left + ( bOver ? 4 : 3 ), rc.top + ( bOver ? 4 : 3 )),
							CSize(IMGWIDTH, IMGHEIGHT), 
							m_hBitmap, DSS_MONO, 
							Brush().CreateSolidBrush(crBrush));
                    }
                    if ( !GetDisabled() )
                    {
						HIMAGELIST hImgList;
						hImgList = ::ImageList_Create(IMGWIDTH, IMGHEIGHT, ILC_COLOR16|ILC_MASK, 1, 1);
						WINX_ASSERT(hImgList);
						::ImageList_Add(hImgList, m_hBitmap, NULL);
						::ImageList_Draw(hImgList, 0, pDC->m_hDC, 
							pRect->left+((bSelected && !GetChecked()) ? 2 : 3),
							rc.top+( (bSelected && !GetChecked()) ? 2 : 3 ),
							ILD_TRANSPARENT);
						::ImageList_Destroy(hImgList);
                    }
                }
                else if ( GetChecked() )
                {
                    // Draw the check mark
                    rc.left  = pRect->left+IMGWIDTH/4+1;
                    rc.right = rc.left + IMGWIDTH-1;

                    if (GetRadio())
                    {
                        CPoint ptCenter = rc.CenterPoint();
                        COLORREF crBullet = GetDisabled() ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -27, 0) : ::GetSysColor (COLOR_MENUTEXT);
                        CPenDC pen (*pDC, crBullet);
                        CBrushDC brush (*pDC, crBullet);
                        
                        pDC->Ellipse(CRect(ptCenter.x-4-1, ptCenter.y-3, ptCenter.x+3-1, ptCenter.y+4));
						//pDC->Ellipse(CRect(ptCenter.x-4, ptCenter.y-3, ptCenter.x+3, ptCenter.y+4));
                        //pDC->SetPixel(ptCenter.x+1, ptCenter.y+2, crBackImg);
                    }
                    else
                    {
                        pDC->SetBkColor(crBackImg);
                        HBITMAP hBmp = ::LoadBitmap(NULL, MAKEINTRESOURCE(OBM_CHECK));
                        pDC->DrawState (CPoint (rc.left,rc.top+3), CSize(rc.Size()), hBmp, DSS_NORMAL, (HBRUSH)NULL);
                        DeleteObject (hBmp);
                    }
                }
            }
        }
    }
    return bMenuBarItemSelected;
}

// -------------------------------------------------------------------------
// $Log: $

__WINX_END

#endif /* __WINX_WIN_MENU_MENUITEM_H__ */
