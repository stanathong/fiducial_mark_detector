/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// ProcessSummaryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FiducialMarkProject.h"
#include "ProcessSummaryDlg.h"
#include ".\processsummarydlg.h"


// CProcessSummaryDlg dialog

IMPLEMENT_DYNAMIC(CProcessSummaryDlg, CDialog)
CProcessSummaryDlg::CProcessSummaryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessSummaryDlg::IDD, pParent)
{
}

CProcessSummaryDlg::~CProcessSummaryDlg()
{
}

void CProcessSummaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// To initialize the dialog by loading the data data to display.
BOOL CProcessSummaryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*** General Information ***/

	// Access the document.
	CFMDoc * pDoc = theApp.GetCurrentDocument();
	{
		if (pDoc == NULL)
		{
			MessageBox("Failed to load dialog due to internal error.", 
						"Error", 
						MB_ICONERROR);
			return FALSE;
		}
	}

	CFMImage * pTemplate, * pBaseImage;

	if (theApp.m_Setting.m_bEnableGrayscale == true)
	{
		if ((pDoc->m_pBaseGrayImage == NULL) || (pDoc->m_pTemplateGrayImage == NULL))
		{
			MessageBox("Failed to load dialog due to internal error.", 
						"Error", 
						MB_ICONERROR);
			return FALSE;
		}

		pTemplate = pDoc->m_pTemplateGrayImage;
		pBaseImage = pDoc->m_pBaseGrayImage;
	}
	else
	{
		if ((pDoc->m_pBaseImage == NULL) || (pDoc->m_pTemplateImage == NULL))
		{
			MessageBox("Failed to load dialog due to internal error.", 
						"Error", 
						MB_ICONERROR);
			return FALSE;
		}

		pTemplate = pDoc->m_pTemplateImage;
		pBaseImage = pDoc->m_pBaseImage;	
	}

	CString str;

	// Display image dimension
	str.Format("W %d x H %d", pBaseImage->GetWidth(), pBaseImage->GetHeight());
	CStatic * ctrlImageDim = (CStatic *) GetDlgItem(IDC_TEXT_IMAGE_DIM);
	ctrlImageDim->SetWindowText(str);

	// Display template dimension
	str.Format("W %d x H %d", pTemplate->GetWidth(), pTemplate->GetHeight());
	CStatic * ctrlTempDim = (CStatic *) GetDlgItem(IDC_TEXT_TEMP_DIM);
	ctrlTempDim->SetWindowText(str);

	/*** Accuray ***/
	if (pDoc->CalculateRMSE() == true)
	{
		// Display RMSE in X coordinate
		str.Format("%.6f", pDoc->m_xRMSE);
		CStatic * ctrlxRMSE = (CStatic *) GetDlgItem(IDC_TEXT_RMSE_X);
		ctrlxRMSE->SetWindowText(str);

		// Display RMSE in Y coordinate
		str.Format("%.6f", pDoc->m_yRMSE);
		CStatic * ctrlyRMSE = (CStatic *) GetDlgItem(IDC_TEXT_RMSE_Y);
		ctrlyRMSE->SetWindowText(str);

		// Display average RMSE
		str.Format("%.6f", (pDoc->m_xRMSE+pDoc->m_yRMSE)/2);
		CStatic * ctrlavgRMSE = (CStatic *) GetDlgItem(IDC_TEXT_RMSE_AVG);
		ctrlavgRMSE->SetWindowText(str);
	}

	/*** Computation ***/
	str.Format("%d", pDoc->m_msProcessingTime/1000);
	CStatic * ctrlTime = (CStatic *) GetDlgItem(IDC_TEXT_COMP_TIME);
	ctrlTime->SetWindowText(str);

	if (pDoc->GetNumCCCOperations() == true)
	{
		str.Format("%d", (int) pDoc->m_nNumOperations);
		CStatic * ctrlOperations = (CStatic *) GetDlgItem(IDC_TEXT_CC_OPERATIONS);
		ctrlOperations->SetWindowText(str);
	}	

	return TRUE;
}



BEGIN_MESSAGE_MAP(CProcessSummaryDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnCloseDialog)
END_MESSAGE_MAP()


// CProcessSummaryDlg message handlers

void CProcessSummaryDlg::OnCloseDialog()
{
	DestroyWindow();
}
