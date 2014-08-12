/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


#pragma once

#include "stdafx.h"
#include "afxwin.h"


// CTemplateDlg dialog

class CTemplateDlg : public CDialog
{
	DECLARE_DYNAMIC(CTemplateDlg)

public:
	CTemplateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTemplateDlg();

// Dialog Data
	enum { IDD = IDD_TEMPLATE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBrowseTemplate();
	afx_msg BOOL OnInitDialog();
	afx_msg void OnCancelTemplate();
	afx_msg void OnLoadTemplate();

public:
	CString		m_strTemplateFile;	// To store the template file name.
	CFMImage *	m_pPreviewImg;		// To display the preview image.
	CFMImage	m_objPreviewImg;
	CStatic		m_ctrlPreview;
};
