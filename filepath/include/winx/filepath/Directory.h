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
// Module: winx/filepath/Directory.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:23:22
// 
// $Id: Directory.h,v 1.9 2006/12/22 10:22:28 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_FILEPATH_DIRECTORY_H
#define WINX_FILEPATH_DIRECTORY_H

#ifndef WINX_FILEPATH_BASIC_H
#include "Basic.h"
#endif

#ifndef _INC_DIRECT
#include <direct.h>
#endif

NS_FILEPATH_BEGIN

// -------------------------------------------------------------------------
// class GuardChDir

template <class CharT>
class GuardChDir
{
};

template <>
class GuardChDir<char>
{
private:
	char szOldDir[_MAX_PATH];
	
public:
	GuardChDir(LPCSTR szNewDir)
	{
		getcwd(szOldDir, _MAX_PATH);
		chdir(szNewDir);
	}
	
	~GuardChDir()
	{
		chdir(szOldDir);
	}
};

typedef GuardChDir<char> GuardChDirA;

// -------------------------------------------------------------------------
// $Log: Directory.h,v $

NS_FILEPATH_END

#endif /* WINX_FILEPATH_DIRECTORY_H */
