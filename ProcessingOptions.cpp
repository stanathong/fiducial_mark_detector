/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// ProcessingOptions.cpp : implementation file
//

#include "stdafx.h"
#include "FiducialMarkProject.h"
#include "ProcessingOptions.h"
#include ".\processingoptions.h"


// CProcessingOptions dialog

IMPLEMENT_DYNAMIC(CProcessingOptions, CDialog)
CProcessingOptions::CProcessingOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessingOptions::IDD, pParent)
	, m_iSearchingOption(FALSE)
{
	m_bEnableGrayscale = true;
	m_bEnableSubpixel= false;
	m_iSearchingOption = FALSE;		// Tempolary variable
	m_bLikelihoodSearch = true;
	m_iSearchCoverage = DEFAULT_PERCENT_COVERAGE;;
}

CProcessingOptions::~CProcessingOptions()
{
}

void CProcessingOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_GRAYSCALE, m_ctrlEnableGrayscale);
	DDX_Control(pDX, IDC_CHECK_SUBPIXEL, m_ctrlEnableSubpixel);
	DDX_Radio(pDX, IDC_RADIO_LIKELIHOOD, m_iSearchingOption);
	DDX_Control(pDX, IDC_RADIO_LIKELIHOOD, m_ctrlSearchingOption);
}


BEGIN_MESSAGE_MAP(CProcessingOptions, CDialog)
	ON_BN_CLICKED(IDOK, OnApplySetting)
	ON_BN_CLICKED(IDC_RADIO_ENTIRE, OnClickedRadioEntireImage)
	ON_BN_CLICKED(IDC_RADIO_LIKELIHOOD, OnClickedRadioLikelihood)
	ON_BN_CLICKED(IDC_CHECK_SUBPIXEL, OnClickEnableSubpixel)
	ON_BN_CLICKED(IDC_CHECK_GRAYSCALE, OnClickEnableGrayScale)
END_MESSAGE_MAP()


// CProcessingOptions message handlers

void CProcessingOptions::OnApplySetting()
{
	m_bEnableGrayscale = m_ctrlEnableGrayscale.GetCheck();
	m_bEnableSubpixel= m_ctrlEnableSubpixel.GetCheck();

	// m_iSearchingOption = m_ctrlSearchingOption.GetCheck(); // Tempolary variable
    // m_bLikelihoodSearch = (m_iSearchingOption == FALSE);

	CButton * pRadio = (CButton *) GetDlgItem(IDC_RADIO_LIKELIHOOD);
	m_bLikelihoodSearch = pRadio->GetCheck();
	m_iSearchCoverage = GetDlgItemInt(IDC_PERCENT_COVERAGE);

	UpdateData(TRUE);

	OnOK();
}

// To initialize the dialog by loading the saved data or default data.
BOOL CProcessingOptions::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctrlEnableGrayscale.SetCheck(m_bEnableGrayscale);
	m_ctrlEnableSubpixel.SetCheck(m_bEnableSubpixel);
	// m_ctrlSearchingOption.SetCheck(m_iSearchingOption == FALSE);
	m_ctrlSearchingOption.SetCheck(m_bLikelihoodSearch);
	SetDlgItemInt(IDC_PERCENT_COVERAGE, m_iSearchCoverage);

	UpdateData(FALSE);

	return TRUE;
}
void CProcessingOptions::OnClickedRadioEntireImage()
{
	CEdit * ctrlEditCoverage = reinterpret_cast<CEdit *> (GetDlgItem(IDC_PERCENT_COVERAGE));
	ctrlEditCoverage->EnableWindow(FALSE);
}

void CProcessingOptions::OnClickedRadioLikelihood()
{
	CEdit * ctrlEditCoverage = reinterpret_cast<CEdit *> (GetDlgItem(IDC_PERCENT_COVERAGE));
	ctrlEditCoverage->EnableWindow(TRUE);
}

void CProcessingOptions::OnClickEnableSubpixel()
{
	MessageBox("This option is not available in this version.", 
			"Information", 
			MB_ICONINFORMATION|MB_OK);

	m_ctrlEnableSubpixel.SetCheck(0);
	m_bEnableSubpixel = false;

	UpdateData(TRUE);
}

void CProcessingOptions::OnClickEnableGrayScale()
{
	MessageBox("Only grayscale matching is available in this version.", 
			"Information", 
			MB_ICONINFORMATION|MB_OK);

	m_ctrlEnableGrayscale.SetCheck(1);
	m_bEnableGrayscale = 1;

	UpdateData(TRUE);
}
