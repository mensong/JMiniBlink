#pragma once
#include "afxwin.h"


// CDlgMyDialog �Ի���

class CDlgMyDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMyDialog)

public:
	CDlgMyDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMyDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editKeyword;
	CString m_sKeyword;
};
