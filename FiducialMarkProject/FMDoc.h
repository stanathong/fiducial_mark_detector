/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// FMDoc.h : interface of the CFMDoc class
//


#pragma once

#include "stdafx.h"
#include "ResultFiducialMarkDlg.h"
#include "CalibrationDlg.h"
#include "AffineDlg.h"
#include "ProcessSummaryDlg.h"
#include "RMSEDlg.h"

class CFMDoc : public CDocument
{
protected: // create from serialization only
	CFMDoc();
	DECLARE_DYNCREATE(CFMDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	// To open the image file 
	virtual	BOOL OnOpenDocument(LPCTSTR lpszPathName);

	// To open the template image file 
	bool OnOpenTemplateImage(LPCTSTR lpszPathName);

	// Initialize necessary parameters prior to perform matching.
	void MatchingInitialization();

	MAP_MATCHED GetMatchingResults() {return m_mapMatchingPoints;}

// Implementation
public:
	virtual ~CFMDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	// To return the active instance of the CFMImage class.
	CFMImage * GetImage();

	// To calculate the affine parameters
	bool CalculateAffineParameters();

	// To calculate the RMSE
	bool CalculateRMSE();

	// To obtain the number of cross correlation operation.
	bool GetNumCCCOperations();

	// To handle menu event.
	afx_msg void OnMatchingCrossCorrelation();
	afx_msg void OnResultShowFiducialMarks();
	afx_msg void OnToolsCalibration();
	afx_msg void OnShowAffineParameters();
	afx_msg void OnShowProcessSummary();

public:
	
	CString		m_strImageFile;			// The image file name.
	CFMImage *	m_pBaseImage;			// The pointer to the CFMImage object (original).
	CFMImage *	m_pBaseGrayImage;		// The pointer to the CFMImage object (grayscale image).
	CFMImage *	m_pTemplateImage;		// The pointer to the template image (original)
	CFMImage *	m_pTemplateGrayImage;	// The pointer to the template image (grayscale image)

	MAP_MATCHED m_mapMatchingPoints;	// The matching results.

	CResultFiducialMarkDlg	* m_pResultFiducialMarkDlg;
	CCalibrationDlg			* m_pCalibrationDlg;
	CAffineDlg				* m_pAffineDlg;
	CProcessSummaryDlg		* m_pSummaryDlg;
	CRMSEDlg				* m_pRMSEDlg;

	// Affine transformation parameters
	float		m_a11;
	float		m_a12;
	float		m_a21;
	float		m_a22;
	float		m_xt;
	float		m_yt;

	bool		m_bCalculatedAffine;	// Whether or not the affine parameter is calculated.
	bool		m_bRMSECalculated;		// Whether or not the RMSE is calculated.
	bool		m_bNumCCC;
	DWORD		m_msProcessingTime;
	float		m_xRMSE;
	float		m_yRMSE;
	float		m_nNumOperations;		// The total number of CCC operations.
	SUB_XY_POS	m_dCalFC[MAX_MATCHING];	// The calculated fiducial marks.

	afx_msg void OnShowRMSEDialog();
};


