// FMView.cpp : implementation of the CFMView class
//

#include "stdafx.h"
#include "FiducialMarkProject.h"

#include "FMDoc.h"
#include "FMView.h"

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
END_MESSAGE_MAP()

// CFMView construction/destruction

CFMView::CFMView()
{
	// TODO: add construction code here

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

// CFMView drawing
// This function is to display the image on the screen.
// Adjust the size of the image to display to the screen.
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
	
	CSize	sizeImage;
	CRect	DrawingRect, ClientRect;
	
	// Get the client area
	GetClientRect(&ClientRect);
	

/*
	// Get the image size
	sizeImage.cx = pImage->GetWidth();
	sizeImage.cy = pImage->GetHeight();

	pImage->StretchBlt(pDC->m_hDC, 0, 0, sizeImage.cx, sizeImage.cy, SRCCOPY);

	SetScrollSizes(MM_TEXT, sizeImage);


	sizeImage = AdjustImageToFitScreenSize(pImage);

	pImage->StretchBlt(pDC->m_hDC, 0, 0, sizeImage.cx, sizeImage.cy, SRCCOPY);

	//SetScrollSizes(MM_TEXT, sizeImage);

	//UpdateStatusBar();
*/

	sizeImage.cx = pImage->GetWidth();
	sizeImage.cy = pImage->GetHeight();

	// Set the drawing area position
	DrawingRect.top		= 0;
	DrawingRect.left	= 0;
	DrawingRect.right	= max(sizeImage.cx, ClientRect.right);
	DrawingRect.bottom	= max(sizeImage.cy, ClientRect.bottom);

	// Draw the image on the view
	pDC->FillSolidRect(DrawingRect, RGB(128, 128, 128));

	pImage->StretchBlt(pDC->m_hDC, 0, 0, sizeImage.cx, sizeImage.cy, SRCCOPY);

	SetScrollSizes(MM_TEXT, sizeImage);
}

/* 
 *	To get the coordinate of the pixel
 */
void CFMView::OnLButtonDown(UINT nFlags, CPoint point)
{
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
	// Release the mouse
	m_bMouseDown = false;

	SetCursor(LoadCursor(NULL, IDC_ARROW));	
}

/* 
 *	Handle mouse move
 */
void CFMView::OnMouseMove(UINT nFlags, CPoint point)
{
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
