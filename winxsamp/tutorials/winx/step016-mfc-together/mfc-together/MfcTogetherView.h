// MfcTogetherView.h : interface of the CMfcTogetherView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCTOGETHERVIEW_H__3CCAA869_1C3F_4728_A972_3A3353040272__INCLUDED_)
#define AFX_MFCTOGETHERVIEW_H__3CCAA869_1C3F_4728_A972_3A3353040272__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMfcTogetherView : public CListView
{
protected: // create from serialization only
	CMfcTogetherView();
	DECLARE_DYNCREATE(CMfcTogetherView)

// Attributes
public:
	CMfcTogetherDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcTogetherView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMfcTogetherView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMfcTogetherView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MfcTogetherView.cpp
inline CMfcTogetherDoc* CMfcTogetherView::GetDocument()
   { return (CMfcTogetherDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCTOGETHERVIEW_H__3CCAA869_1C3F_4728_A972_3A3353040272__INCLUDED_)
