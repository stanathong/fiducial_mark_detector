/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// TemplateFormView.cpp : implementation file
//

#include "stdafx.h"
#include "FiducialMarkProject.h"
#include "TemplateFormView.h"


// CTemplateFormView

IMPLEMENT_DYNCREATE(CTemplateFormView, CFormView)

CTemplateFormView::CTemplateFormView()
	: CFormView(CTemplateFormView::IDD)
{
}

CTemplateFormView::~CTemplateFormView()
{
}

void CTemplateFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTemplateFormView, CFormView)
END_MESSAGE_MAP()


// CTemplateFormView diagnostics

#ifdef _DEBUG
void CTemplateFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTemplateFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CTemplateFormView message handlers
