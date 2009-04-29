// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__D117D482_76B3_4D66_97C7_76BF79FEE8A3__INCLUDED_)
#define AFX_STDAFX_H__D117D482_76B3_4D66_97C7_76BF79FEE8A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED
#pragma warning(disable:4530)
#pragma warning(disable:4146)
#pragma warning(disable:4192)

#include <winx.h>

#define VarMsAddndrTlb     "C:\Program Files\Common Files\designer\msaddndr.dll"
#import VarMsAddndrTlb named_guids, raw_interfaces_only

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D117D482_76B3_4D66_97C7_76BF79FEE8A3__INCLUDED)
