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
// Module: winx/win/Gdi.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:20:11
// 
// $Id: Gdi.h,v 1.4 2006/11/30 09:16:43 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_GDI_H__
#define __WINX_WIN_GDI_H__

#ifndef __WINX_WIN_BASICTYPES_H__
#include "BasicTypes.h"
#endif

__WINX_BEGIN

// =========================================================================
// Rgn

typedef WTL::CRgnHandle RgnHandle;
typedef WTL::CRgn Rgn;

// -------------------------------------------------------------------------
// Pen

typedef WTL::CPenHandle PenHandle;
typedef WTL::CPen Pen;

// -------------------------------------------------------------------------
// Brush

enum StockBrushStyle
{
	WhiteBrush = 0,
	LightGrayBrush = 1,
	GrayBrush = 2,
	DarkGrayBrush = 3,
	BlackBrush = 4,
	NullBrush = 5,
	HollowBrush = NullBrush,
};

typedef WTL::CBrushHandle BrushHandle;
typedef WTL::CBrush Brush;

// -------------------------------------------------------------------------
// Font

typedef WTL::CLogFont LogFont;
typedef WTL::CFontHandle FontHandle;
typedef WTL::CFont Font;

// -------------------------------------------------------------------------
// Bitmap

template <bool t_bManaged>
class BitmapT : public WTL::CBitmapT<t_bManaged>
{
	WINX_THISCLASS(BitmapT);
private:
	typedef WTL::CBitmapT<t_bManaged> BaseClass;
	
	BitmapT(const BitmapT&);
	void operator=(const BitmapT&);

public:
	using BaseClass::m_hBitmap;

	POINTER_ALIAS_RW(HBITMAP, m_hObject, m_hBitmap);

public:
	BitmapT(HBITMAP hBitmap = NULL) : BaseClass(hBitmap) {
	}

	HBITMAP operator=(HBITMAP hBitmap)
	{
		Attach(hBitmap);
		return hBitmap;
	}

	HBITMAP CreateCompatibleBitmap(HDC hDC, int nWidth, int nHeight)
	{
		WINX_ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
		return m_hBitmap;
	}
	HBITMAP CreateCompatibleBitmap(WTL::CDC* pDC, int nWidth, int nHeight)
	{
		WINX_ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::CreateCompatibleBitmap(pDC->m_hDC, nWidth, nHeight);
		return m_hBitmap;
	}
	HBITMAP CreateCompatibleBitmap(WTL::CDCHandle* pDC, int nWidth, int nHeight)
	{
		WINX_ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::CreateCompatibleBitmap(pDC->m_hDC, nWidth, nHeight);
		return m_hBitmap;
	}

	int GetObject(int cbDib, DIBSECTION* pDib) const
	{
		WINX_ASSERT(m_hBitmap != NULL);
		WINX_ASSERT(cbDib == sizeof(DIBSECTION));
		return ::GetObject(m_hBitmap, cbDib, pDib);
	}
	int GetObject(int cbBitMap, BITMAP* pBitMap) const
	{
		WINX_ASSERT(m_hBitmap != NULL);
		WINX_ASSERT(cbBitMap == sizeof(BITMAP));
		return ::GetObject(m_hBitmap, cbBitMap, pBitMap);
	}

	HBITMAP LoadBitmap(ATL::_U_STRINGorID bitmap, HINSTANCE hInst = GetThisModule())
	{
		WINX_ASSERT(m_hBitmap == NULL);
		m_hBitmap = ::LoadBitmap(hInst, bitmap.m_lpstr);
		return m_hBitmap;
	}
};

typedef BitmapT<false> BitmapHandle;
typedef BitmapT<true> Bitmap;

// -------------------------------------------------------------------------
// ScreenCapture

inline HBITMAP winx_call ScreenCapture(const RECT& rc)
{
    HDC     hDC;
    HDC     hMemDC;
    HBITMAP hNewBitmap = NULL;

    if ( (hDC = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL)) != NULL )
    {
        if ( (hMemDC = ::CreateCompatibleDC(hDC)) != NULL )
        {
            if ( (hNewBitmap = ::CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top)) != NULL )
            {
                HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hNewBitmap);
                ::BitBlt(hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
                          hDC, rc.left, rc.top, SRCCOPY);
                ::SelectObject(hMemDC, (HGDIOBJ) hOldBitmap);
            }
            ::DeleteDC(hMemDC);
        }
        ::DeleteDC(hDC);
    }
    return hNewBitmap;
}

