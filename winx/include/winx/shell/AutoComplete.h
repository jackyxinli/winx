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
// Module: winx/shell/AutoComplete.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-26 1:47:19
// 
// $Id: AutoComplete.h,v 1.2 2006/10/18 12:28:33 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_SHELL_AUTOCOMPLETE_H
#define WINX_SHELL_AUTOCOMPLETE_H

#ifndef WINX_BASIC_H
#include "../Basic.h"
#endif

#ifndef WINX_COM_OBJECT_H
#include "../com/Object.h"
#endif

#ifndef WINX_WINSDK_SHELL_H
#include "../winsdk/Shell.h"
#endif

#ifndef STDEXT_ARCHIVE_H
#include "../../../../stdext/include/stdext/Archive.h"
#endif

NS_WINX_BEGIN

// =========================================================================
// GetAutoComplete/BindAutoComplete

#if !defined(WINX_NO_AUTOCOMPLETE)

typedef WindowMapEx<cateIAutoCompletePtr> ACWindowMap;

inline IAutoComplete* winx_call GetAutoComplete(IN HWND hWndEdit)
{
	return (IAutoComplete*)ACWindowMap::GetWindow(hWndEdit);
}

inline HRESULT winx_call ResetACEnumerator(IN HWND hWndEdit)
{
	IAutoComplete* pAC = GetAutoComplete(hWndEdit);
	if (pAC == NULL)
		return E_INVALIDARG;

	IAutoCompleteDropDown* pACDD;
	HRESULT hr = pAC->QueryInterface(__uuidof(IAutoCompleteDropDown), (void**)&pACDD);
	if (SUCCEEDED(hr))
		pACDD->ResetEnumerator();

	return hr;
}

inline HRESULT winx_call BindAutoComplete(
	IN HWND hWndEdit,
	IN IUnknown* punkACL,
	IN DWORD dwOptions = ACO_AUTOSUGGEST|ACO_USETAB|ACO_UPDOWNKEYDROPSLIST,
	IN LPCOLESTR pwszRegKeyPath = NULL,
	IN LPCOLESTR pwszQuickComplete = NULL)
{
	WINX_ASSERT(::IsWindow(hWndEdit));
	WINX_ASSERT(GetAutoComplete(hWndEdit) == NULL);

	IAutoComplete* pAC;
	HRESULT hr = ::CoCreateInstance(
		CLSID_AutoComplete, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IAutoComplete), (void**)&pAC
		);
	if (SUCCEEDED(hr))
	{
		hr = pAC->Init(hWndEdit, punkACL, pwszRegKeyPath, pwszQuickComplete);
		WINX_ASSERT_OK(hr);

		if (SUCCEEDED(hr) && dwOptions)
		{
			IAutoComplete2* pAC2;
			if (S_OK == pAC->QueryInterface(__uuidof(IAutoComplete2), (void**)&pAC2))
			{
				pAC2->SetOptions(dwOptions);
				pAC2->Release();
			}
		}

		WINX_VERIFY(pAC->Release());

		ACWindowMap::SetWindow(hWndEdit, pAC);
	}
	return hr;
}

// =========================================================================
// Read/Write ACHistory

template <class ArchiveType, class ACHistoryType>
inline VOID winx_call SaveACHistory(
	IN ArchiveType& ar,
	IN const ACHistoryType& acHis)
{
	for (ACHistoryType::const_iterator it = acHis.begin(); it != acHis.end(); ++it)
		ar.wputs(*it);
}

template <class ArchiveType, class ACHistoryType>
inline VOID winx_call LoadACHistory(
	IN ArchiveType& ar,
	IN OUT ACHistoryType& acHis)
{
	typedef typename ACHistoryType::value_type ACString;
	ACString str;
	while (ar.wgets(str))
		acHis.push_back(str);
}

template <class StringType, class ACHistoryType>
inline HRESULT SaveACHistoryToFile(
	IN const StringType& szFile, 
	IN const ACHistoryType& acHis)
{
	NS_STDEXT::FileWriter ar;
	HRESULT hr = ar.open(szFile);
	if (SUCCEEDED(hr))
		SaveACHistory(ar, acHis);
	return hr;
}

template <class StringType, class ACHistoryType>
inline HRESULT LoadACHistoryFromFile(
	IN const StringType& szFile,
	IN OUT ACHistoryType& acHis)
{
	NS_STDEXT::FileReader ar;
	HRESULT hr = ar.open(szFile);
	if (SUCCEEDED(hr))
		LoadACHistory(ar, acHis);
	return hr;
}

template <class ACHistoryType>
inline HRESULT SaveACHistoryToRegistry(
	IN HKEY hKeyParent,
	IN LPCTSTR lpszKeyName,
	IN const ACHistoryType& acHis)
{
	NS_STDEXT::WinRegWriter ar;
	HRESULT hr = ar.open(hKeyParent, lpszKeyName);
	if (SUCCEEDED(hr))
		SaveACHistory(ar, acHis);
	return hr;
}

