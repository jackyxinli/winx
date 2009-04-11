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
// Module: winx/com/SafeArray.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-2-9 12:26:25
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef WINX_COM_SAFEARRAY_H
#define WINX_COM_SAFEARRAY_H

#ifndef WINX_BASIC_H
#include "../Basic.h"
#endif

struct SABUFFER : public SAFEARRAY {};

inline void winx_call VariantInit(VARIANT* var, SABUFFER** saBuf)
{
	var->vt = VT_ARRAY|VT_UI1|VT_BYREF;
	var->pparray = (SAFEARRAY**)saBuf;
}

NS_WINX_BEGIN

// -------------------------------------------------------------------------
// SafeArrayGetSize

inline UINT SafeArrayGetSize(SAFEARRAY* lpsa, UINT iDim = 1)
{
	LONG upBound = 0, loBound = 0;
	::SafeArrayGetUBound(lpsa, iDim, &upBound);
	::SafeArrayGetLBound(lpsa, iDim, &loBound);
	return upBound - loBound + 1;
}

// -------------------------------------------------------------------------
// SACreateBuffer/SADestroyBuffer/SAGetBuffer

using ::SABUFFER;
using ::VariantInit;

inline SABUFFER* winx_call SACreateBuffer(UINT cbBuf)
{
	return (SABUFFER*)::SafeArrayCreateVector(VT_UI1, 0, cbBuf);
}

inline SABUFFER* winx_call SACreateBuffer(LPCVOID pBuf, UINT cbBuf)
{
	SAFEARRAY* lpsa = ::SafeArrayCreateVector(VT_UI1, 0, cbBuf);
	if (lpsa)
	{
		if (pBuf)
		{
			LPVOID pbData = NULL;
			::SafeArrayAccessData(lpsa, &pbData);
			::CopyMemory(pbData, pBuf, cbBuf);
			::SafeArrayUnaccessData(lpsa);
		}
	}
	return (SABUFFER*)lpsa;
}

inline HRESULT winx_call SAGetBuffer(SABUFFER* saBuf, LPVOID pBuf, UINT cbBuf)
{
	LPVOID pbData;
	HRESULT hr = ::SafeArrayAccessData(saBuf, &pbData);
	if (SUCCEEDED(hr)) {
		::CopyMemory(pBuf, pbData, cbBuf);
		::SafeArrayUnaccessData(saBuf);
	}
	return hr;
}

inline HRESULT winx_call SADestroyBuffer(SABUFFER* saBuf)
{
	return ::SafeArrayDestroy(saBuf);
}

// -------------------------------------------------------------------------
// class SABufferAccess

template <class Type>
class SABufferAccess
{
private:
	SABUFFER* m_saBuf;
	Type* m_pData;

	SABufferAccess(const SABufferAccess&);
	void operator=(const SABufferAccess&);

public:
	SABufferAccess(SABUFFER* saBuf) {
		m_pData = NULL;
		m_saBuf = saBuf;
		WINX_ASSERT(SafeArrayGetSize(saBuf) == sizeof(Type));
		::SafeArrayAccessData(saBuf, (void**)&m_pData);
	}
	~SABufferAccess() {
		if (m_pData)
			::SafeArrayUnaccessData(m_saBuf);
	}
	Type& operator* () const {
		return *m_pData;
	}
	operator Type*() const {
		return m_pData;
	}
};

// -------------------------------------------------------------------------
// class SABuffer

class SABuffer
{
private:
	SABUFFER* m_saBuf;

	SABuffer(const SABuffer&);
	void operator=(const SABuffer&);

public:
	SABuffer(UINT cbBuf) {
		m_saBuf = (SABUFFER*)::SafeArrayCreateVector(VT_UI1, 0, cbBuf);
	}
	~SABuffer() { 
		::SafeArrayDestroy(m_saBuf);
	}
	operator SABUFFER*() const {
		return m_saBuf;
	}
	SABUFFER** operator&() {
		return &m_saBuf;
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_WINX_END

#endif /* WINX_COM_SAFEARRAY_H */