inline HBITMAP winx_call ScreenCapture(HWND hWnd = ::GetDesktopWindow())
{
	WindowRect rcWin(hWnd);
	return ScreenCapture(rcWin);
}

// =========================================================================
// Palette

template <bool t_bManaged>
class PaletteT : public WTL::CPaletteT<t_bManaged>
{
	WINX_THISCLASS(PaletteT);
private:
	typedef WTL::CPaletteT<t_bManaged> BaseClass;

public:
	using BaseClass::m_hPalette;
	
	POINTER_ALIAS_RW(HPALETTE, m_hObject, m_hPalette);

public:
	PaletteT(HPALETTE hPalette = NULL) : BaseClass(hPalette) {}

#ifndef _WIN32_WCE
	HPALETTE CreateHalftonePalette(HDC hDC)
	{
		WINX_ASSERT(m_hPalette == NULL);
		WINX_ASSERT(hDC != NULL);
		m_hPalette = ::CreateHalftonePalette(hDC);
		return m_hPalette;
	}
	HPALETTE CreateHalftonePalette(const WTL::CDC* pDC)
	{
		WINX_ASSERT(m_hPalette == NULL);
		WINX_ASSERT(pDC != NULL);
		m_hPalette = ::CreateHalftonePalette(pDC->m_hDC);
		return m_hPalette;
	}
	HPALETTE CreateHalftonePalette(const WTL::CDCHandle* pDC)
	{
		WINX_ASSERT(m_hPalette == NULL);
		WINX_ASSERT(pDC != NULL);
		m_hPalette = ::CreateHalftonePalette(pDC->m_hDC);
		return m_hPalette;
	}
#endif // !_WIN32_WCE
};

typedef PaletteT<false> PaletteHandle;
typedef PaletteT<true> Palette;

// =========================================================================
// DC

template <bool t_bManaged>
class DCT : public WTL::CDCT<t_bManaged>
{
private:
	typedef WTL::CDCT<t_bManaged> BaseClass;

public:
	using BaseClass::m_hDC;
	
	DCT(HDC hDC = NULL) : BaseClass(hDC) {}

	BOOL GetTextExtent(LPCTSTR lpszString, int nCount, LPSIZE lpSize) const {
		return BaseClass::GetTextExtent(lpszString, nCount, lpSize);
	}
	CSize GetTextExtent(LPCTSTR lpszString, int nCount) const {
		CSize sz;
		BaseClass::GetTextExtent(lpszString, nCount, &sz);
		return sz;
	}
	CSize GetTextExtent(const CString& str) const {
		WINX_ASSERT(m_hDC != NULL);
		CSize sz;
		::GetTextExtentPoint32(m_hDC, str, str.GetLength(), &sz);
		return sz;
	}

	BOOL BitBlt(int x, int y, int nWidth, int nHeight, HDC hSrcDC,
		int xSrc, int ySrc, DWORD dwRop) {
		WINX_ASSERT(m_hDC != NULL);
		return ::BitBlt(m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, dwRop);
	}
	BOOL BitBlt(int x, int y, int nWidth, int nHeight, WTL::CDC* srcDC,
		int xSrc, int ySrc, DWORD dwRop) {
		WINX_ASSERT(m_hDC != NULL);
		return ::BitBlt(m_hDC, x, y, nWidth, nHeight, srcDC->m_hDC, xSrc, ySrc, dwRop);
	}
	BOOL BitBlt(int x, int y, int nWidth, int nHeight, WTL::CDCHandle* srcDC,
		int xSrc, int ySrc, DWORD dwRop) {
		WINX_ASSERT(m_hDC != NULL);
		return ::BitBlt(m_hDC, x, y, nWidth, nHeight, srcDC->m_hDC, xSrc, ySrc, dwRop);
	}
	
