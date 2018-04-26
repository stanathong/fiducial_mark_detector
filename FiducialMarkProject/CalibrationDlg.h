/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


#pragma once


// CCalibrationDlg dialog

class CCalibrationDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalibrationDlg)

public:
	CCalibrationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCalibrationDlg();

// Dialog Data
	enum { IDD = IDD_CAMERA_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnInitDialog();

	// Read the calibration data from the ".\Data\Calibration.txt" file if exists.
	// Return true if the file exist.
	bool ReadCalibrationData();

	// Save the calibration data to the ".\Data\Calibration.txt" file.
	bool SaveCalibrationData();

public:
	// The string file name of the IO data.
	CString		m_strIOFile;
	// The position in floating point for related camera calibration data.
	SUB_XY_POS	m_FC[MAX_MATCHING];
	SUB_XY_POS	m_PPA;
	SUB_XY_POS	m_PPS;
	afx_msg void OnSaveCalibrationData();
};
