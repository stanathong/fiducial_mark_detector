/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


#include "StdAfx.h"
#include "FiducialMarkProject.h"
#include ".\fmimage.h"

CFMImage::CFMImage(void)
{
	m_nSubImageIndex = 0;
}

CFMImage::~CFMImage(void)
{
	if (m_ppMatchingBuffer)
		free(m_ppMatchingBuffer);

	if (m_pMatchingBuffer)
		free(m_pMatchingBuffer);
}

/* void CFMImage::ExchangeImage(CFMImage & Img)
 * 
 * Description: To detache and destroy the current bitmap from this object.
 *				Then, attache the passed-in image to this object. After that,
 *				detache the bitmap from the passed-in object (since it now attach
 *				to a new object)
 * @param Img	The new image to be attached
 */
void CFMImage::ExchangeImage(CFMImage & Img)
{
	HBITMAP hBitmap = (HBITMAP) Img;
	
	// DETACHES THE BITMAP FROM THE CImage OBJECT AND DESTROY THE BITMAP
	Destroy();
	
	// ATTACHES AN HBITMAP TO A CImage OBJECT. 
	Attach(hBitmap);

	// DETACHES THE BITMAP FROM THE PASSED-IN OBJECT
	Img.Detach();
}

void CFMImage::Initialization(void)
{
	m_nHeight	= GetHeight();	// Get the height of the image.
	m_nWidth	= GetWidth();	// Get the width of the image.
}


/* bool CFMImage::InitializeMatchingBuffer()
 * 
 * Description: Initialize a buffer for matching results.
 *
 */
bool CFMImage::InitializeMatchingBuffer()
{
	// Initialize the object immediately before other activities.
	Initialization();

	// Allocate array buffer storing the matching results.
	m_ppMatchingBuffer	= (float **) malloc(m_nHeight * sizeof(float *));	
	m_pMatchingBuffer	= (float *)  malloc(m_nHeight * m_nWidth * sizeof(float));

	if ((m_ppMatchingBuffer == NULL) || (m_pMatchingBuffer == NULL))
	{
		MessageBox(NULL,	"Application terminated due to insufficient memory", 
							"Terminate", MB_ICONERROR|MB_OK);
		exit(1);
	}

	memset(m_pMatchingBuffer, 0, m_nHeight * m_nWidth * sizeof(float));

	for (int i = 0; i < m_nHeight ; i++) 
	{
		m_ppMatchingBuffer[i]=&m_pMatchingBuffer[i*m_nWidth];
	}

	return true;
}

/*
int CFMImage::PerformMatching(CImage * pTemplate, bool bGrayscale, int iMatchingTechnique)
{
	if (iMatchingTechnique == CROSS_CORRELATION)
		return CrossCorrelationMatchingEntire(pTemplate, bGrayscale);

	return 0;
}
*/