	HBITMAP SelectObject(const WTL::CBitmapHandle& bitmap) {
		WINX_ASSERT(m_hDC != NULL);
		WINX_ASSERT(::GetObjectType(bitmap.m_hBitmap) == OBJ_BITMAP);
		return (HBITMAP)::SelectObject(m_hDC, bitmap.m_hBitmap);
	}
	HBITMAP SelectObject(const WTL::CBitmapHandle* bitmap) {
		WINX_ASSERT(m_hDC != NULL);
		WINX_ASSERT(bitmap != NULL && ::GetObjectType(bitmap->m_hBitmap) == OBJ_BITMAP);
		return (HBITMAP)::SelectObject(m_hDC, bitmap->m_hBitmap);
	}
	HBITMAP SelectObject(const WTL::CBitmap* bitmap) {
		WINX_ASSERT(m_hDC != NULL);
		WINX_ASSERT(bitmap != NULL && ::GetObjectType(bitmap->m_hBitmap) == OBJ_BITMAP);
		return (HBITMAP)::SelectObject(m_hDC, bitmap->m_hBitmap);
	}

	HFONT SelectObject(const WTL::CFontHandle& font) {
		WINX_ASSERT(m_hDC != NULL);
		WINX_ASSERT(::GetObjectType(font.m_hFont) == OBJ_FONT);
		return (HFONT)::SelectObject(m_hDC, font.m_hFont);
	}
	HFONT SelectObject(const WTL::CFontHandle* font) {
		WINX_ASSERT(m_hDC != NULL);
		WINX_ASSERT(font != NULL && ::GetObjectType(font->m_hFont) == OBJ_FONT);
		return (HFONT)::SelectObject(m_hDC, font->m_hFont);
	}
	HFONT SelectObject(const WTL::CFont* font) {
		WINX_ASSERT(m_hDC != NULL);
		WINX_ASSERT(font != NULL && ::GetObjectType(font->m_hFont) == OBJ_FONT);
		return (HFONT)::SelectObject(m_hDC, font->m_hFont);
	}

	HGDIOBJ SelectStockObject(int nIndex)
	{
		WINX_ASSERT(m_hDC != NULL);
		HGDIOBJ hObject = ::GetStockObject(nIndex);
		WINX_ASSERT(hObject != NULL);
		return ::SelectObject(m_hDC, hObject);
	}
	
	HPALETTE SelectPalette(HPALETTE hPalette, BOOL bForceBackground) {
		WINX_ASSERT(m_hDC != NULL);	
		return ::SelectPalette(m_hDC, hPalette, bForceBackground);
	}
	HPALETTE SelectPalette(const WTL::CPalette* pPal, BOOL bForceBackground) {
		WINX_ASSERT(m_hDC != NULL);	
		return ::SelectPalette(m_hDC, pPal->m_hPalette, bForceBackground);
	}
	HPALETTE SelectPalette(const WTL::CPaletteHandle* pPal, BOOL bForceBackground) {
		WINX_ASSERT(m_hDC != NULL);	
		return ::SelectPalette(m_hDC, pPal->m_hPalette, bForceBackground);
	}
	
	BOOL ScrollDC(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, HRGN hRgnUpdate, LPRECT lpRectUpdate) {
		WINX_ASSERT(m_hDC != NULL);
		return ::ScrollDC(m_hDC, dx, dy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate);
	}
	BOOL ScrollDC(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, const WTL::CRgn* pRgnUpdate, LPRECT lpRectUpdate) {
		WINX_ASSERT(m_hDC != NULL);
		return ::ScrollDC(m_hDC, dx, dy, lpRectScroll, lpRectClip, pRgnUpdate->m_hRgn, lpRectUpdate);
	}
	BOOL ScrollDC(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, const WTL::CRgnHandle* pRgnUpdate, LPRECT lpRectUpdate) {
		WINX_ASSERT(m_hDC != NULL);
		return ::ScrollDC(m_hDC, dx, dy, lpRectScroll, lpRectClip, pRgnUpdate->m_hRgn, lpRectUpdate);
	}
	
	HDC CreateCompatibleDC(HDC hDC = NULL) {
		WINX_ASSERT(m_hDC == NULL);
		m_hDC = ::CreateCompatibleDC(hDC);
		return m_hDC;
	}
	HDC CreateCompatibleDC(const WTL::CDCHandle* pDC) {
		WINX_ASSERT(m_hDC == NULL);
		m_hDC = ::CreateCompatibleDC(pDC->m_hDC);
		return m_hDC;
	}
	HDC CreateCompatibleDC(const WTL::CDC* pDC) {
		WINX_ASSERT(m_hDC == NULL);
		m_hDC = ::CreateCompatibleDC(pDC->m_hDC);
		return m_hDC;
	}
	
