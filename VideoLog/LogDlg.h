#pragma once
#include "LogProc.h"
#include <GdiPlus.h>
using namespace Gdiplus;
// CLogDlg �Ի���

class CLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLogDlg)

public:
	CLogDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogDlg();

// �Ի�������
	enum { IDD = IDD_LOG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	// �׽���������
	SOCKET m_sock;
public:
	afx_msg void OnBnClickedLogButton();
	static void TcharToChar(const TCHAR* tchar, char* _char);
private:
	// �û���½����
	int login(void);
	// �û��˺��ַ���ָ��
	char* account;
	// �û������ַ���ָ��
	char* password;
protected:
	afx_msg LRESULT OnUdpsockread(WPARAM wParam, LPARAM lParam);
private:
	CLogProc* pLogProc;
public:
	afx_msg void OnClickedRegister();
	afx_msg void OnClickedForgotPassword();
protected:
	afx_msg LRESULT OnLogCancel(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPaint();
private:
	Graphics* pGraphics;
	ULONG_PTR gdiplusToken;
};
