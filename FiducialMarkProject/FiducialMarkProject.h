/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// FiducialMarkProject.h : main header file for the FiducialMarkProject application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "FMDoc.h"
#include "TemplateDlg.h"
#include "ProcessingOptions.h"


// CFMApp:
// See FiducialMarkProject.cpp for the implementation of this class
//

class CFMApp : public CWinApp
{
public:
	CFMApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBaseImageOpen();

public:
	CString				m_strFileName;			// Based image file name.
	CProcessSetting		m_Setting;				// Properties of the application.
	CProcessingOptions	* m_pDlgProcessingOptions;
	CTemplateDlg		* m_pDlgTemplate;	

	CFMDoc * GetCurrentDocument();	// This function is to return the (only single) current CDocument object.
	afx_msg void OnToolsTemplate();
	afx_msg void OnToolsOptions();
	// afx_msg void OnMatchingCrossCorrelation();
	CString GetApplicationPath();
};

extern CFMApp theApp;