MAP_MATCHED CFMImage::PerformMatching(CImage * pTemplate, bool bGrayscale, int iMatchingTechnique, 
										bool bEnableSubpixel, bool bLikelihoodSearch, int iSearchCoverage)
{
	// Initialise a buffer to store the matching results.
	InitializeMatchingBuffer();

	if (iMatchingTechnique == CROSS_CORRELATION)
	{
		// Limit the search space with coverage iSearchCoverage %
		//
		//
		//			1--------2--------3
		//			|				  |	
		//			|				  |
		//			|				  |
		//			4				  5
		//			|				  |
		//			|				  |
		//			|				  |
		//			6--------7--------8
		//
		//

		if (bLikelihoodSearch == true)
		{
			int idPartition;
			int iStartX, iStartY;
			int iWidth, iHeight;
			int iCenterX, iCenterY;
			
			// The center position of the base image.
			iCenterX = m_nWidth/2;
			iCenterY = m_nHeight/2;

			// The size of each partition.
			iWidth = iSearchCoverage * m_nWidth / 100;
			iHeight = iSearchCoverage * m_nHeight / 100;

			// To speed up the process
			PreCalculateTemplate(pTemplate, bGrayscale);

			// Partition#1
			iStartX = 0; iStartY = 0;
			m_MatchingResults.insert(PAIR_MATCHED(1,CrossCorrelationMatchingPartition(pTemplate, bGrayscale, iStartX, iStartY, iWidth, iHeight)));

			// Partition#2
			iStartX = iCenterX-iWidth/2; iStartY = 0;
			m_MatchingResults.insert(PAIR_MATCHED(2,CrossCorrelationMatchingPartition(pTemplate, bGrayscale, iStartX, iStartY, iWidth, iHeight)));

			// Partition#3
			iStartX = m_nWidth - iWidth; iStartY = 0;
			m_MatchingResults.insert(PAIR_MATCHED(3,CrossCorrelationMatchingPartition(pTemplate, bGrayscale, iStartX, iStartY, iWidth, iHeight)));

			// Partition#4
			iStartX = 0; iStartY = iCenterY-iHeight/2;
			m_MatchingResults.insert(PAIR_MATCHED(4,CrossCorrelationMatchingPartition(pTemplate, bGrayscale, iStartX, iStartY, iWidth, iHeight)));

			// Partition#5
			iStartX = m_nWidth - iWidth; iStartY = iCenterY-iHeight/2;;
			m_MatchingResults.insert(PAIR_MATCHED(5,CrossCorrelationMatchingPartition(pTemplate, bGrayscale, iStartX, iStartY, iWidth, iHeight)));

			// Partition#6
			iStartX = 0; iStartY = m_nHeight-iHeight;
			m_MatchingResults.insert(PAIR_MATCHED(6,CrossCorrelationMatchingPartition(pTemplate, bGrayscale, iStartX, iStartY, iWidth, iHeight)));

			// Partition#7
			iStartX = iCenterX-iWidth/2; iStartY = m_nHeight-iHeight;
			m_MatchingResults.insert(PAIR_MATCHED(7,CrossCorrelationMatchingPartition(pTemplate, bGrayscale, iStartX, iStartY, iWidth, iHeight)));

			// Partition#8
			iStartX = m_nWidth - iWidth; iStartY = m_nHeight-iHeight;
			m_MatchingResults.insert(PAIR_MATCHED(8,CrossCorrelationMatchingPartition(pTemplate, bGrayscale, iStartX, iStartY, iWidth, iHeight)));
		}
		else // Entire area search
		{
//			return CrossCorrelationMatchingEntire(pTemplate, bGrayscale);
		}
	}

	return m_MatchingResults;
}


/* int CFMImage::CrossCorrelationMatchingEntire(CImage * pTemplate)
 * 
 * Description: Perform image matching based on Cross Correlation.
 *
 * @param pTemplate		The template image.
 *		  bGrayscale	true: if the process is based on grayscale
						false: if the process is based on color
 */
