/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "FiducialMarkProject.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	// Modify the Window class or styles here by modifying the CREATESTRUCT cs.
	// This is to set the main frame to maximize since start up.
	cs.style |= WS_MAXIMIZE;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// Maximize the window frame
void CChildFrame::ActivateFrame(int nCmdShow) 
{
	CMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED);
}



// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers
