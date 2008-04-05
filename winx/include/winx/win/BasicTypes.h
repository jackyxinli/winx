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
// Module: winx/win/BasicTypes.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 15:50:43
// 
// $Id: BasicTypes.h,v 1.3 2006/08/21 18:45:48 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WIN_BASICTYPES_H__
#define __WINX_WIN_BASICTYPES_H__

#ifndef __WINX_BASIC_H__
#include "../Basic.h"
#endif

#pragma pack(1)

__WINX_BEGIN

// =========================================================================
// BasicTypes

typedef WTL::CPoint CPoint;
typedef WTL::CRect CRect;
typedef WTL::CSize CSize;

typedef WTL::CRecentDocumentList CRecentDocumentList;
typedef WTL::CFindFile CFindFile;

typedef WTL::CAppModule CAppModule; 

typedef ::ATL::CComBSTR CComBSTR;
typedef ::ATL::CComVariant CComVariant;
typedef ::ATL::CRegKey CRegKey;

// =========================================================================
// class MENUorID, RESID

class MENUorID
{
public:
	HMENU m_hMenu;

	MENUorID(int nID) : m_hMenu((HMENU)LongToHandle(nID)) {}
	MENUorID(HMENU hMenu) : m_hMenu(hMenu) {}

	(operator HMENU)() const { return m_hMenu; }
	void operator=(int nID)  { m_hMenu = (HMENU)LongToHandle(nID); }
};

class RESID
{
public:
	LPCTSTR m_lpstr;

	RESID(int nID) : m_lpstr(MAKEINTRESOURCE(nID)) {}
	RESID(LPCTSTR lpString) : m_lpstr(lpString) {}

	(operator LPCTSTR)() const { return m_lpstr; }
};

// =========================================================================
// class CString

typedef WTL::CString CString;

inline int winx_call _LoadString(HINSTANCE hInst, UINT nID, LPTSTR lpszBuf, UINT nMaxBuf)
{
	int nLen = ::LoadString(hInst, nID, lpszBuf, nMaxBuf);
	if (nLen == 0)
		lpszBuf[0] = _T('\0');
	return nLen;
}

inline CString winx_call LoadString(UINT nID, HINSTANCE hInst = GetThisModule())
{
#ifdef _UNICODE
	enum { CHAR_FUDGE = 1 };    // one TCHAR unused is good enough
#else
	enum { CHAR_FUDGE = 2 };    // two BYTES unused for case of DBC last char
#endif

	// try fixed buffer first (to avoid wasting space in the heap)

	TCHAR szTemp[256];
	int nCount =  sizeof(szTemp) / sizeof(szTemp[0]);
	int nLen = _LoadString(hInst, nID, szTemp, nCount);
	if (nCount - nLen > CHAR_FUDGE)
		return szTemp;
	
	// try buffer size of 512, then larger size until entire string is retrieved
	
	CString str;
	int nSize = 256;
	do
	{
		nSize += 256;
		LPTSTR lpstr = str.GetBuffer(nSize - 1);
		if(lpstr == NULL)
		{
			nLen = 0;
			break;
		}
		nLen = _LoadString(hInst, nID, lpstr, nSize);
	}
	while (nSize - nLen <= CHAR_FUDGE);
	
	str.ReleaseBuffer();		
	return str;
}

// =========================================================================
// class AnsiString/UniString

typedef std::basic_string<char> AnsiString;
typedef std::basic_string<WCHAR> UniString;

// =========================================================================
// class CWaitCursor

class CWaitCursor
{
private:
	HCURSOR m_hCursorPrev;

public:
	CWaitCursor(LPCTSTR szSysRes = IDC_WAIT) {
		m_hCursorPrev = ::SetCursor(::LoadCursor(NULL, szSysRes));
	}
	~CWaitCursor() {
		::SetCursor(m_hCursorPrev);
	}
};

// =========================================================================
// HLSCOLOR

#if (0)//_WIN32_IE >= 0x0500)


typedef WORD HLS_HTYPE;
typedef WORD HLS_LTYPE;
typedef WORD HLS_STYPE;

struct HLSCOLOR
{
	WORD wHue;			// 色调
	WORD wLuminance;	// 亮度
	WORD wSaturation;	// 饱和度
	WORD wReserve;
};

#define HLS_H(hls) ((hls).wHue)
#define HLS_L(hls) ((hls).wLuminance)
#define HLS_S(hls) ((hls).wSaturation)

