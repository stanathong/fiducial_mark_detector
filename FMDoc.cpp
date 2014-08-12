/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/

// FMDoc.cpp : implementation of the CFMDoc class
//

#include "stdafx.h"
#include "FiducialMarkProject.h"

#include "FMDoc.h"
#include "ResultFiducialMarkDlg.h"
#include "CalibrationDlg.h"
#include "Matrix.h"

#include "winbase.h" // To obtain the processing time.
#include ".\fmdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFMDoc

IMPLEMENT_DYNCREATE(CFMDoc, CDocument)

BEGIN_MESSAGE_MAP(CFMDoc, CDocument)
	ON_COMMAND(ID_MATCHING_CROSSCORRELATION, OnMatchingCrossCorrelation)
	ON_COMMAND(ID_RESULT_FIDUCIALMARKMATCHINGPOSITIONS, OnResultShowFiducialMarks)
	ON_COMMAND(ID_TOOLS_CAMERACALIBRATION, OnToolsCalibration)
	ON_COMMAND(ID_RESULT_AFFINETRANSFORMATION, OnShowAffineParameters)
	ON_COMMAND(ID_RESULT_SUMMARY, OnShowProcessSummary)
	ON_COMMAND(ID_RESULT_RMSE, OnShowRMSEDialog)
END_MESSAGE_MAP()


// CFMDoc construction/destruction

CFMDoc::CFMDoc()
{
	m_pBaseImage = NULL;			// The pointer to the CFMImage object (original).
	m_pBaseGrayImage = NULL;		// The pointer to the CFMImage object (grayscale image).
	m_pTemplateImage = NULL;		// The pointer to the template image (original)
	m_pTemplateGrayImage = NULL;	// The pointer to the template image (grayscale image)

	m_pResultFiducialMarkDlg = NULL;	// The pointer to the dialog: fiducial mark
	m_pCalibrationDlg = NULL;			// The pointer to the dialog: calibration
	m_pSummaryDlg = NULL;				// The pointer to the dialog: process summary
	m_pRMSEDlg = NULL;					// The pointer to the dialog: RMSE
	
	m_bCalculatedAffine = false;
	m_bRMSECalculated = false;
	m_bNumCCC = false;
}

CFMDoc::~CFMDoc()
{
	if (!m_pBaseImage)
		delete m_pBaseImage;

	if (!m_pBaseGrayImage)
		delete m_pBaseGrayImage;

	if (!m_pTemplateImage)
		delete m_pTemplateImage;

	if (!m_pTemplateGrayImage)
		delete m_pTemplateGrayImage;

	if (!m_pResultFiducialMarkDlg)
		delete m_pResultFiducialMarkDlg;

	if (!m_pCalibrationDlg)
		delete m_pCalibrationDlg;

	if (!m_pAffineDlg)
		delete m_pAffineDlg;

	if (!m_pSummaryDlg)
		delete m_pSummaryDlg;

	if (!m_pRMSEDlg)
		delete m_pRMSEDlg;
}

BOOL CFMDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


/* void BOOL CFMDoc::OnOpenDocument(LPCTSTR lpszPathName)
 * 
 * Description: To open the image file. This function works after the 
 *				CFMApp::OnFileOpen function that will forward
 *				the lpszPathName to this function.
 * @param lpszPathName	The path to the image file
 */
BOOL CFMDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_strImageFile = CString(lpszPathName);

	m_pBaseImage = new CFMImage;			// Create an instance of CFMImage

	HRESULT hResult = m_pBaseImage->Load(lpszPathName); // Load the image specified by lpszPathName
	if (FAILED(hResult))
	{
		CString strError;
		strError.Format("Error: Unable to open the specified image:\n%x - %s",
						hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(strError);
		return FALSE;
	}

	return TRUE;
}

/* bool CFMDoc::OnOpenTemplateImage(LPCTSTR lpszPathName)
 * 
 * Description: To open the template image file.
 *
 * @param lpszPathName	The path to the template file.
 */
