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
// Module: stdext/registry/WinRegistry.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-2-7 13:35:33
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_REGISTRY_WINREGISTRY_H__
#define __STDEXT_REGISTRY_WINREGISTRY_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

// -------------------------------------------------------------------------
// class WinRegKeyBase

class WinRegKeyBase
{
private:
	HKEY m_hKey;

public:
	WinRegKeyBase() { m_hKey = NULL; }
	~WinRegKeyBase() { close(); }

	void winx_call close()
	{
		if (m_hKey != NULL)
		{
			::RegCloseKey(m_hKey);
			m_hKey = NULL;
		}
	}

	void winx_call flush()
	{
		ATLASSUME(m_hKey != NULL);
		::RegFlushKey(m_hKey);
	}

	HRESULT winx_call create(
		HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPTSTR lpszClass = REG_NONE,
		DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		REGSAM samDesired = KEY_ALL_ACCESS,
		LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
		LPDWORD lpdwDisposition = NULL)
	{
		ATLASSERT(hKeyParent != NULL);
		DWORD dw;
		HKEY hKey = NULL;
		LONG lRes = RegCreateKeyEx(hKeyParent, lpszKeyName, 0,
			lpszClass, dwOptions, samDesired, NULL, &hKey, &dw);
		if (lpdwDisposition != NULL)
			*lpdwDisposition = dw;
		if (lRes == ERROR_SUCCESS)
		{
			lRes = Close();
			m_hKey = hKey;
		}
		return HRESULT_FROM_WIN32(lRes);
	}

	HRESULT winx_call open(
		HKEY hKeyParent, LPCTSTR lpszKeyName,
		REGSAM samDesired = KEY_ALL_ACCESS)
	{
		ATLASSERT(hKeyParent != NULL);
		HKEY hKey = NULL;
		LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyName, 0, samDesired, &hKey);
		if (lRes == ERROR_SUCCESS)
		{
			lRes = Close();
			ATLASSERT(lRes == ERROR_SUCCESS);
			m_hKey = hKey;
		}
		return HRESULT_FROM_WIN32(lRes);
	}

	HRESULT winx_call put32i()
	{
	}
};

/////////////////////////////////////////////////////////////////////////////
// CRegKey

class CRegKey
{
public:
	CRegKey() throw();
	CRegKey( CRegKey& key ) throw();
	explicit CRegKey(HKEY hKey) throw();
	~CRegKey() throw();

	CRegKey& operator=( CRegKey& key ) throw();

// Attributes
public:
	operator HKEY() const throw();
	HKEY m_hKey;

// Operations
public:
	LONG SetGUIDValue(LPCTSTR pszValueName, REFGUID guidValue) throw();
	LONG SetBinaryValue(LPCTSTR pszValueName, const void* pValue, ULONG nBytes) throw();
	LONG SetDWORDValue(LPCTSTR pszValueName, DWORD dwValue) throw();
	LONG SetQWORDValue(LPCTSTR pszValueName, ULONGLONG qwValue) throw();
	LONG SetStringValue(LPCTSTR pszValueName, LPCTSTR pszValue, DWORD dwType = REG_SZ) throw();
	LONG SetMultiStringValue(LPCTSTR pszValueName, LPCTSTR pszValue) throw();

	ATL_DEPRECATED("CRegKey::QueryValue(DWORD, TCHAR *valueName) has been superseded by CRegKey::QueryDWORDValue")
	LONG QueryValue(__out DWORD& dwValue, __in_z_opt LPCTSTR lpszValueName);
	ATL_DEPRECATED("CRegKey::QueryValue(TCHAR *value, TCHAR *valueName) has been superseded by CRegKey::QueryStringValue and CRegKey::QueryMultiStringValue")
	LONG QueryValue(__out_ecount_part_z_opt(*pdwCount, *pdwCount) LPTSTR szValue, __in_z_opt LPCTSTR lpszValueName, __inout DWORD* pdwCount);
	LONG QueryValue(__in_z_opt LPCTSTR pszValueName, DWORD* pdwType, void* pData, ULONG* pnBytes) throw();
	LONG QueryGUIDValue(__in_z_opt LPCTSTR pszValueName, GUID& guidValue) throw();
	LONG QueryBinaryValue(__in_z_opt LPCTSTR pszValueName, void* pValue, ULONG* pnBytes) throw();
	LONG QueryDWORDValue(__in_z_opt LPCTSTR pszValueName, DWORD& dwValue) throw();
	LONG QueryQWORDValue(__in_z_opt LPCTSTR pszValueName, ULONGLONG& qwValue) throw();
	LONG QueryStringValue(__in_z_opt LPCTSTR pszValueName, __out_ecount_part_z_opt(*pnChars, *pnChars) __out_z LPTSTR pszValue, __inout ULONG* pnChars) throw();
	LONG QueryMultiStringValue(__in_z_opt LPCTSTR pszValueName, __out_ecount_part_z_opt(*pnChars, *pnChars) __out_z LPTSTR pszValue, __inout ULONG* pnChars) throw();

