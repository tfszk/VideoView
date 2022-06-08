#include "stdafx.h"
#include "ScrollHelper.h"

int ScrollHelper::calc_scroll_pos(
        int nSBCode, int nPos,
        int cur_pos, int doc_sz, int wnd_sz, int scr_step, int pag_step)
{
    int max_val = doc_sz - wnd_sz;

	switch (nSBCode) {
	case SB_TOP:		   cur_pos = 0; break;
	case SB_BOTTOM:		   cur_pos = max_val;; break;
	case SB_LINEUP:		   cur_pos -= scr_step; break;
	case SB_LINEDOWN:      cur_pos += scr_step; break;
	case SB_PAGEUP:        cur_pos -= pag_step; break;
	case SB_PAGEDOWN:      cur_pos += pag_step; break;
	case SB_THUMBPOSITION: cur_pos = nPos; break;
	case SB_THUMBTRACK:    cur_pos = nPos; break;
    case SB_ENDSCROLL: /* Do Nothing */; break;
	}

	if (cur_pos < 0) cur_pos = 0;
	if (cur_pos >= max_val) cur_pos = max_val;

    return cur_pos;
}

void ScrollHelper::onVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (m_wnd == NULL || (m_flag & NO_VERT)) return;
    int pos_y = calc_scroll_pos(
                    nSBCode, nPos, m_pos_y, m_doc_sz.cy,
                    m_wnd_sz.cy, m_step_sz.cy, m_wnd_sz.cy / 2);

	if (m_pos_y != pos_y)
    {
		m_pos_y = pos_y;
	    m_wnd->SetScrollPos(SB_VERT, m_pos_y, 1);
	    m_wnd->Invalidate();
    }
}

void ScrollHelper::onHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (m_wnd == NULL || (m_flag & NO_HORZ)) return;
    int pos_x = calc_scroll_pos(
                    nSBCode, nPos, m_pos_x, m_doc_sz.cx,
                    m_wnd_sz.cx, m_step_sz.cx, m_wnd_sz.cx / 2);

	if (m_pos_x != pos_x)
    {
		m_pos_x = pos_x;
	    m_wnd->SetScrollPos(SB_HORZ, m_pos_x, 1);
	    m_wnd->Invalidate();
    }
}

BOOL ScrollHelper::onMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int cnt, pos_y;

    if (m_wnd == NULL || (m_flag & NO_VERT)) return FALSE;
    if (m_doc_sz.cy < m_wnd_sz.cy) return FALSE;

    cnt = zDelta / WHEEL_DELTA;
	pos_y = m_pos_y - cnt * m_step_sz.cy;

    pos_y = calc_scroll_pos(
                    SB_ENDSCROLL, 0, pos_y, m_doc_sz.cy,
                    m_wnd_sz.cy, m_step_sz.cy, m_wnd_sz.cy / 2);

	if (m_pos_y != pos_y) {
		m_pos_y = pos_y;
		m_wnd->SetScrollPos(SB_VERT, m_pos_y, 1);
		m_wnd->Invalidate();
		return TRUE;
	}

	return FALSE;
}

int ScrollHelper::moveScrollPos(int dx, int dy)
{
    onVScroll(SB_THUMBPOSITION, m_pos_y+dy, NULL);
    onHScroll(SB_THUMBPOSITION, m_pos_x+dx, NULL);
    return TRUE;
}

/* ---------------------------------------------- */

void ScrollHelper::onSize(UINT nType, int cx, int cy)
{
    if (m_wnd == NULL) return;
    
    int tmp_cx = m_wnd_sz.cx; // To detect change while recursive call

    if ((m_flag & NO_VERT) == 0) {
        if (m_wnd_sz.cy != cy)
        {
            m_wnd_sz.cy = cy;
            setVertScroll();
        }
    } else {
        m_wnd->ShowScrollBar(SB_VERT, FALSE);
    }

    if ((m_flag & NO_HORZ) == 0) {
        if (tmp_cx == m_wnd_sz.cx && 
            m_wnd_sz.cx != cx)
        {
            m_wnd_sz.cx = cx;
            setHorzScroll();
        }
    } else {
        m_wnd->ShowScrollBar(SB_HORZ, FALSE);
    }
}

