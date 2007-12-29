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
// Module: aboutdlg.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-13 20:44:26
// 
// $Id: aboutdlg.h,v 1.1 2006/09/18 05:23:26 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __ABOUTDLG_H__
#define __ABOUTDLG_H__

// -------------------------------------------------------------------------
// class CAboutDlg

class CAboutDlg : public winx::ModelDialog<CAboutDlg, IDD_ABOUTBOX>
{
public:
    LRESULT OnInitDialog(HWND hDlg, HWND hDefaultFocus);
};

// -------------------------------------------------------------------------
// $Log: aboutdlg.h,v $
// Revision 1.1  2006/09/18 05:23:26  xushiwei
// port WTL application to winx
//

#endif /* __ABOUTDLG_H__ */
