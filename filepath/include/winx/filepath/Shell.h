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
// Module: winx/filepath/Shell.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:23:22
// 
// $Id: Shell.h,v 1.9 2006/12/22 10:22:28 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_FILEPATH_SHELL_H
#define WINX_FILEPATH_SHELL_H

#ifndef WINX_FILEPATH_BASIC_H
#include "Basic.h"
#endif

#ifndef _INC_SHLWAPI
#include <shlwapi.h>
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
// FileExists

#if !defined(WINX_NO_SHLWAPI)

inline BOOL winx_call FileExists(LPCSTR szFile)
{
	return PathFileExistsA(szFile);
}

inline BOOL winx_call FileExists(LPCWSTR szFile)
{
	return PathFileExistsW(szFile);
}

inline BOOL winx_call IsDirectory(LPCSTR szFile)
{
	return PathIsDirectoryA(szFile);
}

inline BOOL winx_call IsDirectory(LPCWSTR szFile)
{
	return PathIsDirectoryW(szFile);
}

#else

inline BOOL winx_call PathExists(LPCSTR szFile)
{
	return GetFileAttributesA(szFile) != (DWORD)-1;
}

inline BOOL winx_call FileExists(LPCSTR szFile)
{
	DWORD dw = GetFileAttributesA(szFile);
	if (dw == (DWORD)-1)
		return FALSE;
	
	return !(dw & FILE_ATTRIBUTE_DIRECTORY);
}

inline BOOL winx_call FileExists(LPCWSTR szFile)
{
	DWORD dw = GetFileAttributesW(szFile);
	if (dw == (DWORD)-1)
		return FALSE;
	
	return !(dw & FILE_ATTRIBUTE_DIRECTORY);
}

inline BOOL winx_call IsDirectory(LPCSTR szFile)
{
	DWORD dw = GetFileAttributesA(szFile);
	if (dw == (DWORD)-1)
		return FALSE;
	
	return (dw & FILE_ATTRIBUTE_DIRECTORY);
}

inline BOOL winx_call IsDirectory(LPCWSTR szFile)
{
	DWORD dw = GetFileAttributesW(szFile);
	if (dw == (DWORD)-1)
		return FALSE;
	
	return (dw & FILE_ATTRIBUTE_DIRECTORY);
}

#endif

// -------------------------------------------------------------------------
// RenameExtension

inline BOOL RenameExtension(LPSTR pszPath, LPCSTR pszExt)
{
	return PathRenameExtensionA(pszPath, pszExt);
}

inline BOOL RenameExtension(LPWSTR pszPath, LPCWSTR pszExt)
{
	return PathRenameExtensionW(pszPath, pszExt);
}

// -------------------------------------------------------------------------
// FindExtension

inline LPCSTR winx_call FindExtension(LPCSTR pszPath)
{
	return PathFindExtensionA(pszPath);
}

inline LPCWSTR winx_call FindExtension(LPCWSTR pszPath)
{
	return PathFindExtensionW(pszPath);
}

// -------------------------------------------------------------------------
// FindFileName

inline LPCSTR winx_call FindFileName(LPCSTR pszPath)
{
	return PathFindFileNameA(pszPath);
}

inline LPCWSTR winx_call FindFileName(LPCWSTR pszPath)
{
	return PathFindFileNameW(pszPath);
}

// -------------------------------------------------------------------------
// CombinePath

inline LPSTR winx_call CombinePath(LPSTR szDest, LPCSTR lpszDir, LPCSTR lpszFile)
{
	return PathCombineA(szDest, lpszDir, lpszFile);
}

inline LPWSTR winx_call CombinePath(LPWSTR szDest, LPCWSTR lpszDir, LPCWSTR lpszFile)
{
	return PathCombineW(szDest, lpszDir, lpszFile);
}

// -------------------------------------------------------------------------
// CanonicalizePath

inline BOOL winx_call CanonicalizePath(LPSTR pszBuf, LPCSTR pszPath)
{
	return PathCanonicalizeA(pszBuf, pszPath);
}

inline BOOL winx_call CanonicalizePath(LPWSTR pszBuf, LPCWSTR pszPath)
{
	return PathCanonicalizeW(pszBuf, pszPath);
}

template <class CharT>
inline BOOL winx_call CanonicalizePath(
	CharT* pszBuf, const CharT* szBasePath, const CharT* szFile)
{
	CharT szDestFile[_MAX_PATH];
	CombinePath(szDestFile, szBasePath, szFile);
	UnixPathToWindows(szDestFile);
	return CanonicalizePath(pszBuf, szDestFile);
}

// -------------------------------------------------------------------------
// CommonPrefixPath

inline int winx_call CommonPrefixPath(LPCSTR pszFile1, LPCSTR pszFile2, LPSTR achPath)
{
	return PathCommonPrefixA(pszFile1, pszFile2, achPath);
}

inline int winx_call CommonPrefixPath(LPCWSTR pszFile1, LPCWSTR pszFile2, LPWSTR achPath)
{
	return PathCommonPrefixW(pszFile1, pszFile2, achPath);
}

// -------------------------------------------------------------------------
// SearchFilePath

template <class CharT>
inline BOOL winx_call SearchFilePath(
	CharT* szDestFile, const CharT* szFileSearch, const CharT* szDir)
{
	CombinePath(szDestFile, szDir, szFileSearch);
	return FileExists(szDestFile);
}

template <class CharT>
inline BOOL winx_call SearchFilePath(
	CharT* szDestFile,
	const CharT* szFileSearch,
	const NS_STDEXT::BasicString<CharT>& strDir)
{
	CombinePath(szDestFile, strDir.stl_str().c_str(), szFileSearch);
	return FileExists(szDestFile);
}

template <class CharT, class PathT, class ValT>
inline BOOL winx_call SearchFilePath(
	CharT* szDestFile, const CharT* szFileSearch,
	const NS_STDEXT::Range<PathT, ValT>& dirs, const CharT* szBasePath = NULL)
{
	typedef typename NS_STDEXT::Range<PathT, ValT>::const_iterator Iter;
	if (szBasePath)
	{
		if (SearchFilePath(szDestFile, szFileSearch, szBasePath))
			return TRUE;
	}
	for (Iter it = dirs.begin(); it != dirs.end(); ++it)
	{
		if (SearchFilePath(szDestFile, szFileSearch, *it))
			return TRUE;
	}
	return FALSE;
}

// -------------------------------------------------------------------------
// $Log: Shell.h,v $

NS_FILEPATH_END

#endif /* WINX_FILEPATH_SHELL_H */
