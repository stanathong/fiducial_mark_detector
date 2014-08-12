/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// ResultFiducialMarkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FiducialMarkProject.h"
#include "FMDoc.h"
#include "ResultFiducialMarkDlg.h"
#include ".\resultfiducialmarkdlg.h"


// CResultFiducialMarkDlg dialog

IMPLEMENT_DYNAMIC(CResultFiducialMarkDlg, CDialog)
CResultFiducialMarkDlg::CResultFiducialMarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResultFiducialMarkDlg::IDD, pParent)
{
}

CResultFiducialMarkDlg::~CResultFiducialMarkDlg()
{
}

void CResultFiducialMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FIDUCIAL, m_ctrlStaticImg);
}


BEGIN_MESSAGE_MAP(CResultFiducialMarkDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnCloseDlg)
END_MESSAGE_MAP()


// CResultFiducialMarkDlg message handlers

void CResultFiducialMarkDlg::OnCloseDlg()
{
	DestroyWindow();
}

// To initialize the dialog by loading the resulting data to display.
BOOL CResultFiducialMarkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int pID;
	int RID = IDC_P1_X;
	int PIC_CTRL_ID = IDC_MARK1;
	MATCHING_RESULT point;
	CString strX, strY, strCCC;
	ITER_MATCHED iter;
	CRect	ClientRect;
	int iCtrlSizeX, iCtrlSizeY;

	// Get the dimension of the picture control.
	CStatic * pPictureCtrl = (CStatic *) GetDlgItem(IDC_MARK1);
	pPictureCtrl->GetClientRect(&ClientRect);
	iCtrlSizeY = ClientRect.Height();
	iCtrlSizeX = ClientRect.Width();
	
	//--- Display the fiducial mark coordinate ---
	CFMDoc * pDoc = theApp.GetCurrentDocument();
	MAP_MATCHED mapPoints = pDoc->GetMatchingResults();
	CFMImage * pImage = pDoc->GetImage();

	for (iter = mapPoints.begin(); iter != mapPoints.end(); iter++)
	{
		// Get the value of the matched point.
		pID = (*iter).first;
		point = (*iter).second;

		// Display the coordinate and CCC results on the right hand side window.
		strX.Format("%.2f", point.fX);
		SetDlgItemText(RID+(pID-1)*3, strX);

		strY.Format("%.2f", point.fY);
		SetDlgItemText(RID+(pID-1)*3+1, strY);

		strCCC.Format("%.4f", point.fCCC);
		SetDlgItemText(RID+(pID-1)*3+2, strCCC);

		CString strName = pImage->CreateSubImage(point.fX, point.fY, iCtrlSizeX, iCtrlSizeY, 1);
		m_MarkImg[pID-1].Load(strName);

		pPictureCtrl = (CStatic *) GetDlgItem(IDC_MARK1+pID-1);
		pPictureCtrl->SetBitmap((HBITMAP) m_MarkImg[pID-1]);
		pPictureCtrl->ShowWindow(SW_SHOW);
	}

	return TRUE;
}
