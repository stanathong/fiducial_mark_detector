/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// CalibrationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FiducialMarkProject.h"
#include "CalibrationDlg.h"
#include ".\calibrationdlg.h"


// CCalibrationDlg dialog

IMPLEMENT_DYNAMIC(CCalibrationDlg, CDialog)
CCalibrationDlg::CCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibrationDlg::IDD, pParent)
{
	m_strIOFile = "";
}

CCalibrationDlg::~CCalibrationDlg()
{
}

void CCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCalibrationDlg, CDialog)
	ON_BN_CLICKED(ID_CALIBRATION_SAVE, OnSaveCalibrationData)
END_MESSAGE_MAP()

// Read the calibration data from the ".\Data\Calibration.txt" file if exists.
bool CCalibrationDlg::ReadCalibrationData()
{
	char	line[128];
	int		nFC = 0;

	// Construct the path to the calibration data.
	m_strIOFile = theApp.GetApplicationPath() + "\\Data\\Calibration.txt";

	// If the ".\Data\Calibration.txt" file exists, read the calibration data from file.
	FILE * fCalibration = fopen(m_strIOFile, "r");

	// If the file exist load the data from file.
	if (fCalibration)
	{
		// Get the coordinate of fiducial marks.
		while ((nFC < MAX_MATCHING) /*&& !_eof(fCalibration)*/)
		{
			// Read line by line from file.
			fgets(line, sizeof(line), fCalibration);
			if (line[0] == '*')	
				continue;	// Skip if it is comment.
			else
				sscanf(line, "%f,%f\n", &m_FC[nFC].fX, &m_FC[nFC].fY);

			nFC++;
		}

		// Get the coordinate of the principal point.
		// fgets(line, sizeof(line), fCalibration);
		fscanf(fCalibration, "%f,%f\n", &m_PPA.fX, &m_PPA.fY);
		// fgets(line, sizeof(line), fCalibration);
		fscanf(fCalibration, "%f,%f\n", &m_PPS.fX, &m_PPS.fY);

		fclose(fCalibration);

		return true;
	}

	// File does not exist
	return false;
}

// CCalibrationDlg message handlers
// To initialize the dialog by loading the resulting data to display.
BOOL CCalibrationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Read the calibration data from file if exist.
	if (!ReadCalibrationData())
	{
		// The file does not exist.

		return TRUE;
	}

	int iFC;
	int BASE_ID = IDC_FC1_X;
	CString strX, strY;

	for (iFC = 0; iFC < MAX_MATCHING; iFC++)
	{
		// Display the X coordinate on the control.
		strX.Format("%.3f", m_FC[iFC].fX);
		SetDlgItemText(IDC_FC1_X + 2*iFC, strX);

		// Display the Y coordinate on the control.
		strY.Format("%.3f", m_FC[iFC].fY);
		SetDlgItemText(IDC_FC1_X + 2*iFC + 1, strY);
	}

	// Display the PPA coordinate.
	strX.Format("%.3f", m_PPA.fX);
	SetDlgItemText(IDC_PPA_X, strX);

	strY.Format("%.3f", m_PPA.fY);
	SetDlgItemText(IDC_PPA_Y, strY);

	// Display the PPS coordinate.
	strX.Format("%.3f", m_PPS.fX);
	SetDlgItemText(IDC_PPS_X, strX);

	strY.Format("%.3f", m_PPS.fY);
	SetDlgItemText(IDC_PPS_Y, strY);
	
	return TRUE;
}

// Save the calibration data to the ".\Data\Calibration.txt" file.
bool CCalibrationDlg::SaveCalibrationData()
{
	int		nFC = 0;

	// Construct the path to the calibration data.
	m_strIOFile = theApp.GetApplicationPath() + "\\Data\\Calibration.txt";

	// Open the ".\Data\Calibration.txt" file to write.
	FILE * fCalibration = fopen(m_strIOFile, "w");

	// If the file exist load the data from file.
	if (fCalibration)
	{
		// Get the coordinate of fiducial marks.
		while ((nFC < MAX_MATCHING) /*&& !_eof(fCalibration)*/)
		{
			fprintf(fCalibration, "%.3f,%.3f\n", m_FC[nFC].fX, m_FC[nFC].fY);
			nFC++;
		}

		// Print the coordinate of the PPA.
		fprintf(fCalibration, "%.3f,%.3f\n", m_PPA.fX, m_PPA.fY);
		// Print the coordinate of the PPS.
		fprintf(fCalibration, "%.3f,%.3f\n", m_PPS.fX, m_PPS.fY);

		fclose(fCalibration);

		return true;
	}

	// File does not exist
	return false;
}
void CCalibrationDlg::OnSaveCalibrationData()
{
	// Obtain the setting values from the dialog and save to the file.
	int iFC;
	int BASE_ID = IDC_FC1_X;
	CString strX, strY;
	char * cstrX, * cstrY;

	// Obtain the setting of Fiducial Coordinate from the control.
	for (iFC = 0; iFC < MAX_MATCHING; iFC++)
	{
		// Save the X coordinate from the control.
		GetDlgItemText(IDC_FC1_X + 2*iFC, strX);
		cstrX = strX.GetBuffer(strX.GetLength());
		m_FC[iFC].fX = atof(cstrX);

		// Save the Y coordinate from the control.
		GetDlgItemText(IDC_FC1_X + 2*iFC+1, strY);
		cstrY = strY.GetBuffer(strY.GetLength());
		m_FC[iFC].fY = atof(cstrY);
	}

	// Save the X,Y coordinate from PPA
	GetDlgItemText(IDC_PPA_X, strX);
	cstrX = strX.GetBuffer(strX.GetLength());
	m_PPA.fX = atof(cstrX);

	// Save the Y coordinate from the control.
	GetDlgItemText(IDC_PPA_Y, strY);
	cstrY = strY.GetBuffer(strY.GetLength());
	m_PPA.fY = atof(cstrY);

	// Save the X,Y coordinate from PPS
	GetDlgItemText(IDC_PPS_X, strX);
	cstrX = strX.GetBuffer(strX.GetLength());
	m_PPS.fX = atof(cstrX);

	// Save the Y coordinate from the control.
	GetDlgItemText(IDC_PPS_Y, strY);
	cstrY = strY.GetBuffer(strY.GetLength());
	m_PPS.fY = atof(cstrY);

	// Save the calibration data.
	SaveCalibrationData();

	OnOK();
}
