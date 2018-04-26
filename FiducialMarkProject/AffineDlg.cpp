/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// AffineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FiducialMarkProject.h"
#include "AffineDlg.h"
#include ".\affinedlg.h"


// CAffineDlg dialog

IMPLEMENT_DYNAMIC(CAffineDlg, CDialog)
CAffineDlg::CAffineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAffineDlg::IDD, pParent)
{
}

CAffineDlg::~CAffineDlg()
{
}

void CAffineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAffineDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnCloseDialog)
END_MESSAGE_MAP()


// CAffineDlg message handlers

void CAffineDlg::OnCloseDialog()
{
	DestroyWindow();
}

// To initialize the dialog by loading the resulting data to display.
BOOL CAffineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Access the document to obtain the affine parameters.
	CFMDoc * pDoc = theApp.GetCurrentDocument();

	// Display the affine tranformation parameters.
	CString str;

	// a11
	str.Format("%.6f", pDoc->m_a11);
	SetDlgItemText(IDC_A11, str);

	// a12
	str.Format("%.6f", pDoc->m_a12);
	SetDlgItemText(IDC_A12, str);

	// a21
	str.Format("%.6f", pDoc->m_a21);
	SetDlgItemText(IDC_A21, str);

	// a22
	str.Format("%.6f", pDoc->m_a22);
	SetDlgItemText(IDC_A22, str);

	// xt
	str.Format("%.6f", pDoc->m_xt);
	SetDlgItemText(IDC_XT, str);

	// yt
	str.Format("%.6f", pDoc->m_yt);
	SetDlgItemText(IDC_YT, str);

	return TRUE;
}