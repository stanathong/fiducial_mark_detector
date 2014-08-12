/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// FMView.cpp : implementation of the CFMView class
//

#include "stdafx.h"
#include "FiducialMarkProject.h"

#include "FMDoc.h"
#include "FMView.h"
#include ".\fmview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFMView

IMPLEMENT_DYNCREATE(CFMView, CScrollView)

BEGIN_MESSAGE_MAP(CFMView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_UPDATE_COMMAND_UI(ID_VIEW_FITSCREEN, OnUpdateViewFitScreen)
	ON_COMMAND(ID_VIEW_FULLSIZE, OnViewFullsize)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSIZE, OnUpdateViewFullsize)
	ON_COMMAND(ID_VIEW_FITSCREEN, OnViewFitscreen)
END_MESSAGE_MAP()

// CFMView construction/destruction

CFMView::CFMView()
{
	m_iViewSize = 0;

}

CFMView::~CFMView()
{
}

BOOL CFMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

void CFMView::OnDraw(CDC* pDC)
{
	CFMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get the pointer to the CFMImage object
	CFMImage * pImage = pDoc->GetImage();
	if (pImage == NULL)
		return;
	
	// Get the client area	
	CRect	ClientRect;
	GetClientRect(&ClientRect);

	// Get the size of the image
	CSize	sizeImage;
	sizeImage.cx = pImage->GetWidth();
	sizeImage.cy = pImage->GetHeight();

	// FIT SCREEN
	if (m_iViewSize == FIT_SCREEN)
	{
		// Compute the ratio between widths and the ratio between the heights and pick the smallest one:
		float ratio = min ((float) ClientRect.right / sizeImage.cx, (float) ClientRect.bottom / sizeImage.cy);

		// Compute the necessary size of the image:
		CSize sizeDrawingImage;
		sizeDrawingImage.cx = (LONG) (sizeImage.cx * ratio);
		sizeDrawingImage.cy = (LONG) (sizeImage.cy * ratio);

		// The rect for drawing:
		CRect rectDrawing(CPoint(0,0), sizeDrawingImage);

		// Center the rect:
		rectDrawing.OffsetRect((ClientRect.right - sizeDrawingImage.cx) / 2, (ClientRect.bottom - sizeDrawingImage.cy) / 2);

		// Draw the image:
		pImage->StretchBlt(pDC->m_hDC, rectDrawing.left, rectDrawing.top, rectDrawing.Width(), rectDrawing.Height(), SRCCOPY);	
	}
	else if (m_iViewSize == FULL_IMAGE)
	{
		pImage->StretchBlt(pDC->m_hDC, 0, 0, sizeImage.cx, sizeImage.cy, SRCCOPY);

		SetScrollSizes(MM_TEXT, sizeImage);
	}
}

/* 
 *	To get the coordinate of the pixel
 */
void CFMView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_iViewSize == FIT_SCREEN)
		return;

	// Set flag that the mouse is down.
	m_bMouseDown = true;

	// Store the coordinate of the center pixel.
	m_xyLCoord.iX	= point.x;	
	m_xyLCoord.iY	= point.y;	

	SetCursor(LoadCursor(NULL, IDC_SIZEALL));
}

/* 
 *	Reset the panning operation
 */
void CFMView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_iViewSize == FIT_SCREEN)
		return;

	// Release the mouse
	m_bMouseDown = false;

	SetCursor(LoadCursor(NULL, IDC_ARROW));	
}

/* 
 *	Handle mouse move
 */
void CFMView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_iViewSize == FIT_SCREEN)
		return;

	// Start the panning operation
	if (m_bMouseDown == true)
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));

		// Update the origin of the image
        m_xyOrigin.iX	= m_xyOrigin.iX + point.x - m_xyLCoord.iX;
		m_xyOrigin.iY	= m_xyOrigin.iY + point.y - m_xyLCoord.iY;

		// Handle the case that the coordinate beyond the valid value.
		if (m_xyOrigin.iX < 0)	m_xyOrigin.iX = 0;
		if (m_xyOrigin.iY < 0)	m_xyOrigin.iY = 0;

		// Get the pointer to the CLiDARImage object
		CFMDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		CFMImage * pImage = pDoc->GetImage();
		if (pImage == NULL)
			return;

		if (m_xyOrigin.iX > pImage->GetWidth())		m_xyOrigin.iX = pImage->GetWidth();
		if (m_xyOrigin.iY > pImage->GetHeight())	m_xyOrigin.iY = pImage->GetHeight();
	
		// Set the scroll bar position
		POINT ptOrigin;
		ptOrigin.x = m_xyOrigin.iX;
		ptOrigin.y = m_xyOrigin.iY;

		// Adjust the scroll bar to the new position
		ScrollToPosition(ptOrigin);
		SetScrollPos(SB_HORZ, ptOrigin.x, TRUE);
		SetScrollPos(SB_VERT, ptOrigin.y, TRUE);
	}
}

/*
 * To calculate the size of imagge to fit the window
 */
CSize CFMView::AdjustImageToFitScreenSize(CFMImage * pImage)
{
	CSize	sizeImage, sizeDrawingImage;
	CRect	ClientRect;
	
	// Get the client area
	GetClientRect(&ClientRect);

	// Get the size of image
	sizeImage.cx = pImage->GetWidth();
	sizeImage.cy = pImage->GetHeight();

	// Calculate the ratio to adjust the image to fit the window size.
	int dMinWindowSize	= min(ClientRect.Width(), ClientRect.Height());
	int dMaxImageSize	= max(sizeImage.cx, sizeImage.cy);
	double dFitRatio	= (double)dMinWindowSize/(double)dMaxImageSize;

	sizeDrawingImage.cx = (double)sizeImage.cx * dFitRatio;
	sizeDrawingImage.cy = (double)sizeImage.cy * dFitRatio;

	return sizeDrawingImage;
}


void CFMView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CFMView printing

BOOL CFMView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFMView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFMView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CFMView diagnostics

#ifdef _DEBUG
void CFMView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFMView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFMDoc* CFMView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFMDoc)));
	return (CFMDoc*)m_pDocument;
}
#endif //_DEBUG


// CFMView message handlers

void CFMView::OnUpdateViewFitScreen(CCmdUI *pCmdUI)
{
	if (m_iViewSize == FIT_SCREEN)
	{
		// Set check on the menu
		pCmdUI->SetCheck(TRUE);
		// m_iViewSize = FIT_SCREEN;
	}
	else
	{
		// Set check on the menu
		pCmdUI->SetCheck(FALSE);
		// m_iViewSize = FIT_SCREEN;
	}
}

void CFMView::OnViewFullsize()
{
	m_iViewSize = FULL_IMAGE;

	CFMDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL);
}

void CFMView::OnUpdateViewFullsize(CCmdUI *pCmdUI)
{
	if (m_iViewSize == FULL_IMAGE)
	{
		// Set check on the menu
		pCmdUI->SetCheck(TRUE);
		// m_iViewSize = FIT_SCREEN;
	}
	else
	{
		// Set check on the menu
		pCmdUI->SetCheck(FALSE);
		// m_iViewSize = FIT_SCREEN;
	}
}

void CFMView::OnViewFitscreen()
{
	m_iViewSize = FIT_SCREEN;

	CFMDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL);
}
