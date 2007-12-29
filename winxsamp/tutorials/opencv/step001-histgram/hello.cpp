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
// Module: hello.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 18:26:15
// 
// $Id: hello.cpp,v 1.1 2006/10/06 05:00:21 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx/CommonDialogs.h>
#include <winx/OpenCV.h>

// -------------------------------------------------------------------------

#define HDIM    256    // bin of HIST, default = 256

int main() 
{
    IplImage *src = 0, *dst = 0;
    CvHistogram *hist = 0;
    
    int n = HDIM;     
    double nn[HDIM];
    uchar T[HDIM];
    CvMat *T_mat;
    
    int x;
    int sum = 0; // sum of pixels of the source image 图像中象素点的总和
    double val = 0;

	winx::OpenFileDialog dlg(_T("Image Files(*.jpg;*.png)\0*.jpg;*.png\0All Files(*.*)\0*.*\0"));
	if (IDOK != dlg.DoModal())
		return -1;
    
    if ((src=cvLoadImage(dlg.lpstrFile, 0)) == NULL)  // force to gray image
        return -1;
    
    cvNamedWindow( "source", 1 );
    cvNamedWindow( "result", 1 );
    
    // calculate histgram 计算直方图
    hist = cvCreateHist( 1, &n, CV_HIST_ARRAY, 0, 1 );  
    cvCalcHist( &src, hist, 0, 0 ); 
    
    // Create Accumulative Distribute Function of histgram
    val = 0;
    for ( x = 0; x < n; x++)
    {
        val = val + cvGetReal1D (hist->bins, x);
        nn[x] = val;
    }
	
    // Compute intensity transformation 计算变换函数的离散形式
    sum = src->height * src->width;
    for( x = 0; x < n; x++ )
    {
        T[x] = (uchar) (255 * nn[x] / sum); // range is [0,255]
    }
	
    // Do intensity transform for source image
    dst = cvCloneImage( src );
    T_mat = cvCreateMatHeader( 1, 256, CV_8UC1 );
    cvSetData( T_mat, T, 0 );    
    // directly use look-up-table function 直接调用内部函数完成 look-up-table 的过程
    cvLUT( src, dst, T_mat ); 
	
    cvShowImage( "source", src );
    cvShowImage( "result", dst );
    cvWaitKey(0);
	
    cvDestroyWindow("source");
    cvDestroyWindow("result");
    cvReleaseImage( &src );
    cvReleaseImage( &dst );
    cvReleaseHist ( &hist );
    
    return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.1  2006/10/06 05:00:21  xushiwei
// histgram
//