template <class ACHistoryType>
inline HRESULT LoadACHistoryFromRegistry(
	IN HKEY hKeyParent,
	IN LPCTSTR lpszKeyName,
	IN OUT ACHistoryType& acHis)
{
	NS_STDEXT::WinRegReader ar;
	HRESULT hr = ar.open(hKeyParent, lpszKeyName);
	if (SUCCEEDED(hr))
		LoadACHistory(ar, acHis);
	return hr;
}

// =========================================================================
// class ACHistory

typedef std::basic_string<WCHAR> ACString;

template <class CollType>
class ACHistoryImpl : public CollType, public EnumStringSTLImpl<CollType>
{
private:
	typedef EnumStringSTLImpl<CollType> Base;

	UINT m_nLimitCount;

public:
	explicit ACHistoryImpl(UINT nLimitCount = 0xffffffff)
		: Base(*static_cast<CollType*>(this)), m_nLimitCount(nLimitCount)
	{
		WINX_ASSERT(m_nLimitCount > 0);
	}

	HRESULT winx_call Bind(
		IN HWND hWndEdit,
		IN DWORD dwOptions = ACO_AUTOSUGGEST|ACO_USETAB|ACO_UPDOWNKEYDROPSLIST,
		IN LPCOLESTR pwszRegKeyPath = NULL,
		IN LPCOLESTR pwszQuickComplete = NULL)
	{
		return BindAutoComplete(
			hWndEdit, this, dwOptions, pwszRegKeyPath, pwszQuickComplete);
	}
	
	static void winx_call Reset(HWND hWndEdit) {
		ResetACEnumerator(hWndEdit);
	}
	
	void winx_call AddItem(const ACString& acStr) {
		if (acStr.empty())
			return;
		CollType::iterator it = std::find(begin(), end(), acStr);
		if (it == end()) {
			if (size() >= m_nLimitCount)
				erase(begin(), begin()+(size()-m_nLimitCount+1));
			push_back(acStr);
			Base::Reset();
		}
	}

	void winx_call AddItem(HWND hWndEdit, const ACString& acStr) {
		AddItem(acStr);
		ResetACEnumerator(hWndEdit);
	}
	
	void winx_call SetLimitCount(UINT nLimitCount = 0xffffffff) {
		m_nLimitCount = nLimitCount;
		if (size() > m_nLimitCount) {
			erase(begin(), begin()+(size()-m_nLimitCount));
			Base::Reset();
		}
	}
	
	void winx_call SetLimitCount(HWND hWndEdit, UINT nLimitCount = 0xffffffff) {
		SetLimitCount(nLimitCount);
		ResetACEnumerator(hWndEdit);
	}
	
	void winx_call Commit(HWND hWndEdit) {
		ACString acStr;
		GetWindowText(hWndEdit, acStr);
		AddItem(acStr);
		ResetACEnumerator(hWndEdit);
	}

	template <class ArchiveType>
	void winx_call Save(ArchiveType& ar) const {
		SaveACHistory(ar, *static_cast<const CollType*>(this));
	}

	template <class ArchiveType>
	void winx_call Load(ArchiveType& ar) {
		LoadACHistory(ar, *static_cast<CollType*>(this));
	}

	template <class StringType>
	HRESULT winx_call SaveToFile(const StringType& szFile) const {
		return SaveACHistoryToFile(szFile, *static_cast<const CollType*>(this));
	}
	
	template <class StringType>
	HRESULT winx_call LoadFromFile(const StringType& szFile) {
		return LoadACHistoryFromFile(szFile, *static_cast<CollType*>(this));
	}

	HRESULT winx_call SaveToRegistry(
		IN HKEY hKeyParent,
		IN LPCTSTR lpszKeyName) const
	{
		return SaveACHistoryToRegistry(
			hKeyParent, lpszKeyName,
			*static_cast<const CollType*>(this));
	}	

	HRESULT winx_call LoadFromRegistry(
		IN HKEY hKeyParent,
		IN LPCTSTR lpszKeyName)
	{
		return LoadACHistoryFromRegistry(
			hKeyParent, lpszKeyName, *static_cast<CollType*>(this));
	}
};

typedef ACHistoryImpl<std::vector<ACString> > ACHistory;

#endif // !defined(WINX_NO_AUTOCOMPLETE)

// =========================================================================
// $Log: AutoComplete.h,v $
// Revision 1.2  2006/10/18 12:28:33  xushiwei
// stdext as independent component
//
// Revision 1.1  2006/08/26 03:47:05  xushiwei
// Winx-Extension:
//    AutoComplete support
//    DropFileHandle(simple wrapper for HDROP)
//

NS_WINX_END

#endif /* WINX_SHELL_AUTOCOMPLETE_H */
