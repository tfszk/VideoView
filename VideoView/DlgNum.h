#pragma once


// CDlgNum ダイアログ

class CDlgNum : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNum)

public:
	CDlgNum(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CDlgNum();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_NUM };
#endif

	int m_num;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