	int DrawText(LPCTSTR lpstrText, int cchText, LPRECT lpRect, UINT uFormat) {
		WINX_ASSERT(m_hDC != NULL);
		return ::DrawText(m_hDC, lpstrText, cchText, lpRect, uFormat);
	}
	int DrawText(const CString& str, LPRECT lpRect, UINT uFormat) {
		WINX_ASSERT(m_hDC != NULL);
		return ::DrawText(m_hDC, str, str.GetLength(), lpRect, uFormat);
	}
};

typedef DCT<false> DCHandle;

// -------------------------------------------------------------------------
// EnhMetaFileHandle

typedef WTL::CEnhMetaFileInfo EnhMetaFileInfo;
typedef WTL::CEnhMetaFileHandle EnhMetaFileHandle;
typedef WTL::CEnhMetaFile EnhMetaFile;

// -------------------------------------------------------------------------
// class EnhMetaFileDC

class EnhMetaFileDC : public DCHandle
{
public:
	EnhMetaFileDC()
	{
	}

	EnhMetaFileDC(HDC hdc, LPCRECT lpRect)
	{
		Create(hdc, NULL, lpRect, NULL);
		WINX_ASSERT(m_hDC != NULL);
	}

	EnhMetaFileDC(HDC hdcRef, LPCTSTR lpFilename, LPCRECT lpRect, LPCTSTR lpDescription)
	{
		Create(hdcRef, lpFilename, lpRect, lpDescription);
		WINX_ASSERT(m_hDC != NULL);
	}

	~EnhMetaFileDC()
	{
		if (m_hDC != NULL)
			::DeleteEnhMetaFile(::CloseEnhMetaFile(m_hDC));
	}

	VOID Create(HDC hdcRef, LPCTSTR lpFilename, LPCRECT lpRect, LPCTSTR lpDescription)
	{
		WINX_ASSERT(m_hDC == NULL);
		m_hDC = ::CreateEnhMetaFile(hdcRef, lpFilename, lpRect, lpDescription);
	}

	HENHMETAFILE Close()
	{
		HENHMETAFILE hEMF = NULL;
		if (m_hDC != NULL)
		{
			hEMF = ::CloseEnhMetaFile(m_hDC);
			m_hDC = NULL;
		}
		return hEMF;
	}
};

// -------------------------------------------------------------------------
// class ClientDC

class ClientDC : public DCHandle
{
public:
	HWND m_hWnd;

	ClientDC(int desktopWnd) {
		WINX_ASSERT(desktopWnd == NULL);
		m_hWnd = NULL;
		m_hDC = ::GetDC(NULL);
	}
	ClientDC(HWND hWnd)
	{
		WINX_ASSERT(hWnd == NULL || ::IsWindow(hWnd));
		m_hWnd = hWnd;
		m_hDC = ::GetDC(m_hWnd);
	}
#if !defined(WINX_NO_ALTER_WINDOW)
	ClientDC(_AlterCWindow* pWnd)
	{
		WINX_ASSERT(pWnd != NULL && ::IsWindow(pWnd->m_hWnd));
		m_hWnd = pWnd->m_hWnd;
		m_hDC = ::GetDC(m_hWnd);
	}
#endif
	ClientDC(BaseWindowHandle* pWnd)
	{
		WINX_ASSERT(pWnd != NULL && ::IsWindow(pWnd->m_hWnd));
		m_hWnd = pWnd->m_hWnd;
		m_hDC = ::GetDC(m_hWnd);
	}
	~ClientDC()
	{
		WINX_ASSERT(m_hDC != NULL);
		::ReleaseDC(m_hWnd, m_hDC);
	}
};

// -------------------------------------------------------------------------
// class WindowDC

class WindowDC : public DCHandle
{
public:
	HWND m_hWnd;

	WindowDC(HWND hWnd)
	{
		WINX_ASSERT(hWnd == NULL || ::IsWindow(hWnd));
		m_hWnd = hWnd;
		m_hDC = ::GetWindowDC(hWnd);
	}
	~WindowDC()
	{
		WINX_ASSERT(m_hDC != NULL);
		::ReleaseDC(m_hWnd, m_hDC);
	}
};

// -------------------------------------------------------------------------
// class WindowDCEx

