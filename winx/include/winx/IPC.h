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
// Module: winx/IPC.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-2-9 12:26:25
// Description: InterProcess Communication
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef WINX_IPC_H
#define WINX_IPC_H

// -------------------------------------------------------------------------

#ifndef WINX_COM_IPCSERVICE_H
#include "com/IpcService.h"
#endif

#ifndef WINX_COM_ROT_H
#include "com/ROT.h"
#endif

// -------------------------------------------------------------------------
// GetActiveObject

template <class Interface>
inline HRESULT winx_call GetActiveObject(REFCLSID rclsid, Interface** pp)
{
	IUnknown* pUnk;
	HRESULT hr = GetActiveObject(rclsid, NULL, &pUnk);
	if (SUCCEEDED(hr))
	{
		hr = pUnk->QueryInterface(WINX_UUID(Interface), (void**)pp);
		pUnk->Release();
	}
	return hr;
}

// -------------------------------------------------------------------------
// $Log: ipc.h,v $

#endif /* WINX_IPC_H */
