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
// Module: winx/msxml/XSLT.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-5-30 22:04:01
// 
// $Id: XSLT.h,v 1.1 2006/09/25 02:31:01 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_MSXML_XSLT_H
#define WINX_MSXML_XSLT_H

#ifndef WINX_MSXML_BASIC_H
#include "Basic.h"
#endif

NS_WINX_BEGIN

// -------------------------------------------------------------------------
// class InputBSTRVar

class InputBSTRVar : public VARIANT
{
public:
	explicit InputBSTRVar(LPCWSTR szFile) {
		vt = VT_BSTR;
		bstrVal = SysAllocString(szFile);
	}
	explicit InputBSTRVar(LPCSTR szFile) {
		USES_CONVERSION;
		vt = VT_BSTR;
		bstrVal = SysAllocString(A2W(szFile));
	}
	~InputBSTRVar() {
		SysFreeString(bstrVal);
	}
};

// -------------------------------------------------------------------------
// class InputStreamVar

class InputStreamVar : public VARIANT
{
public:
	InputStreamVar(LPCWSTR szFile, DWORD grfMode) {
		punkVal = NULL;
		SHCreateStreamOnFileW(szFile, grfMode, (IStream**)&punkVal);
		vt = punkVal ? VT_UNKNOWN : VT_EMPTY;
		WINX_ASSERT_ONCE(punkVal);
	}
	InputStreamVar(LPCSTR szFile, DWORD grfMode) {
		punkVal = NULL;
		SHCreateStreamOnFileA(szFile, grfMode, (IStream**)&punkVal);
		vt = punkVal ? VT_UNKNOWN : VT_EMPTY;
		WINX_ASSERT_ONCE(punkVal);
	}
	~InputStreamVar() {
		if (punkVal)
			punkVal->Release();
	}
};

// -------------------------------------------------------------------------
// class InputUnknownVar

class InputUnknownVar : public VARIANT
{
public:
	explicit InputUnknownVar(IUnknown* pstmFile) {
		vt = VT_UNKNOWN;
		punkVal = pstmFile;
	}
};

// -------------------------------------------------------------------------
// XSLTConvert

inline HRESULT winx_call XSLTConvert(
	const VARIANT& xslFile, const VARIANT& xmlFile, const VARIANT& outFile)
{
	VARIANT_BOOL sResult=FALSE;
	IXMLDOMDocument2* pStyleSheet=NULL;
	IXSLTemplate* pIXSLTemplate=NULL;
	IXSLProcessor* pIXSLProcessor=NULL;
	IXMLDOMDocument2* pIXMLDOMDocument=NULL;
	
	HRESULT hr = CoCreateInstance(
		CLSID_XSLTemplate, NULL, CLSCTX_SERVER, IID_IXSLTemplate, (LPVOID*)(&pIXSLTemplate));
	CHECK(hr);
	{
		// 1) load xsl file
		hr=CoCreateInstance(
			CLSID_FreeThreadedDOMDocument, NULL, CLSCTX_SERVER,
			IID_IXMLDOMDocument2, (LPVOID*)(&pStyleSheet));
		CHECK(hr);
	 
		hr=pStyleSheet->put_async(VARIANT_FALSE);
		CHECK(hr);

		hr=pStyleSheet->load(xslFile, &sResult);
		CHECK(hr);
		CHECK_BOOLEX(sResult==VARIANT_TRUE, hr=E_FAIL);

		// 2) load xml source file
        hr=CoCreateInstance(
			CLSID_DOMDocument, NULL, CLSCTX_SERVER, 
			IID_IXMLDOMDocument2, (LPVOID*)(&pIXMLDOMDocument));
		CHECK(hr);

        hr=pIXMLDOMDocument->put_async(VARIANT_FALSE);
		CHECK(hr);

		hr=pIXMLDOMDocument->load(xmlFile, &sResult);
		CHECK(hr);
		CHECK_BOOLEX(sResult==VARIANT_TRUE, hr=E_FAIL);

		// 3) transform
		hr=pIXSLTemplate->putref_stylesheet(pStyleSheet);
		CHECK(hr);
		
        hr=pIXSLTemplate->createProcessor(&pIXSLProcessor);
		CHECK(hr);
		
		hr=pIXSLProcessor->put_input(InputUnknownVar(pIXMLDOMDocument));
		CHECK(hr);
		
		hr=pIXSLProcessor->put_output(outFile);
		CHECK(hr);
		
		hr=pIXSLProcessor->transform(&sResult);
		CHECK(hr);
		CHECK_BOOLEX(sResult==VARIANT_TRUE, hr=E_FAIL);
	}

EXIT:
	RELEASE(pIXSLProcessor);
	RELEASE(pStyleSheet);
    RELEASE(pIXSLTemplate);
    RELEASE(pIXMLDOMDocument);
	WINX_ASSERT_OK(hr);
	return hr;
}

inline HRESULT winx_call XSLTConvert(
	const char* xslFile, const char* xmlFile, const char* outFile)
{
	return XSLTConvert(
		InputBSTRVar(xslFile), InputBSTRVar(xmlFile),
		InputStreamVar(outFile, STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE));
}

inline HRESULT winx_call XSLTConvert(
	const WCHAR* xslFile, const WCHAR* xmlFile, const WCHAR* outFile)
{
	return XSLTConvert(
		InputBSTRVar(xslFile), InputBSTRVar(xmlFile),
		InputStreamVar(outFile, STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE));
}

inline HRESULT winx_call XSLTConvert(
	IStream* xslFile, IStream* xmlFile, IStream* outFile)
{
	return XSLTConvert(
		InputUnknownVar(xslFile), InputUnknownVar(xmlFile), InputUnknownVar(outFile));
}

// -------------------------------------------------------------------------
// $Log: XSLT.h,v $
// Revision 1.1  2006/09/25 02:31:01  xushiwei
// WINX-Extension: msxml (XSLT)
//

NS_WINX_END

#endif /* WINX_MSXML_XSLT_H */
