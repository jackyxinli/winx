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
// Module: winx/AxImpl.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:22:03
// 
// $Id: AxImpl.h,v 1.1 2006/08/25 15:13:18 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_AXIMPL_H__
#define __WINX_AXIMPL_H__

// -------------------------------------------------------------------------

#ifndef __WINX_WINSDK_IE_H__
#include "winsdk/IE.h"	// to use the new version winsdk
#endif

#ifndef __ATLCOM_H__
	#if defined(WINX_USE_ATLPORT)
		#include "../../../atlport/include/atlcom.h"
	#else
		#include <atlcom.h>
	#endif
#endif

#ifndef __ATLHOST_H__
	#if defined(WINX_USE_ATLPORT)
		#include "../../../atlport/include/atlhost.h"
	#else
		#include <atlhost.h>
	#endif
#endif

// -------------------------------------------------------------------------
// $Log: AxImpl.h,v $
// Revision 1.1  2006/08/25 15:13:18  xushiwei
// Winx-Extension:
//    ActiveX, WebBrowser(IE), Theme(XPStyle) support
//

#endif /* __WINX_AXIMPL_H__ */