void ScrollHelper::changeDocSize(const CSize& doc_size)
{
    if (m_wnd == NULL) return;

    if ((m_flag & NO_VERT) == 0) {
        int doc_sz_mag = (int)(doc_size.cy * m_zoom_rate);
        if (m_doc_sz.cy != doc_sz_mag)
        {
            m_doc_sz.cy = doc_sz_mag;
            m_doc_org_sz.cy = doc_size.cy;
            setVertScroll();
            m_wnd->Invalidate();
        }
    } else {
        m_wnd->ShowScrollBar(SB_VERT, FALSE);
    }

    if ((m_flag & NO_HORZ) == 0) {
        int doc_sz_mag = (int)(doc_size.cx * m_zoom_rate);
        if ((m_doc_sz.cx != doc_sz_mag))
        {
            m_doc_sz.cx = doc_sz_mag;
            m_doc_org_sz.cx = doc_size.cx;
            setHorzScroll();
            m_wnd->Invalidate();
        }
    } else {
        m_wnd->ShowScrollBar(SB_HORZ, FALSE);
    }
}

void ScrollHelper::setZoomRate(float zoom)
{
    if (m_wnd == NULL) return;
    if (m_zoom_rate == zoom) return;

    m_zoom_rate = zoom;
    changeDocSize(m_doc_org_sz);
}

int ScrollHelper::setVertScroll()
{
    SCROLLINFO scr = {sizeof(SCROLLINFO), SIF_ALL};
    if ((m_flag & NO_FIX_POS) == 0 &&
        ((m_flag & FIX_POS_ONLY_HIDDEN) == 0 ||
         m_wnd_sz.cy > m_doc_sz.cy-1))
    {
        if (m_pos_y > (m_doc_sz.cy) - m_wnd_sz.cy)
            m_pos_y = (m_doc_sz.cy) - m_wnd_sz.cy;
        if (m_pos_y < 0) m_pos_y = 0;
    }
    scr.nMin  = 0;
    scr.nMax  = m_doc_sz.cy-1;
    scr.nPage = m_wnd_sz.cy;
    scr.nPos  = m_pos_y;
    return m_wnd->SetScrollInfo(SB_VERT, &scr, 1);
}

int ScrollHelper::setHorzScroll()
{
    SCROLLINFO scr = {sizeof(SCROLLINFO), SIF_ALL};
    if ((m_flag & NO_FIX_POS) == 0 &&
        ((m_flag & FIX_POS_ONLY_HIDDEN) == 0 ||
         m_wnd_sz.cx > m_doc_sz.cx-1))
    {
        if (m_pos_x > (m_doc_sz.cx) - m_wnd_sz.cx)
            m_pos_x = (m_doc_sz.cx) - m_wnd_sz.cx;
        if (m_pos_x < 0) m_pos_x = 0;
    }
    scr.nMin  = 0;
    scr.nMax  = m_doc_sz.cx-1;
    scr.nPage = m_wnd_sz.cx;
    scr.nPos  = m_pos_x;
    return m_wnd->SetScrollInfo(SB_HORZ, &scr, 1);
}

#if 0
void ScrollHelper::onSize(UINT nType, int cx, int cy)
{
	nType;

	if (m_wnd == NULL) {
		return;
	}
	SCROLLINFO scr = { sizeof(SCROLLINFO), SIF_ALL };

	if (m_wnd_sz.cy != cy)
	{
		m_wnd_sz.cy = cy;
		scr.nMin = 0;
		scr.nMax = m_doc_sz.cy - 1;
		scr.nPage = cy;
		scr.nPos = m_pos_y;
		m_wnd->SetScrollInfo(SB_VERT, &scr, 1);
	}

	if (m_wnd_sz.cy != cx)
	{
		m_wnd_sz.cx = cx;
		scr.nMin = 0;
		scr.nMax - m_doc_sz.cx - 1;
		scr.nPage = cx;
		scr.nPos = m_pos_x;
		m_wnd->SetScrollInfo(SB_HORZ, &scr, 1);
	}
}

void ScrollHelper::chageDocSize(const CSize& doc_size)
{
	if (m_wnd == NULL) {
		return;
	}

	SCROLLINFO scr = { sizeof(SCROLLINFO), SIF_ALL };

	if (m_doc_sz.cy != doc_size.cy)
	{
		m_doc_sz.cy = doc_size.cy;
		scr.nMin = 0;
		scr.nMax - m_doc_sz.cy - 1;
		scr.nPage = m_wnd_sz.cy;
		scr.nPos = m_pos_y;
		m_wnd->SetScrollInfo(SB_VERT, &scr, 1);;
		m_wnd->Invalidate();
	}

	if (m_doc_sz.cx != doc_size.cx)
	{
		m_doc_sz.cx = doc_size.cx;
		scr.nMin = 0;
		scr.nMax = m_doc_sz.cx - 1;
		scr.nPage = m_wnd_sz.cx - 1;
		scr.nPage = m_wnd_sz.cx;
		scr.nPos = m_pos_x;
		m_wnd->SetScrollInfo(SB_HORZ, &scr, 1);
		m_wnd->Invalidate();
	}
}

#endif
