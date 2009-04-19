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
// Module: MyTestAddin.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-6 22:52:03
// 
// $Id: MyTestAddin.cpp,v 1.1 2006/12/06 15:25:43 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "resource.h"
#include "MyTestAddin.h"
#include <string>

// -------------------------------------------------------------------------

STDMETHODIMP CMyTestAddin::OnConnection(IDispatch * Application, 
	ext_ConnectMode ConnectMode, IDispatch * AddInInst, SAFEARRAY * * custom)
{
	Application->QueryInterface(IID__Application, (void**)&m_app);
	if (m_app == NULL)
		return E_UNEXPECTED;

	m_app->MsgBox(L"OnConnection succeed!!!", L"Information");
	return S_OK;
}

STDMETHODIMP CMyTestAddin::OnDisconnection(ext_DisconnectMode RemoveMode, SAFEARRAY * * custom)
{
	if (m_app)
		m_app->Release();
	return S_OK;
}

STDMETHODIMP CMyTestAddin::OnAddInsUpdate(SAFEARRAY * * custom)
{
	m_app->MsgBox(L"OnAddInsUpdate", L"Information");
	return S_OK;
}

STDMETHODIMP CMyTestAddin::OnStartupComplete(SAFEARRAY * * custom)
{
	CComBSTR caption;
	m_app->get_Caption(&caption);
	m_app->MsgBox(caption, L"Haha, I get the caption!!!");
	return S_OK;
}

STDMETHODIMP CMyTestAddin::OnBeginShutdown(SAFEARRAY * * custom)
{
	m_app->MsgBox(L"OnBeginShutdown", L"Information");
	return S_OK;
}

// -------------------------------------------------------------------------
// $Log: MyTestAddin.cpp,v $
// Revision 1.1  2006/12/06 15:25:43  xushiwei
// ComAddin - Interact: How an com application interact with the addins.
//