	// Get the key's security attributes.
	LONG GetKeySecurity(SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd, LPDWORD pnBytes) throw();
	// Set the key's security attributes.
	LONG SetKeySecurity(SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd) throw();

	LONG SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL) throw();
	static LONG WINAPI SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);

	// Create a new registry key (or open an existing one).
	LONG Create(__in HKEY hKeyParent, __in_z LPCTSTR lpszKeyName,
		__in_z LPTSTR lpszClass = REG_NONE, __in DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		__in REGSAM samDesired = KEY_READ | KEY_WRITE,
		__in_opt LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
		__in_opt LPDWORD lpdwDisposition = NULL) throw();
	// Open an existing registry key.
	LONG Open(HKEY hKeyParent, LPCTSTR lpszKeyName,
		REGSAM samDesired = KEY_READ | KEY_WRITE) throw();
	// Close the registry key.
	LONG Close() throw();
	// Flush the key's data to disk.
	LONG Flush() throw();

	// Detach the CRegKey object from its HKEY.  Releases ownership.
	HKEY Detach() throw();
	// Attach the CRegKey object to an existing HKEY.  Takes ownership.
	void Attach(HKEY hKey) throw();

	// Enumerate the subkeys of the key.
	LONG EnumKey(__in DWORD iIndex, __out_ecount_part_z(*pnNameLength, *pnNameLength) LPTSTR pszName, __inout LPDWORD pnNameLength, __out_opt FILETIME* pftLastWriteTime = NULL) throw();
	LONG NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent, BOOL bAsync = TRUE) throw();

	LONG DeleteSubKey(LPCTSTR lpszSubKey) throw();
	LONG RecurseDeleteKey(LPCTSTR lpszKey) throw();
	LONG DeleteValue(LPCTSTR lpszValue) throw();
};

inline CRegKey::CRegKey() throw() : 
	m_hKey( NULL )
{
}

inline CRegKey::CRegKey( CRegKey& key ) throw() :
	m_hKey( NULL )
{
	Attach( key.Detach() );
}

inline CRegKey::CRegKey(HKEY hKey) throw() :
	m_hKey(hKey)
{
}

inline CRegKey::~CRegKey() throw()
{Close();}

inline CRegKey& CRegKey::operator=( CRegKey& key ) throw()
{
    if(m_hKey!=key.m_hKey)
    {
	    Close();
	    Attach( key.Detach() );
    }
	return( *this );
}

inline CRegKey::operator HKEY() const throw()
{return m_hKey;}

inline HKEY CRegKey::Detach() throw()
{
	HKEY hKey = m_hKey;
	m_hKey = NULL;
	return hKey;
}

inline void CRegKey::Attach(HKEY hKey) throw()
{
	ATLASSUME(m_hKey == NULL);
	m_hKey = hKey;
}

inline LONG CRegKey::DeleteSubKey(LPCTSTR lpszSubKey) throw()
{
	ATLASSUME(m_hKey != NULL);
	return RegDeleteKey(m_hKey, lpszSubKey);
}

inline LONG CRegKey::DeleteValue(LPCTSTR lpszValue) throw()
{
	ATLASSUME(m_hKey != NULL);
	return RegDeleteValue(m_hKey, (LPTSTR)lpszValue);
}

