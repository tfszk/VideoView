
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "VideoView.h"
#include "ChildView.h"
#include "DlgNum.h"

#include <gdiplus.h>
#include <string>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world341d.lib")
#else
#pragma comment(lib, "opencv_world341.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_frmno = 0;
	m_frame_cnt = 0;
	m_zoom_rate = 100;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_FILE_OPEN, &CChildView::OnFileOpen)
	ON_COMMAND(ID_EDIT_NEXT, &CChildView::OnEditNext)
	ON_COMMAND(ID_EDIT_PREV, &CChildView::OnEditPrev)
	ON_COMMAND(ID_EDIT_JUMP, &CChildView::OnEditJump)
	ON_COMMAND(ID_VIEW_ZOOM_IN, &CChildView::OnViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, &CChildView::OnViewZoomOut)
	ON_COMMAND(ID_EDIT_FORWARD, &CChildView::OnEditForward)
	ON_COMMAND(ID_EDIT_BACKWARD, &CChildView::OnEditBackward)
	ON_UPDATE_COMMAND_UI(ID_EDIT_JUMP, &CChildView::OnUpdateEditJump)
	ON_COMMAND(ID_VIEW_ZOOM_RESET, &CChildView::OnViewZoomReset)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_COPY, &CChildView::OnEditCopy)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	m_scrl.setup(this,
		CSize((int)(1280 * m_zoom_rate / 100.0f), (int)(720 * m_zoom_rate / 100.0f)),
		CSize(24, 24));

	adjustScrollBar();

	return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// For Drag&Drop
	m_droptarget.Register(this);

	return 0;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (m_mat.empty()) return;

	dc.SetMapMode(MM_ANISOTROPIC);

	int w = m_mat.cols;
	int h = m_mat.rows;
	int x0 = 0;
	int y0 = 0;

	dc.SetWindowExt(w, h);
	dc.SetViewportExt((int)(w*m_zoom_rate / 100.0f), (int)(h*m_zoom_rate / 100.0f));
	dc.SetViewportOrg(x0 - m_scrl.m_pos_x, y0 - m_scrl.m_pos_y);

	cv::Mat m_rgb;
	// cv::cvtColor(m_mat, m_rgb, CV_BGR2RGB);
	CDC *pDC = CDC::FromHandle(m_img.GetDC());

	dc.BitBlt(0, 0, m_img.GetWidth(), m_img.GetHeight(), pDC, 0, 0, SRCCOPY);
	dc.SelectStockObject(WHITE_BRUSH);

	CFont fnt;
	CString mesg;
	dc.SetTextColor(RGB(255, 255, 255));
	mesg.Format(_T("Frame:%d"), m_frmno);
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(0, 0, mesg);

	m_img.ReleaseDC();
}


void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_scrl.onHScroll(nSBCode, nPos, pScrollBar);
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CChildView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_scrl.onVScroll(nSBCode, nPos, pScrollBar);
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	float xx = (m_scrl.m_pos_x + point.x) * 100.0f / m_zoom_rate;
	float yy = (m_scrl.m_pos_y + point.y) * 100.0f / m_zoom_rate;

	m_mouse = CPoint((int)xx, (int)yy);

	CWnd::OnMouseMove(nFlags, point);
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (GetKeyState(VK_CONTROL) < 0) {
		if (zDelta < 0) {
			OnViewZoomOut();
		}
		else {
			OnViewZoomIn();
		}
        return TRUE;
	}

	return m_scrl.onMouseWheel(nFlags, zDelta, pt);
}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CSize new_doc_sz;

	new_doc_sz.cx = 1280;
	new_doc_sz.cy = 720;

	new_doc_sz.cx = (int)(new_doc_sz.cx * m_zoom_rate / 100.0f);
	new_doc_sz.cy = (int)(new_doc_sz.cy * m_zoom_rate / 100.0f);

	m_scrl.changeDocSize(new_doc_sz);
}

/* Expand/Shrinke */
void CChildView::OnViewZoomIn()
{
	m_zoom_rate += 10;
	if (m_zoom_rate > 400) m_zoom_rate = 400;

	adjustScrollBar();
	Invalidate(TRUE);
}

void CChildView::OnViewZoomOut()
{
	m_zoom_rate -= 10;
	if (m_zoom_rate < 20) m_zoom_rate = 20;

	adjustScrollBar();
	Invalidate(TRUE);
}

void CChildView::OnViewZoomReset()
{
	m_zoom_rate = 100;
	adjustScrollBar();
	Invalidate(TRUE);
}


void CChildView::adjustScrollBar()
{
	CSize new_doc_sz;

	new_doc_sz.cx = 1280;
	new_doc_sz.cy = 720;

	new_doc_sz.cx = (int)(new_doc_sz.cx * m_zoom_rate / 100.0f);
	new_doc_sz.cy = (int)(new_doc_sz.cy * m_zoom_rate / 100.0f);

	//if ((m_draw_flag & DrawFlag::NoMargin) == 0) {
	// new_doc_sz.cx += 
	//}

	m_scrl.changeDocSize(new_doc_sz);
}

