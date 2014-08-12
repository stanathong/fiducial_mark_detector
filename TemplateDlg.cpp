/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// TemplateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FiducialMarkProject.h"
#include "TemplateDlg.h"
#include ".\templatedlg.h"


// CTemplateDlg dialog

IMPLEMENT_DYNAMIC(CTemplateDlg, CDialog)
CTemplateDlg::CTemplateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTemplateDlg::IDD, pParent)
{
	m_pPreviewImg = NULL;
}

CTemplateDlg::~CTemplateDlg()
{
}

void CTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW, m_ctrlPreview);
}


BEGIN_MESSAGE_MAP(CTemplateDlg, CDialog)
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, OnBrowseTemplate)
	ON_BN_CLICKED(IDCANCEL, OnCancelTemplate)
	ON_BN_CLICKED(IDC_LOAD, OnLoadTemplate)
END_MESSAGE_MAP()


// CTemplateDlg message handlers

void CTemplateDlg::OnBrowseTemplate()
{
	CString				strFilter = "JPEG (*.JPG;*.JPEG;*.JPE;*.JFIF)|*.JPG;*.JPEG;*.JPE;*.JFIF|BMP (*.BMP;*.DIB;*.RLE)|*.BMP;*.DIB;*.RLE|GIF (*.GIF)|*.GIF|TIFF (*.TIF;*.TIFF)|*.TIF;*.TIFF|PNG (*.PNG)|All Files (*.*)|*.*||";

	CFileDialog objFileDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	
	if (objFileDialog.DoModal() != IDOK)
		return;

	// Store the file name.
	m_strTemplateFile = objFileDialog.GetPathName();	

	// Display the file name on the edit box.
	SetDlgItemText(IDC_TEMPLATE_EDIT, m_strTemplateFile);

	// Load the image into the control box.
	if (m_strTemplateFile)
	{
		m_objPreviewImg.Destroy();
		if (m_objPreviewImg.Load(m_strTemplateFile) == S_OK)
		{
			m_ctrlPreview.SetBitmap((HBITMAP) m_objPreviewImg);
			m_ctrlPreview.Invalidate();		
		}
	}
}

void CTemplateDlg::OnCancelTemplate()
{
	// m_strTemplateFile = "";

	OnOK();
}

void CTemplateDlg::OnLoadTemplate()
{
	// Do nothing just simply close the dialog.
	OnOK();
}

// To initialize the dialog by loading the saved data or default data.
BOOL CTemplateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_TEMPLATE_EDIT, m_strTemplateFile);

	// Load the image into the control box.
	if (m_strTemplateFile != "")
	{
		m_objPreviewImg.Destroy();
		if (m_objPreviewImg.Load(m_strTemplateFile) == S_OK)
		{
			m_ctrlPreview.SetBitmap((HBITMAP) m_objPreviewImg);
			m_ctrlPreview.Invalidate();		
		}
	}

	return TRUE;
}