int CFMImage::CrossCorrelationMatchingEntire(CImage * pTemplate, bool bGrayscale)
{
	BYTE bRed, bGreen, bBlue, bAvg;	// The color component of the pixel.
	BYTE * pPixel;					// Pointer to the pixel address.
	BYTE * pPixelT;					// Pointer to the pixel address.
	int  x, y;						// The pixel coordinate.
	int	 xt, yt;					// The template coordinate.
	int TEMPLATE_HEIGHT, TEMPLATE_WIDTH;	// The height and width of the template image.
	int H_T_HEIGHT, H_T_WIDTH;		// Half of template height and half of template width.
	int TEMPLATE_H_X_W;				// Template height x Template width
	int n_yRound, n_xRound;			// The actual round of processing (not equal real size of image due to template size).
	float fdif;						// Tempolarly buffer.

	/* Image Matching Variables */
	float d_std_tm;					// Covariance between template and image patch.
	float d_std_t, d_std_m;			// Standard deviation for template and image patch.
	
	// Initialise a buffer to store the matching results.
	InitializeMatchingBuffer();

	// Obtain the size of template image.
	TEMPLATE_HEIGHT = pTemplate->GetHeight();
	TEMPLATE_WIDTH	= pTemplate->GetWidth();
	TEMPLATE_H_X_W	= TEMPLATE_HEIGHT * TEMPLATE_WIDTH;

	// Half of the template height and width.
	H_T_HEIGHT	= TEMPLATE_HEIGHT/2;
	H_T_WIDTH	= TEMPLATE_WIDTH/2;

	// Calculate the actual round of processing.
	n_yRound = m_nHeight - (TEMPLATE_HEIGHT - 1);
	n_xRound = m_nWidth - (TEMPLATE_WIDTH - 1);

	// If the process is based on grayscale.
	if (true == bGrayscale)
	{
		// To speed up the computational time, precalculate the value of template.
		float d_sum_sqare_t = 0.0, d_sum_t = 0.0;

		// Optain sum(T^2) and sum(T) for mean and variance calculation
		for (yt = 0; yt < TEMPLATE_HEIGHT; yt++)
		{
			for (xt = 0; xt < TEMPLATE_WIDTH; xt++)
			{
				// Get the pointer to the pixel at (x,y).
				pPixelT	= (BYTE *) pTemplate->GetPixelAddress(xt,yt);
				d_sum_sqare_t += pPixelT[0] * pPixelT[0];
				d_sum_t += pPixelT[0];
			}
		}

		d_std_t = sqrt((TEMPLATE_H_X_W*d_sum_sqare_t - d_sum_t*d_sum_t)/TEMPLATE_H_X_W/(TEMPLATE_H_X_W-1));

		// End of precalculating the template
		

		// Iterate to each pixel for the whole image.
		for (y = 0; y < n_yRound; y++)
		{
			for (x = 0; x < n_xRound; x++)
			{
				// Declare tempolary variable.
				float d_sum_sqare_m = 0.0, d_sum_txm = 0.0, d_sum_m = 0.0;

				// Looping for variance calculation
				for (yt = 0; yt < TEMPLATE_HEIGHT; yt++)
				{
					for (xt = 0; xt < TEMPLATE_WIDTH; xt++)
					{
						// Get the pointer to the pixel of image patch at (x,y).
						pPixel	= (BYTE *) GetPixelAddress(x+xt,y+yt);
						
						// Get the pointer to the pixel of template at (x,y).
						pPixelT	= (BYTE *) pTemplate->GetPixelAddress(xt,yt);
						
						d_sum_sqare_m += pPixel[0] * pPixel[0];
						d_sum_txm += pPixel[0] * pPixelT[0];
						d_sum_m += pPixel[0];
					}
				}

				d_std_m		= sqrt((TEMPLATE_H_X_W*d_sum_sqare_m - d_sum_m*d_sum_m)/TEMPLATE_H_X_W/(TEMPLATE_H_X_W-1));
				d_std_tm	= (TEMPLATE_H_X_W*d_sum_txm - d_sum_t*d_sum_m)/TEMPLATE_H_X_W/(TEMPLATE_H_X_W-1);

				m_ppMatchingBuffer[y+H_T_HEIGHT][x+H_T_WIDTH] = d_std_tm/(d_std_m*d_std_t);

				// Find the maximum value points.
				// Cannot apply this right now as we may need to consider local maximum.
				// UpdateMaxMatchedPoints(m_ppMatchingBuffer[y+H_T_HEIGHT][x+H_T_WIDTH], x+H_T_WIDTH, y+H_T_HEIGHT);

			} //end x-round
		} // end y-round for the whole image
	} // end of process based on gray-scale
	
	else // Color based processing
	
	{
		// Have to perform 3 colors
	}

	//---------- DEBUG ----------
	FILE * fMatching = fopen("MatchingPoints.txt", "w");
	for (ITER_MATCHED iter = m_MatchingResults.begin(); iter != m_MatchingResults.end(); iter++)
	{
		fprintf(fMatching, "(%d,%d) with CCC = %f\n", (int)((*iter).second).fX,(int)((*iter).second).fY, (*iter).first);
	}
	fclose(fMatching);


	FILE * fCCC = fopen("CCC.txt", "w");
	for (y = 0; y < m_nHeight; y++)
	{
		for (x = 0; x < m_nWidth; x++)
		{
			fprintf(fCCC, "%f\t", m_ppMatchingBuffer[y][x]);
		}
		fprintf(fCCC,"\n");
	}
	fclose(fCCC);
	//---------- DEBUG ----------

	return 0;
}

