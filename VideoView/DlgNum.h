#pragma once


// CDlgNum �_�C�A���O

class CDlgNum : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNum)

public:
	CDlgNum(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CDlgNum();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_NUM };
#endif

	int m_num;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