void CChildView::loadFrameImage(int frmno)
{
	if (m_cap.isOpened() == false) return;
	if (frmno < 0) frmno = 0;

	if (m_frmno + 1 != frmno) {
		m_cap.set(CV_CAP_PROP_POS_FRAMES, frmno);
	}
	m_frmno = frmno;

	m_cap.read(m_mat);

	if (m_mat.rows > 0) {

		Gdiplus::Bitmap img(m_mat.cols, m_mat.rows, (int)m_mat.step1(), PixelFormat24bppRGB, m_mat.data);

		HBITMAP hbmp = NULL;
		Gdiplus::Status stat;
		stat = img.GetHBITMAP(Gdiplus::Color::White, &hbmp);

		if (m_img.IsNull() == FALSE) {
			m_img.Destroy();
		}
		m_img.Attach(hbmp);

		//cv::imwrite("tmp.bmp", m_mat);
		//if (m_img.IsNull() == FALSE) {
		//	m_img.Destroy();
		//}
		//m_img.Load(_T("tmp.bmp"));
	}
}

void CChildView::OnFileOpen()
{
	CFileDialog dlg(TRUE, _T("*.avi"));

	if (dlg.DoModal() != IDOK) {
		return;
	}

	openFileByFilename(dlg.GetPathName());

}

void CChildView::openFileByFilename(const CString& fnm_in)
{
	CStringA fnm = CStringA(fnm_in);

	if (m_cap.open((LPCSTR)fnm) == false) {
		AfxMessageBox(_T("Failed to open Video File"));
		return;
	}

	m_frame_cnt = int(m_cap.get(cv::CAP_PROP_FRAME_COUNT));

	loadFrameImage(0);

	Invalidate();
}

void CChildView::OnEditNext()
{
	if (m_frmno >= m_frame_cnt) return;
	// m_frmno++;
	loadFrameImage(m_frmno + 1);
	Invalidate(FALSE);
}

void CChildView::OnEditPrev()
{
	if (m_frmno <= 0) return;
	// m_frmno--;
	loadFrameImage(m_frmno - 1);
	Invalidate(FALSE);
}

void CChildView::OnEditJump()
{
	CDlgNum dlg;

	dlg.m_num = m_frmno;

	if (dlg.DoModal() != IDOK) return;

	int frmno = dlg.m_num;

	loadFrameImage(frmno);
	Invalidate(FALSE);

}

void CChildView::OnUpdateEditJump(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CChildView::OnEditForward()
{
	// if (m_frmno >= max_frmno) return;
	// m_frmno += 10;
	loadFrameImage(m_frmno + 10);
	Invalidate(FALSE);
}

void CChildView::OnEditBackward()
{
	int frmno = m_frmno;
	if (frmno <= 0) {
		if (frmno == 1) return;
		frmno = 11;
	}

	loadFrameImage(m_frmno - 10);
	Invalidate(FALSE);
}

/*
 * Drag&Drop
 */
DROPEFFECT CChildView::CustomOleDropTarget::OnDragEnter(
	CWnd* pWnd,
	COleDataObject* pDataObject,
	DWORD dwKeyState,
	CPoint point)
{
	return DROPEFFECT_COPY;
}
DROPEFFECT CChildView::CustomOleDropTarget::OnDragOver(
	CWnd* pWnd,
	COleDataObject* pDataObject,
	DWORD dwKeyState,
	CPoint point)
{
	return DROPEFFECT_COPY;
}
BOOL CChildView::CustomOleDropTarget::OnDrop(
	CWnd* pWnd,
	COleDataObject* pDataObject,
	DROPEFFECT dropEffect,
	CPoint point)
{
	STGMEDIUM medium;

	if (pWnd->IsKindOf(RUNTIME_CLASS(CChildView))) {

		if (pDataObject->GetData(CF_HDROP, &medium)) {
			CChildView *wnd = (CChildView*)pWnd;
			wnd->OnDropFiles((HDROP)medium.hGlobal);
		}
	}

	return TRUE;
}

BOOL CChildView::OnDropFiles(HDROP hdrop)
{
	CString fnm;
	DragQueryFile(hdrop, 0, fnm.GetBuffer(MAX_PATH), MAX_PATH);
	fnm.ReleaseBuffer();

	openFileByFilename(fnm);

	return TRUE;
}



void CChildView::OnEditCopy()
{
	if (m_img.IsNull()) return;

	CRect rct(0, 0, m_img.GetWidth(), m_img.GetHeight());

	CDC *pBaseDC = CDC::FromHandle(m_img.GetDC());
	CDC *pBgDC = this->GetDC();

	CBitmap bmp;
	CDC dc;
	dc.CreateCompatibleDC(pBgDC);
	bmp.CreateCompatibleBitmap(pBgDC, rct.Width(), rct.Height());
	HGDIOBJ old = dc.SelectObject(&bmp);

	dc.BitBlt(0, 0,
		rct.Width(), rct.Height(),
		pBaseDC, rct.left, rct.top, SRCCOPY);

	dc.SelectObject(old);
	m_img.ReleaseDC();
	this->ReleaseDC(pBgDC);

	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bmp.GetSafeHandle());
	CloseClipboard();
}