class WindowDCEx : public DCHandle
{
public:
	HWND m_hWnd;

public:
	WindowDCEx(HWND hWnd, HRGN hrgnClip, DWORD flags = DCX_WINDOW | DCX_INTERSECTRGN)
	{
		WINX_ASSERT(hWnd == NULL || ::IsWindow(hWnd));
		m_hWnd = hWnd;
		m_hDC = ::GetDCEx(hWnd, hrgnClip, flags);
		WINX_ASSERT(m_hDC);
	}
	~WindowDCEx()
	{
		WINX_ASSERT(m_hDC != NULL);
		::ReleaseDC(m_hWnd, m_hDC);
	}
};

// -------------------------------------------------------------------------
// class PaintDC

class PaintDC : public DCHandle
{
public:
	HWND m_hWnd;
	PAINTSTRUCT m_ps;

public:
	PaintDC(HWND hWnd)
	{
		WINX_ASSERT(::IsWindow(hWnd));
		m_hWnd = hWnd;
		m_hDC = ::BeginPaint(m_hWnd, &m_ps);
	}
#if !defined(WINX_NO_ALTER_WINDOW)
	PaintDC(_AlterCWindow* pWnd)
	{
		m_hWnd = pWnd->m_hWnd;
		m_hDC = ::BeginPaint(m_hWnd, &m_ps);
	}
#endif
	PaintDC(BaseWindowHandle* pWnd)
	{
		m_hWnd = pWnd->m_hWnd;
		m_hDC = ::BeginPaint(m_hWnd, &m_ps);
	}
	~PaintDC()
	{
		WINX_ASSERT(m_hDC != NULL);
		WINX_ASSERT(::IsWindow(m_hWnd));
		::EndPaint(m_hWnd, &m_ps);
	}

public:
	const RECT& winx_call GetPaintRect() const
		{ return m_ps.rcPaint; }
};

// -------------------------------------------------------------------------
// class MemoryDC

class MemoryDC : public DCHandle
{
public:
	HDC m_hDCOriginal;
	RECT m_rcPaint;
	Bitmap m_bmp;
	HBITMAP m_hBmpOld;

public:
	MemoryDC(HDC hDC, const RECT& rcPaint) : m_hDCOriginal(hDC), m_hBmpOld(NULL)
	{
		m_rcPaint = rcPaint;
		CreateCompatibleDC(m_hDCOriginal);
		WINX_ASSERT(m_hDC != NULL);
		m_bmp.CreateCompatibleBitmap(
			m_hDCOriginal, m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top);
		WINX_ASSERT(m_bmp.m_hBitmap != NULL);
		m_hBmpOld = SelectBitmap(m_bmp);
		SetViewportOrg(-m_rcPaint.left, -m_rcPaint.top);
	}
	~MemoryDC()
	{
		WINX_ASSERT(m_hDC != NULL);
		::BitBlt(m_hDCOriginal, 
			m_rcPaint.left, m_rcPaint.top, 
			m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top,
			m_hDC, m_rcPaint.left, m_rcPaint.top, SRCCOPY);
		SelectBitmap(m_hBmpOld);
		::DeleteDC(m_hDC);
	}

public:
	const RECT& winx_call GetPaintRect() const
		{ return m_rcPaint; }
};

// =========================================================================
// class DoubleBuffer

#ifndef RECTWIDTH
#define RECTWIDTH(rc)               ((rc).right - (rc).left)
#define RECTHEIGHT(rc)              ((rc).bottom - (rc).top)
#endif

class DoubleBuffer
{
private:
	HDC     m_hdcMem;
	HBITMAP m_hbmpMem;
	HDC     m_hdcPaint;
	RECT    m_rcPaint;

public:
	VOID winx_call Begin(IN OUT HDC &hdc, IN const RECT& rcPaint)
	{
		WINX_ASSERT(
			m_hdcMem == NULL && m_hdcPaint == NULL &&
			m_hbmpMem == NULL);

		if (hdc)
		{
			m_hdcMem = CreateCompatibleDC(hdc);
			if (m_hdcMem)
			{
				m_hbmpMem = CreateCompatibleBitmap(hdc,
												  RECTWIDTH(rcPaint),
												  RECTHEIGHT(rcPaint));

				if (m_hbmpMem)
				{
					SelectObject(m_hdcMem, m_hbmpMem);
					m_rcPaint = rcPaint;
					OffsetWindowOrgEx(m_hdcMem,
									  m_rcPaint.left,
									  m_rcPaint.top,
									  NULL);

					// cache the original DC and pass out the memory DC
					m_hdcPaint = hdc;
					hdc = m_hdcMem;
				}
				else
				{
					DeleteDC(m_hdcMem);
					m_hdcMem = NULL;
				}
			}
		}
	}
	
