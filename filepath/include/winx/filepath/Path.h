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
// Module: winx/filepath/Path.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:23:22
// 
// $Id: Path.h,v 1.9 2006/12/22 10:22:28 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_FILEPATH_PATH_H
#define WINX_FILEPATH_PATH_H

#ifndef WINX_FILEPATH_SHELL_H
#include "Shell.h"
#endif

NS_FILEPATH_BEGIN

// -------------------------------------------------------------------------
// WindowsPathToUnix

template <class CharT>
inline void winx_call WindowsPathToUnix(CharT* szFile)
{
	for (; *szFile; ++szFile)
	{
		if (*szFile == '\\')
			*szFile = '/';
	}
}

// -------------------------------------------------------------------------
// UnixPathToWindows

template <class CharT>
inline void winx_call UnixPathToWindows(CharT* szFile)
{
	for (; *szFile; ++szFile)
	{
		if (*szFile == '/')
			*szFile = '\\';
	}
}

// -------------------------------------------------------------------------
// $Log: Path.h,v $

NS_FILEPATH_END

#endif /* WINX_FILEPATH_PATH_H */