inline HLSCOLOR winx_call HLS(WORD wHue, WORD wLuminance, WORD wSaturation)
{
	HLSCOLOR hls = { wHue, wLuminance, wSaturation, 0 };
	return hls;
}

inline HLSCOLOR winx_call RGB2HLS(COLORREF rgb)
{
	HLSCOLOR hls;
	::ColorRGBToHLS(rgb, &hls.wHue, &hls.wLuminance, &hls.wSaturation);
	hls.wReserve = 0;
	return hls;
}

inline COLORREF winx_call HLS2RGB(HLSCOLOR hls)
{
	return ::ColorHLSToRGB(HLS_H(hls), HLS_L(hls), HLS_S(hls));
}

// =========================================================================
// HLSCOLOR

#else

typedef BYTE HLS_HTYPE;
typedef BYTE HLS_LTYPE;
typedef BYTE HLS_STYPE;
typedef DWORD HLSCOLOR;

#ifndef HLS
#define HLS(h,l,s) ((HLSCOLOR)(((BYTE)(h)|((WORD)((BYTE)(l))<<8))|(((DWORD)(BYTE)(s))<<16)))
#endif

#define HLS_H(hls) ((BYTE)(hls))
#define HLS_L(hls) ((BYTE)(((WORD)(hls)) >> 8))
#define HLS_S(hls) ((BYTE)((hls)>>16))

inline HLSCOLOR winx_call RGB2HLS(COLORREF rgb)
{
	unsigned char min1 = MIN(GetGValue(rgb), GetBValue(rgb));
	unsigned char max1 = MAX(GetGValue(rgb), GetBValue(rgb));
    unsigned char minval = MIN(GetRValue(rgb), min1);
    unsigned char maxval = MAX(GetRValue(rgb), max1);
    float mdiff  = float(maxval) - float(minval);
    float msum   = float(maxval) + float(minval);
	
    float luminance = msum / 510.0f;
    float saturation = 0.0f;
    float hue = 0.0f; 
	
    if ( maxval != minval )
    { 
        float rnorm = (maxval - GetRValue(rgb)  ) / mdiff;      
        float gnorm = (maxval - GetGValue(rgb)) / mdiff;
        float bnorm = (maxval - GetBValue(rgb) ) / mdiff;   
		
        saturation = (luminance <= 0.5f) ? (mdiff / msum) : (mdiff / (510.0f - msum));
		
        if (GetRValue(rgb) == maxval) hue = 60.0f * (6.0f + bnorm - gnorm);
        if (GetGValue(rgb) == maxval) hue = 60.0f * (2.0f + rnorm - bnorm);
        if (GetBValue(rgb) == maxval) hue = 60.0f * (4.0f + gnorm - rnorm);
        if (hue > 360.0f) hue = hue - 360.0f;
    }
    return HLS ((hue*255)/360, luminance*255, saturation*255);
}

inline BYTE winx_call _ToRGB (float rm1, float rm2, float rh)
{
	if      (rh > 360.0f) rh -= 360.0f;
	else if (rh <   0.0f) rh += 360.0f;
	
	if      (rh <  60.0f) rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;   
	else if (rh < 180.0f) rm1 = rm2;
	else if (rh < 240.0f) rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;      
	
	return (BYTE)(rm1 * 255);
}

inline COLORREF winx_call HLS2RGB(HLSCOLOR hls)
{
    float hue        = ((int)HLS_H(hls)*360)/255.0f;
    float luminance  = HLS_L(hls)/255.0f;
    float saturation = HLS_S(hls)/255.0f;
	
    if ( saturation == 0.0f )
    {
        return RGB(HLS_L(hls), HLS_L(hls), HLS_L(hls));
    }
    float rm1, rm2;
	
    if ( luminance <= 0.5f ) rm2 = luminance + luminance * saturation;  
    else                     rm2 = luminance + saturation - luminance * saturation;
    rm1 = 2.0f * luminance - rm2;   
    BYTE red   = _ToRGB (rm1, rm2, hue + 120.0f);   
    BYTE green = _ToRGB (rm1, rm2, hue);
    BYTE blue  = _ToRGB (rm1, rm2, hue - 120.0f);
	
    return RGB (red, green, blue);
}

#endif

///////////////////////////////////////////////////////////////////////////////
// Performs some modifications on the specified color : luminance and saturation

