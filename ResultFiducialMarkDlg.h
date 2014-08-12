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


// CResultFiducialMarkDlg dialog
// Implemented as a modeless dialog

class CResultFiducialMarkDlg : public CDialog
{
	DECLARE_DYNAMIC(CResultFiducialMarkDlg)

public:
	CResultFiducialMarkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CResultFiducialMarkDlg();

	// To create a modeless dialog.
	BOOL Create(UINT nID, CWnd * pWnd)
		{return CDialog::Create(nID, pWnd);}

	// To destroy the modeless dialog.
	virtual void PostNCDestroy() {delete this;}

	afx_msg BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DLG_RESULT_MATCHING_POSITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCloseDlg();

public:
	CFMImage	m_objStaticImg;
	CStatic		m_ctrlStaticImg;
	CFMImage	m_MarkImg[MAX_MATCHING];
};
