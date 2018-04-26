/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// RMSEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FiducialMarkProject.h"
#include "RMSEDlg.h"
#include ".\rmsedlg.h"
#include "CalibrationDlg.h"


// CRMSEDlg dialog

IMPLEMENT_DYNAMIC(CRMSEDlg, CDialog)
CRMSEDlg::CRMSEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRMSEDlg::IDD, pParent)
{
}

CRMSEDlg::~CRMSEDlg()
{
}

void CRMSEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// To initialize the dialog by loading the resulting data to display.
BOOL CRMSEDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Access the document to obtain the affine parameters.
	CFMDoc * pDoc = theApp.GetCurrentDocument();

	bool bCreateDlgLocal = false;
	CString str, strX, strY;
	CCalibrationDlg * pCalDlg;
	
	// Display the RMSE Error
	if (pDoc->CalculateRMSE() == true)
	{
		// Display RMSE in X coordinate
		str.Format("%.6f", pDoc->m_xRMSE);
		SetDlgItemText(IDC_RMSE_X, str);

		// Display RMSE in Y coordinate
		str.Format("%.6f", pDoc->m_yRMSE);
		SetDlgItemText(IDC_RMSE_Y, str);

		// Display average RMSE
		str.Format("%.6f", (pDoc->m_xRMSE+pDoc->m_yRMSE)/2);
		SetDlgItemText(IDC_RMSE_AVG, str);
	}

	// Display the calibration data.
	if (NULL == pDoc->m_pCalibrationDlg)
	{
		pCalDlg = new CCalibrationDlg;

		// Set flag if it is created for this purpose.
		bCreateDlgLocal = true;
	}
	else
		pCalDlg = pDoc->m_pCalibrationDlg;

	// If failed to optain the calibration data, exit.
	if (false == pCalDlg->ReadCalibrationData())
	{
		MessageBox("Failed to load the dialog.", "Internal Error", MB_ICONERROR);
		
		return FALSE;
	}

	// Display the calibration data.
	// FC#1
	strX.Format("%.3f", pCalDlg->m_FC[0].fX);
	SetDlgItemText(IDC_FC1_X, strX);
	strY.Format("%.3f", pCalDlg->m_FC[0].fY);
	SetDlgItemText(IDC_FC1_Y, strY);

	// FC#2
	strX.Format("%.3f", pCalDlg->m_FC[1].fX);
	SetDlgItemText(IDC_FC2_X, strX);
	strY.Format("%.3f", pCalDlg->m_FC[1].fY);
	SetDlgItemText(IDC_FC2_Y, strY);

	// FC#3
	strX.Format("%.3f", pCalDlg->m_FC[2].fX);
	SetDlgItemText(IDC_FC3_X, strX);
	strY.Format("%.3f", pCalDlg->m_FC[2].fY);
	SetDlgItemText(IDC_FC3_Y, strY);

	// FC#4
	strX.Format("%.3f", pCalDlg->m_FC[3].fX);
	SetDlgItemText(IDC_FC4_X, strX);
	strY.Format("%.3f", pCalDlg->m_FC[3].fY);
	SetDlgItemText(IDC_FC4_Y, strY);

	// FC#5
	strX.Format("%.3f", pCalDlg->m_FC[4].fX);
	SetDlgItemText(IDC_FC5_X, strX);
	strY.Format("%.3f", pCalDlg->m_FC[4].fY);
	SetDlgItemText(IDC_FC5_Y, strY);

	// FC#6
	strX.Format("%.3f", pCalDlg->m_FC[5].fX);
	SetDlgItemText(IDC_FC6_X, strX);
	strY.Format("%.3f", pCalDlg->m_FC[5].fY);
	SetDlgItemText(IDC_FC6_Y, strY);

	// FC#7
	strX.Format("%.3f", pCalDlg->m_FC[6].fX);
	SetDlgItemText(IDC_FC7_X, strX);
	strY.Format("%.3f", pCalDlg->m_FC[6].fY);
	SetDlgItemText(IDC_FC7_Y, strY);

	// FC#8
	strX.Format("%.3f", pCalDlg->m_FC[7].fX);
	SetDlgItemText(IDC_FC8_X, strX);
	strY.Format("%.3f", pCalDlg->m_FC[7].fY);
	SetDlgItemText(IDC_FC8_Y, strY);


	// Display the calculated fiducial coordinate.

	// FC#1
	strX.Format("%.3f", pDoc->m_dCalFC[0].fX);
	SetDlgItemText(IDC_MFC1_X, strX);
	strY.Format("%.3f", pDoc->m_dCalFC[0].fY);
	SetDlgItemText(IDC_MFC1_Y, strY);

	// FC#2
	strX.Format("%.3f", pDoc->m_dCalFC[1].fX);
	SetDlgItemText(IDC_MFC2_X, strX);
	strY.Format("%.3f", pDoc->m_dCalFC[1].fY);
	SetDlgItemText(IDC_MFC2_Y, strY);

	// FC#3
	strX.Format("%.3f", pDoc->m_dCalFC[2].fX);
	SetDlgItemText(IDC_MFC3_X, strX);
	strY.Format("%.3f", pDoc->m_dCalFC[2].fY);
	SetDlgItemText(IDC_MFC3_Y, strY);

	// FC#4
	strX.Format("%.3f", pDoc->m_dCalFC[3].fX);
	SetDlgItemText(IDC_MFC4_X, strX);
	strY.Format("%.3f", pDoc->m_dCalFC[3].fY);
	SetDlgItemText(IDC_MFC4_Y, strY);

	// FC#5
	strX.Format("%.3f", pDoc->m_dCalFC[4].fX);
	SetDlgItemText(IDC_MFC5_X, strX);
	strY.Format("%.3f", pDoc->m_dCalFC[4].fY);
	SetDlgItemText(IDC_MFC5_Y, strY);

	// FC#6
	strX.Format("%.3f", pDoc->m_dCalFC[5].fX);
	SetDlgItemText(IDC_MFC6_X, strX);
	strY.Format("%.3f", pDoc->m_dCalFC[5].fY);
	SetDlgItemText(IDC_MFC6_Y, strY);

	// FC#7
	strX.Format("%.3f", pDoc->m_dCalFC[6].fX);
	SetDlgItemText(IDC_MFC7_X, strX);
	strY.Format("%.3f", pDoc->m_dCalFC[6].fY);
	SetDlgItemText(IDC_MFC7_Y, strY);

	// FC#8
	strX.Format("%.3f", pDoc->m_dCalFC[7].fX);
	SetDlgItemText(IDC_MFC8_X, strX);
	strY.Format("%.3f", pDoc->m_dCalFC[7].fY);
	SetDlgItemText(IDC_MFC8_Y, strY);

	// Delete the object
	if (bCreateDlgLocal == true)
		delete pCalDlg;

	return TRUE;
}


BEGIN_MESSAGE_MAP(CRMSEDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnCloseDlg)
END_MESSAGE_MAP()


// CRMSEDlg message handlers

void CRMSEDlg::OnCloseDlg()
{
	DestroyWindow();
}
