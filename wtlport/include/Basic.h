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
// Module: wtlport/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:36:01
// 
// $Id: Basic.h,v 1.11 2006/12/11 05:19:17 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WTLPORT_BASIC_H__
#define __WTLPORT_BASIC_H__

namespace wtlport {};

#ifndef __WTLPORT
#define __WTLPORT				wtlport::
#define __WTLPORT_BEGIN			namespace wtlport {
#define __WTLPORT_END			}
#endif

__WTLPORT_BEGIN

// -------------------------------------------------------------------------
// class CWindow

class CWindow
{
};

// -------------------------------------------------------------------------
// $Log: Basic.h,v $
//

__WTLPORT_END

#endif /* __WTLPORT_BASIC_H__ */
