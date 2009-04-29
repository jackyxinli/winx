// MfcTogetherDoc.cpp : implementation of the CMfcTogetherDoc class
//

#include "stdafx.h"
#include "MfcTogether.h"

#include "MfcTogetherDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcTogetherDoc

IMPLEMENT_DYNCREATE(CMfcTogetherDoc, CDocument)

BEGIN_MESSAGE_MAP(CMfcTogetherDoc, CDocument)
	//{{AFX_MSG_MAP(CMfcTogetherDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcTogetherDoc construction/destruction

CMfcTogetherDoc::CMfcTogetherDoc()
{
	// TODO: add one-time construction code here

}

CMfcTogetherDoc::~CMfcTogetherDoc()
{
}

BOOL CMfcTogetherDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMfcTogetherDoc serialization

void CMfcTogetherDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMfcTogetherDoc diagnostics

#ifdef _DEBUG
void CMfcTogetherDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMfcTogetherDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMfcTogetherDoc commands
