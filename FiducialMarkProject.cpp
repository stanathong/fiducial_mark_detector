/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// FiducialMarkProject.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FiducialMarkProject.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "FMDoc.h"
#include "FMView.h"
#include ".\fiducialmarkproject.h"
#include "TemplateDlg.h"
#include "ProcessingOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFMApp

BEGIN_MESSAGE_MAP(CFMApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	// ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	// ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, OnBaseImageOpen)
	ON_COMMAND(ID_TOOLS_TEMPLATE, OnToolsTemplate)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	// ON_COMMAND(ID_MATCHING_CROSSCORRELATION, OnMatchingCrossCorrelation)
END_MESSAGE_MAP()


// CFMApp construction

CFMApp::CFMApp()
{
	m_pDlgProcessingOptions = NULL;
	m_pDlgTemplate = NULL;	
}


// The one and only CFMApp object

CFMApp theApp;

// CFMApp initialization

BOOL CFMApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_FMLocatorTYPE,
		RUNTIME_CLASS(CFMDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CFMView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd

	// NOT TO OPEN NEW DOCUMENT WHEN THE PROGRAM IS LOADED.
	/*
		// Parse command line for standard shell commands, DDE, file open
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);
		// Dispatch commands specified on the command line.  Will return FALSE if
		// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	*/

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

END_MESSAGE_MAP()

// App command to run the dialog
void CFMApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CFMApp message handlers


/*
 * This function is to open the base image file that containing 8 fiducial marks.
 */
void CFMApp::OnBaseImageOpen()
{
	// Image file textension
	CString	strFilter = "JPEG (*.JPG;*.JPEG;*.JPE;*.JFIF)|*.JPG;*.JPEG;*.JPE;*.JFIF|BMP (*.BMP;*.DIB;*.RLE)|*.BMP;*.DIB;*.RLE|GIF (*.GIF)|*.GIF|TIFF (*.TIF;*.TIFF)|*.TIF;*.TIFF|PNG (*.PNG)|All Files (*.*)|*.*||";

	CFileDialog objFileDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	
	if (objFileDialog.DoModal() != IDOK)
		return;

	// Store the file name.
	m_strFileName = objFileDialog.GetPathName();	

	// Open the selected file by the CDocument class.
	OpenDocumentFile(m_strFileName);
}


/*	NOT MFC function - written by AUI
 *	This function is to retrieve the current document.
 */
CFMDoc * CFMApp::GetCurrentDocument()
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();

	if (NULL == pChild)
		return NULL;

	CDocument * pDoc = pChild->GetActiveDocument();

	if (NULL == pDoc)
		return NULL;

	// Check if it is the correct document
	if (!pDoc->IsKindOf(RUNTIME_CLASS(CFMDoc)))
		return NULL;

      return (CFMDoc *) pDoc;
}
	

void CFMApp::OnToolsTemplate()
{
	if (m_pDlgTemplate == NULL)
		m_pDlgTemplate = new CTemplateDlg;

	m_pDlgTemplate->DoModal();	

	// Store the template image file.
	m_Setting.m_strTemplateFile = m_pDlgTemplate->m_strTemplateFile;

	// Get the pointer to the current document.
	CFMDoc * pCurrentDoc = GetCurrentDocument();

	if (!pCurrentDoc)
		exit(1);

	// Open the template file.
	pCurrentDoc->OnOpenTemplateImage(m_Setting.m_strTemplateFile);
}

void CFMApp::OnToolsOptions()
{
	if (m_pDlgProcessingOptions == NULL)
		m_pDlgProcessingOptions = new CProcessingOptions;

	m_pDlgProcessingOptions->DoModal();	

	// Store the setting into the m_Setting object.
	m_Setting.m_bEnableGrayscale	= m_pDlgProcessingOptions->m_bEnableGrayscale;
	m_Setting.m_bEnableSubpixel		= m_pDlgProcessingOptions->m_bEnableSubpixel;
	m_Setting.m_bLikelihoodSearch	= m_pDlgProcessingOptions->m_bLikelihoodSearch;
	m_Setting.m_iSearchCoverage		= m_pDlgProcessingOptions->m_iSearchCoverage;
}

CString CFMApp::GetApplicationPath()
{
	char		strModuleFileName[_MAX_PATH] = {0};
	int			nIndex = -1;

	// Get the path of the application.
	GetModuleFileName(NULL, strModuleFileName, _MAX_PATH);
	
	// Get the start position of the file name, after \\.
	nIndex = CString(strModuleFileName).ReverseFind('\\') + 1;

	// Set the file name to zero to get only the path.
	memset(strModuleFileName+nIndex, 0, sizeof(strModuleFileName)-nIndex);

	return CString(strModuleFileName);
}
