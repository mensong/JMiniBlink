#pragma once
#include "afxwin.h"


// CDlgMyDialog 对话框

class CDlgMyDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMyDialog)

public:
	CDlgMyDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMyDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editKeyword;
	CString m_sKeyword;
};
