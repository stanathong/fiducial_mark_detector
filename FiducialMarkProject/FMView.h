/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// FMView.h : interface of the CFMView class
//


#pragma once


class CFMView : public CScrollView
{
protected: // create from serialization only
	CFMView();
	DECLARE_DYNCREATE(CFMView)

// Attributes
public:
	CFMDoc* GetDocument() const;

// Operations
public:
	// To adjust the image to fit the window size.
	CSize AdjustImageToFitScreenSize(CFMImage * pImage);

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CFMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	// To handle panning.
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
	// To handle panning
	bool		m_bMouseDown;		// Flag whether mouse is hold down.
	XY_POSITION m_xyLCoord;			// The coordinate that the left mouse is down.
	XY_POSITION m_xyOrigin;			// The coordinate of the origin of the image.

	// To handle view size;
	int			m_iViewSize;		// The size of the image that display on the view.
									// 0 : Fit Screen
public:
	afx_msg void OnUpdateViewFitScreen(CCmdUI *pCmdUI);
	afx_msg void OnViewFullsize();
	afx_msg void OnUpdateViewFullsize(CCmdUI *pCmdUI);
	afx_msg void OnViewFitscreen();
};

#ifndef _DEBUG  // debug version in FMView.cpp
inline CFMDoc* CFMView::GetDocument() const
   { return reinterpret_cast<CFMDoc*>(m_pDocument); }
#endif

