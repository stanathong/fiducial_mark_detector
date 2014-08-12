/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


#pragma once


// CAffineDlg dialog
// Implement as a modeless dialog
class CAffineDlg : public CDialog
{
	DECLARE_DYNAMIC(CAffineDlg)

public:
	CAffineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAffineDlg();

	// To create a modeless dialog.
	BOOL Create(UINT nID, CWnd * pWnd)
		{return CDialog::Create(nID, pWnd);}

	// To destroy the modeless dialog.
	virtual void PostNCDestroy() {delete this;}

	afx_msg BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_AFFINE_PARAMETERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCloseDialog();
};