inline LONG CRegKey::Close() throw()
{
	LONG lRes = ERROR_SUCCESS;
	if (m_hKey != NULL)
	{
		lRes = RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
	return lRes;
}

inline LONG CRegKey::Flush() throw()
{
	ATLASSUME(m_hKey != NULL);

	return ::RegFlushKey(m_hKey);
}

inline LONG CRegKey::EnumKey(__in DWORD iIndex, __out_ecount_part_z(*pnNameLength, *pnNameLength) LPTSTR pszName, __inout LPDWORD pnNameLength, __out_opt FILETIME* pftLastWriteTime) throw()
{
	FILETIME ftLastWriteTime;

	ATLASSUME(m_hKey != NULL);
	if (pftLastWriteTime == NULL)
	{
		pftLastWriteTime = &ftLastWriteTime;
	}

	return ::RegEnumKeyEx(m_hKey, iIndex, pszName, pnNameLength, NULL, NULL, NULL, pftLastWriteTime);
}

inline LONG CRegKey::NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent, BOOL bAsync) throw()
{
	ATLASSUME(m_hKey != NULL);
	ATLASSERT((hEvent != NULL) || !bAsync);

	return ::RegNotifyChangeKeyValue(m_hKey, bWatchSubtree, dwNotifyFilter, hEvent, bAsync);
}

inline LONG CRegKey::Create(__in HKEY hKeyParent, __in_z LPCTSTR lpszKeyName,
	__in_z LPTSTR lpszClass, __in DWORD dwOptions, __in REGSAM samDesired,
	__in_opt LPSECURITY_ATTRIBUTES lpSecAttr, __in_opt LPDWORD lpdwDisposition) throw()
{
	ATLASSERT(hKeyParent != NULL);
	DWORD dw;
	HKEY hKey = NULL;
	LONG lRes = RegCreateKeyEx(hKeyParent, lpszKeyName, 0,
		lpszClass, dwOptions, samDesired, lpSecAttr, &hKey, &dw);
	if (lpdwDisposition != NULL)
		*lpdwDisposition = dw;
	if (lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		m_hKey = hKey;
	}
	return lRes;
}

inline LONG CRegKey::Open(HKEY hKeyParent, LPCTSTR lpszKeyName, REGSAM samDesired) throw()
{
	ATLASSERT(hKeyParent != NULL);
	HKEY hKey = NULL;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyName, 0, samDesired, &hKey);
	if (lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		ATLASSERT(lRes == ERROR_SUCCESS);
		m_hKey = hKey;
	}
	return lRes;
}

#pragma warning(push)
#pragma warning(disable: 4996)
inline LONG CRegKey::QueryValue(DWORD& dwValue, LPCTSTR lpszValueName)
{
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	LONG lRes = RegQueryValueEx(m_hKey, lpszValueName, NULL, &dwType,
		(LPBYTE)&dwValue, &dwCount);
	ATLASSERT((lRes!=ERROR_SUCCESS) || (dwType == REG_DWORD));
	ATLASSERT((lRes!=ERROR_SUCCESS) || (dwCount == sizeof(DWORD)));
	if (dwType != REG_DWORD)
		return ERROR_INVALID_DATA;
	return lRes;
}

#pragma warning(disable: 6053)
inline LONG CRegKey::QueryValue(__out_ecount_part_z_opt(*pdwCount, *pdwCount) LPTSTR pszValue, __in_z_opt LPCTSTR lpszValueName, __inout DWORD* pdwCount)
{
	ATLENSURE(pdwCount != NULL);
	DWORD dwType = NULL;
	LONG lRes = RegQueryValueEx(m_hKey, lpszValueName, NULL, &dwType, (LPBYTE)pszValue, pdwCount);
	ATLASSERT((lRes!=ERROR_SUCCESS) || (dwType == REG_SZ) ||
			 (dwType == REG_MULTI_SZ) || (dwType == REG_EXPAND_SZ));
	if (pszValue != NULL)
	{
		if(*pdwCount>0)
		{
			switch(dwType)
			{
				case REG_SZ:
				case REG_EXPAND_SZ:
					if ((*pdwCount) % sizeof(TCHAR) != 0 || pszValue[(*pdwCount) / sizeof(TCHAR) - 1] != 0)
					{
						pszValue[0]=_T('\0');
		 				return ERROR_INVALID_DATA;
					}
					break;
				case REG_MULTI_SZ:
					if ((*pdwCount) % sizeof(TCHAR) != 0 || (*pdwCount) / sizeof(TCHAR) < 1 || pszValue[(*pdwCount) / sizeof(TCHAR) -1] != 0 || (((*pdwCount) / sizeof(TCHAR))>1 && pszValue[(*pdwCount) / sizeof(TCHAR) - 2] != 0) )
					{
						pszValue[0]=_T('\0');
						return ERROR_INVALID_DATA;
					}
					break;
				default:
					// Ensure termination
					pszValue[0]=_T('\0');
					return ERROR_INVALID_DATA;
			}
		}
		else
		{
			// this is a blank one with no data yet
			// Ensure termination
			pszValue[0]=_T('\0');
		}
	}
	return lRes;
}
#pragma warning(pop)	

