/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


#include "StdAfx.h"
#include ".\processsetting.h"

CProcessSetting::CProcessSetting(void)
{
	m_bEnableGrayscale = true;
	m_bEnableSubpixel = false;
	m_strTemplateFile = "";
	m_bLikelihoodSearch = true;
	m_iSearchCoverage = DEFAULT_PERCENT_COVERAGE;
}

CProcessSetting::~CProcessSetting(void)
{
}