void CFMImage::PreCalculateTemplate(CImage * pTemplate, bool bGrayscale)
{
	BYTE bRed, bGreen, bBlue, bAvg;	// The color component of the pixel.
	BYTE * pPixelT;					// Pointer to the pixel address.
	int	 xt, yt;					// The template coordinate.
	int TEMPLATE_HEIGHT, TEMPLATE_WIDTH;	// The height and width of the template image.
	int TEMPLATE_H_X_W;				// Template height x Template width

	/* Image Matching Variables */
	float d_std_t;					// Standard deviation for template.
	float d_sum_sqare_t = 0.0, d_sum_t = 0.0;

	// Obtain the size of template image.
	TEMPLATE_HEIGHT = pTemplate->GetHeight();
	TEMPLATE_WIDTH	= pTemplate->GetWidth();
	TEMPLATE_H_X_W	= TEMPLATE_HEIGHT * TEMPLATE_WIDTH;

	if (true == bGrayscale) //If the process is based on grayscale.
	{
		// Optain sum(T^2) and sum(T) for mean and variance calculation
		for (yt = 0; yt < TEMPLATE_HEIGHT; yt++)
		{
			for (xt = 0; xt < TEMPLATE_WIDTH; xt++)
			{
				// Get the pointer to the pixel at (x,y).
				pPixelT	= (BYTE *) pTemplate->GetPixelAddress(xt,yt);
				d_sum_sqare_t += pPixelT[0] * pPixelT[0];
				d_sum_t += pPixelT[0];
			}
		}

		d_std_t = sqrt((TEMPLATE_H_X_W*d_sum_sqare_t - d_sum_t*d_sum_t)/TEMPLATE_H_X_W/(TEMPLATE_H_X_W-1));
	}

	m_d_sum_t = d_sum_t;
	m_d_std_t = d_std_t;

	// End of precalculating the template
}

/* MATCHING_RESULT CFMImage::CrossCorrelationMatchingPartition
 * 
 * Description: Perform image matching based on Cross Correlation by specify the position to search for.
 *
 * @param pTemplate		The template image.
 *		  bGrayscale	true: if the process is based on grayscale
 *						false: if the process is based on color
 *		  iStartX, iStartY	The starting position to search for
 *		  iWidth, iHeight	The dimension of the search space
 *
 * @Return PAIR_MATCHED	return the position of matching and the ccc value of the highest search in this partition
 */
MATCHING_RESULT CFMImage::CrossCorrelationMatchingPartition(CImage * pTemplate, bool bGrayscale, int iStartX, int iStartY, int iWidth, int iHeight)
{
	BYTE bRed, bGreen, bBlue, bAvg;	// The color component of the pixel.
	BYTE * pPixel;					// Pointer to the pixel address.
	BYTE * pPixelT;					// Pointer to the pixel address.
	int  x, y;						// The pixel coordinate.
	int	 xt, yt;					// The template coordinate.
	int TEMPLATE_HEIGHT, TEMPLATE_WIDTH;	// The height and width of the template image.
	int H_T_HEIGHT, H_T_WIDTH;		// Half of template height and half of template width.
	int TEMPLATE_H_X_W;				// Template height x Template width
	int n_yRound, n_xRound;			// The actual round of processing (not equal real size of image due to template size).
	float fdif;						// Tempolarly buffer.
	MATCHING_RESULT matchedPoint;	// This buffer stores the point which has maximum ccc.

	// Initialize matchedPoint result;
	matchedPoint.fCCC = -100.0;

	/* Image Matching Variables */
	float d_std_tm;					// Covariance between template and image patch.
	float d_std_t, d_std_m;			// Standard deviation for template and image patch.
	float d_ccc;					// Cross-correlation coefficient
	float d_sum_sqare_t = 0.0, d_sum_t = 0.0;

	// Obtain the size of template image.
	TEMPLATE_HEIGHT = pTemplate->GetHeight();
	TEMPLATE_WIDTH	= pTemplate->GetWidth();
	TEMPLATE_H_X_W	= TEMPLATE_HEIGHT * TEMPLATE_WIDTH;

	// Half of the template height and width.
	H_T_HEIGHT	= TEMPLATE_HEIGHT/2;
	H_T_WIDTH	= TEMPLATE_WIDTH/2;

	// To speed up the computational time, precalculate the value of template.
	
	d_sum_t = m_d_sum_t;
	d_std_t = m_d_std_t;	

	// End of precalculating the template

	// If the process is based on grayscale.
	if (true == bGrayscale)
	{
		// Determine the starting and ending search area.
		// Starting position is at (iStartY, iStartX)
		// Finishing position is:
		n_yRound = iStartY + iHeight - (TEMPLATE_HEIGHT - 1);
		n_xRound = iStartX + iWidth -  (TEMPLATE_WIDTH - 1);

		// Iterate to each pixel for the whole image.
		for (y = iStartY; y < n_yRound; y++)
		{
			for (x = iStartX; x < n_xRound; x++)
			{
				// Declare tempolary variable.
				float d_sum_sqare_m = 0.0, d_sum_txm = 0.0, d_sum_m = 0.0;

				// Looping for variance calculation
				for (yt = 0; yt < TEMPLATE_HEIGHT; yt++)
				{
					for (xt = 0; xt < TEMPLATE_WIDTH; xt++)
					{
						// Get the pointer to the pixel of image patch at (x,y).
						pPixel	= (BYTE *) GetPixelAddress(x+xt,y+yt);
						
						// Get the pointer to the pixel of template at (x,y).
						pPixelT	= (BYTE *) pTemplate->GetPixelAddress(xt,yt);
						
						d_sum_sqare_m += pPixel[0] * pPixel[0];
						d_sum_txm += pPixel[0] * pPixelT[0];
						d_sum_m += pPixel[0];
					}
				}

				d_std_m		= sqrt((TEMPLATE_H_X_W*d_sum_sqare_m - d_sum_m*d_sum_m)/TEMPLATE_H_X_W/(TEMPLATE_H_X_W-1));
				d_std_tm	= (TEMPLATE_H_X_W*d_sum_txm - d_sum_t*d_sum_m)/TEMPLATE_H_X_W/(TEMPLATE_H_X_W-1);

				d_ccc = d_std_tm/(d_std_m*d_std_t);

				m_ppMatchingBuffer[y+H_T_HEIGHT][x+H_T_WIDTH] = d_ccc;

				// Find the maximum value points.
				if DBL_GT(d_ccc, matchedPoint.fCCC)
				{
					matchedPoint.fX = x+H_T_WIDTH;
					matchedPoint.fY = y+H_T_HEIGHT;
					matchedPoint.fCCC = d_ccc;
				}
				// Cannot apply this right now as we may need to consider local maximum.
				// UpdateMaxMatchedPoints(m_ppMatchingBuffer[y+H_T_HEIGHT][x+H_T_WIDTH], x+H_T_WIDTH, y+H_T_HEIGHT);
			} //end x-round
		} // end y-round for the whole image
	} // end of process based on gray-scale
	
	else // Color based processing
	
	{
		// Have to perform 3 colors
	}

	// If SubPixel Accuracy is enable.
//	?????

	return matchedPoint;
}