inline LONG CRegKey::QueryValue(LPCTSTR pszValueName, DWORD* pdwType, void* pData, ULONG* pnBytes) throw()
{
	ATLASSUME(m_hKey != NULL);

	return( ::RegQueryValueEx(m_hKey, pszValueName, NULL, pdwType, static_cast< LPBYTE >( pData ), pnBytes) );
}

inline LONG CRegKey::QueryDWORDValue(LPCTSTR pszValueName, DWORD& dwValue) throw()
{
	LONG lRes;
	ULONG nBytes;
	DWORD dwType;

	ATLASSUME(m_hKey != NULL);

	nBytes = sizeof(DWORD);
	lRes = ::RegQueryValueEx(m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(&dwValue),
		&nBytes);
	if (lRes != ERROR_SUCCESS)
		return lRes;
	if (dwType != REG_DWORD)
		return ERROR_INVALID_DATA;

	return ERROR_SUCCESS;
}
inline LONG CRegKey::QueryQWORDValue(LPCTSTR pszValueName, ULONGLONG& qwValue) throw()
{
	LONG lRes;
	ULONG nBytes;
	DWORD dwType;

	ATLASSUME(m_hKey != NULL);

	nBytes = sizeof(ULONGLONG);
	lRes = ::RegQueryValueEx(m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(&qwValue),
		&nBytes);
	if (lRes != ERROR_SUCCESS)
		return lRes;
	if (dwType != REG_QWORD)
		return ERROR_INVALID_DATA;

	return ERROR_SUCCESS;
}

inline LONG CRegKey::QueryBinaryValue(LPCTSTR pszValueName, void* pValue, ULONG* pnBytes) throw()
{
	LONG lRes;
	DWORD dwType;

	ATLASSERT(pnBytes != NULL);
	ATLASSUME(m_hKey != NULL);

	lRes = ::RegQueryValueEx(m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(pValue),
		pnBytes);
	if (lRes != ERROR_SUCCESS)
		return lRes;
	if (dwType != REG_BINARY)
		return ERROR_INVALID_DATA;

	return ERROR_SUCCESS;
}

#pragma warning(push)
#pragma warning(disable: 6053)
/* prefast noise VSW 496818 */
inline LONG CRegKey::QueryStringValue(LPCTSTR pszValueName, LPTSTR pszValue, ULONG* pnChars) throw()
{
	LONG lRes;
	DWORD dwType;
	ULONG nBytes;

	ATLASSUME(m_hKey != NULL);
	ATLASSERT(pnChars != NULL);

	nBytes = (*pnChars)*sizeof(TCHAR);
	*pnChars = 0;
	lRes = ::RegQueryValueEx(m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(pszValue),
		&nBytes);
	
	if (lRes != ERROR_SUCCESS)
	{
		return lRes;
	}

	if(dwType != REG_SZ && dwType != REG_EXPAND_SZ)
	{
		return ERROR_INVALID_DATA;
	}

	if (pszValue != NULL)
	{
		if(nBytes!=0)
		{
			if ((nBytes % sizeof(TCHAR) != 0) || (pszValue[nBytes / sizeof(TCHAR) -1] != 0))
			{
 				return ERROR_INVALID_DATA;
            }
        }
        else
        {
            pszValue[0]=_T('\0');
        }
    }

 	*pnChars = nBytes/sizeof(TCHAR);
	
	return ERROR_SUCCESS;
}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 6053)
/* prefast noise VSW 496818 */
inline LONG CRegKey::QueryMultiStringValue(LPCTSTR pszValueName, LPTSTR pszValue, ULONG* pnChars) throw()
{
	LONG lRes;
	DWORD dwType;
	ULONG nBytes;

	ATLASSUME(m_hKey != NULL);
	ATLASSERT(pnChars != NULL);

	if (pszValue != NULL && *pnChars < 2)
		return ERROR_INSUFFICIENT_BUFFER;
		
	nBytes = (*pnChars)*sizeof(TCHAR);
	*pnChars = 0;
	
	lRes = ::RegQueryValueEx(m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(pszValue),
		&nBytes);
	if (lRes != ERROR_SUCCESS)
		return lRes;
	if (dwType != REG_MULTI_SZ)
		return ERROR_INVALID_DATA;
	if (pszValue != NULL && (nBytes % sizeof(TCHAR) != 0 || nBytes / sizeof(TCHAR) < 1 || pszValue[nBytes / sizeof(TCHAR) -1] != 0 || ((nBytes/sizeof(TCHAR))>1 && pszValue[nBytes / sizeof(TCHAR) - 2] != 0)))
		return ERROR_INVALID_DATA;

	*pnChars = nBytes/sizeof(TCHAR);

	return ERROR_SUCCESS;
}
#pragma warning(pop)

