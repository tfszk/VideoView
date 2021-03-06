
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "VideoView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_POS, &CMainFrame::OnUpdateMousePos)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_ZOOM, &CMainFrame::OnUpdateIndicatorZoom)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_FRMNO, &CMainFrame::OnUpdateIndicatorFrmno)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_POS,
	ID_INDICATOR_ZOOM,
	ID_INDICATOR_FRMNO,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_POS, SBPS_NORMAL, 250);
	m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_ZOOM, SBPS_NORMAL, 40);
	m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_FRMNO, SBPS_NORMAL, 40);

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnUpdateMousePos(CCmdUI *pCmdUI)
{
	CString txt;
	CChildView* wnd;

	wnd = &this->m_wndView;

	if (wnd) {
		txt.Format(_T("(%4d,%4d)"),
			wnd->m_mouse.x, wnd->m_mouse.y);
	}

	pCmdUI->SetText(txt);
}

void CMainFrame::OnUpdateIndicatorZoom(CCmdUI *pCmdUI)
{
	CString txt;
	CChildView* wnd = &this->m_wndView;

	if (wnd) {
		txt.Format(_T("%d%%"), wnd->m_zoom_rate);
	}

	pCmdUI->SetText(txt);
}

void CMainFrame::OnUpdateIndicatorFrmno(CCmdUI *pCmdUI)
{
	CString txt;
	CChildView* wnd = &this->m_wndView;

	if (wnd) {
		txt.Format(_T("%d/%d"), wnd->m_frmno, wnd->m_frame_cnt);
	}

	pCmdUI->SetText(txt);
}
