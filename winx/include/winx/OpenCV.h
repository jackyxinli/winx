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
// Module: winx/OpenCV.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-10-6 11:55:16
// 
// $Id: OpenCV.h,v 1.3 2006/10/08 01:54:06 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_OPENCV_H__
#define __WINX_OPENCV_H__

#if (0)
#define WINX_DEBUG_OPENCV
#define WINX_DEBUG_HIGHGUI
#endif

#if !defined(_DEBUG)
#undef WINX_DEBUG_OPENCV
#undef WINX_DEBUG_HIGHGUI
#endif

// -------------------------------------------------------------------------

#ifndef _CV_H_
#include "../../../opencv/include/cv.h"
#endif

#ifndef _HIGH_GUI_
#include "../../../opencv/include/highgui.h"
#endif

// -------------------------------------------------------------------------

#if defined(WINX_DEBUG_OPENCV)

#pragma comment(lib, "cvd.lib")
#pragma comment(lib, "cvauxd.lib")
#pragma comment(lib, "cvcamd.lib")
#pragma comment(lib, "cvhaartrainingd.lib")
#pragma comment(lib, "cxcored.lib")
#pragma comment(lib, "cxtsd.lib")
#pragma comment(lib, "mld.lib")
#pragma comment(lib, "trsd.lib")

#else

#pragma comment(lib, "cv.lib")
#pragma comment(lib, "cvaux.lib")
#pragma comment(lib, "cvcam.lib")
#pragma comment(lib, "cvhaartraining.lib")
#pragma comment(lib, "cxcore.lib")
#pragma comment(lib, "cxts.lib")
#pragma comment(lib, "ml.lib")
#pragma comment(lib, "trs.lib")

#endif

// -------------------------------------------------------------------------

#if defined(WINX_DEBUG_HIGHGUI)
#pragma comment(lib, "highguid.lib")
#else
#pragma comment(lib, "highgui.lib")
#endif

// -------------------------------------------------------------------------

#if defined(__WINX_WINCORE_H__)

__WINX_BEGIN

template <
	class WindowClass, class HandleClass = WindowHandle,
	class Base = Window<WindowClass, HandleClass>
	>
class CvWindowT : public Base
{
	WINX_DEFAULT_BRUSH(NULL_BRUSH);
	WINX_DEFAULT_STYLE(WS_OVERLAPPEDWINDOW);

protected:
	HGDIOBJ m_imgOld;
	UINT m_uFlags;
	HDC m_dcMem;
	
private:
	static void winx_call _FillBitmapInfo(
		BITMAPINFO* bmi, int width, int height, int bpp, int origin )
	{
		WINX_ASSERT( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
		
		BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);
		
		memset( bmih, 0, sizeof(*bmih));
		bmih->biSize = sizeof(BITMAPINFOHEADER);
		bmih->biWidth = width;
		bmih->biHeight = origin ? abs(height) : -abs(height);
		bmih->biPlanes = 1;
		bmih->biBitCount = (unsigned short)bpp;
		bmih->biCompression = BI_RGB;
		
		if( bpp == 8 )
		{
			RGBQUAD* palette = bmi->bmiColors;
			int i;
			for( i = 0; i < 256; i++ )
			{
				palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
				palette[i].rgbReserved = 0;
			}
		}
	}
	
	// returns TRUE if there is a problem such as ERROR_IO_PENDING.
	bool winx_call _GetBitmapData( SIZE* size, int* channels, void** data )
	{
		BITMAP bmp;
		::GdiFlush();
		HGDIOBJ h = ::GetCurrentObject( m_dcMem, OBJ_BITMAP );
		if( size )
			size->cx = size->cy = 0;
		if( data )
			*data = 0;
		
		if (h == NULL)
			return true;
		if (GetObject(h, sizeof(bmp), &bmp) == 0)
			return true;
		
		if( size )
		{
			size->cx = abs(bmp.bmWidth);
			size->cy = abs(bmp.bmHeight);
		}
		
		if( channels )
			*channels = bmp.bmBitsPixel/8;
		
		if( data )
			*data = bmp.bmBits;		

		return false;
	}

