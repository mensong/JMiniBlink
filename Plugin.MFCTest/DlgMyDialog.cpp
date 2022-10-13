// DlgMyDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Plugin.MFCTest.h"
#include "DlgMyDialog.h"
#include "afxdialogex.h"


// CDlgMyDialog 对话框

IMPLEMENT_DYNAMIC(CDlgMyDialog, CDialogEx)

CDlgMyDialog::CDlgMyDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_sKeyword(_T(""))
{

}

CDlgMyDialog::~CDlgMyDialog()
{
}

void CDlgMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editKeyword);
	DDX_Text(pDX, IDC_EDIT1, m_sKeyword);
}


BEGIN_MESSAGE_MAP(CDlgMyDialog, CDialogEx)
END_MESSAGE_MAP()


// CDlgMyDialog 消息处理程序
