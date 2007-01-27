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
// Module: mfcport/afxv_cpu.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-27 17:43:34
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __MFCPORT_AFXV_CPU_H__
#define __MFCPORT_AFXV_CPU_H__

/////////////////////////////////////////////////////////////////////////////

#ifdef _MIPS_
// specific overrides for MIPS...
#define _AFX_PACKING    8       // default MIPS alignment (required)
#endif //_MIPS_

/////////////////////////////////////////////////////////////////////////////

#ifdef _ALPHA_
// specific overrides for ALPHA...
#define _AFX_PACKING    8       // default AXP alignment (required)
#define AfxDebugBreak() _CrtDbgBreak()
#endif  //_ALPHA_

/////////////////////////////////////////////////////////////////////////////

#ifdef _PPC_
// specific overrides for PPC...
#define _AFX_PACKING    8       // default PPC alignment (required)
#endif //_PPC_

/////////////////////////////////////////////////////////////////////////////

#ifdef _IA64_
// specific overrides for IA64...
#define _AFX_PACKING    8
#define _SHADOW_DOUBLES 8
#endif //_IA64_

/////////////////////////////////////////////////////////////////////////////

#ifdef _AMD64_
// specific overrides for AMD64...
#define _AFX_PACKING    8
#endif //_AMD64_

/////////////////////////////////////////////////////////////////////////////
// $Log: $

#endif /* __MFCPORT_AFXV_CPU_H__ */