inline LONG CRegKey::QueryGUIDValue(LPCTSTR pszValueName, GUID& guidValue) throw()
{
	TCHAR szGUID[64];
	LONG lRes;
	ULONG nCount;
	HRESULT hr;

	ATLASSUME(m_hKey != NULL);

	guidValue = GUID_NULL;

	nCount = 64;
	lRes = QueryStringValue(pszValueName, szGUID, &nCount);

	if (lRes != ERROR_SUCCESS)
		return lRes;

	if(szGUID[0] != _T('{'))
		return ERROR_INVALID_DATA;

	USES_CONVERSION_EX;
	LPOLESTR lpstr = T2OLE_EX(szGUID, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
#ifndef _UNICODE
	if(lpstr == NULL) 
		return E_OUTOFMEMORY;
#endif	
		
	hr = ::CLSIDFromString(lpstr, &guidValue);
	if (FAILED(hr))
		return ERROR_INVALID_DATA;

	return ERROR_SUCCESS;
}

inline LONG WINAPI CRegKey::SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	ATLASSERT(lpszValue != NULL);
	CRegKey key;
	LONG lRes = key.Create(hKeyParent, lpszKeyName);
	if (lRes == ERROR_SUCCESS)
		lRes = key.SetStringValue(lpszValueName, lpszValue);
	return lRes;
}

inline LONG CRegKey::SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName) throw()
{
	ATLASSERT(lpszValue != NULL);
	CRegKey key;
	LONG lRes = key.Create(m_hKey, lpszKeyName);
	if (lRes == ERROR_SUCCESS)
		lRes = key.SetStringValue(lpszValueName, lpszValue);
	return lRes;
}

#pragma warning(push)
#pragma warning(disable: 4996)
inline LONG CRegKey::SetValue(DWORD dwValue, LPCTSTR pszValueName)
{
	ATLASSUME(m_hKey != NULL);
	return SetDWORDValue(pszValueName, dwValue);
}

inline LONG CRegKey::SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName, bool bMulti, int nValueLen)
{
	ATLENSURE(lpszValue != NULL);
	ATLASSUME(m_hKey != NULL);

	if (bMulti && nValueLen == -1)
		return ERROR_INVALID_PARAMETER;

	if (nValueLen == -1)
		nValueLen = lstrlen(lpszValue) + 1;

	DWORD dwType = bMulti ? REG_MULTI_SZ : REG_SZ;

	return ::RegSetValueEx(m_hKey, lpszValueName, NULL, dwType,
		reinterpret_cast<const BYTE*>(lpszValue), nValueLen*sizeof(TCHAR));
}
#pragma warning(pop)

inline LONG CRegKey::SetValue(LPCTSTR pszValueName, DWORD dwType, const void* pValue, ULONG nBytes) throw()
{
	ATLASSUME(m_hKey != NULL);
	return ::RegSetValueEx(m_hKey, pszValueName, NULL, dwType, static_cast<const BYTE*>(pValue), nBytes);
}

inline LONG CRegKey::SetBinaryValue(LPCTSTR pszValueName, const void* pData, ULONG nBytes) throw()
{
	ATLASSUME(m_hKey != NULL);
	return ::RegSetValueEx(m_hKey, pszValueName, NULL, REG_BINARY, reinterpret_cast<const BYTE*>(pData), nBytes);
}