	VOID winx_call End(IN OUT HDC& hdc)
	{
		if (hdc)
		{
			if (hdc == m_hdcMem)
			{
				BitBlt(m_hdcPaint,
					   m_rcPaint.left,
					   m_rcPaint.top,
					   RECTWIDTH(m_rcPaint),
					   RECTHEIGHT(m_rcPaint),
					   m_hdcMem,
					   m_rcPaint.left,
					   m_rcPaint.top,
					   SRCCOPY);

				// restore the original DC
				hdc = m_hdcPaint;

				DeleteObject(m_hbmpMem);
				DeleteDC(m_hdcMem);
				m_hbmpMem = NULL;
				m_hdcMem = NULL;
				m_hdcPaint = NULL;
			}
		}
	}

public:
    DoubleBuffer(IN OUT HDC& hdc, const RECT& rcPaint)
		: m_hdcMem(NULL), m_hbmpMem(NULL), m_hdcPaint(NULL)
	{
	}

#if defined(_DEBUG)
    ~DoubleBuffer()
	{
		WINX_ASSERT(
			m_hdcMem == NULL && m_hdcPaint == NULL &&
			m_hbmpMem == NULL);
	}
#endif
};

// =========================================================================
// class GdiObjectDC

class GdiObjectDC
{
protected:
    HDC m_hDC;
    HGDIOBJ m_hOldGdiObj;
	HGDIOBJ m_hOwnedGdiObj;
	
public:
	GdiObjectDC(HDC hDC) : m_hDC(hDC), m_hOldGdiObj(NULL), m_hOwnedGdiObj(NULL) {}
	GdiObjectDC(HDC hDC, HGDIOBJ hGdiObj, BOOL fOwn) : m_hDC(hDC)
	{
		m_hOldGdiObj = ::SelectObject(m_hDC, hGdiObj);
		m_hOwnedGdiObj = fOwn ? hGdiObj : NULL;
	}
	~GdiObjectDC()
	{
		if (m_hOldGdiObj)
			::SelectObject(m_hDC, m_hOldGdiObj);
		if (m_hOwnedGdiObj)
			::DeleteObject(m_hOwnedGdiObj);
	}

public:
	VOID winx_call Unselect()
	{
		if (m_hOldGdiObj)
		{
			::SelectObject(m_hDC, m_hOldGdiObj);
			m_hOldGdiObj = NULL;
		}
		if (m_hOwnedGdiObj)
		{
			::DeleteObject(m_hOwnedGdiObj);
			m_hOwnedGdiObj = NULL;
		}
	}

	VOID winx_call Select(HGDIOBJ hGdiObj, BOOL fOwn)
	{
		if (m_hOldGdiObj)
			::SelectObject(m_hDC, m_hOldGdiObj);
		if (m_hOwnedGdiObj)
			::DeleteObject(m_hOwnedGdiObj);
		m_hOldGdiObj = ::SelectObject(m_hDC, hGdiObj);
		m_hOwnedGdiObj = fOwn ? hGdiObj : NULL;
	}
};

// =========================================================================
// class BrushDC

class BrushDC : public GdiObjectDC
{
private:
	VOID winx_call _SelectBrush(COLORREF crColor)
	{
		WINX_ASSERT(m_hOldGdiObj == NULL && m_hOwnedGdiObj == NULL);
		WINX_ASSERT(crColor != CLR_DEFAULT);
		if (crColor == CLR_NONE)
		{
			m_hOldGdiObj = ::SelectObject(m_hDC, ::GetStockObject(NULL_BRUSH));
		}
		else
		{
			m_hOwnedGdiObj = (HGDIOBJ)::CreateSolidBrush(crColor);
			m_hOldGdiObj = ::SelectObject(m_hDC, m_hOwnedGdiObj);
		}
	}
	
public:
	BrushDC(HDC hDC, StockBrushStyle stockBrush)
		: GdiObjectDC(hDC, ::GetStockObject(stockBrush), NULL)
	{
	}
	BrushDC(HDC hDC, COLORREF crColor) : GdiObjectDC(hDC)
	{
		_SelectBrush(crColor);
	}

