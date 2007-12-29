 /* -------------------------------------------------------------------------
//	文件名		：	MyTestAddin.h
//	创建者		：	xushiwei
//	创建时间	：	12/06/06 15:56:14
//	功能描述	：	MyDescription
//
// -----------------------------------------------------------------------*/
#ifndef __MyTestAddin_h__
#define __MyTestAddin_h__

// -------------------------------------------------------------------------
// class CMyTestAddin

class __declspec(uuid("367E9E30-2C68-435D-A1ED-A5CBF3D5533A")) CMyTestAddin;

using namespace AddInDesignerObjects;

class CMyTestAddin : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMyTestAddin, &__uuidof(CMyTestAddin)>,
	public IDispatchImpl<_IDTExtensibility2, &IID__IDTExtensibility2, &LIBID_AddInDesignerObjects>
{
public:
	DECLARE_REGISTRY_RESOURCEID(ID_RGS_MyTestAddin)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CMyTestAddin)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(_IDTExtensibility2)
	END_COM_MAP()

	CMyTestAddin()
	{
		::MessageBox(NULL, _T("CMyTestAddin constructor"), _T("Information"), MB_TOPMOST);
	}

	~CMyTestAddin()
	{
	}

public:
	STDMETHOD(OnConnection)(IDispatch * Application, 
		ext_ConnectMode ConnectMode, IDispatch * AddInInst, SAFEARRAY * * custom)
	{
		::MessageBox(NULL, _T("OnConnection"), _T("Information"), MB_TOPMOST);
		return S_OK;
	}

	STDMETHOD(OnDisconnection)(ext_DisconnectMode RemoveMode, SAFEARRAY * * custom)
	{
		::MessageBox(NULL, _T("OnDisconnection"), _T("Information"), MB_TOPMOST);
		return S_OK;
	}

	STDMETHOD(OnAddInsUpdate)(SAFEARRAY * * custom)
	{
		::MessageBox(NULL, _T("OnAddInsUpdate"), _T("Information"), MB_TOPMOST);
		return S_OK;
	}

	STDMETHOD(OnStartupComplete)(SAFEARRAY * * custom)
	{
		::MessageBox(NULL, _T("OnStartupComplete"), _T("Information"), MB_TOPMOST);
		return S_OK;
	}
	STDMETHOD(OnBeginShutdown)(SAFEARRAY * * custom)
	{
		::MessageBox(NULL, _T("OnBeginShutdown"), _T("Information"), MB_TOPMOST);
		return S_OK;
	}
};

// -------------------------------------------------------------------------

#endif /* __MyTestAddin_h__ */