bool CFMDoc::OnOpenTemplateImage(LPCTSTR lpszPathName)
{
	// Create an instance of the CFMImage for the template image.
	m_pTemplateImage = new CFMImage;

	HRESULT hResult = m_pTemplateImage->Load(lpszPathName); // Load the image specified by lpszPathName
	if (FAILED(hResult))
	{
		CString strError;
		strError.Format("Error: Unable to open the specified image:\n%x - %s",
						hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(strError);
		return false;
	}

	return true;
}


/*
 * To return the pointer to the image object.
 */ 
CFMImage * CFMDoc::GetImage(void)
{
	// If the image object does not exist, abort the program.
	ASSERT(m_pBaseImage != NULL);

	return m_pBaseImage;
}


// CFMDoc serialization

void CFMDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CFMDoc diagnostics

#ifdef _DEBUG
void CFMDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFMDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// Initialize necessary parameters prior to perform matching.
void CFMDoc::MatchingInitialization()
{
	HRESULT hResult;

	// If the template image is not yet defined, bring up the dialog for the user to browse.
	while (theApp.m_Setting.m_strTemplateFile == "")
	{
		theApp.OnToolsTemplate();
	}

	// If the matching is performed based on grayscale, we need to construct the object for grayscale.
	if (true == theApp.m_Setting.m_bEnableGrayscale)
	{
		// Convert the base image to grayscale.

		if (NULL == m_pBaseGrayImage)	
			m_pBaseGrayImage = new CFMImage;			// Create an instance of CFMImage

		hResult = m_pBaseGrayImage->Load(m_strImageFile); // Load the image specified by lpszPathName
		if (FAILED(hResult))
		{
			CString strError;
			strError.Format("Error: Unable to open the specified image:\n%x - %s",
							hResult, _com_error(hResult).ErrorMessage());
			::AfxMessageBox(strError);

			// exit?
		}

		m_pBaseGrayImage->ConvertToGrayscale();

		// Convert the template image to grayscale.

		if (NULL == m_pTemplateGrayImage)	
			m_pTemplateGrayImage = new CFMImage;			// Create an instance of CFMImage

		hResult = m_pTemplateGrayImage->Load(theApp.m_Setting.m_strTemplateFile); // Load the image specified by lpszPathName
		if (FAILED(hResult))
		{
			CString strError;
			strError.Format("Error: Unable to open the specified image:\n%x - %s",
							hResult, _com_error(hResult).ErrorMessage());
			::AfxMessageBox(strError);

			// exit?
		}

		m_pTemplateGrayImage->ConvertToGrayscale();
	}
}


// This function is to perform the image matching based on the Cross Correlation Coefficient.

void CFMDoc::OnMatchingCrossCorrelation()
{
	// Change to wait cursor at this point
	AfxGetApp()->DoWaitCursor(1);

	// Initialize necessary parameters prior to perform matching.
	MatchingInitialization();

	if (true == theApp.m_Setting.m_bEnableGrayscale)
	{
		if (!m_pBaseGrayImage || !m_pTemplateGrayImage)
		{
			CString strError;
			strError.Format("Internal error: the operation failed to process.");
			::AfxMessageBox(strError);		
			return;
		}

		// Perform the matching.

		DWORD dStart, dEnd;
		dStart = GetTickCount();
		m_mapMatchingPoints = m_pBaseGrayImage->PerformMatching(m_pTemplateGrayImage, 
																true, 
																CROSS_CORRELATION, 
																theApp.m_Setting.m_bEnableSubpixel, 
																theApp.m_Setting.m_bLikelihoodSearch, 
																theApp.m_Setting.m_iSearchCoverage);
		dEnd = GetTickCount();
		m_msProcessingTime = dEnd - dStart;

	}
	else
	{
		if (!m_pBaseImage || !m_pTemplateImage)
		{
			CString strError;
			strError.Format("Internal error: the operation failed to process.");
			::AfxMessageBox(strError);		
			return;
		}
		
		// Perform the matching.
		DWORD dStart, dEnd;
		dStart = GetTickCount();

		m_mapMatchingPoints = m_pBaseGrayImage->PerformMatching(	m_pTemplateGrayImage, 
																false, 
																CROSS_CORRELATION, 
																theApp.m_Setting.m_bEnableSubpixel, 
																theApp.m_Setting.m_bLikelihoodSearch, 
																theApp.m_Setting.m_iSearchCoverage);
		dEnd = GetTickCount();
		m_msProcessingTime = dEnd - dStart;
	}

	// Return the original cursor
	AfxGetApp()->DoWaitCursor(-1);
}

// To display the result fiducial mark dialog.
void CFMDoc::OnResultShowFiducialMarks()
{
	m_pResultFiducialMarkDlg = new CResultFiducialMarkDlg;
	m_pResultFiducialMarkDlg->Create(IDD_DLG_RESULT_MATCHING_POSITION, AfxGetMainWnd());
	m_pResultFiducialMarkDlg->ShowWindow(SW_SHOW);
}


void CFMDoc::OnToolsCalibration()
{
	m_pCalibrationDlg = new CCalibrationDlg;
	m_pCalibrationDlg->DoModal();
}

// The affine parameters can be calculated by:
//
//		|xf|  =  |a11	a12|  *  |xm|   +   |xt|
//		|yf|     |a21	a22|     |ym|		|yt|	
//
//	Construct in the form of Gauss Markov Model as
//
//		|xf| = |xm ym  0  0  1  0| * |a11|
//		|yf|   |0  0   xm ym 0  1|   |a12|
//									 |a21|
//									 |a22|
//									 |xt |
//									 |yt |
//		y = AX
//		
//		X = inv(AT A) (AT y)
//
//
bool CFMDoc::CalculateAffineParameters()
{
	if (m_bCalculatedAffine == true)
		return true;

	int pID;
	ITER_MATCHED iter;
	MATCHING_RESULT point;
	bool bCreateDlgLocal = false;

	// Construct matrix for Least Square calculation.
	Matrix A(MAX_MATCHING*2,6);	// Construct the A matrix with size (16*6)
	Matrix y(MAX_MATCHING*2,1);	// Construct the y matrix with size (16*1)
	Matrix X;					// Construct the X matrix as a result, with size (6*1)

	// Matrix A is constructed from the matching results.

	// If less than 8 matching points found, exit.
	if (m_mapMatchingPoints.size() < MAX_MATCHING)
	{
		MessageBox(NULL, 
			"Failed to calculate the affine parameters.\nThe application failed to locate eight matching points.", 
			"Notification", 
			MB_ICONINFORMATION|MB_OK);
		
		return false;
	}

	// Matrix y is constructed from the fiducial data.

	if (NULL == m_pCalibrationDlg)
	{
		m_pCalibrationDlg = new CCalibrationDlg;

		// Set flag if it is created for this purpose.
		bCreateDlgLocal = true;
	}

	// If failed to optain the calibration data, exit.
	if (false == m_pCalibrationDlg->ReadCalibrationData())
	{
		MessageBox(NULL, 
			"Failed to calculate the affine parameters.\nThe application failed to obtain the calibration data.", 
			"Notification", 
			MB_ICONINFORMATION|MB_OK);
		
		return false;
	}

	for (iter = m_mapMatchingPoints.begin(); iter != m_mapMatchingPoints.end(); iter++)
	{
		// Get the value of the matched point.
		pID = (*iter).first;
		point = (*iter).second;
		
		// Assign the value to the matrix.
		//	[	xm	ym	0	0	1	0;	<-- row (pID-1)*2
		//		0	0	xm	ym	0	1]	<-- row (pID-1)*2 + 1

		// Set the value for the first row of each point
		A.Set( (pID-1)*2, 0, point.fX);	
		A.Set( (pID-1)*2, 1, point.fY);
		A.Set( (pID-1)*2, 2, 0);
		A.Set( (pID-1)*2, 3, 0);
		A.Set( (pID-1)*2, 4, 1);
		A.Set( (pID-1)*2, 5, 0);

		// Set the value for the second row of each point
		A.Set( (pID-1)*2 +1, 0, 0);	
		A.Set( (pID-1)*2 +1, 1, 0);
		A.Set( (pID-1)*2 +1, 2, point.fX);
		A.Set( (pID-1)*2 +1, 3, point.fY);
		A.Set( (pID-1)*2 +1, 4, 0);
		A.Set( (pID-1)*2 +1, 5, 1);

		//	Assign the value to the matrix y.
		//	[	xf;				<-- row (pID-1)*2
		//		yf	]			<-- row (pID-1)*2 + 1
		y.Set( (pID-1)*2,		0,	m_pCalibrationDlg->m_FC[(pID-1)].fX);
		y.Set( (pID-1)*2 +1,	0,	m_pCalibrationDlg->m_FC[(pID-1)].fY);
	}

	// Calculate the results as "X = inv(AT A) (AT y)"
	Matrix N = A.Trans() * A;

	// Check if N inverse exist.
	double det = N.Det();
	if(det == 0.0 || det <= 1e-10) 
	{
		MessageBox(NULL, 
			"Failed to calculate the affine parameters.\nThe coordinates are not in a good condition.", 
			"Notification", 
			MB_ICONINFORMATION|MB_OK);
		
		return false;
	}

	// Find the X matrix.
	X = N.Inv() * A.Trans() * y;

	// Then obtain the affine transformation parameters.
	m_a11	=	X.Get(0, 0);	
	m_a12	=	X.Get(1, 0);
	m_a21	=	X.Get(2, 0);
	m_a22	=	X.Get(3, 0);
	m_xt	=	X.Get(4, 0);
	m_yt	=	X.Get(5, 0);

	// Delete the object since it is no longer used.
	if (true == bCreateDlgLocal)
	{
		delete m_pCalibrationDlg;
		m_pCalibrationDlg = NULL;
	}

	m_bCalculatedAffine = true;

	return true;
}

void CFMDoc::OnShowAffineParameters()
{
	// Calculate affine transformation parameters.
	if (true == CalculateAffineParameters())
	{
		// Display the affine transformation parameters.
		m_pAffineDlg = new CAffineDlg;
		m_pAffineDlg->Create(IDD_AFFINE_PARAMETERS, AfxGetMainWnd());
		m_pAffineDlg->ShowWindow(SW_SHOW);
	}
}


// To calculate the RMSE
bool CFMDoc::CalculateRMSE()
{
	if (m_bRMSECalculated == true)
		return true;

	// If the number of matching is less than 8, return false.
	if (m_mapMatchingPoints.size() < MAX_MATCHING)
		return false;

	// If the affine parameters has not yet been defined, then calculate the affine parameters.
	if (m_bCalculatedAffine == false)
		if ( (CalculateAffineParameters() == false) || (m_pCalibrationDlg == NULL) )
			return false;

	int pID;
	ITER_MATCHED iter;
	MATCHING_RESULT point;
	float fxRMSE = 0.0;
	float fyRMSE = 0.0;
	bool bCreateDlgLocal = false;

	for (iter = m_mapMatchingPoints.begin(); iter != m_mapMatchingPoints.end(); iter++)
	{
		float fxDummy, fyDummy;
		SUB_XY_POS fc;

		// Get the value of the matched point.
		pID = (*iter).first;
		point = (*iter).second;

		// Calculate the fiducial mark (in mm) from the calculated affine parameters.
		fxDummy = point.fX * m_a11 + point.fY * m_a12 + m_xt;
		fyDummy = point.fX * m_a21 + point.fY * m_a22 + m_yt;

		m_dCalFC[pID-1].fX = fxDummy; m_dCalFC[pID-1].fY = fyDummy; 

		// Calculate the root mean square error
		if (NULL == m_pCalibrationDlg)
		{
			m_pCalibrationDlg = new CCalibrationDlg;

			// Set flag if it is created for this purpose.
			bCreateDlgLocal = true;
		}

		// If failed to optain the calibration data, exit.
		if (false == m_pCalibrationDlg->ReadCalibrationData())
		{
			MessageBox(NULL, 
				"Failed to calculate the affine parameters.\nThe application failed to obtain the calibration data.", 
				"Notification", 
				MB_ICONINFORMATION|MB_OK);
			
			return false;
		}

		fc = m_pCalibrationDlg->m_FC[pID-1];
		fxRMSE += (fc.fX - fxDummy) * (fc.fX - fxDummy);
		fyRMSE += (fc.fY - fyDummy) * (fc.fY - fyDummy);
	}

	m_xRMSE = sqrt(fxRMSE/MAX_MATCHING);
	m_yRMSE = sqrt(fyRMSE/MAX_MATCHING);		

	// Delete the object since it is no longer used.
	if (true == bCreateDlgLocal)
	{
		delete m_pCalibrationDlg;
		m_pCalibrationDlg = NULL;
	}

	m_bRMSECalculated = true;

	return true;
}

// To obtain the number of cross correlation operation.
bool CFMDoc::GetNumCCCOperations()
{
	if (m_bNumCCC == true)
		return true;

	CFMImage * pTemplate, * pBaseImage;

	if (theApp.m_Setting.m_bEnableGrayscale == true)
	{
		if ((m_pBaseGrayImage == NULL) || (m_pTemplateGrayImage == NULL))
			return false;

		pTemplate = m_pTemplateGrayImage;
		pBaseImage = m_pBaseGrayImage;
	}
	else
	{
		if ((m_pBaseImage == NULL) || (m_pTemplateImage == NULL))
			return false;

		pTemplate = m_pTemplateImage;
		pBaseImage = m_pBaseImage;	
	}

	int nPixTemplate = pTemplate->GetWidth() * pTemplate->GetHeight();
	int nPixBase = pBaseImage->GetWidth() * pBaseImage->GetHeight();
	double nSearchSpace = (double)MAX_MATCHING * theApp.m_Setting.m_iSearchCoverage * nPixBase / 100;

	m_nNumOperations = (double) nPixTemplate * nSearchSpace;

	m_bNumCCC = true;

	return true;
}
void CFMDoc::OnShowProcessSummary()
{
	// Show the process summary dialog
	m_pSummaryDlg = new CProcessSummaryDlg;
	m_pSummaryDlg->Create(IDD_PROCESS_SUMMARY, AfxGetMainWnd());
	m_pSummaryDlg->ShowWindow(SW_SHOW);
}

void CFMDoc::OnShowRMSEDialog()
{
	// Show the process summary dialog
	m_pRMSEDlg = new CRMSEDlg;
	m_pRMSEDlg->Create(IDD_RMSE, AfxGetMainWnd());
	m_pRMSEDlg->ShowWindow(SW_SHOW);
}
