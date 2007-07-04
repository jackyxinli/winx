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
// Module: winx/com/OLESTR.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 15:58:31
// 
// $Id: OLESTR.h,v 1.1 2006/08/19 09:42:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_COM_OLESTR_H__
#define __WINX_COM_OLESTR_H__

#ifndef __WINX_BASIC_H__
#include "../Basic.h"
#endif

__WINX_BEGIN

// -------------------------------------------------------------------------
// AllocOLESTR

inline LPOLESTR winx_call AllocOLESTR(const OLECHAR* pwcs)
{
	int cb = (wcslen(pwcs) + 1) * sizeof(OLECHAR);
	LPOLESTR pdest = (LPOLESTR)::CoTaskMemAlloc(cb);
	memcpy(pdest, pwcs, cb);
	return pdest;
}

inline LPOLESTR winx_call AllocOLESTR(const OLECHAR* pwcs, UINT cch)
{
	int cb = cch * sizeof(OLECHAR);
	LPOLESTR pdest = (LPOLESTR)::CoTaskMemAlloc(cb + sizeof(OLECHAR));
	memcpy(pdest, pwcs, cb);
	pdest[cch] = '\0';
	return pdest;
}

inline VOID winx_call FreeOLESTR(LPOLESTR pdest)
{
	if (pdest)
		::CoTaskMemFree(pdest);
}

inline UINT winx_call SizeOLESTR(LPOLESTR pdest)
{
	LPMALLOC pMalloc;
	if (SUCCEEDED(::CoGetMalloc(1, &pMalloc))) {
		SIZE_T nSize = pMalloc->GetSize(pdest);
		pMalloc->Release();
		return nSize/sizeof(OLECHAR) - 1;
	}
	else {
		return 0;
	}
}

// -------------------------------------------------------------------------
// class COLESTR

class COLESTR
{
private:
	COLESTR(const COLESTR&);
	void operator=(const COLESTR&);

public:
	typedef UINT size_type;

	LPOLESTR m_str;

public:
	COLESTR() {
		m_str = NULL;
	}
	COLESTR(const OLECHAR* wcs, size_type cch) {
		m_str = AllocOLESTR(wcs, cch);
	}
	~COLESTR() {
		FreeOLESTR(m_str);
	}

	LPOLESTR* winx_call operator&() {
		WINX_ASSERT(m_str == NULL);
		return &m_str;
	}

	(operator LPOLESTR)() const {
		return m_str;
	}

	LPCOLESTR winx_call c_str() const	{
		return m_str;
	}

	VOID winx_call assign(const OLECHAR* wcs) {
		FreeOLESTR(m_str);
		m_str = AllocOLESTR(wcs);
	}

	VOID winx_call assign(const OLECHAR* wcs, size_type cch)	{
		FreeOLESTR(m_str);
		m_str = AllocOLESTR(wcs, cch);
	}

	BOOL winx_call empty() const {
		return *m_str == '\0';
	}

	size_type winx_call size() const {
		return lstrlenW(m_str);
	}

	LPOLESTR winx_call detach() {
		LPOLESTR tmp = m_str;
		m_str = NULL;
		return tmp;
	}

	void winx_call attach(LPOLESTR str) {
		FreeOLESTR(m_str);
		m_str = str;
	}

	void winx_call clear() {
		FreeOLESTR(m_str);
		m_str = NULL;
	}
};

// -------------------------------------------------------------------------
// $Log: OLESTR.h,v $
// Revision 1.1  2006/08/19 09:42:10  xushiwei
// WINX-Basic:
//   BasicTypes(CString, CFindFile, CRecentDocumentList, CComBSTR, etc)
//   Helper(MsgBox, GetThisModule, InitCommonControls, etc)
//   Init(CComAppInit, COleAppInit, CDebugAppInit, CComModuleInit, etc)
//   Resource(GetModuleIniPath, Bitmap, Icon, Menu, Brush, Accelerator, etc)
//   Gdi(Rgn, Pen, Brush, Font, Bitmap, DC, DoubleBuffer, etc)
//   WindowHandle, WindowMap, MessageMap
//   COM Support(Object, StackObject, FakeObject, COLESTR, etc)
//

__WINX_END

#endif /* __WINX_COM_OLESTR_H__ */