	VOID Select(StockBrushStyle stockBrush)
	{
		Unselect();
		m_hOldGdiObj = ::SelectObject(m_hDC, ::GetStockObject(stockBrush));
	}

	VOID Select(COLORREF crColor)
	{
		Unselect();
		_SelectBrush(crColor);
	}
};

// =========================================================================
// class PenDC

class PenDC : public GdiObjectDC
{
public:
	PenDC(HDC hDC, COLORREF crColor, int nWidth = 1, int nPenStyle = PS_SOLID)
		: GdiObjectDC(hDC, (HGDIOBJ)::CreatePen(nPenStyle, nWidth, crColor), TRUE)
	{
	}

	VOID Select(COLORREF crColor, int nWidth = 1, int nPenStyle = PS_SOLID)
	{
		GdiObjectDC::Select(
			(HGDIOBJ)::CreatePen(nPenStyle, nWidth, crColor), TRUE);
	}
};

// =========================================================================
// class BoldDC

class BoldDC : public GdiObjectDC
{
public:
    BoldDC(HDC hDC, BOOL bBold) : GdiObjectDC(hDC)
	{
		HFONT hFont = (HFONT)::GetCurrentObject(m_hDC, OBJ_FONT);

		LOGFONT lf;
		::GetObject(hFont, sizeof(LOGFONT), &lf);

		if (( bBold && lf.lfWeight != FW_BOLD) ||
			 (!bBold && lf.lfWeight == FW_BOLD) )
		{
			lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;

			m_hOwnedGdiObj = (HGDIOBJ)::CreateFontIndirect(&lf);
			m_hOldGdiObj = ::SelectObject(m_hDC, m_hOwnedGdiObj);
		}
	}
};

// =========================================================================
// FillClientRect

inline VOID winx_call FillClientRect(HWND hWnd, HDC hDC, HBRUSH hbrFill)
{
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	FillRect(hDC, &rcClient, hbrFill);
}

// -------------------------------------------------------------------------
// DrawBitmap - 以横纵比相同的方式缩放绘制一个Bitmap。

inline VOID winx_call DrawBitmap(
	IN HDC hdc, IN HBITMAP hbmObj, IN RECT rcPaint,
	IN INT borderWith = 0, IN HBRUSH hbrBorder = NULL, OUT RECT* rcPaintNew = NULL)
{
	if (borderWith)
		::InflateRect(&rcPaint, -borderWith, -borderWith);

	SIZE sizePaint = {
		rcPaint.right - rcPaint.left,
		rcPaint.bottom - rcPaint.top
	};

	BITMAP bmp;
	::GetObject(hbmObj, sizeof(bmp), &bmp);
	
	//if (bmp.bmWidth / bmp.bmHeight < sizePaint.cx / sizePaint.cy)
	if (bmp.bmWidth * sizePaint.cy < sizePaint.cx * bmp.bmHeight)
	{
		INT cxNew = ::MulDiv(sizePaint.cy, bmp.bmWidth, bmp.bmHeight);
		rcPaint.left += (sizePaint.cx - cxNew) >> 1;
		rcPaint.right = (rcPaint.left + cxNew);
	}
	else
	{
		INT cyNew = ::MulDiv(sizePaint.cx, bmp.bmHeight, bmp.bmWidth);
		rcPaint.top += (sizePaint.cy - cyNew) >> 1;
		rcPaint.bottom = (rcPaint.top + cyNew);
	}

	RECT rcBorder = rcPaint;
	if (borderWith)
	{
		::InflateRect(&rcBorder, borderWith, borderWith);
		::FillRect(hdc, &rcBorder, hbrBorder);
	}

	HDC hdcMem = ::CreateCompatibleDC(hdc);
	HGDIOBJ hbmOld = ::SelectObject(hdcMem, hbmObj);
	INT iOldMode = ::SetStretchBltMode(hdc, STRETCH_HALFTONE);
	::StretchBlt(
		hdc,
		rcPaint.left,
		rcPaint.top,
		rcPaint.right - rcPaint.left,
		rcPaint.bottom - rcPaint.top,
		hdcMem,
		0,
		0,
		bmp.bmWidth,
		bmp.bmHeight,
		SRCCOPY);
	::SetStretchBltMode(hdc, iOldMode);
	::SelectObject(hdcMem, hbmOld);
	::DeleteDC(hdcMem);

	if (rcPaintNew)
		*rcPaintNew = rcBorder;
}

