/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


#pragma once



// CTemplateFormView form view

class CTemplateFormView : public CFormView
{
	DECLARE_DYNCREATE(CTemplateFormView)

protected:
	CTemplateFormView();           // protected constructor used by dynamic creation
	virtual ~CTemplateFormView();

public:
	enum { IDD = IDD_FORMTEMPLATE };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


