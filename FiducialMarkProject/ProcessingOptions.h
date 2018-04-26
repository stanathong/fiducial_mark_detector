/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


#pragma once
#include "afxwin.h"


// CProcessingOptions dialog

class CProcessingOptions : public CDialog
{
	DECLARE_DYNAMIC(CProcessingOptions)

public:
	CProcessingOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProcessingOptions();

// Dialog Data
	enum { IDD = IDD_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnApplySetting();
	afx_msg BOOL OnInitDialog();
	afx_msg void OnClickedRadioEntireImage();
	afx_msg void OnClickedRadioLikelihood();

	// Checkbox control objects
	CButton m_ctrlEnableGrayscale;
	CButton m_ctrlEnableSubpixel;
	CButton m_ctrlSearchingOption;
	BOOL m_iSearchingOption;		// Tempolary variable
    
	bool m_bEnableGrayscale;
	bool m_bEnableSubpixel;
	bool m_bLikelihoodSearch;
	int	 m_iSearchCoverage;

	afx_msg void OnClickEnableSubpixel();
	afx_msg void OnClickEnableGrayScale();
};