// -----------------------------------------------------------------------
// DrawShadow

#ifndef WINX_SM_CXSHADOW
#define WINX_SM_CXSHADOW 4
#endif

inline VOID winx_call DrawShadow(
	HDC hDCIn, HDC hDCOut, const RECT& rc, const INT cxShadow = WINX_SM_CXSHADOW)
{
	int x;
    for ( x = 0; x < rc.right-1; x++ )
    {
        int nEnd = ( x > rc.right-cxShadow*2 ) ? rc.right-cxShadow-x : cxShadow;

        for ( int y = ( x < 2 ) ? 2-x : x > rc.right-cxShadow-3 ? x-rc.right+cxShadow+3 : 0; y < nEnd; y++ )
        {
            int nMakeSpec = 78+(3-(x==0?0:(x==1?(y<2?0:1):(x==2?(y<2?y:2):y))))*5;
            COLORREF cr = ::GetPixel(hDCIn, x+cxShadow, rc.bottom-y-1);
            COLORREF cr2 = RGB(((nMakeSpec * int(GetRValue(cr))) / 100),
			                   ((nMakeSpec * int(GetGValue(cr))) / 100),
			                   ((nMakeSpec * int(GetBValue(cr))) / 100));
			::SetPixel(hDCOut, x+cxShadow, rc.bottom-y-1, cr2);
        }
    }
    for ( x = 0; x < cxShadow; x++ )
    {
        for ( int y = ( x < 2 ) ? 2-x : 0; y < rc.bottom-x-cxShadow-((x>0)?1:2); y++ )
        {
            int nMakeSpec = 78+(3-(y==0?0:(y==1?(x<2?0:1):(y==2?(x<2?x:2):x))))*5;
            COLORREF cr = ::GetPixel (hDCIn, rc.right-x-1, y+cxShadow);
            COLORREF cr2 = RGB(((nMakeSpec * int(GetRValue(cr))) / 100),
			                   ((nMakeSpec * int(GetGValue(cr))) / 100),
			                   ((nMakeSpec * int(GetBValue(cr))) / 100));
			::SetPixel(hDCOut, rc.right-x-1, y+cxShadow, cr2);
        }
    }
}

// =========================================================================
// for MFC Compatibility

typedef WTL::CRgn CRgn;
typedef WTL::CPen CPen;
typedef WTL::CBrush CBrush;
typedef WTL::CFont CFont;

typedef Bitmap CBitmap;
typedef Palette CPalette;

class CDC : public DCHandle
{
public:
	CDC(HDC hDC = NULL) : DCHandle(hDC) {}
	~CDC() {
		if (m_hDC)
			::DeleteDC(m_hDC);
	}
};

typedef ClientDC CClientDC;
typedef WindowDC CWindowDC;
typedef PaintDC  CPaintDC;

// =========================================================================
// $Log: Gdi.h,v $
// Revision 1.4  2006/11/30 09:16:43  xushiwei
// "DISPLAY" missing _T()
//
// Revision 1.3  2006/08/27 19:24:35  xushiwei
// ScreenCapture
//
// Revision 1.2  2006/08/20 04:49:57  xushiwei
// MFC-Compatibility:
//   GdiObject(CDC, CClientDC, CPaintDC, CBitmap, CPalette, etc),  Diagnost(WINX_ASSERT, VERIFY, etc)
//   CreditStatic - Demonstrate how to port MFC code to WINX --- see @@code in source code
//
// Revision 1.1  2006/08/19 09:42:10  xushiwei
// WINX-Basic:
//   BasicTypes(CString, CFindFile, CRecentDocumentList, CComBSTR, etc)
//   Helper(MsgBox, GetThisModule, InitCommonControls, etc)
//   Init(CComAppInit, COleAppInit, CDebugAppInit, CComModuleInit, etc)
//   Resource(GetModuleIniPath, Bitmap, Icon, Menu, Brush, Accelerator, etc)
//   Gdi(Rgn, Pen, Brush, Font, Bitmap, DC, DoubleBuffer, etc)
//   WindowHandle, WindowMap, MessageMap
//   COM Support(Object, StackObject, FakeObject, COLESTR, etc)
//

__WINX_END

#endif /* __WINX_WIN_GDI_H__ */
