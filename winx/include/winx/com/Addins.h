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
// Module: winx/com/Addins.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-6 21:52:24
// 
// $Id: Addins.h,v 1.1 2006/12/06 14:06:25 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_COM_ADDINS_H
#define WINX_COM_ADDINS_H

NS_WINX_BEGIN

// -------------------------------------------------------------------------
// class ComAddins

namespace impl_detail {

using namespace AddInDesignerObjects;

class ComAddins
{
private:
	typedef std::vector<_IDTExtensibility2*> AddinContainer;

	IDispatch* m_app;
	AddinContainer m_addins;
	mutable ext_ConnectMode m_connMode;

public:
	ComAddins(IDispatch* app = NULL) : m_app(app), m_connMode(ext_cm_Startup)
	{
	}
	~ComAddins()
	{
		DisconnectAll();
	}

	void winx_call InitApplication(IDispatch* app)
	{
		m_app = app;
	}

	IDispatch* winx_call GetApplication() const
	{
		return m_app;
	}

	HRESULT winx_call ConnectAll(
		HKEY hKeyParent, LPCTSTR szRegKey, DWORD dwClsContext = CLSCTX_ALL, SAFEARRAY** custom = NULL)
	{
		HKEY hKey;
		LONG lRes = ::RegOpenKeyEx(hKeyParent, szRegKey, 0, KEY_READ, &hKey);
		if (lRes != ERROR_SUCCESS)
			return HRESULT_FROM_WIN32(lRes);

		FILETIME time;
		DWORD index = 0;
		DWORD dwSize = 256;
		TCHAR szBuffer[256];
		while (RegEnumKeyEx(hKey, index, szBuffer, &dwSize, NULL, NULL, NULL, &time) == ERROR_SUCCESS)
		{
			Connect(szBuffer, dwClsContext, custom);
			++index;
			dwSize = 256;
		}

		return S_OK;
	}

	HRESULT winx_call Connect(
		const CLSID& clsid, DWORD dwClsContext = CLSCTX_ALL, SAFEARRAY** custom = NULL)
	{
		_IDTExtensibility2* addin;
		HRESULT hr = ::CoCreateInstance(clsid, NULL, dwClsContext, __uuidof(_IDTExtensibility2), (void**)&addin);
		if (FAILED(hr))
			return hr;

		try
		{
			hr = addin->OnConnection(m_app, m_connMode, addin, custom);
			if (FAILED(hr))
			{
				addin->Release();
				return hr;
			}
		}
		catch (...)
		{
			WINX_REPORT("Connect addin failed!!!");
			return E_ABORT;
		}

		m_addins.push_back(addin);
		return hr;
	}

	HRESULT winx_call Connect(LPCWSTR lpszProgID, DWORD dwClsContext = CLSCTX_ALL, SAFEARRAY** custom = NULL)
	{
		CLSID clsid;
		HRESULT hr = ::CLSIDFromProgID(lpszProgID, &clsid);
		if (SUCCEEDED(hr))
			hr = Connect(clsid, dwClsContext, custom);
		return hr;
	}

	HRESULT winx_call Connect(LPCSTR lpszProgID, DWORD dwClsContext = CLSCTX_ALL, SAFEARRAY** custom = NULL)
	{
		WCHAR szBuffer[256];
		std::copy(lpszProgID, lpszProgID+strlen(lpszProgID)+1, szBuffer);
		return Connect(szBuffer, dwClsContext, custom);
	}

	HRESULT winx_call StartupComplete(SAFEARRAY** custom = NULL) const
	{
		WINX_ASSERT(m_connMode != ext_cm_AfterStartup);

		for (AddinContainer::const_iterator it = m_addins.begin(); it != m_addins.end(); ++it)
		{
			_IDTExtensibility2* addin = *it;
			try {
				addin->OnStartupComplete(custom);
			}
			catch (...) {
				//continue...
			}
		}
		m_connMode = ext_cm_AfterStartup;
		return S_OK;
	}

	HRESULT winx_call BeginShutdown(SAFEARRAY** custom = NULL) const
	{
		for (AddinContainer::const_iterator it = m_addins.begin(); it != m_addins.end(); ++it)
		{
			_IDTExtensibility2* addin = *it;
			try {
				addin->OnBeginShutdown(custom);
			}
			catch (...) {
				//continue...
			}
		}
		return S_OK;
	}

	HRESULT winx_call DisconnectAll(ext_DisconnectMode disconnMode = ext_dm_UserClosed, SAFEARRAY** custom = NULL)
	{
		while (m_addins.size())
		{
			_IDTExtensibility2* addin = m_addins.back();
			m_addins.pop_back();
			try {
				addin->OnDisconnection(disconnMode, custom);
				addin->Release();
			}
			catch (...) {
				//continue...
			}
		}
		return S_OK;
	}
}; // class ComAddins

}; // namespace impl_detail

// -------------------------------------------------------------------------

typedef impl_detail::ComAddins ComAddins;

// -------------------------------------------------------------------------
// $Log: Addins.h,v $
// Revision 1.1  2006/12/06 14:06:25  xushiwei
// class ComAddins
//

NS_WINX_END

#endif /* WINX_COM_ADDINS_H */
