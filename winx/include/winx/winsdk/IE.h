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
// Module: winx/winsdk/IE.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:22:03
// 
// $Id: IE.h,v 1.1 2006/08/25 15:13:19 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_WINSDK_IE_H__
#define __WINX_WINSDK_IE_H__

#if (0)
#define WINX_INC_MSHTML
#endif

#ifndef __WINX_WINSDK_BASIC_H__
#include "Basic.h"
#endif

// =========================================================================
// WINX_USE_WINSDK

#if defined(WINX_USE_WINSDK)

#ifndef EXDISPID_H_
#include "../../../../winsdk/include/exdispid.h"
#endif

#ifndef __exdisp_h__
#include "../../../../winsdk/include/exdisp.h"
#endif

#ifndef __tlogstg_h__
#include "../../../../winsdk/include/tlogstg.h"
#endif

#ifndef __mshtmhst_h__
#include "../../../../winsdk/include/mshtmhst.h"
#endif

#if defined(WINX_INC_MSHTML)
#ifndef __mshtml_h__
#include "../../../../winsdk/include/mshtml.h"
#endif
#endif

// =========================================================================
// WINX_USE_DEFSDK

#else // defined(WINX_USE_DEFSDK)

#ifndef EXDISPID_H_
#include <exdispid.h>
#endif

#ifndef __exdisp_h__
#include <exdisp.h>
#endif

#if !defined(WINX_VC6)
#ifndef __tlogstg_h__
#include <tlogstg.h>
#endif
#endif // !defined(WINX_VC6)

#ifndef __mshtmhst_h__
#include <mshtmhst.h>
#endif

#if defined(WINX_INC_MSHTML)
#ifndef __mshtml_h__
#include <mshtml.h>
#endif
#endif // defined(WINX_INC_MSHTML)

#endif // defined(WINX_USE_DEFSDK)

// =========================================================================
// $Log: IE.h,v $
// Revision 1.1  2006/08/25 15:13:19  xushiwei
// Winx-Extension:
//    ActiveX, WebBrowser(IE), Theme(XPStyle) support
//

#endif /* __WINX_WINSDK_IE_H__ */
