
// InitCardDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CInitCardDlg �Ի���
class CInitCardDlg : public CDialogEx
{
// ����
public:
	CInitCardDlg(CWnd* pParent = NULL);	// ��׼���캯��
//	CString m_cardno;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INITCARD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	//CEdit m_cardno;
	afx_msg void OnBnClickedButton2();
	CString m_cardno;
};
