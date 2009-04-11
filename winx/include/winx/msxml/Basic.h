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
// Module: winx/msxml/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-24 22:19:04
// 
// $Id: Basic.h,v 1.1 2006/09/25 02:31:01 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_MSXML_BASIC_H
#define WINX_MSXML_BASIC_H

#ifndef WINX_BASIC_H
#include "../Basic.h"
#endif

// -------------------------------------------------------------------------
// include

#if defined(WINX_USE_WINSDK)

#ifndef _INC_SHLWAPI
#include "../../../../winsdk/include/shlwapi.h"
#endif

#ifndef __msxml2_h__
#include "../../../../winsdk/include/msxml2.h"
#endif

#ifndef __MSXML2DID_H__
#include "../../../../winsdk/include/MsXml2DId.h"
#endif

#else // defined(WINX_USE_DEFSDK)

#ifndef _INC_SHLWAPI
#include <shlwapi.h>
#endif

#ifndef __msxml2_h__
#include <msxml2.h>
#endif

#ifndef __MSXML2DID_H__
#include <MsXml2DId.h>
#endif

#endif

// -------------------------------------------------------------------------
// linklib

#pragma comment(lib, "msxml2")

// -------------------------------------------------------------------------
// $Log: Basic.h,v $
// Revision 1.1  2006/09/25 02:31:01  xushiwei
// WINX-Extension: msxml (XSLT)
//

#endif /* WINX_MSXML_BASIC_H */
