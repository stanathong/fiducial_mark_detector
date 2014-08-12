/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/

#pragma once


// CRMSEDlg dialog

class CRMSEDlg : public CDialog
{
	DECLARE_DYNAMIC(CRMSEDlg)

public:
	CRMSEDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRMSEDlg();

	// To create a modeless dialog.
	BOOL Create(UINT nID, CWnd * pWnd)
		{return CDialog::Create(nID, pWnd);}

	// To destroy the modeless dialog.
	virtual void PostNCDestroy() {delete this;}

	afx_msg BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_RMSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCloseDlg();
};
