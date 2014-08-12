// FMDoc.cpp : implementation of the CFMDoc class
//

#include "stdafx.h"
#include "FiducialMarkProject.h"

#include "FMDoc.h"
#include ".\fmdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFMDoc

IMPLEMENT_DYNCREATE(CFMDoc, CDocument)

BEGIN_MESSAGE_MAP(CFMDoc, CDocument)
	ON_COMMAND(ID_MATCHING_CROSSCORRELATION, OnMatchingCrossCorrelation)
END_MESSAGE_MAP()


// CFMDoc construction/destruction

CFMDoc::CFMDoc()
{
	m_pBaseImage = NULL;

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
void CFMDoc::MatchingInitialization(CFMImage * pBaseImage, CFMImage * pTemplateImage)
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

		hResult = m_pTemplateGrayImage->Load(m_strImageFile); // Load the image specified by lpszPathName
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

	// For simplicity without concerning gray/color, refer to the base image and template by the following vector.

	if (true == theApp.m_Setting.m_bEnableGrayscale)
	{
		pBaseImage		= m_pBaseGrayImage;
		pTemplateImage	= m_pTemplateGrayImage; 
	}
	else
	{
		if (NULL == m_pBaseImage)
		{
			CString strError;
			strError.Format("Error: Unable to open the specified based image");
			::AfxMessageBox(strError);		
		}
		else
		{
            pBaseImage = m_pBaseImage;
		}

		if (NULL == m_pTemplateImage)
		{
			CString strError;
			strError.Format("Error: Unable to open the specified template image");
			::AfxMessageBox(strError);		
		}
		else
		{
            pTemplateImage = m_pTemplateImage;
		}
	}
}


// This function is to perform the image matching based on the Cross Correlation Coefficient.

void CFMDoc::OnMatchingCrossCorrelation()
{
	CFMImage * pBaseImage;
	CFMImage * pTemplateImage;

	// Initialize necessary parameters prior to perform matching.
	// Obtain the pointer to the correct base and template image.
	MatchingInitialization(pBaseImage, pTemplateImage);

	// Perform the matching.

}
