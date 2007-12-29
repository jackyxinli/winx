// MfcTogetherView.cpp : implementation of the CMfcTogetherView class
//

#include "stdafx.h"
#include "MfcTogether.h"

#include "MfcTogetherDoc.h"
#include "MfcTogetherView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcTogetherView

IMPLEMENT_DYNCREATE(CMfcTogetherView, CListView)

BEGIN_MESSAGE_MAP(CMfcTogetherView, CListView)
	//{{AFX_MSG_MAP(CMfcTogetherView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcTogetherView construction/destruction

CMfcTogetherView::CMfcTogetherView()
{
	// TODO: add construction code here

}

CMfcTogetherView::~CMfcTogetherView()
{
}

BOOL CMfcTogetherView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMfcTogetherView drawing

void CMfcTogetherView::OnDraw(CDC* pDC)
{
	CMfcTogetherDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CListCtrl& refCtrl = GetListCtrl();
	refCtrl.InsertItem(0, "Item!");
	// TODO: add draw code for native data here
}

void CMfcTogetherView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CMfcTogetherView diagnostics

#ifdef _DEBUG
void CMfcTogetherView::AssertValid() const
{
	CListView::AssertValid();
}

void CMfcTogetherView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CMfcTogetherDoc* CMfcTogetherView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcTogetherDoc)));
	return (CMfcTogetherDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMfcTogetherView message handlers
void CMfcTogetherView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}