// This function is to calculate subpixel accuracy.
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
/*
MATCHING_RESULT CFMImage::CalculateSubPixelAccuracy(MATCHING_RESULT matchedPoint)
{
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

	return true;
}
*/

/* void CFMImage::UpdateMaxMatchedPoints()
 * 
 * Description: Add the matching points into the map collection of positions and ccc
 *
 */
/*
void CFMImage::UpdateMaxMatchedPoints(float fMatchingValue, int x, int y)
{
	// Since the CCC value is between [-1,1] and 0 means not correlate, compare only absolute value.
	fMatchingValue = fabs(fMatchingValue);

	// Simply put into the map if #points is less than 8.
	if (m_MatchingResults.size() < MAX_MATCHING)
	{
		m_MatchingResults.insert(PAIR_MATCHING(fMatchingValue, SUB_XY_POS(x,y)));
		return;
	}

	// Get the last component from the map as it is the minimal matched result.
	ITER_MATCHING iter = m_MatchingResults.end();
    --iter;
	float fMinMatchingVal = (*iter).first;

	// If the CCC of passed-in point is less than the minimum, simply return.
	if DBL_LT(fMatchingValue, fMinMatchingVal)
		return;

	// If the CCC of passed-in point is greater than the smallest in m_MatchingResults, erase the last element.
	m_MatchingResults.erase(iter);	

	// Insert the new point.
	m_MatchingResults.insert(PAIR_MATCHING(fMatchingValue, SUB_XY_POS(x,y)));
}
*/

/* void CFMImage::ConvertToGrayscale()
 * 
 * Description: Convert the color of the image from RGB to gray color
 *				1. Check if the bitmap is not indexed (indexed if 8 bits or less)
 *					1.1 Check if bits-per-pixel is greater than 24
 *						Read the read, green and blue value of a pixel and calculate to gray value
 *				2. If the bitmap is indexed 
 *					Average total color and set back to the pixel value.
 */
