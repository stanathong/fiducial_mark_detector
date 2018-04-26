/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


#pragma once

class CProcessSetting
{
public:
	CProcessSetting(void);
	~CProcessSetting(void);
public:
	bool		m_bEnableGrayscale;
	bool		m_bEnableSubpixel;
	CString		m_strTemplateFile;	// To store the template file name
	bool		m_bLikelihoodSearch;
	int			m_iSearchCoverage;
};
