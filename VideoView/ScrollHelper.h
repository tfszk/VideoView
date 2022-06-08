#ifndef SCROLL_HELPER_H
#define SCROLL_HELPER_H

/*
 * Scroll Helper
 *   Implementation of scroll function for CWnd.
 *
 *   Usage:
 *     1. define ScrollHelper m_scrl; member at CWnd class.
 *     2. Call m_scrl.setup() to set sizes.
 *     3. Add following message handlers:
 *        - WM_VSCROLL    // OnVScroll()
 *        - WM_HSCROLL    // OnHScroll()
 *        - WM_SIZE       // OnSize()
 *        - WM_MOUSEWHEEL // OnMouseWheel() - maybe needs to forward from Frame.
 *     4. Call following function from above funcs:
 *        - onVScroll()
 *        - onHScroll()
 *        - onSize()
 *        - onMouseWheel()
 *     5. dc.SetViewPointOrg(-m_scrl.m_pos_x, -m_scrl.m_pos_y) when paint.
 *
 *   Assumption:
 *     - All unit of size are same -- device(screen) unit.
 *     - Scroll half of view when scroll by page.
 *   Todo/Restriction:
 *     - Wheel scroll only vertical bar.
 *     - Needs to reset position when bar is disappeared.
 */

class ScrollHelper {
public:
    enum {
        NO_HORZ = 0x0001,
        NO_VERT = 0x0002,
        NO_FIX_POS = 0x0004,
        FIX_POS_ONLY_HIDDEN = 0x0008,
    };
    int m_flag;

    CSize m_step_sz; // Line Step Delta

    CWnd *m_wnd;     // Target Window

    float m_zoom_rate;

	int m_pos_x;     // Scroll Position
	int m_pos_y;

protected:
	CSize m_wnd_sz;  // Window size of m_wnd
    CSize m_doc_sz;  // Document Size (zoomed)
    CSize m_doc_org_sz;  // Document Size (orig)

public:
    ScrollHelper()
    {
        m_flag = 0;

        m_pos_x = 0;
        m_pos_y = 0;
        m_wnd_sz = CSize(0, 0);
        m_zoom_rate = 1.0;

        m_doc_org_sz = m_doc_sz = CSize(1,1);
        m_step_sz = CSize(1,1);
        m_wnd = NULL;
    }

    void setup(CWnd *wnd, const CSize& doc_sz, const CSize& step_sz)
    {
        m_wnd = wnd;
        m_doc_org_sz = m_doc_sz = doc_sz;
        m_step_sz = step_sz;
    }
    void changeDocSize(const CSize& doc_size);

    void onVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    void onHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    BOOL onMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    void onSize(UINT nType, int cx, int cy);

    int moveScrollPos(int dx, int dy);
    void setZoomRate(float zoom);

    int getPosX() const { return m_pos_x; }
    int getPosY() const { return m_pos_y; }
    CSize getDocSize() const { return m_doc_sz; }
    CSize getDocOrgSize() const { return m_doc_org_sz; }
    CSize getWndSize() const { return m_wnd_sz; }

protected:
    int calc_scroll_pos(
        int nSBCode, int nPos, int cur_pos,
        int doc_sz, int wnd_sz, int scr_step, int pag_step);
    int setHorzScroll();
    int setVertScroll();
};

#endif /* SCROLL_HELPER_H */
