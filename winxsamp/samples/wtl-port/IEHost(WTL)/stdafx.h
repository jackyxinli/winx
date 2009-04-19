// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E0F6A7A7_66BE_4419_8130_AC9BCE746845__INCLUDED_)
#define AFX_STDAFX_H__E0F6A7A7_66BE_4419_8130_AC9BCE746845__INCLUDED_

// Change these values to use different versions
#define WINVER          0x0400
#define _WIN32_WINNT    0x0400
//#define _WIN32_IE       0x0501

#define _WTL_USE_CSTRING
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlctl.h>

#include <atlframe.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlddx.h>

#include <exdisp.h>
#include <exdispid.h>

#if _ATL_VER < 0x0700
#undef BEGIN_MSG_MAP
#define BEGIN_MSG_MAP(x) BEGIN_MSG_MAP_EX(x)
#endif

class CListViewCtrlImpl : public CWindowImpl<CListViewCtrlImpl, CListViewCtrl>
    { DECLARE_EMPTY_MSG_MAP() };

class CStaticImpl : public CWindowImpl<CStaticImpl, CStatic>
    { DECLARE_EMPTY_MSG_MAP() };

class CEditImpl : public CWindowImpl<CEditImpl, CEdit>
    { DECLARE_EMPTY_MSG_MAP() };


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E0F6A7A7_66BE_4419_8130_AC9BCE746845__INCLUDED_)