inline LONG CRegKey::SetDWORDValue(LPCTSTR pszValueName, DWORD dwValue) throw()
{
	ATLASSUME(m_hKey != NULL);
	return ::RegSetValueEx(m_hKey, pszValueName, NULL, REG_DWORD, reinterpret_cast<const BYTE*>(&dwValue), sizeof(DWORD));
}

inline LONG CRegKey::SetQWORDValue(LPCTSTR pszValueName, ULONGLONG qwValue) throw()
{
	ATLASSUME(m_hKey != NULL);
	return ::RegSetValueEx(m_hKey, pszValueName, NULL, REG_QWORD, reinterpret_cast<const BYTE*>(&qwValue), sizeof(ULONGLONG));
}

inline LONG CRegKey::SetStringValue(__in_z_opt LPCTSTR pszValueName, __in_z LPCTSTR pszValue, __in DWORD dwType) throw()
{
	ATLASSUME(m_hKey != NULL);
	ATLENSURE(pszValue != NULL);
	ATLASSERT((dwType == REG_SZ) || (dwType == REG_EXPAND_SZ));

	return ::RegSetValueEx(m_hKey, pszValueName, NULL, dwType, reinterpret_cast<const BYTE*>(pszValue), (lstrlen(pszValue)+1)*sizeof(TCHAR));
}

inline LONG CRegKey::SetMultiStringValue(LPCTSTR pszValueName, LPCTSTR pszValue) throw()
{
	LPCTSTR pszTemp;
	ULONG nBytes;
	ULONG nLength;

	ATLASSUME(m_hKey != NULL);
	ATLENSURE(pszValue != NULL);

	// Find the total length (in bytes) of all of the strings, including the
	// terminating '\0' of each string, and the second '\0' that terminates
	// the list.
	nBytes = 0;
	pszTemp = pszValue;
	do
	{
		nLength = lstrlen(pszTemp)+1;
		pszTemp += nLength;
		nBytes += nLength*sizeof(TCHAR);
	} while (nLength != 1);

	return ::RegSetValueEx(m_hKey, pszValueName, NULL, REG_MULTI_SZ, reinterpret_cast<const BYTE*>(pszValue),
		nBytes);
}

inline LONG CRegKey::SetGUIDValue(LPCTSTR pszValueName, REFGUID guidValue) throw()
{
	OLECHAR szGUID[64];

	ATLASSUME(m_hKey != NULL);

	::StringFromGUID2(guidValue, szGUID, 64);

	USES_CONVERSION_EX;
	LPCTSTR lpstr = OLE2CT_EX(szGUID, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
#ifndef _UNICODE
	if(lpstr == NULL) 
		return E_OUTOFMEMORY;
#endif	
	return SetStringValue(pszValueName, lpstr);
}

inline LONG CRegKey::GetKeySecurity(SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd, LPDWORD pnBytes) throw()
{
	ATLASSUME(m_hKey != NULL);
	ATLASSERT(pnBytes != NULL);

	return ::RegGetKeySecurity(m_hKey, si, psd, pnBytes);
}

inline LONG CRegKey::SetKeySecurity(SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd) throw()
{
	ATLASSUME(m_hKey != NULL);
	ATLASSERT(psd != NULL);

	return ::RegSetKeySecurity(m_hKey, si, psd);
}

inline LONG CRegKey::RecurseDeleteKey(LPCTSTR lpszKey) throw()
{
	CRegKey key;
	LONG lRes = key.Open(m_hKey, lpszKey, KEY_READ | KEY_WRITE);
	if (lRes != ERROR_SUCCESS)
	{
		if (lRes != ERROR_FILE_NOT_FOUND && lRes != ERROR_PATH_NOT_FOUND)
		{
			ATLTRACE(atlTraceCOM, 0, _T("CRegKey::RecurseDeleteKey : Failed to Open Key %s(Error = %d)\n"), lpszKey, lRes);
		}
		return lRes;
	}
	FILETIME time;
	DWORD dwSize = 256;
	TCHAR szBuffer[256];
	while (RegEnumKeyEx(key.m_hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL,
		&time)==ERROR_SUCCESS)
	{
		lRes = key.RecurseDeleteKey(szBuffer);
		if (lRes != ERROR_SUCCESS)
			return lRes;
		dwSize = 256;
	}
	key.Close();
	return DeleteSubKey(lpszKey);
}

// -------------------------------------------------------------------------
// $Log: $

#endif /* __STDEXT_REGISTRY_WINREGISTRY_H__ */
