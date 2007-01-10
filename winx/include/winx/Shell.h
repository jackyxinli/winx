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
// Module: winx/Shell.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-25 9:47:19
// 
// $Id: Shell.h,v 1.3 2006/08/26 03:47:50 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_SHELL_H__
#define __WINX_SHELL_H__

#ifndef __WINX_BASIC_H__
#include "Basic.h"
#endif

#ifndef __WINX_SHELL_AUTOCOMPLETE_H__
#include "shell/AutoComplete.h"
#endif

__WINX_BEGIN

// =========================================================================
// DropFileHandle - simple wrapper for HDROP

class DropFileHandle
{
public:
	HDROP m_hDrop;
	
public:
	DropFileHandle(HDROP hDrop = NULL) : m_hDrop(hDrop) {}
	HDROP operator=(HDROP hDrop) {
		return m_hDrop = hDrop;
	}
	
	operator HDROP() const {
		return m_hDrop;
	}
	
	UINT winx_call GetCount() const {
		return ::DragQueryFile(m_hDrop, 0xFFFFFFFF, NULL, 0);
	}
	
	UINT winx_call GetFile(UINT iFile, LPTSTR szFileName, UINT cchFileName = _MAX_PATH) const {
		return ::DragQueryFile(m_hDrop, iFile, szFileName, cchFileName);
	}
	
	CString winx_call GetFile(UINT iFile) const {
		TCHAR szFileName[_MAX_PATH];
		UINT cchFileName = ::DragQueryFile(m_hDrop, iFile, szFileName, _MAX_PATH);
		return CString(szFileName, cchFileName);
	}
	
	VOID winx_call Delete() {
		if (m_hDrop) {
			::DragFinish(m_hDrop);
			m_hDrop = NULL;
		}
	}
};

// =========================================================================
// $Log: Shell.h,v $
// Revision 1.3  2006/08/26 03:47:50  xushiwei
// Winx-Extension:
//    AutoComplete support
//    DropFileHandle(simple wrapper for HDROP)
//

__WINX_END

#endif /* __WINX_SHELL_H__ */