inline COLORREF winx_call HLS_TRANSFORM(COLORREF rgb, int percent_L, int percent_S)
{
    HLSCOLOR hls = RGB2HLS(rgb);
   
	HLS_HTYPE h = HLS_H(hls);
    HLS_LTYPE l = HLS_L(hls);
    HLS_STYPE s = HLS_S(hls);
	
    if ( percent_L > 0 )
    {
        l = HLS_LTYPE(l + ((255 - l) * percent_L) / 100);
    }
    else if ( percent_L < 0 )
    {
        l = HLS_LTYPE((l * (100+percent_L)) / 100);
    }
    if ( percent_S > 0 )
    {
        s = HLS_STYPE(s + ((255 - s) * percent_S) / 100);
    }
    else if ( percent_S < 0 )
    {
        s = HLS_STYPE((s * (100+percent_S)) / 100);
    }
    return HLS2RGB(HLS(h, l, s));
}

// =========================================================================
// RECT - 矩形区 - for general use

inline BOOL winx_call IsEmtpyRect(const RECT& rc) //see WinApi: IsRectEmpty
{
	return (rc.bottom <= rc.top) || (rc.right <= rc.left);
}

inline LONG winx_call GetRectArea(const RECT& rc)
{
	return (rc.bottom - rc.top) * (rc.right - rc.left);
}

inline LONGLONG winx_call GetRectAreaL(const RECT& rc)
{
	return (LONGLONG)(rc.bottom - rc.top) * (rc.right - rc.left);
}

inline POINT winx_call GetCenterPoint(const RECT& rc)
{
	POINT ptCenter = 
	{
		(rc.left + rc.right) >> 1,
		(rc.top + rc.bottom) >> 1
	};
	return ptCenter;
}

inline SIZE winx_call GetRectSize(const RECT& rc)
{
	SIZE sizeRect = 
	{
		(rc.right - rc.left),
		(rc.bottom - rc.top)
	};
	return sizeRect;
}

inline void winx_call ScreenToClient(HWND hWnd, LPRECT lpRect)
{
	WINX_ASSERT(::IsWindow(hWnd));
	::ScreenToClient(hWnd, (LPPOINT)lpRect);
	::ScreenToClient(hWnd, ((LPPOINT)lpRect)+1);
}

inline void winx_call ClientToScreen(HWND hWnd, LPRECT lpRect)
{
	WINX_ASSERT(::IsWindow(hWnd));
	::ClientToScreen(hWnd, (LPPOINT)lpRect);
	::ClientToScreen(hWnd, ((LPPOINT)lpRect)+1);
}

inline void winx_call SetWindowCenter(HWND hDlg)
{
	int width = ::GetSystemMetrics(SM_CXSCREEN);
	int height = ::GetSystemMetrics(SM_CYSCREEN);
	RECT rcDlg;
	::GetWindowRect(hDlg, &rcDlg);
	int x = (width-rcDlg.right+rcDlg.left) /2;
	int y = (height-rcDlg.bottom+rcDlg.top) /2;

	::SetWindowPos(hDlg, NULL, x, y, rcDlg.right-rcDlg.left, rcDlg.bottom-rcDlg.top, SWP_NOREPOSITION);
}

// =========================================================================
// class WindowRect, ClientRect

class WindowRect : public CRect
{
public:
    WindowRect(HWND hWnd)
		{ ::GetWindowRect(hWnd, this); }
};

class ClientRect : public CRect
{
public:
	ClientRect(HWND hWnd)
		{ ::GetClientRect(hWnd, this); }
};

// =========================================================================
// class MaximizedSize, FrameSize, ScreenSize

class MaximizedSize : public CSize
{
public:
	MaximizedSize()
		: CSize(::GetSystemMetrics(SM_CXMAXIMIZED), ::GetSystemMetrics(SM_CYMAXIMIZED))
	{
	}
};

class FrameSize : public CSize
{
public:
	FrameSize()
		: CSize(::GetSystemMetrics(SM_CXFRAME), ::GetSystemMetrics(SM_CYFRAME))
	{
	}
};

class ScreenSize : public CSize
{
public:
	ScreenSize()
		: CSize(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN))
	{
	}
};

// =========================================================================
// $Log: BasicTypes.h,v $
// Revision 1.3  2006/08/21 18:45:48  xushiwei
// WINX-Core:
//    Window::Create, MainFrame, StackWindowObject(WINX_STACK_OBJECT) support
//
// Revision 1.2  2006/08/19 16:55:05  xushiwei
// winx::LoadString
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

#pragma pack()

#endif /* __WINX_WIN_BASICTYPES_H__ */