	void winx_call _UpdateWindowPos()
	{
		if( (CV_WINDOW_AUTOSIZE & m_uFlags) && m_imgOld )
		{
			SIZE size;
			_GetBitmapData( &size, 0, 0 );
			::SetWindowPos(
				m_hWnd, NULL, 0, 0, size.cx, size.cy,
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}
	
public:
	CvWindowT()
		: m_uFlags(CV_WINDOW_AUTOSIZE), m_dcMem(0), m_imgOld(0)
	{
	}

	LRESULT winx_msg OnCreate(HWND hWnd, LPCREATESTRUCT lpCS)
	{
		m_dcMem = CreateCompatibleDC(0);
		return 0;
	}

	void winx_msg OnDestroy(HWND hWnd)
	{
		if (m_imgOld)
			DeleteObject(SelectObject(m_dcMem, m_imgOld));

		DeleteDC(m_dcMem);
	}

	void winx_msg OnPaint(HWND hwnd)
	{
        if( m_imgOld == 0 )
		{
			Default(hwnd);
			return;
		}

        // Determine the bitmap's dimensions
        int nchannels = 3;
        SIZE size;
        _GetBitmapData( &size, &nchannels, 0 );

        PAINTSTRUCT paint;		
        HDC hdc = ::BeginPaint(hwnd, &paint);
		::SetStretchBltMode( hdc, COLORONCOLOR );
		
        if( nchannels == 1 )
        {
			RGBQUAD table[256];
            for(int i = 0; i < 256; i++)
            {
                table[i].rgbBlue = (unsigned char)i;
                table[i].rgbGreen = (unsigned char)i;
                table[i].rgbRed = (unsigned char)i;
            }
            ::SetDIBColorTable(hdc, 0, 255, table);
        }
		
        if( CV_WINDOW_AUTOSIZE & m_uFlags )
        {
            ::BitBlt( hdc, 0, 0, size.cx, size.cy, m_dcMem, 0, 0, SRCCOPY );
        }
        else
        {
            RECT rect;
            ::GetClientRect( hwnd, &rect );
            ::StretchBlt( hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
				m_dcMem, 0, 0, size.cx, size.cy, SRCCOPY );
        }
        ::EndPaint(hwnd, &paint);
	}

	UINT winx_call GetWindowFlags() const
	{
		return m_uFlags;
	}

	void winx_call SetWindowFlags(UINT uFlags)
	{
		m_uFlags = uFlags;
	}

	void winx_call SetImage(const CvArr* arr)
	{
		CV_FUNCNAME("SetImage");
		__BEGIN__;

		SIZE size = { 0, 0 };
		int channels = 0;
		void* dst_ptr = 0;
		const int channels0 = 3;
		int origin = 0;
		CvMat stub, dst, *image;
		bool changed_size = false; // philipg
		
		if( CV_IS_IMAGE_HDR( arr ))
			origin = ((IplImage*)arr)->origin;
		
		CV_CALL( image = cvGetMat( arr, &stub ));
		
		if( m_imgOld )
		{
			// if there is something wrong with these system calls, we cannot display image...
			if (_GetBitmapData( &size, &channels, &dst_ptr ))
				return;
		}

		if( size.cx != image->width || size.cy != image->height || channels != channels0 )
		{
			changed_size = true;
			
			uchar buffer[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
			BITMAPINFO* binfo = (BITMAPINFO*)buffer;
			
			if( m_imgOld )
			{
				DeleteObject( SelectObject( m_dcMem, m_imgOld ));
				m_imgOld = 0;
			}
			
			size.cx = image->width;
			size.cy = image->height;
			channels = channels0;
			
			_FillBitmapInfo( binfo, size.cx, size.cy, channels*8, 1 );
			
			m_imgOld = SelectObject( m_dcMem, CreateDIBSection(m_dcMem, binfo,
				DIB_RGB_COLORS, &dst_ptr, 0, 0));
		}
		
		cvInitMatHeader(
			&dst, size.cy, size.cx, CV_8UC3,
			dst_ptr, (size.cx * channels + 3) & -4 );

		cvConvertImage( image, &dst, origin == 0 ? CV_CVTIMG_FLIP : 0 );
		
		// ony resize window if needed
		if (changed_size)
			_UpdateWindowPos();

		::InvalidateRect(m_hWnd, 0, 0);
		__END__;
	}

	void winx_call ShowImage(
		const CvArr* arr, HWND hWndParent, LPCTSTR szCaption = NULL)
	{
		if (m_hWnd == NULL)
			Create(hWndParent, szCaption);
		SetImage(arr);
		ShowWindow(SW_SHOW);
	}
};

class CvWindow : public CvWindowT<CvWindow>
{
	WINX_CLASS("Winx.CvWindow");
public:
	CvWindow()
	{
		RegisterClass();
	}
};

class CvMainFrame : public MainFrame<CvMainFrame>
{
	WINX_CLASS("Winx.CvMainFrame");	
public:
	CvMainFrame()
	{
		RegisterClass();
	}
};

__WINX_END

#endif // defined(__WINX_WINCORE_H__)

// -------------------------------------------------------------------------
// $Log: OpenCV.h,v $
// Revision 1.3  2006/10/08 01:54:06  xushiwei
// CvWindowT增加灵活性，可指定基类(Base)。
//
// Revision 1.2  2006/10/06 09:00:23  xushiwei
// WINX-Extension-OpenCV:
//   class CvWindowT, CvWindow, CvMainFrame
//

#endif /* __WINX_OPENCV_H__ */
