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
// Module: RemoveIntermedialFiles.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: RemoveIntermedialFiles.cpp,v 1.8 2006/12/23 09:48:13 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>

// -------------------------------------------------------------------------

#define _EXTS_ALL_DIR \
	_T("ncb"), _T("opt"), _T("scc"), _T("suo"), _T("tlh"), NULL

LPCTSTR g_extsAllDir[] = {
	_EXTS_ALL_DIR
};

LPCTSTR g_extsDspDir[] = {
	_T("aps"), _T("plg"), _T("clw"), _T("dsw"), _T("user"), _T("sln"),
	_T("o"), _T("layout"), _T("win"), _T("out"),
	_EXTS_ALL_DIR
};

LPCTSTR g_extsObjDir[] = {	
	_T("obj"), _T("ilk"), _T("pdb"), _T("pch"), _T("sbr"), _T("map"),
	_T("exe"), _T("idb"), _T("res"), _T("manifest"), _T("dep"), _T("htm"),
	_T("bsc"), _T("lib"), _T("asm"), _T("dll"), _T("exp"), _T("trg"),
	_T("o"), _T("out"), _T("awx"),
	_EXTS_ALL_DIR
};

inline BOOL CanRemove(LPCTSTR szFileName, LPCTSTR szExts[])
{
	if (*szFileName == '.') // generate by cvs, etc
		return TRUE;

	LPCTSTR szExt = _tcsrchr(szFileName, '.');
	if (szExt == NULL)
		return FALSE;

	++szExt;
	for (UINT i = 0; szExts[i]; ++i) {
		if (_tcsicmp(szExts[i], szExt) == 0)
			return TRUE;
	}
	return FALSE;
}

inline void RemoveExts(const winx::CString& strDir, LPCTSTR szExts[])
{
	winx::CFindFile finder;
	for (BOOL f = finder.FindFile(strDir + _T("\\*.*")); f; f = finder.FindNextFile()) {
		if (!finder.IsDirectory()) {
			if (CanRemove(finder.m_fd.cFileName, szExts)) {
				DeleteFile(finder.GetFilePath());
			}
		}
	}
}

inline void RemovePrivateExts(const winx::CString& strDir)
{
	winx::CFindFile finder;
	for (BOOL f = finder.FindFile(strDir + _T("\\*_private.*")); f; f = finder.FindNextFile()) {
		if (!finder.IsDirectory()) {
			LPCTSTR szExt = PathFindExtension(finder.m_fd.cFileName);
			if (_tcsicmp(szExt, _T(".h")) == 0 || _tcsicmp(szExt, _T(".rc")) == 0) {
				DeleteFile(finder.GetFilePath());
			}
		}
	}
}

inline void RemoveIntermedialFiles(const winx::CString& strDir)
{
	winx::CFindFile finder;
	BOOL f = finder.FindFile(strDir + _T("\\*.dsp"));
	if (!f)
		f = finder.FindFile(strDir + _T("\\*.vcproj"));
	if (f && !finder.IsDirectory()) {
		RemoveExts(strDir, g_extsDspDir);
		RemovePrivateExts(strDir);
	}
	else {
		f = finder.FindFile(strDir + _T("\\*.obj"));
		if (!f)
			f = finder.FindFile(strDir + _T("\\*.o"));
		if (f && !finder.IsDirectory())
			RemoveExts(strDir, g_extsObjDir);
		else {
			RemoveExts(strDir, g_extsAllDir);
		}
	}
	for (f = finder.FindFile(strDir + _T("\\*.*")); f; f = finder.FindNextFile()) {
		if (finder.IsDirectory() && *finder.m_fd.cFileName != '.') {
			RemoveIntermedialFiles(finder.GetFilePath());
		}
	}
	finder.Close();
	RemoveDirectory(strDir);
}

// -------------------------------------------------------------------------
// main

CComModule _Module;

void _tmain(int argc, TCHAR* argv[])
{
	if (argc <= 1)
		return;

	RemoveIntermedialFiles(argv[1]);
}

// -------------------------------------------------------------------------
// $Log: RemoveIntermedialFiles.cpp,v $
// Revision 1.8  2006/12/23 09:48:13  xushiwei
// don't remove any files in .svn directory.
//
// Revision 1.7  2006/11/23 06:14:59  xushiwei
// remove .dll etc
//
// Revision 1.6  2006/09/18 05:15:24  xushiwei
// RemoveIntermedialFiles - remove *.bsc, *.lib, *.asm files
//
// Revision 1.5  2006/09/12 00:33:44  xushiwei
// RemoveIntermedialFiles - remove all cvs history files
//
// Revision 1.4  2006/08/26 09:33:26  xushiwei
// vs2005 support
//
// Revision 1.3  2006/08/22 10:36:53  xushiwei
// WINX-Core:
//   Property(Bkgrnd, Accel, Layout), MainFrame support
//   CommandDispatch, CommandState, Demo: User-defined-control(Subclass, Superclass, SuperDialog)
//
// Revision 1.2  2006/08/21 19:08:20  xushiwei
// RemoveIntermedialFiles tool
//
