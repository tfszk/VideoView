// DlgNum.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "VideoView.h"
#include "DlgNum.h"
#include "afxdialogex.h"


// CDlgNum �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgNum, CDialogEx)

CDlgNum::CDlgNum(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_NUM, pParent)
{

}

CDlgNum::~CDlgNum()
{
}

void CDlgNum::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNum, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgNum::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNum ���b�Z�[�W �n���h���[

BOOL CDlgNum::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDlgItemInt(IDC_EDIT1, m_num);

	return TRUE;
}

void CDlgNum::OnBnClickedOk()
{
	m_num = GetDlgItemInt(IDC_EDIT1);

	CDialogEx::OnOK();
}
