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

class CFMImage :
	public CImage
{
public:
	CFMImage(void);
	~CFMImage(void);

	// Destroy and detach the current bitmap then attach the passed-in bitmap with
	void ExchangeImage(CFMImage & Img);

	// Get the height of the image
	int GetImageHeight();

	// Get the width of the image
	int GetImageWidth();

	// Perform Image Matching
	// int PerformMatching(CImage * pTemplate, bool bGrayscale, int iMatchingTechnique);
	MAP_MATCHED PerformMatching(CImage * pTemplate, bool bGrayscale, int iMatchingTechnique, bool bEnableSubpixel, bool bLikelihoodSearch, int iSearchCoverage);

	// Perform Image Matching Cross Correlation
	int CrossCorrelationMatchingEntire(CImage * pTemplate, bool bGrayscale=true);
	MATCHING_RESULT CrossCorrelationMatchingPartition(CImage * pTemplate, bool bGrayscale, int iStartX, int iStartY, int iWidth, int iHeight);

	// Evaluate and add the matching points into the map collection of positions and ccc.
	void UpdateMaxMatchedPoints(float fMatchingValue, int x, int y);

	// To initialize the parameters of the the image and must be called immediately after instantiation.
	void Initialization();

	// Initialize a buffer for matching results.
	bool InitializeMatchingBuffer();

	// To convert the image to grayscale.
	void ConvertToGrayscale();

	// To speed up the process of calcuating the Cross Correlation Coefficient
	void PreCalculateTemplate(CImage * pTemplate, bool bGrayscale);

	// Create subimage from the original image.
	CString CreateSubImage(int iStartX, int iStartY, int iWidth, int iHeight);

	// The passed-in coordinate is the center of the image.
	CString CreateSubImage(int iCenterX, int iCenterY, int iWidth, int iHeight, bool bXYCenter);

public:
	CView * m_pParent;					// The parent view.
	char m_cstrAppPath[_MAX_PATH];		// The current executable path.

	float ** m_ppMatchingBuffer;		// The pointer to the 2 dimensional array storing the matching results.
	float *	m_pMatchingBuffer;			// The pointer to the raw data of the matching results.

	MAP_MATCHED m_MatchingResults;		// The matching results.

private:
	int		m_nHeight;							// The height of the image.
	int		m_nWidth;							// The width of the image.

	// Adhoc to speed up the calculation
	float	m_d_sum_t;
	float	m_d_std_t;
	int		m_nSubImageIndex;
};