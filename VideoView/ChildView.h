
// ChildView.h : interface of the CChildView class
//


#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "ScrollHelper.h"

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
	int m_frmno;
	int m_frame_cnt;
	cv::VideoCapture m_cap;
	cv::Mat m_mat;
	CImage m_img;

	ScrollHelper m_scrl;
	int m_zoom_rate;

	CPoint m_mouse;

protected:
	// For Drag&Drop
	class CustomOleDropTarget : public COleDropTarget {
		virtual DROPEFFECT OnDragEnter(
			CWnd* pWnd,
			COleDataObject* pDataObject,
			DWORD dwKeyState,
			CPoint point);
		virtual DROPEFFECT OnDragOver(
			CWnd* pWnd,
			COleDataObject* pDataObject,
			DWORD dwKeyState,
			CPoint point);
		virtual BOOL OnDrop(
			CWnd* pWnd,
			COleDataObject* pDataObject,
			DROPEFFECT dropEffect,
			CPoint point);
	} m_droptarget;

	BOOL OnDropFiles(HDROP hdrop);

	void openFileByFilename(const CString& fnm);

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	void loadFrameImage(int frmno);
	void adjustScrollBar();

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnFileOpen();
	afx_msg void OnEditNext();
	afx_msg void OnEditPrev();
	afx_msg void OnEditJump();
	afx_msg void OnUpdateEditJump(CCmdUI *pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditForward();
	afx_msg void OnEditBackward();
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewZoomOut();
	afx_msg void OnViewZoomReset();
	afx_msg void OnEditCopy();
};