void CFMImage::ConvertToGrayscale()
{
	// Initialize the object immediately before other activities.
	Initialization();

	// A bitmap is indexed if it is 8 bit or less.
	if (!IsIndexed())
	{
		// Check if bits-per-pixel is greater than 24
		if (GetBPP() >= 24)
		{
			BYTE bRed, bGreen, bBlue, bAvg;	// The color component of the pixel.
			BYTE * pPixel;					// Pointer to the pixel address.
			int  x, y;						// The pixel coordinate.

			// Iterate to each pixel to calculate the gray value.
			for (y = 0; y < m_nHeight; y++)
			{
				for (x = 0; x < m_nWidth; x++)
				{
					// Get the pointer to the pixel at (x,y).
					pPixel	= (BYTE *) GetPixelAddress(x,y);

					bRed	= pPixel[2];
					bGreen	= pPixel[1];
					bBlue	= pPixel[0];
					
					// Calculate the gray value.
					bAvg	= (BYTE) (0.299*bRed + 0.587*bGreen + 0.114*bBlue);
					
					// Set the gray color back to three color components of a pixel.
					pPixel[0]	= bAvg;		// Replace the blue value.
					pPixel[1]	= bAvg;		// Replace the green value.
					pPixel[2]	= bAvg;		// Replace the red value.
				} 
			} 
		} 
	} 
	else // The bitmap is indexed.
	{
		int			nMaxColors;		// The maximum number of entries in the color table.
		RGBQUAD *	rgbTable;		// The color table consisting of intensity values of red, green and blue.
		int			avgGray;		// Average gray value.
		
		// Retrieves the number of entries in the color table.
		nMaxColors	= GetMaxColorTableEntries();
		rgbTable	= new RGBQUAD[nMaxColors];  // Construct RGB color table as number of entries.
		GetColorTable(0, nMaxColors, rgbTable); // Get RGB color from entries in the pallete.
		
		// Iterate through each entries in the color table.
		for (int i=0; i<nMaxColors; i++)
		{
			avgGray = (rgbTable[i].rgbBlue + rgbTable[i].rgbGreen + rgbTable[i].rgbRed)/3;

			// Set the gray color back to three components of a pixel.
			rgbTable[i].rgbBlue		= avgGray;
			rgbTable[i].rgbGreen	= avgGray;
			rgbTable[i].rgbRed		= avgGray;
		}

		SetColorTable(0, nMaxColors, rgbTable);
		delete rgbTable;
	}
}

// The passed-in coordinate is the center of the image.
CString CFMImage::CreateSubImage(int iCenterX, int iCenterY, int iWidth, int iHeight, bool bXYCenter)
{
	Initialization();

	// If the passed-in X,Y coordinate is already the starting point, just call the function directly.
	if (!bXYCenter)
		return CreateSubImage(/*iStartX = */iCenterX, /*iStartY = */iCenterY, iWidth, iHeight);

	int iStartX, iStartY;

	if ((iCenterX - iWidth/2) < 0)
		iStartX = 0;
	else if ((iCenterX + iWidth/2) > m_nWidth)
		iStartX = m_nWidth - iWidth;
	else
		iStartX = iCenterX - iWidth/2;

	if ((iCenterY - iHeight/2) < 0)
		iStartY = 0;
	else if ((iCenterY + iHeight/2) > m_nHeight)
		iStartY = m_nHeight - iHeight;
	else
		iStartY = iCenterY - iHeight/2;

	return CreateSubImage(iStartX, iStartY, iWidth, iHeight);
}


//
// Create a subset of image.
// 
CString CFMImage::CreateSubImage(int iStartX, int iStartY, int iWidth, int iHeight)
{
	const int BPP = 3 * 8;		// Bits per pixel.
	CImage objImage;			// The returned image object.
	int n_yRound, n_xRound;		// The round of processing
	int x, y;
	int xn, yn;
	BYTE * pPixel;				// Pointer to the pixel address.
	BYTE * pPixelN;				// Pointer to the pixel address of new created image.
	
	if (false == objImage.Create(iWidth, 0-iHeight, BPP))
		return CString("");
	
	n_yRound = iStartY + iHeight;
	n_xRound = iStartX + iWidth;

	for (yn = 0, y = iStartY; y < n_yRound; yn++, y++)
	{
		for (xn = 0, x = iStartX; x < n_xRound; xn++, x++)
		{
			// Get the pointer to the pixel of image patch at (x,y).
			pPixel	= (BYTE *) GetPixelAddress(x,y);
			
			// Get the pointer to the pixel of new image at (x,y).
			pPixelN = (BYTE *) objImage.GetPixelAddress(xn,yn);
			
			// Assign the value to each pixel.
			pPixelN[0] = pPixel[0];	
			pPixelN[1] = pPixel[1];	
			pPixelN[2] = pPixel[2];
		}
	}

	m_nSubImageIndex = m_nSubImageIndex%8;
	CString strName;
	strName.Format("%s\\%d.JPG", theApp.GetApplicationPath(), m_nSubImageIndex++);

	objImage.Save(strName);

	return strName;
}
